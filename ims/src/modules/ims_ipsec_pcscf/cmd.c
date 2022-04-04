/*
 * Copyright (C) 2012 Smile Communications, jason.penton@smilecoms.com
 * Copyright (C) 2012 Smile Communications, richard.good@smilecoms.com
 * Copyright (C) 2019 Aleksandar Yosifov
 *
 * The initial version of this code was written by Dragos Vingarzan
 * (dragos(dot)vingarzan(at)fokus(dot)fraunhofer(dot)de and the
 * Fruanhofer Institute. It was and still is maintained in a separate
 * branch of the original SER. We are therefore migrating it to
 * Kamailio/SR and look forward to maintaining it from here on out.
 * 2011/2012 Smile Communications, Pty. Ltd.
 * ported/maintained/improved by
 * Jason Penton (jason(dot)penton(at)smilecoms.com and
 * Richard Good (richard(dot)good(at)smilecoms.com) as part of an
 * effort to add full IMS support to Kamailio/SR using a new and
 * improved architecture
 *
 * NB: Alot of this code was originally part of OpenIMSCore,
 * FhG Fokus.
 * Copyright (C) 2004-2006 FhG Fokus
 * Thanks for great work! This is an effort to
 * break apart the various CSCF functions into logically separate
 * components. We hope this will drive wider use. We also feel
 * that in this way the architecture is more complete and thereby easier
 * to manage in the Kamailio/SR environment
 *
 * This file is part of Kamailio, a free SIP server.
 *
 * Kamailio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version
 *
 * Kamailio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "../../core/parser/msg_parser.h"
#include "../../core/socket_info.h"
#include "../../lib/ims/ims_getters.h"
#include "../../modules/tm/tm_load.h"
#include "../ims_usrloc_pcscf/usrloc.h"

#include "ipsec.h"
#include "spi_gen.h"
#include "port_gen.h"
#include "cmd.h"
#include "sec_agree.h"

#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>


extern str ipsec_listen_addr;
extern str ipsec_listen_addr6;
extern int ipsec_server_port;
extern int ipsec_client_port;

extern int spi_id_start;

extern unsigned int init_flag;

// check http://www.asipto.com/pub/kamailio-devel-guide/#c16return_values
const int IPSEC_CMD_FAIL = -1;
const int IPSEC_CMD_SUCCESS = 1;

extern usrloc_api_t ul;
extern struct tm_binds tmb;

static int pv_t_copy_msg(struct sip_msg *src, struct sip_msg *dst)
{
        dst->id = src->id;
        dst->rcv = src->rcv;
        dst->set_global_address = src->set_global_address;
        dst->set_global_port = src->set_global_port;
        dst->flags = src->flags;
        dst->fwd_send_flags = src->fwd_send_flags;
        dst->rpl_send_flags = src->rpl_send_flags;
        dst->force_send_socket = src->force_send_socket;

        if (parse_msg(dst->buf, dst->len, dst) != 0) {
                LM_ERR("parse msg failed\n");
                return -1;
        }
        return 0;
}

int bind_ipsec_pcscf(ipsec_pcscf_api_t* api) {
	if(!api){
		LM_ERR("invalid parameter value\n");
		return -1;
	}
	if(init_flag == 0){
		LM_ERR("configuration error - trying to bind to ipsec pscscf module before being initialized\n");
		return -1;
	}

	api->ipsec_on_expire	  = ipsec_on_expire;
	api->ipsec_reconfig		  = ipsec_reconfig;

	return 0;
}

static str get_www_auth_param(const char* param_name, str www_auth)
{
    str val = {0,0};
    int i = 0;
    int param_len = strlen(param_name);
    int start = 0;
    int end = 0;

    for(i = 0; i < www_auth.len; i++) {
        if (www_auth.s[i] == ' ') continue;

        if(strncasecmp(www_auth.s+i, param_name, param_len) == 0) {
            i += param_len;

            //find first double quote
            while(www_auth.s[i] != '"' && i < www_auth.len) i++;
            i++; //and skip it

            if (i == www_auth.len)
                return val; //error
            start = i;
            i++;

            //find second double quote
            while(www_auth.s[i] != '"' && i < www_auth.len) i++;
            if (i == www_auth.len)
                return val; //error
            end = i;
            i++;

            val.s = www_auth.s + start;
            val.len = end - start;
            break;
        }

        //parameter not relevant - fast forward
        do { i++; } while (www_auth.s[i] != ',' && i < www_auth.len);
    }

    return val;
}

static int fill_contact(struct pcontact_info* ci, struct sip_msg* m)
{
    contact_body_t* cb = NULL;
    struct via_body* vb = NULL;
    struct sip_msg* req = NULL;
    str aor, f_uri;
    int i = 0;

    if(!ci) {
        LM_ERR("called with null ptr\n");
        return -1;
    }

    memset(ci, 0, sizeof(struct pcontact_info));

    if(m->first_line.type == SIP_REQUEST) {
        struct sip_uri uri;
        memset(&uri, 0, sizeof(struct sip_uri));

        if(parse_uri(m->first_line.u.request.uri.s, m->first_line.u.request.uri.len, &uri)) {
            LM_ERR("Can't parse the request URI from first line\n");
            return -1;
        }

        // populate host,port, aor in CI
        ci->via_host = uri.host;
        ci->via_port = uri.port_no ? uri.port_no : 5060;
        ci->via_prot = 0;

        aor.s = pkg_malloc(m->first_line.u.request.uri.len);
        if (aor.s == NULL) {
            LM_ERR("memory allocation failure\n");
            return -1;
        }
        memcpy(aor.s, m->first_line.u.request.uri.s, m->first_line.u.request.uri.len);
        aor.len = m->first_line.u.request.uri.len;

        //ci->aor = m->first_line.u.request.uri;
        ci->searchflag = SEARCH_NORMAL;

        req = m;
    }
    else if(m->first_line.type == SIP_REPLY) {
        struct cell *t = tmb.t_gett();
        if (!t || t == (void*) -1) {
            LM_ERR("Reply without transaction\n");
            return -1;
        }

        req = t->uas.request;

        /* Do not use t->uas.request for getting contacts - it has garbage */
        struct sip_msg req_msg;
        memset(&req_msg, 0, sizeof(struct sip_msg));
        req_msg.buf =
            (char*) pkg_malloc((t->uas.request->len + 1) * sizeof(char));
        memcpy(req_msg.buf, t->uas.request->buf, t->uas.request->len);
        req_msg.buf[t->uas.request->len] = '\0';
        req_msg.len = t->uas.request->len;
        if (pv_t_copy_msg(t->uas.request, &req_msg) != 0) {
            pkg_free(req_msg.buf);
            req_msg.buf = NULL;
            return -1;
        }
        if (req_msg.contact && ((contact_body_t *) req_msg.contact->parsed)) {
            free_contact((contact_body_t **) &req_msg.contact->parsed);
        }

        cb = cscf_parse_contacts(&req_msg);
        if (!cb || (!cb->contacts)) {
            LM_ERR("Reply No contact headers. Will try with request's From URI\n");
            if (!cscf_get_from_uri(&req_msg, &f_uri)) {
                LM_INFO("From URI in request <%.*s>\n", f_uri.len, f_uri.s);
                pkg_free(req_msg.buf);
                return -1;
            }
        }

        vb = cscf_get_ue_via(m);
        if (!vb) {
            LM_ERR("Reply No via body headers\n");
            pkg_free(req_msg.buf);
            return -1;
        }

        // populate CI with bare minimum
        ci->via_host = vb->host;
        ci->via_port = vb->port;
        ci->via_prot = vb->proto;

        aor.s = pkg_malloc((cb && cb->contacts) ? cb->contacts->uri.len : f_uri.len);
        if (aor.s == NULL) {
            LM_ERR("memory allocation failure\n");
            pkg_free(req_msg.buf);
            return -1;
        }
        memcpy(aor.s, (cb && cb->contacts) ? cb->contacts->uri.s : f_uri.s, (cb && cb->contacts) ? cb->contacts->uri.len : f_uri.len);
        aor.len = (cb && cb->contacts) ? cb->contacts->uri.len : f_uri.len;

        //ci->aor = cb->contacts->uri;
        ci->searchflag = SEARCH_RECEIVED;
        pkg_free(req_msg.buf);
    }
    else {
        LM_ERR("Unknown first line type: %d\n", m->first_line.type);
        return -1;
    }

    for (i = 4; i < aor.len; i++)
        if (aor.s[i] == ';') {
            aor.len = i;
            break;
        }

    LM_DBG("AOR <%.*s>\n", aor.len, aor.s);
    ci->aor = aor;

    char* srcip = NULL;
    if((srcip = pkg_malloc(50)) == NULL) {
        LM_ERR("Error allocating memory for source IP address\n");
        return -1;
    }

    ci->received_host.len = ip_addr2sbuf(&req->rcv.src_ip, srcip, 50);
    ci->received_host.s = srcip;
    ci->received_port = req->rcv.src_port;
    ci->received_proto = req->rcv.proto;

    LM_DBG("SIP %s fill contact with AOR [%.*s], VIA [%d://%.*s:%d], received_host [%d://%.*s:%d]\n",
            m->first_line.type == SIP_REQUEST ? "REQUEST" : "REPLY",
            ci->aor.len, ci->aor.s, ci->via_prot, ci->via_host.len, ci->via_host.s, ci->via_port,
            ci->received_proto, ci->received_host.len, ci->received_host.s, ci->received_port);

    // Set to default, if not set:
    if (ci->received_port == 0)
        ci->received_port = 5060;


    return 0;
}

// Get CK and IK from WWW-Authenticate
static int get_ck_ik(const struct sip_msg* m, str* ck, str* ik)
{
    struct hdr_field *www_auth_hdr = NULL;
    str www_auth;
    memset(&www_auth, 0, sizeof(str));

    www_auth = cscf_get_authenticate((sip_msg_t*)m, &www_auth_hdr);

    *ck = get_www_auth_param("ck", www_auth);
    if (ck->len == 0) {
        LM_ERR("Error getting CK\n");
        return -1;
    }

    *ik = get_www_auth_param("ik", www_auth);
    if (ck->len == 0) {
        LM_ERR("Error getting IK\n");
        return -1;
    }

    return 0;
}

static int update_contact_ipsec_params(ipsec_t* s, const struct sip_msg* m, int acquire_new_proxy_ports)
{
    // Get CK and IK
    str ck, ik;
    if(get_ck_ik(m, &ck, &ik) != 0) {
        return -1;
    }

    // Save CK and IK in the contact
    s->ck.s = shm_malloc(ck.len);
    if(s->ck.s == NULL) {
        LM_ERR("Error allocating memory for CK\n");
        return -1;
    }
    memcpy(s->ck.s, ck.s, ck.len);
    s->ck.len = ck.len;

    s->ik.s = shm_malloc(ik.len);
    if(s->ik.s == NULL) {
        LM_ERR("Error allocating memory for IK\n");
        shm_free(s->ck.s);
        s->ck.s = NULL; s->ck.len = 0;
        s->ik.s = NULL; s->ik.len = 0;
        return -1;
    }
    memcpy(s->ik.s, ik.s, ik.len);
    s->ik.len = ik.len;

    // Generate SPI
    if((s->spi_pc = acquire_spi()) == 0) {
        LM_ERR("Error generating client SPI for IPSEC tunnel creation\n");
        shm_free(s->ck.s);
        s->ck.s = NULL; s->ck.len = 0;
        shm_free(s->ik.s);
        s->ik.s = NULL; s->ik.len = 0;
        return -1;
    }

    if((s->spi_ps = acquire_spi()) == 0) {
        LM_ERR("Error generating server SPI for IPSEC tunnel creation\n");
        shm_free(s->ck.s);
        s->ck.s = NULL; s->ck.len = 0;
        shm_free(s->ik.s);
        s->ik.s = NULL; s->ik.len = 0;

		release_spi(s->spi_pc);
        return -1;
    }

    if (acquire_new_proxy_ports) {
        if((s->port_pc = acquire_cport()) == 0){
            LM_ERR("No free client port for IPSEC tunnel creation\n");
            shm_free(s->ck.s);
            s->ck.s = NULL; s->ck.len = 0;
            shm_free(s->ik.s);
            s->ik.s = NULL; s->ik.len = 0;

            release_spi(s->spi_pc);
            release_spi(s->spi_ps);
            return -1;
        }

        if((s->port_ps = acquire_sport()) == 0){
            LM_ERR("No free server port for IPSEC tunnel creation\n");
            shm_free(s->ck.s);
            s->ck.s = NULL; s->ck.len = 0;
            shm_free(s->ik.s);
            s->ik.s = NULL; s->ik.len = 0;

            // release_cport(s->port_pc);

            release_spi(s->spi_pc);
            release_spi(s->spi_ps);
            return -1;
        }
    }

    return 0;
}

static int create_ipsec_tunnel(const struct ip_addr *remote_addr, ipsec_t* s)
{
    struct mnl_socket* sock = init_mnl_socket();
    if (sock == NULL) {
        return -1;
    }

    //Convert ipsec address from str to struct ip_addr
	ip_addr_t ipsec_addr;

    if(remote_addr->af == AF_INET){
        if(str2ipbuf(&ipsec_listen_addr, &ipsec_addr) < 0){
            LM_ERR("Unable to convert ipsec addr4 [%.*s]\n", ipsec_listen_addr.len, ipsec_listen_addr.s);
            close_mnl_socket(sock);
            return -1;
        }
    } else if(remote_addr->af == AF_INET6){
        if(str2ip6buf(&ipsec_listen_addr6, &ipsec_addr) < 0){
            LM_ERR("Unable to convert ipsec addr6 [%.*s]\n", ipsec_listen_addr6.len, ipsec_listen_addr6.s);
            close_mnl_socket(sock);
            return -1;
        }
    } else {
        LM_ERR("Unsupported AF %d\n", remote_addr->af);
        close_mnl_socket(sock);
        return -1;
    }

    //Convert to char* for logging
    char remote_addr_str[128];
    memset(remote_addr_str, 0, sizeof(remote_addr_str));
    if(inet_ntop(remote_addr->af, remote_addr->u.addr, remote_addr_str, sizeof(remote_addr_str)) == NULL) {
        LM_CRIT("Error converting remote IP address: %s\n", strerror(errno));
        close_mnl_socket(sock);
        return -1;
    }

    LM_DBG("Creating security associations: Local IP: %.*s port_pc: %d port_ps: %d; UE IP: %s; port_uc %d port_us %d; spi_pc %u, spi_ps %u, spi_uc %u, spi_us %u, alg %.*s, ealg %.*s\n",
            remote_addr->af == AF_INET ? ipsec_listen_addr.len : ipsec_listen_addr6.len,
            remote_addr->af == AF_INET ? ipsec_listen_addr.s : ipsec_listen_addr6.s,
            s->port_pc, s->port_ps, remote_addr_str, s->port_uc, s->port_us, s->spi_pc, s->spi_ps, s->spi_uc, s->spi_us, s->r_alg.len, s->r_alg.s, s->r_ealg.len, s->r_ealg.s);

    // SA1 UE client to P-CSCF server
    //               src adrr     dst addr     src port    dst port
    add_sa    (sock, remote_addr, &ipsec_addr, s->port_uc, s->port_ps, s->spi_ps, s->ck, s->ik, s->r_alg, s->r_ealg);
    add_policy(sock, remote_addr, &ipsec_addr, s->port_uc, s->port_ps, s->spi_ps, IPSEC_POLICY_DIRECTION_IN);

    // SA2 P-CSCF client to UE server
    //               src adrr     dst addr     src port           dst port
    add_sa    (sock, &ipsec_addr, remote_addr, s->port_pc, s->port_us, s->spi_us, s->ck, s->ik, s->r_alg, s->r_ealg);
    add_policy(sock, &ipsec_addr, remote_addr, s->port_pc, s->port_us, s->spi_us, IPSEC_POLICY_DIRECTION_OUT);

    // SA3 P-CSCF server to UE client
    //               src adrr     dst addr     src port           dst port
    add_sa    (sock, &ipsec_addr, remote_addr, s->port_ps, s->port_uc, s->spi_uc, s->ck, s->ik, s->r_alg, s->r_ealg);
    add_policy(sock, &ipsec_addr, remote_addr, s->port_ps, s->port_uc, s->spi_uc, IPSEC_POLICY_DIRECTION_OUT);

    // SA4 UE server to P-CSCF client
    //               src adrr     dst addr     src port    dst port
    add_sa    (sock, remote_addr, &ipsec_addr, s->port_us, s->port_pc, s->spi_pc, s->ck, s->ik, s->r_alg, s->r_ealg);
    add_policy(sock, remote_addr, &ipsec_addr, s->port_us, s->port_pc, s->spi_pc, IPSEC_POLICY_DIRECTION_IN);

    /* Fix for some broken In-Dialog routing */

    // SA5 UE client to P-CSCF client
    //               src adrr     dst addr     src port    dst port
    add_sa    (sock, remote_addr, &ipsec_addr, s->port_uc, s->port_pc, s->spi_ps, s->ck, s->ik, s->r_alg, s->r_ealg);
    add_policy(sock, remote_addr, &ipsec_addr, s->port_uc, s->port_pc, s->spi_ps, IPSEC_POLICY_DIRECTION_IN);

    // SA6 P-CSCF client to UE client
    //               src adrr     dst addr     src port    dst port
    add_sa    (sock, &ipsec_addr, remote_addr, s->port_pc, s->port_uc, s->spi_us, s->ck, s->ik, s->r_alg, s->r_ealg);
    add_policy(sock, &ipsec_addr, remote_addr, s->port_pc, s->port_uc, s->spi_us, IPSEC_POLICY_DIRECTION_OUT);

    // SA7 P-CSCF server to UE server
    //               src adrr     dst addr     src port    dst port
    add_sa    (sock, &ipsec_addr, remote_addr, s->port_ps, s->port_us, s->spi_uc, s->ck, s->ik, s->r_alg, s->r_ealg);
    add_policy(sock, &ipsec_addr, remote_addr, s->port_ps, s->port_us, s->spi_uc, IPSEC_POLICY_DIRECTION_OUT);

    // SA8 UE server to P-CSCF server
    //               src adrr     dst addr     src port    dst port
    add_sa    (sock, remote_addr, &ipsec_addr, s->port_us, s->port_ps, s->spi_pc, s->ck, s->ik, s->r_alg, s->r_ealg);
    add_policy(sock, remote_addr, &ipsec_addr, s->port_us, s->port_ps, s->spi_pc, IPSEC_POLICY_DIRECTION_IN);

    close_mnl_socket(sock);

    return 0;
}

static int destroy_ipsec_tunnel(str remote_addr, ipsec_t* s, unsigned short received_port, int release_proxy_ports)
{
    struct mnl_socket* sock = init_mnl_socket();
    if (sock == NULL) {
        return -1;
    }

    ip_addr_t   ip_addr;
    str         ipsec_addr;

    // convert 'remote_addr' ip string to ip_addr_t
    if(str2ipxbuf(&remote_addr, &ip_addr) < 0){
        LM_ERR("Unable to convert remote address [%.*s]\n", remote_addr.len, remote_addr.s);
        close_mnl_socket(sock);
        return -1;
    }

    if(ip_addr.af == AF_INET6){
        ipsec_addr = ipsec_listen_addr6;
    }else{
        ipsec_addr = ipsec_listen_addr;
    }

    LM_DBG("Destroying security associations: Local IP: %.*s client port: %d server port: %d; UE IP: %.*s; client port %d server port %d; spi_ps %u, spi_pc %u, spi_us %u, spi_uc %u\n",
            ipsec_addr.len, ipsec_addr.s, s->port_pc, s->port_ps,
            remote_addr.len, remote_addr.s, s->port_uc, s->port_us, s->spi_ps, s->spi_pc, s->spi_us, s->spi_uc);

    // SA1 UE client to P-CSCF server
    remove_sa    (sock, remote_addr, ipsec_addr, s->port_uc, s->port_ps, s->spi_ps, ip_addr.af);
    remove_policy(sock, remote_addr, ipsec_addr, s->port_uc, s->port_ps, s->spi_ps, ip_addr.af, IPSEC_POLICY_DIRECTION_IN);

    // SA2 P-CSCF client to UE server
    remove_sa    (sock, ipsec_addr, remote_addr, s->port_pc, s->port_us, s->spi_us, ip_addr.af);
    remove_policy(sock, ipsec_addr, remote_addr, s->port_pc, s->port_us, s->spi_us, ip_addr.af, IPSEC_POLICY_DIRECTION_OUT);

    // SA3 P-CSCF server to UE client
    remove_sa    (sock, ipsec_addr, remote_addr, s->port_ps, s->port_uc, s->spi_uc, ip_addr.af);
    remove_policy(sock, ipsec_addr, remote_addr, s->port_ps, s->port_uc, s->spi_uc, ip_addr.af, IPSEC_POLICY_DIRECTION_OUT);

    // SA4 UE server to P-CSCF client
    remove_sa    (sock, remote_addr, ipsec_addr, s->port_us, s->port_pc, s->spi_pc, ip_addr.af);
    remove_policy(sock, remote_addr, ipsec_addr, s->port_us, s->port_pc, s->spi_pc, ip_addr.af, IPSEC_POLICY_DIRECTION_IN);

    /* Fix for some broken In-Dialog routing */

    // SA5 UE client to P-CSCF client
    remove_sa    (sock, remote_addr, ipsec_addr, s->port_uc, s->port_pc, s->spi_ps, ip_addr.af);
    remove_policy(sock, remote_addr, ipsec_addr, s->port_uc, s->port_pc, s->spi_ps, ip_addr.af, IPSEC_POLICY_DIRECTION_IN);

    // SA6 P-CSCF client to UE client
    remove_sa    (sock, ipsec_addr, remote_addr, s->port_pc, s->port_uc, s->spi_us, ip_addr.af);
    remove_policy(sock, ipsec_addr, remote_addr, s->port_pc, s->port_uc, s->spi_us, ip_addr.af, IPSEC_POLICY_DIRECTION_OUT);

    // SA7 P-CSCF server to UE server
    remove_sa    (sock, ipsec_addr, remote_addr, s->port_ps, s->port_us, s->spi_uc, ip_addr.af);
    remove_policy(sock, ipsec_addr, remote_addr, s->port_ps, s->port_us, s->spi_uc, ip_addr.af, IPSEC_POLICY_DIRECTION_OUT);

    // SA8 UE server to P-CSCF server
    remove_sa    (sock, remote_addr, ipsec_addr, s->port_us, s->port_ps, s->spi_pc, ip_addr.af);
    remove_policy(sock, remote_addr, ipsec_addr, s->port_us, s->port_ps, s->spi_pc, ip_addr.af, IPSEC_POLICY_DIRECTION_IN);

    // Release SPIs
    release_spi(s->spi_pc);
    release_spi(s->spi_ps);

    if (release_proxy_ports) {
        // Release the client and the server ports
        // Do not release proxy IPSec ports at all just remove SA and Policies
        // release_cport(s->port_pc);
        // release_sport(s->port_ps);
    }

    close_mnl_socket(sock);
    return 0;
}

void ipsec_on_expire(struct pcontact *c, int type, void *param)
{
    if(type != PCSCF_CONTACT_EXPIRE && type != PCSCF_CONTACT_DELETE) {
        LM_ERR("Unexpected event type %d\n", type);
        return;
    }


    if(c->security_temp == NULL) {
        LM_ERR("No security parameters found in contact\n");
        return;
    }

    //get security parameters
    if(c->security_temp->type != SECURITY_IPSEC ) {
        LM_ERR("Unsupported security type: %d\n", c->security_temp->type);
        return;
    }

    destroy_ipsec_tunnel(c->received_host, c->security_temp->data.ipsec, c->contact_port, 1);
}

int add_supported_secagree_header(struct sip_msg* m)
{
    // Add sec-agree header in the reply
    const char* supported_sec_agree = "Supported: sec-agree\r\n";
    const int supported_sec_agree_len = 22;

    str* supported = NULL;
    if((supported = pkg_malloc(sizeof(str))) == NULL) {
        LM_ERR("Error allocating pkg memory for supported header\n");
        return -1;
    }

    if((supported->s = pkg_malloc(supported_sec_agree_len)) == NULL) {
        LM_ERR("Error allcationg pkg memory for supported header str\n");
        pkg_free(supported);
        return -1;
    }
    memcpy(supported->s, supported_sec_agree, supported_sec_agree_len);
    supported->len = supported_sec_agree_len;

    if(cscf_add_header(m, supported, HDR_SUPPORTED_T) != 1) {
		pkg_free(supported->s);
		pkg_free(supported);
        LM_ERR("Error adding security header to reply!\n");
        return -1;
    }
    pkg_free(supported);

    return 0;
}

int add_security_server_header(struct sip_msg* m, ipsec_t* s)
{
    // allocate memory for the header itself
    str* sec_header = NULL;
    if((sec_header = pkg_malloc(sizeof(str))) == NULL) {
        LM_ERR("Error allocating pkg memory for security header\n");
        return -1;
    }
    memset(sec_header, 0, sizeof(str));

    // create a temporary buffer and set the value in it
    char sec_hdr_buf[1024];
    memset(sec_hdr_buf, 0, sizeof(sec_hdr_buf));
    sec_header->len = snprintf(sec_hdr_buf, sizeof(sec_hdr_buf) - 1,
                                "Security-Server: ipsec-3gpp;q=0.1;prot=esp;mod=trans;spi-c=%d;spi-s=%d;port-c=%d;port-s=%d;alg=%.*s;ealg=%.*s\r\n",
                                s->spi_pc, s->spi_ps, s->port_pc, s->port_ps,
                                s->r_alg.len, s->r_alg.s,
                                s->r_ealg.len, s->r_ealg.s
                              );

    // copy to the header and add
    if((sec_header->s = pkg_malloc(sec_header->len)) == NULL) {
        LM_ERR("Error allocating pkg memory for security header payload\n");
        pkg_free(sec_header);
        return -1;
    }
    memcpy(sec_header->s, sec_hdr_buf, sec_header->len);

    // add security-server header in reply
    if(cscf_add_header(m, sec_header, HDR_OTHER_T) != 1) {
        LM_ERR("Error adding security header to reply!\n");
        pkg_free(sec_header->s);
        pkg_free(sec_header);
        return -1;
    }

    pkg_free(sec_header);

    return 0;
}

int ipsec_create(struct sip_msg* m, udomain_t* d)
{
    pcontact_t* pcontact = NULL;
    struct pcontact_info ci;
    int ret = IPSEC_CMD_FAIL;   // FAIL by default

    // Find the contact
    if(fill_contact(&ci, m) != 0) {
        LM_ERR("Error filling in contact data\n");
        return ret;
    }

    ul.lock_udomain(d, &ci.via_host, ci.via_port, ci.via_prot);

    if (ul.get_pcontact(d, &ci, &pcontact) != 0) {
        LM_ERR("Contact doesn't exist\n");
        goto cleanup;
    }

    // Get security parameters
    if(pcontact->security_temp == NULL) {
        LM_ERR("No security parameters found in contact\n");
        goto cleanup;
    }

    if(pcontact->security_temp->type != SECURITY_IPSEC ) {
        LM_ERR("Unsupported security type: %d\n", pcontact->security_temp->type);
        goto cleanup;
    }

    // Get request from reply
    struct cell *t = tmb.t_gett();
    if (!t || t == (void*) -1) {
        LM_ERR("Reply without transaction\n");
        goto cleanup;
    }

    struct sip_msg* req = t->uas.request;

    // Update contacts only for initial registration, for re-registration the existing contacts shouldn't be updated.
    if((ci.via_port == SIP_PORT) ||
        (pcontact->security_temp->data.ipsec->port_ps == 0  &&
        pcontact->security_temp->data.ipsec->port_pc == 0)){
        LM_DBG("Registration for contact with AOR [%.*s], VIA [%d://%.*s:%d], received_host [%d://%.*s:%d]\n",
                ci.aor.len, ci.aor.s, ci.via_prot, ci.via_host.len, ci.via_host.s, ci.via_port,
                ci.received_proto, ci.received_host.len, ci.received_host.s, ci.received_port);

        security_t* sec_params = NULL;

        ipsec_t* s = NULL;

        // Parse security parameters from the REGISTER request and get some data for the tunnels
        if((sec_params = cscf_get_security(req)) == NULL) {
            LM_CRIT("No security parameters in REGISTER request\n");
            goto cleanup;
        }

        if (sec_params->data.ipsec->port_uc != pcontact->security_temp->data.ipsec->port_uc ||
            sec_params->data.ipsec->port_us != pcontact->security_temp->data.ipsec->port_us ||
            sec_params->data.ipsec->spi_uc != pcontact->security_temp->data.ipsec->spi_uc ||
            sec_params->data.ipsec->spi_us != pcontact->security_temp->data.ipsec->spi_us) {

            // Backup the Proxy Server and Client port - we re-use them
            unsigned short port_ps = pcontact->security_temp->data.ipsec->port_ps;
            unsigned short port_pc = pcontact->security_temp->data.ipsec->port_pc;

            // Destroy privously existing IPSec tunnels but dont release proxy ports
            destroy_ipsec_tunnel(ci.received_host,
                pcontact->security_temp->data.ipsec, pcontact->contact_port, 0);

            if(pcontact->security_temp->sec_header.s)
                shm_free(pcontact->security_temp->sec_header.s);

            if(pcontact->security_temp->data.ipsec){
                if(pcontact->security_temp->data.ipsec->ealg.s)
                    shm_free(pcontact->security_temp->data.ipsec->ealg.s);
                if(pcontact->security_temp->data.ipsec->r_ealg.s)
                    shm_free(pcontact->security_temp->data.ipsec->r_ealg.s);
                if(pcontact->security_temp->data.ipsec->ck.s)
                    shm_free(pcontact->security_temp->data.ipsec->ck.s);
                if(pcontact->security_temp->data.ipsec->alg.s)
                    shm_free(pcontact->security_temp->data.ipsec->alg.s);
                if(pcontact->security_temp->data.ipsec->r_alg.s)
                    shm_free(pcontact->security_temp->data.ipsec->r_alg.s);
                if(pcontact->security_temp->data.ipsec->ik.s)
                    shm_free(pcontact->security_temp->data.ipsec->ik.s);
                if(pcontact->security_temp->data.ipsec->prot.s)
                    shm_free(pcontact->security_temp->data.ipsec->prot.s);
                if(pcontact->security_temp->data.ipsec->mod.s)
                    shm_free(pcontact->security_temp->data.ipsec->mod.s);

                shm_free(pcontact->security_temp->data.ipsec);
            }
            shm_free(pcontact->security_temp);

            if(ul.update_temp_security(d, sec_params->type, sec_params, pcontact) != 0){
                LM_ERR("Error updating temp security\n");
                goto cleanup;
            }

            s = pcontact->security_temp->data.ipsec;
            // Restore the backed up Proxy Server and Client port
            s->port_ps = port_ps;
            s->port_pc = port_pc;

            if(update_contact_ipsec_params(s, m, 0) != 0) {
                goto cleanup;
            }
        } else {

            s = pcontact->security_temp->data.ipsec;

            if(update_contact_ipsec_params(s, m, 1) != 0) {
                goto cleanup;
            }
        }

        if(create_ipsec_tunnel(&req->rcv.src_ip, s) != 0){
            goto cleanup;
        }

        if (ul.update_pcontact(d, &ci, pcontact) != 0){
            LM_ERR("Error updating contact\n");
            goto cleanup;
        }

        // Update temp security parameters
        if(ul.update_temp_security(d, pcontact->security_temp->type, pcontact->security_temp, pcontact) != 0){
            LM_ERR("Error updating temp security\n");
        }

        if(add_supported_secagree_header(m) != 0) {
            goto cleanup;
        }

        if(add_security_server_header(m, s) != 0) {
            goto cleanup;
        }

        if(ul.register_ulcb(pcontact, PCSCF_CONTACT_EXPIRE|PCSCF_CONTACT_DELETE, ipsec_on_expire, (void*)&pcontact->received_port) != 1) {
            LM_ERR("Error subscribing for contact\n");
            goto cleanup;
        }
    }else{
        LM_DBG("RE-Registration for contact with AOR [%.*s], VIA [%d://%.*s:%d], received_host [%d://%.*s:%d]\n",
                ci.aor.len, ci.aor.s, ci.via_prot, ci.via_host.len, ci.via_host.s, ci.via_port,
                ci.received_proto, ci.received_host.len, ci.received_host.s, ci.received_port);
        
        security_t* req_sec_params = NULL;

        // Parse security parameters from the REGISTER request and get some data for the new tunnels
        if((req_sec_params = cscf_get_security(req)) == NULL) {
            LM_CRIT("No security parameters in REGISTER request\n");
            goto cleanup;
        }

        if(update_contact_ipsec_params(req_sec_params->data.ipsec, m, 0) != 0) {
            goto cleanup;
        }

        // Restore Proxy Server and Client port
        req_sec_params->data.ipsec->port_ps = pcontact->security_temp->data.ipsec->port_ps;
        req_sec_params->data.ipsec->port_pc = pcontact->security_temp->data.ipsec->port_pc;

        if(create_ipsec_tunnel(&req->rcv.src_ip, req_sec_params->data.ipsec) != 0){
            goto cleanup;
        }

        if(add_supported_secagree_header(m) != 0) {
            goto cleanup;
        }

        if(add_security_server_header(m, req_sec_params->data.ipsec) != 0) {
            goto cleanup;
        }
    }

    ret = IPSEC_CMD_SUCCESS;    // all good, set ret to SUCCESS, and exit

cleanup:
    // Do not free str* sec_header! It will be freed in data_lump.c -> free_lump()
    ul.unlock_udomain(d, &ci.via_host, ci.via_port, ci.via_prot);
    pkg_free(ci.received_host.s);
    return ret;
}


int ipsec_forward(struct sip_msg* m, udomain_t* d)
{
    struct pcontact_info ci;
    pcontact_t* pcontact = NULL;
    int ret = IPSEC_CMD_FAIL; // FAIL by default
    unsigned char dst_proto = PROTO_UDP;
    unsigned short dst_port = 0;
    unsigned short src_port = 0;
    ip_addr_t via_host;
    struct via_body *vb;

    struct sip_msg* req = NULL;
    if(m->first_line.type == SIP_REPLY) {
        // Get request from reply
        tm_cell_t *t = 0;
        t = tmb.t_gett();
        if (t == NULL || t == T_UNDEFINED) {
            LM_ERR("Error getting transaction\n");
            return ret;
        }

        req = t->uas.request;
    } else {
        req = m;
    }

    //
    // Find the contact
    //
    if(fill_contact(&ci, m) != 0) {
        LM_ERR("Error filling in contact data\n");
        return ret;
    }

    ul.lock_udomain(d, &ci.via_host, ci.via_port, ci.via_prot);

    if (ul.get_pcontact(d, &ci, &pcontact) != 0) {
        LM_ERR("Contact doesn't exist\n");
        goto cleanup;
    }

    if(str2ipxbuf(&ci.via_host, &via_host) < 0){
        LM_ERR("Error getting AF from ci.via_host\n");
        goto cleanup;
    }

    if(pcontact->security_temp == NULL) {
        LM_ERR("No security parameters found in contact\n");
        goto cleanup;
    }

    //get security parameters
    if(pcontact->security_temp->type != SECURITY_IPSEC ) {
        LM_ERR("Unsupported security type: %d\n", pcontact->security_temp->type);
        goto cleanup;
    }

    ipsec_t* s = pcontact->security_temp->data.ipsec;

    // Update the destination
    //
    //       from sec-agree
    //            v
    // sip:host:port
    //       ^
    //    from URI
    //int uri_len = 4 /* strlen("sip:") */ + ci.via_host.len + 5 /* max len of port number */ ;

    vb = cscf_get_last_via(m);

    char buf[1024];
    if(m->first_line.type == SIP_REPLY){
        dst_proto = vb ? vb->proto : req->rcv.proto;

        // As per ETSI TS 133 203 V11.2.0, 7.1 Security association parameters
        // https://tools.ietf.org/html/rfc3261#section-18

        // for Reply and TCP sends from P-CSCF server port, for Reply and UDP sends from P-CSCF client port
        // src_port = dst_proto == PROTO_TCP ? s->port_ps : s->port_pc;

        // for Reply and TCP sends to UE client port, for Reply and UDP sends to UE server port
        // dst_port = dst_proto == PROTO_TCP ? s->port_uc : s->port_us;

        // From P-CSCF client port
        src_port = s->port_pc;

        if (vb && ((vb->port == s->port_uc) || (vb->port == s->port_us))) {
            dst_port = vb->port;
        } else {
            dst_port = s->port_us;
        }

    }else{
        if (req->first_line.u.request.method_value == METHOD_REGISTER) {
            // for Request get the dest proto from the saved contact
            dst_proto = pcontact->received_proto;
        } else {
			if (strstr(m->dst_uri.s, ";transport=tcp") != NULL) {
				dst_proto = PROTO_TCP;
			} else if (strstr(m->dst_uri.s, ";transport=tls") != NULL) {
				dst_proto = PROTO_TLS;
			} else {
				dst_proto = m->rcv.proto;
			}
        }

        // for Request sends from P-CSCF client port
        src_port = s->port_pc;
        
        // for Request sends to UE server port
        dst_port = s->port_us;
    }

    // Try for send socket
    struct socket_info * client_sock = grep_sock_info(via_host.af == AF_INET ? &ipsec_listen_addr : &ipsec_listen_addr6, src_port, dst_proto);
    if(!client_sock && dst_proto == PROTO_UDP) {
        LM_ERR("UDP socket not found for IPSec forward, trying for TCP\n");
        dst_proto = PROTO_TCP;
    }

    int buf_len = 0;
    if (dst_proto == PROTO_TCP) {
        buf_len = snprintf(buf, sizeof(buf) - 1, "sip:%.*s:%d;transport=tcp", ci.via_host.len, ci.via_host.s, dst_port);
    } else if (dst_proto == PROTO_TLS) {
        buf_len = snprintf(buf, sizeof(buf) - 1, "sip:%.*s:%d;transport=tls", ci.via_host.len, ci.via_host.s, dst_port);
    } else {
        buf_len = snprintf(buf, sizeof(buf) - 1, "sip:%.*s:%d", ci.via_host.len, ci.via_host.s, dst_port);
    }

	if(m->dst_uri.s) {
		pkg_free(m->dst_uri.s);
		m->dst_uri.s = NULL;
		m->dst_uri.len = 0;
	}

    if((m->dst_uri.s = pkg_malloc(buf_len + 1)) == NULL) {
        LM_ERR("Error allocating memory for dst_uri\n");
        goto cleanup;
    }

    memcpy(m->dst_uri.s, buf, buf_len);
    m->dst_uri.len = buf_len;
    m->dst_uri.s[m->dst_uri.len] = '\0';

    // Set send socket
    client_sock = grep_sock_info(via_host.af == AF_INET ? &ipsec_listen_addr : &ipsec_listen_addr6, src_port, dst_proto);
    if(!client_sock) {
        LM_ERR("Error calling grep_sock_info() for ipsec client port\n");
        goto cleanup;
    }
    m->force_send_socket = client_sock;

   // Set destination info
    struct dest_info dst_info;
    dst_info.send_sock = client_sock;
	set_force_socket(m, client_sock);
#ifdef USE_DNS_FAILOVER
    if (!uri2dst(NULL, &dst_info, m, &m->dst_uri, dst_proto)) {
#else
    if (!uri2dst(&dst_info, m, &m->dst_uri, dst_proto)) {
#endif
        LM_ERR("Error converting dst_uri (%.*s) to struct dst_info\n", m->dst_uri.len, m->dst_uri.s);
        goto cleanup;
    }

    // Update dst_info in message
    if(m->first_line.type == SIP_REPLY) {
        tm_cell_t *t = 0;
        t = tmb.t_gett();
        if (t == NULL || t == T_UNDEFINED) {
            LM_ERR("Error getting transaction\n");
            goto cleanup;
        }
        t->uas.response.dst = dst_info;
    }

	LM_INFO("Destination changed to [%d://%.*s], from [%d:%d]\n", dst_info.proto, m->dst_uri.len, m->dst_uri.s,
			dst_info.send_sock->proto, dst_info.send_sock->port_no);

    ret = IPSEC_CMD_SUCCESS; // all good, return SUCCESS

    if (req->first_line.u.request.method_value == METHOD_REGISTER) {
        if(add_supported_secagree_header(m) != 0) {
            goto cleanup;
        }

        if(add_security_server_header(m, s) != 0) {
            goto cleanup;
        }
    }

    ret = IPSEC_CMD_SUCCESS;    // all good, set ret to SUCCESS, and exit

cleanup:
    ul.unlock_udomain(d, &ci.via_host, ci.via_port, ci.via_prot);
    pkg_free(ci.received_host.s);
    return ret;
}


int ipsec_destroy(struct sip_msg* m, udomain_t* d)
{
    struct pcontact_info ci;
    pcontact_t* pcontact = NULL;
    int ret = IPSEC_CMD_FAIL; // FAIL by default

    //
    // Find the contact
    //
    if(fill_contact(&ci, m) != 0) {
        LM_ERR("Error filling in contact data\n");
        return ret;
    }

    ul.lock_udomain(d, &ci.via_host, ci.via_port, ci.via_prot);

    if (ul.get_pcontact(d, &ci, &pcontact) != 0) {
        LM_ERR("Contact doesn't exist\n");
        goto cleanup;
    }


    if(pcontact->security_temp == NULL) {
        LM_ERR("No security parameters found in contact\n");
        goto cleanup;
    }

    //get security parameters
    if(pcontact->security_temp->type != SECURITY_IPSEC ) {
        LM_ERR("Unsupported security type: %d\n", pcontact->security_temp->type);
        goto cleanup;
    }

    destroy_ipsec_tunnel(ci.received_host, pcontact->security_temp->data.ipsec, pcontact->contact_port, 1);

    ret = IPSEC_CMD_SUCCESS;    // all good, set ret to SUCCESS, and exit

cleanup:
    ul.unlock_udomain(d, &ci.via_host, ci.via_port, ci.via_prot);
    pkg_free(ci.received_host.s);
    return ret;
}

int ipsec_destroy_by_contact(udomain_t* _d, str * uri, str * received_host, int received_port) {

    pcontact_t* pcontact = NULL;
    int ret = IPSEC_CMD_FAIL; // FAIL by default

    pcontact_info_t search_ci;
    memset(&search_ci, 0, sizeof(struct pcontact_info));

    sip_uri_t contact_uri;
    if (parse_uri(uri->s, uri->len, &contact_uri) != 0) {
        LM_WARN("Failed to parse aor [%.*s]\n", uri->len, uri->s);
        return ret;
    }

    search_ci.received_host.s = received_host->s;
    search_ci.received_host.len = received_host->len;
    search_ci.received_port = received_port;
    search_ci.received_proto = contact_uri.proto? contact_uri.proto : PROTO_UDP;
    search_ci.searchflag = SEARCH_RECEIVED;
    search_ci.via_host.s = received_host->s;
    search_ci.via_host.len = received_host->len;
    search_ci.via_port = received_port;
    search_ci.via_prot = search_ci.received_proto;
    search_ci.aor.s = uri->s;
    search_ci.aor.len = uri->len;
    search_ci.reg_state = PCONTACT_ANY;

    if (ul.get_pcontact(_d, &search_ci, &pcontact) != 0) {
        LM_ERR("Contact doesn't exist\n");
        return ret;
    }

    /* Lock this record while working with the data: */
    ul.lock_udomain(_d, &pcontact->via_host, pcontact->via_port, pcontact->via_proto);

    if(pcontact->security_temp == NULL) {
        LM_ERR("No security parameters found in contact\n");
        goto cleanup;
    }

    //get security parameters
    if(pcontact->security_temp->type != SECURITY_IPSEC ) {
        LM_ERR("Unsupported security type: %d\n", pcontact->security_temp->type);
        goto cleanup;
    }

    destroy_ipsec_tunnel(search_ci.received_host, pcontact->security_temp->data.ipsec, pcontact->contact_port, 1);

    ret = IPSEC_CMD_SUCCESS;    // all good, set ret to SUCCESS, and exit

cleanup:
    /* Unlock domain */
    ul.unlock_udomain(_d, &pcontact->via_host, pcontact->via_port, pcontact->via_proto);
    return ret;
}

int ipsec_reconfig()
{
	if(ul.get_number_of_contacts() != 0){
		return 0;
	}

	clean_spi_list();
	// clean_port_lists();

	LM_DBG("Clean all ipsec tunnels\n");

	return ipsec_cleanall();
}

int ipsec_cleanall()
{
    struct mnl_socket* nlsock = init_mnl_socket();
    if(!nlsock) {
        return -1;
    }

    if(clean_sa(nlsock) != 0) {
        LM_WARN("Error cleaning IPSec Security associations during startup.\n");
    }

    if(clean_policy(nlsock) != 0) {
        LM_WARN("Error cleaning IPSec Policies during startup.\n");
    }

    close_mnl_socket(nlsock);

    return 0;
}
