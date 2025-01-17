$ORIGIN msg.pc.t-mobile.com.
$TTL 1W
@                       1D IN SOA       localhost. root.localhost. (
                                        1		; serial
                                        3H              ; refresh
                                        15M             ; retry
                                        1W              ; expiry
                                        1D )            ; minimum

                        1D IN SRV 0 0 4060 icscf
                        1D IN NS        ns
                        1D IN A         127.0.0.2
ns                      1D IN A         127.0.0.1

pcscf                   1D IN A         127.0.0.1
_sip._udp.pcscf         1D IN SRV 0 0 5060 pcscf
_sip._tcp.pcscf         1D IN SRV 0 0 5060 pcscf

icscf                   1D IN A         127.0.0.2
_sip._udp               1D IN SRV 0 0 4060 icscf
_sip._tcp               1D IN SRV 0 0 4060 icscf
_sip._udp.ims           1D IN SRV 0 0 4060 icscf
_sip._tcp.ims           1D IN SRV 0 0 4060 icscf

scscf                   1D IN A         127.0.0.3
_sip._udp.scscf         1D IN SRV 0 0 6060 scscf
_sip._tcp.scscf         1D IN SRV 0 0 6060 scscf

hss                     3600 IN CNAME        hss.ims.mnc210.mcc310.3gppnetwork.org.

