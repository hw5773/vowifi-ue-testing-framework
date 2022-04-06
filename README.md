# How to setup the VoWiFi testbed

## Overview
  - Operating System: Ubuntu 20.04 (bento) (I think Ubuntu 18.04 will work as well.)
  - Registration Procedure
    - UE attaches to WiFi AP and is assigned the IPv4 address from the DHCP server on the WiFi AP
    - UE sends the DNS query to the DNS server to acquire the ePDG's address
    - UE executes the IKEv2 protocol with the ePDG
    - UE runs EAP-AKA protocol over IKEv2
    - The IKE channel is established between UE and ePDG once the UE is authenticated
    - ePDG provides the IPv6 address of P-CSCF to UE in the last IKE handshake message
    - UE executes the SIP registration protocol with P-CSCF (in detail, UE exchanges the messages with P-CSCF, but communicates with S-CSCF. P-CSCF simply forwards the messages.)
    - P-CSCF forwards the registration message to I-CSCF.
    - Again, I-CSCF forwards the message to S-CSCF. (In our scenario, there is no special task for I-CSCF. I-CSCF is responsible for managing roaming.)
    - S-CSCF refers to HSS to authenticate the UE
    - Finally, UE establishes the IPsec channel with P-CSCF and is registered to the IMS server (that is, S-CSCF for the UE is alive and provides the service) 

## VoWiFi settings and source codes of the ePDG/IMS server
  - git clone https://github.com/hw5773/vowifi-core-network.git
  - Let's say the absolute path of the vowifi-core-network directory VOWIFI_ROOT

## VoWiFi network topology
  -      UE -------- [WiFi AP/DNS] ------ [ePDG] -- [IMS]
  -  (Smartphone) ----- (laptop) ------- (desktop) - (VM)

## VoWiFi SIM card setting
  - H/W: sysmoISIM-SJA2 (ISIM card), SIM card reader/writer
  - S/W: pysim
 
  - Clone the pysim tool to set up the VoWiFi sim card
    - git clone https://github.com/osmocom/pysim.git
  
  - SIM writing and setting
    - sudo python3 pySim-prog.py -p 0 -a <ADM value> -n "T-Mobile" -x 310 -y 260 --imsi=<IMSI> --msisdn=<Telephone Number> --ims-hdomain=msg.pc.t-mobile.com --impi=<IMSI>@msg.pc.t-mobile.com --impu=sip:<IMSI>@ims.mnc260.mcc310.3gppnetwork.org --iccid=8901260245784161215 --smsp 542d4d6f62696c65fffffffffffffffff1ffffffffffffffffffffffff07912160130300f4ffffffff0000ff --smsc 12063130004 --opmode 80 --acc 0010 -k <key> -o <OPc>
    - sudo python3 pySim-shell.py -p 0 -a <ADM value> --script VOWIFI_ROOT/scripts/vowifi-setting.script
  
  ### Commands with an example value (you can just copy & paste the following command for the two SIM cards and only revise the ADM values)
    - SIM 1
      - sudo python3 pySim-prog.py -p 0 -a <ADM value> -n "T-Mobile" -x 310 -y 260 --imsi=310260123456781 --msisdn=17657751234 --ims-hdomain=msg.pc.t-mobile.com --impi=310260123456781@msg.pc.t-mobile.com --impu=sip:310260123456781@ims.mnc260.mcc310.3gppnetwork.org --iccid=8901260245784161215 --smsp 542d4d6f62696c65fffffffffffffffff1ffffffffffffffffffffffff07912160130300f4ffffffff0000ff --smsc 12063130004 --opmode 80 --acc 0010 -k 11111111111111111111111111111111 -o 99999999999999999999999999999999
      - sudo python3 pySim-shell.py -p 0 -a <ADM value> --script VOWIFI_ROOT/scripts/vowifi-setting.script
  
    - SIM 2
      - sudo python3 pySim-prog.py -p 0 -a <ADM value> -n "T-Mobile" -x 310 -y 260 --imsi=310260123456782 --msisdn=17657751235 --ims-hdomain=msg.pc.t-mobile.com --impi=310260123456782@msg.pc.t-mobile.com --impu=sip:310260123456782@ims.mnc260.mcc310.3gppnetwork.org --iccid=8901260245784161215 --smsp 542d4d6f62696c65fffffffffffffffff1ffffffffffffffffffffffff07912160130300f4ffffffff0000ff --smsc 12063130004 --opmode 80 --acc 0010 -k 22222222222222222222222222222222 -o 99999999999999999999999999999999
      - sudo python3 pySim-shell.py -p 0 -a <ADM value> --script VOWIFI_ROOT/scripts/vowifi-setting.script
  
## WiFi AP
  - S/W: wifi-ap (or hostapd + dnsmasq)
  - sudo apt-get install snapd
  - snap install wifi-ap
  - sudo wifi-ap.config set wifi.interface=<wifi interface>
  - sudo wifi-ap.config set wifi.ssid=VoWiFi
  - sudo wifi-ap.config set wifi.security-passphrase=vowifiaccess
  - sudo wifi-ap.config set wifi.address=10.0.60.1
  - sudo wifi-ap.config set dhcp.range-start=10.0.60.2
  - sudo wifi-ap.config set dhcp.range-stop=10.0.60.254
  - sudo wifi-ap.config set disabled=false
  - sudo wifiap.status restart-ap
 
## ePDG installation and configuration
  - Installation
    - S/W: Strongswan
    - cd VOWIFI_ROOT/epdg
    - ./configure --prefix=/usr/local --enable-eap-aka --enable-eap-aka-3gpp --enable-eap-aka-3gpp2
    - (Please check if eap-aka, eap-aka-3gpp, and eap-aka-3gpp2 modules are all enabled.)
    - make && sudo make install 

  - Configuration - ipsec.conf
    - The important attributes in this configuration file are as follows: 
    - "leftsubnet": the network address of the VoWiFi network (fdad:dabb:ed::/64). The network bits should be 64 bits (it is required by UEs).
    - "rightsubnet": the network address of the IPv6 addresses to be assigned to UEs (2607:fc20:ba53:1538::/64). The network bits should be 64 bits (it is required by UEs).
    - "rightsourceip": the IPv6 pool for UEs (2607:fc20:ba53:1538:0:9:8a73:cc01-2607:fc20:ba53:1538:0:9:8a73:cc10)
 
  - Configuration - strongswan.conf
    - The values in "attr" are provided to UE for the networking information. T-mobile uses 16386 to indicate the IPv6 address of the IMS server (in detail, it is the address of P-CSCF).
 
  ### Settings with an example value (you can revise the addresses)
    - attr {
        16386 = fdad:dabb:ed::2
      }
 
  - Configuration - ipsec.secrets
    - The format (syntax) of the configuration file for the EAP secrets
    - <ID> : EAP <K> <OPc> (The key value and the OPc value are concatenated without any space in-between them)

  ### Settings with an example value
    - 310260123456781@nai.epc.mnc260.mcc310.3gppnetwork.org : EAP 0x1111111111111111111111111111111199999999999999999999999999999999
    - 310260123456782@nai.epc.mnc260.mcc310.3gppnetwork.org : EAP 0x2222222222222222222222222222222299999999999999999999999999999999
 
## IMS server installation and configuration

  - [Host] Install VirtualBox and Vagrant
    - sudo apt-get install virtualbox
    - sudo apt-get install vagrant
 
  - [Host] Create the bridge
    - sudo ip link add vowifi-gw type bridge
    - sudo ip link set vowifi-gw up
    - sudo ip address add fdad:dabb:ed::1 dev vowifi-gw
    - sudo route -A inet6 add fdad:dabb:ed::/64 dev vowifi-gw
 
  - [Host] Initiate the VM
    - cd VOWIFI_ROOT/vagrant
    - vagrant up
    - select vowifi-gw for the bridge
    - vagrant ssh (to get the shell of VM)
 
  - [Guest] Network setting in the VM
    - sudo route -A inet6 add default gw via fdad:dabb:ed::1 dev eth1 (the interface name might be different)

  - [Guest] Downlod the Setting files for the IMS Server
    - git clone https://github.com/hw5773/vowifi-core-network.git
 
  - [Guest] mysql Installation
    - sudo apt-get install mysql-client mysql-server
    - sudo mysql
    - alter user 'root'@'localhost' identified with mysql_native_password by '<password>';

  - [Guest] IMS Server Installation and Configuration
    - S/W: Kamailio
    - cd VOWIFI_ROOT/ims
    - sudo apt-get install automake autoconf build-essential bison flex libxml2-dev libcurl4-gnutls-dev libssl-dev libmysqlclient-dev libpcre3-dev g++ pkg-config libsctp-dev
    - make include_modules="ims_usrloc_pcscf ims_registrar_pcscf xmlrpc db_mysql cdp cdp_avp ims_icscf presence ims_usrloc_scscf ims_registrar_scscf ims_auth ims_isc ims_charging" cfg
    - make && sudo make install

  - [Guest] Kamailio mysql Configuration
    - sudo vi /usr/local/etc/kamailio/kamctlrc
    - uncomment DBENGINE=MYSQL
    - uncommant and revise SIP_DOMAIN=ims.mnc260.mcc310.3gppnetwork.org
    - kamdbctl create

  - [Guest] Database for P-CSCF, I-CSCF, and S-CSCF
    - mysql -u root -p 
    - create database `pcscf`;
    - create database `icscf`;
    - create database `scscf`;
    - quit;

    - cd VOWIFI_ROOT/ims/utils/kamctl/mysql
    - mysql -u root -p pcscf < standard-create.sql
    - mysql -u root -p pcscf < presence-create.sql
    - mysql -u root -p pcscf < ims_usrloc_pcscf-create.sql
    - mysql -u root -p pcscf < ims_dialog-create.sql
    - mysql -u root -p scscf < standard-create.sql
    - mysql -u root -p scscf < presence-create.sql
    - mysql -u root -p scscf < ims_usrloc_scscf-create.sql
    - mysql -u root -p scscf < ims_dialog-create.sql
    - mysql -u root -p scscf < ims_charging-create.sql

    - cd VOWIFI_ROOT/ims/misc/examples/ims/icscf
    - mysql -u root -p icscf < icscf.sql

    - mysql -u root -p
    - create user pcscf@localhost identified by 'heslo';
    - grant delete,insert,select,update on pcscf.* to pcscf@localhost;
    - create user scscf@localhost identified by 'heslo';
    - grant delete,insert,select,update on scscf.* to scscf@localhost;
    - create user icscf@localhost identified by 'heslo';
    - grant delete,insert,select,update on icscf.* to icscf@localhost;
    - create user provisioning@localhost identified by 'provi';
    - grant delete,insert,select,update on icscf.* to provisioning@localhost;
    - flush privileges;

    - use icscf;
    - insert into `nds_trusted_domains` values (1, 'ims.mnc260.mcc310.3gppnetwork.org');
    - insert into `s_cscf` values (1, 'First and only S-CSCF', 'sip:scscf.ims.mnc260.mcc310.3gppnetwork.org:6060');
    - insert into `s_cscf_capabilities` values (1,1,0),(2,1,1);

  - [Guest] Running P-CSCF, I-CSCF, and S-CSCF
    - (You may open three terminals from the host and you can enter into the VM by typing `vagrant ssh`.)

    - [Terminal 1]
      - cd VOWIFI_ROOT/settings/ims/pcscf
      - sudo mkdir -p /var/run/kamailio_pcscf
      - sudo kamailio -f kamailio.cfg -P /kamailio_pcscf.pid -DDeE

    - [Terminal 2]
      - cd VOWIFI_ROOT/settings/ims/icscf
      - sudo mkdir -p /var/run/kamailio_icscf
      - sudo kamailio -f kamailio.cfg -P /kamailio_icscf.pid -DDeE

    - [Terminal 3]
      - cd VOWIFI_ROOT/settings/ims/scscf
      - sudo mkdir -p /var/run/kamailio_scscf
      - sudo kamailio -f kamailio.cfg -P /kamailio_scscf.pid -DDeE

## DNS Setting
  - [Guest] Bind9 Installation
    - sudo apt-get install bind9

  - [Guest] Bind9 Configuration and Restart
    - sudo cp VOWIFI_ROOT/settings/dns/* /etc/bind
    - sudo systemctl restart bind9.service

  - [Guest] Add the nameserver
    - sudo vi /etc/resolv.conf
    - add "nameserver 127.0.0.1"
