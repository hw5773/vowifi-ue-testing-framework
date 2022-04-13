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
  - `git clone https://github.com/hw5773/vowifi-core-network.git`
  - Let's say the absolute path of the vowifi-core-network directory VOWIFI_ROOT

## VoWiFi network topology
  -      UE -------- [WiFi AP/DNS] ------ [ePDG] -- [IMS]
  -  (Smartphone) ----- (laptop) ------- (desktop) - (VM)

## VoWiFi SIM card setting
  - H/W: sysmoISIM-SJA2 (ISIM card), SIM card reader/writer
  - S/W: pysim
 
  - Clone the pysim tool to set up the VoWiFi sim card
    - `git clone https://github.com/osmocom/pysim.git`
  
  - SIM writing and setting
    - `sudo python3 pySim-prog.py -p 0 -a <ADM value> -n "T-Mobile" -x 310 -y 260 --imsi=<IMSI> --msisdn=<Telephone Number> --ims-hdomain=msg.pc.t-mobile.com --impi=<IMSI>@msg.pc.t-mobile.com --impu=sip:<IMSI>@ims.mnc260.mcc310.3gppnetwork.org --iccid=8901260245784161215 --smsp 542d4d6f62696c65fffffffffffffffff1ffffffffffffffffffffffff07912160130300f4ffffffff0000ff --smsc 12063130004 --opmode 80 --acc 0010 -k <key> -o <OPc>`
    - `sudo python3 pySim-shell.py -p 0 -a <ADM value> --script VOWIFI_ROOT/simcard/scripts/vowifi-setting.script`
  
  ### Commands with an example value (you can just copy & paste the following command for the two SIM cards and only revise the ADM values)
    - SIM 1
      - `sudo python3 pySim-prog.py -p 0 -a <ADM value> -n "T-Mobile" -x 310 -y 260 --imsi=310260123456781 --msisdn=17657751234 --ims-hdomain=msg.pc.t-mobile.com --impi=310260123456781@msg.pc.t-mobile.com --impu=sip:310260123456781@ims.mnc260.mcc310.3gppnetwork.org --iccid=8901260245784161215 --smsp 542d4d6f62696c65fffffffffffffffff1ffffffffffffffffffffffff07912160130300f4ffffffff0000ff --smsc 12063130004 --opmode 80 --acc 0010 -k 11111111111111111111111111111111 -o 99999999999999999999999999999999`
      - `sudo python3 pySim-shell.py -p 0 -a <ADM value> --script VOWIFI_ROOT/simcard/scripts/vowifi-setting.script`
  
    - SIM 2
      - `sudo python3 pySim-prog.py -p 0 -a <ADM value> -n "T-Mobile" -x 310 -y 260 --imsi=310260123456782 --msisdn=17657751235 --ims-hdomain=msg.pc.t-mobile.com --impi=310260123456782@msg.pc.t-mobile.com --impu=sip:310260123456782@ims.mnc260.mcc310.3gppnetwork.org --iccid=8901260245784161215 --smsp 542d4d6f62696c65fffffffffffffffff1ffffffffffffffffffffffff07912160130300f4ffffffff0000ff --smsc 12063130004 --opmode 80 --acc 0010 -k 22222222222222222222222222222222 -o 99999999999999999999999999999999`
      - `sudo python3 pySim-shell.py -p 0 -a <ADM value> --script VOWIFI_ROOT/scripts/vowifi-setting.script`
  
## WiFi AP
  - S/W: wifi-ap (or hostapd + dnsmasq)
  - `sudo apt-get install snapd`
  - `snap install wifi-ap`
  - `sudo wifi-ap.config set wifi.interface=<wifi interface>`
  - `sudo wifi-ap.config set wifi.ssid=VoWiFi`
  - `sudo wifi-ap.config set wifi.security-passphrase=vowifiaccess`
  - `sudo wifi-ap.config set wifi.address=10.0.60.1`
  - `sudo wifi-ap.config set dhcp.range-start=10.0.60.2`
  - `sudo wifi-ap.config set dhcp.range-stop=10.0.60.254`
  - `sudo wifi-ap.config set disabled=false`
  - `sudo wifiap.status restart-ap`

## ePDG address setting
  - sudo vi /etc/hosts (on the WiFi AP machine)
  - add the following record
  `<ePDG's IP address> epdg.epc.mnc260.mcc310.pub.3gppnetwork.org`

  ### With one laptop
  Add the record `10.0.60.1 epdg.epc.mnc260.mcc310.pub.3gppnetwork.org` to /etc/hosts
 
## ePDG installation and configuration
  - Installation
    - S/W: Strongswan
    - `cd VOWIFI_ROOT/epdg`
    - `autoreconf -f -i`
    - `./configure --prefix=/usr/local --enable-eap-aka --enable-eap-aka-3gpp --enable-eap-aka-3gpp2`
    - (Please check if eap-aka, eap-aka-3gpp, and eap-aka-3gpp2 modules are all enabled.)
    - `make && sudo make install`

  - Copy and paste all the configuration files
    - `cd VOWIFI_ROOT/settings/epdg`
    - `sudo cp * /usr/local/etc`

  - Configuration - ipsec.conf
    - The important attributes in this configuration file are as follows: 
    - "leftsubnet": the network address of the VoWiFi network (fdad:dabb:ed::/64). The network bits should be 64 bits (it is required by UEs).
    - "rightsubnet": the network address of the IPv6 addresses to be assigned to UEs (2607:fc20:ba53:1538::/64). The network bits should be 64 bits (it is required by UEs).
    - "rightsourceip": the IPv6 pool for UEs (2607:fc20:ba53:1538:0:9:8a73:cc01-2607:fc20:ba53:1538:0:9:8a73:cc10)
 
  - Configuration - strongswan.conf
    - The values in "attr" are provided to UE for the networking information. T-mobile uses 16386 to indicate the IPv6 address of the IMS server (in detail, it is the address of P-CSCF).
 
  - Running Strongswan
    - sudo ipsec restart

  ### Settings with an example value (you can revise the addresses)
    - attr {
        16386 = fdad:dabb:ed::2
      }
 
  - Configuration - ipsec.secrets
    - The format (syntax) of the configuration file for the EAP secrets
    - `<ID> : EAP <K> <OPc>` (The key value and the OPc value are concatenated without any space in-between them)

  ### Settings with an example value
    - `310260123456781@nai.epc.mnc260.mcc310.3gppnetwork.org : EAP 0x1111111111111111111111111111111199999999999999999999999999999999`
    - `310260123456782@nai.epc.mnc260.mcc310.3gppnetwork.org : EAP 0x2222222222222222222222222222222299999999999999999999999999999999`
 
## IMS server installation and configuration

  - [Host] Install VirtualBox and Vagrant
    - `sudo apt-get install virtualbox`
    - `sudo apt-get install vagrant`
 
  - [Host] Create the bridge
    - `sudo ip link add vowifi-gw type bridge`
    - `sudo ip link set vowifi-gw up`
    - `sudo ip address add fdad:dabb:ed::1 dev vowifi-gw`
    - `sudo route -A inet6 add fdad:dabb:ed::/64 dev vowifi-gw`
 
  - [Host] Initiate the VM
    - `cd VOWIFI_ROOT/vagrant`
    - `vagrant up`
    - select vowifi-gw for the bridge
    - `vagrant ssh` (to get the shell of VM)
 
  - [Guest] Network setting in the VM
    - `sudo route -A inet6 add default gw via fdad:dabb:ed::1 dev eth1` (the interface name might be different)

  - [Guest] Downlod the Setting files for the IMS Server
    - `git clone https://github.com/hw5773/vowifi-core-network.git`
 
  - [Guest] mysql Installation
    - `sudo apt-get install mysql-client mysql-server`
    - `sudo mysql`
    - `alter user 'root'@'localhost' identified with mysql_native_password by '<password>';`

  - [Guest] IMS Server Installation and Configuration
    - S/W: Kamailio
    - `cd VOWIFI_ROOT/ims`
    - `sudo apt-get install automake autoconf build-essential bison flex libxml2-dev libcurl4-gnutls-dev libssl-dev libmysqlclient-dev libpcre3-dev g++ pkg-config libsctp-dev libmnl-dev`
    - `make include_modules="ims_usrloc_pcscf ims_registrar_pcscf xmlrpc db_mysql cdp cdp_avp ims_icscf presence ims_usrloc_scscf ims_registrar_scscf ims_auth ims_isc ims_charging ims_dialog ims_ipsec_pcscf" cfg`
    - `make && sudo make install`

  - [Guest] Kamailio mysql Configuration
    - `sudo vi /usr/local/etc/kamailio/kamctlrc`
    - uncomment DBENGINE=MYSQL
    - uncommant and revise SIP_DOMAIN=ims.mnc260.mcc310.3gppnetwork.org
    - `kamdbctl create`

  - [Guest] Database for P-CSCF, I-CSCF, and S-CSCF
    - `mysql -u root -p`
    - `create database `pcscf`;`
    - `create database `icscf`;`
    - `create database `scscf`;`
    - `quit;`

    - `cd VOWIFI_ROOT/ims/utils/kamctl/mysql`
    - `mysql -u root -p pcscf < standard-create.sql`
    - `mysql -u root -p pcscf < presence-create.sql`
    - `mysql -u root -p pcscf < ims_usrloc_pcscf-create.sql`
    - `mysql -u root -p pcscf < ims_dialog-create.sql`
    - `mysql -u root -p scscf < standard-create.sql`
    - `mysql -u root -p scscf < presence-create.sql`
    - `mysql -u root -p scscf < ims_usrloc_scscf-create.sql`
    - `mysql -u root -p scscf < ims_dialog-create.sql`
    - `mysql -u root -p scscf < ims_charging-create.sql`

    - `cd VOWIFI_ROOT/ims/misc/examples/ims/icscf`
    - `mysql -u root -p icscf < icscf.sql`

    - `mysql -u root -p`
    - `create user pcscf@localhost identified by 'heslo';`
    - `grant delete,insert,select,update on pcscf.* to pcscf@localhost;`
    - `create user scscf@localhost identified by 'heslo';`
    - `grant delete,insert,select,update on scscf.* to scscf@localhost;`
    - `create user icscf@localhost identified by 'heslo';`
    - `grant delete,insert,select,update on icscf.* to icscf@localhost;`
    - `create user provisioning@localhost identified by 'provi';`
    - `grant delete,insert,select,update on icscf.* to provisioning@localhost;`
    - `flush privileges;`

    - `use icscf;`
    - `insert into `nds_trusted_domains` values (1, 'ims.mnc260.mcc310.3gppnetwork.org');`
    - `insert into `s_cscf` values (1, 'First and only S-CSCF', 'sip:scscf.ims.mnc260.mcc310.3gppnetwork.org:6060');`
    - `insert into `s_cscf_capabilities` values (1,1,0),(2,1,1);`

## DNS Setting
  - [Guest] Bind9 Installation
    - `sudo apt-get install bind9`

  - [Guest] Bind9 Configuration and Restart
    - `sudo cp VOWIFI_ROOT/settings/dns/* /etc/bind`
    - `sudo systemctl restart bind9.service`

  - [Guest] Add the nameserver
    - `sudo vi /etc/resolv.conf`
    - add `nameserver 127.0.0.1` to the first line of the file.

## HSS Installation and Configuration
  - [Host] Download Oracle Java 7 JDK by using any browser (https://www.oracle.com/java/technologies/javase/javase7-archive-downloads.html)
    - Download jdk-7u80-linux-x64.tar.gz
    - `cp ~/Downloads/jdk-7u80-linux-x64.tar.gz VOWIFI_ROOT/vagrant`
    - `cd VOWIFI_ROOT/vagrant`
    - `vagrant ssh`

  - [Guest] Java 7 Installation
    - `cp /vagrant/jdk-7u80-linux-x64.tar.gz ~`
    - `tar xvzf jdk-7u80-linux-x64.tar.gz`
    - `sudo update-alternatives --install /usr/bin/java java /home/vagrant/jdk1.7.0_80/bin/java 100`
    - `sudo update-alternatives --install /usr/bin/javac javac /home/vagrant/jdk1.7.0_80/bin/javac 100`
    - `sudo update-alternatives --config java`
    - `sudo update-alternatives --config javac`

  - [Guest] Ant Installation
    - `wget http://archive.apache.org/dist/ant/binaries/apache-ant-1.9.14-bin.tar.gz`
    - `tar xvzf apache-ant-1.9.14-bin.tar.gz`
    - `export ANT_HOME=/home/vagrant/apache-ant-1.9.14`
    - `sudo ln -s /home/vagrant/apache-ant-1.9.14/bin/ant /usr/bin/ant`

  - [Guest] FHoSS Installation
    - `cd ~`
    - `git clone https://github.com/herlesupreeth/FHoSS.git`
    - `cd FHoSS`
    - `export JAVA_HOME=/home/vagrant/jdk1.7.0_80`
    - `export CLASSPATH=/home/vagrant/jdk1.7.0_80/jre/lib`
    - `ant compile deploy`
    - `cd deploy`
    - `vi configurator.sh`
    - Copy and paste the following code
    ```
    #!/bin/bash
    # Initialization & global vars
    # if you execute this script for the second time
    # you should change these variables to the latest
    # domain name and ip address
    DDOMAIN="open-ims\.test"
    DSDOMAIN="open-ims\\\.test"
    DEFAULTIP="127\.0\.0\.1"
    CONFFILES=`ls *.cfg *.xml *.sql *.properties 2>/dev/null`

    # Interaction
    printf "Domain Name:"
    read domainname 
    printf "IP Adress:"
    read ip_address

    # input domain is to be slashed for cfg regexes 
    slasheddomain=`echo $domainname | sed 's/\./\\\\\\\\\./g'`

    if [ $# != 0 ] 
    then 
    printf "changing: "
      for j in $* 
      do
        sed -i -e "s/$DDOMAIN/$domainname/g" $j
        sed -i -e "s/$DSDOMAIN/$slasheddomain/g" $j
        sed -i -e "s/$DEFAULTIP/$ip_address/g" $j
        printf "$j " 
      done
      echo 
    else 
    printf "File to change [\"all\" for everything, \"exit\" to quit]:"
    # loop
      while read filename ;
      do
        if [ "$filename" = "exit" ] 
        then 
        printf "exitting...\n"
        break ;

      elif [ "$filename" = "all" ]
      then    
        printf "changing: "
          for i in $CONFFILES 
          do
            sed -i -e "s/$DDOMAIN/$domainname/g" $i
            sed -i -e "s/$DSDOMAIN/$slasheddomain/g" $i
            sed -i -e "s/$DEFAULTIP/$ip_address/g" $i
        
            printf "$i " 
          done 
        echo 
        break;

      elif [ -w $filename ] 
      then
        printf "changing $filename \n"
        sed -i -e "s/$DDOMAIN/$domainname/g" $filename
        sed -i -e "s/$DSDOMAIN/$slasheddomain/g" $filename
        sed -i -e "s/$DEFAULTIP/$ip_address/g" $filename

      else 
        printf "cannot access file $filename. skipping... \n" 
      fi
        printf "File to Change:"
      done 
    fi
    ```

  - [Guest] Copy the configuration file of the Diameter protocol
    - `cp VOWIFI_ROOT/settings/hss/DiameterPeerHSS.xml ~/FHoSS/deploy`

  - [Guest] Change the domain names
    - `chmod +x configurator.sh`
    - `vi webapps/hss.web.console/WEB-INF/web.xml`
    - Change all the "open-ims" to "ims.mnc260.mcc310.3gppnetwork.org" (check them by grep -r -n "open-ims")

    - `cp configurator.sh VOWIFI_ROOT/settings/hss`
    - `cd VOWIFI_ROOT/settings/hss`
    - `./configurator.sh`
    - Domain Name: ims.mnc260.mcc310.3gppnetwork.org
    - IP Address: 127.0.0.1
    - Change all the "open-ims" to "ims.mnc260.mcc310.3gppnetwork.org" (check them by grep -r -n "open-ims")

    - `cp configurator.sh ~/FHoSS/config/`
    - `cd ~/FHoSS/config`
    - `./configurator.sh`
    - Domain Name: ims.mnc260.mcc310.3gppnetwork.org
    - IP Address: 127.0.0.1
    - Change all the "open-ims" to "ims.mnc260.mcc310.3gppnetwork.org" (check them by grep -r -n "open-ims")

    - `cd ../src-web`
    - `vi WEB-INF/web.xml`
    - Change "open-ims.test" to "ims.mnc260.mcc310.3gppnetwork.org"

  - [Guest] Prepare the database for HSS
    - `mysql -u root -p`
    - `drop database hss_db;`
    - `create database hss_db;`
    - `quit;`

    - `cd VOWIFI_ROOT/settings/hss`
    - `mysql -u root -p hss_db < hss_db.sql`
    - `mysql -u root -p hss_db < userdata.sql`

## Running P-CSCF, I-CSCF, S-CSCF, and HSS
  - Open four terminals

  ### Terminal 1 - P-CSCF
    - [Host] `cd VOWIFI_ROOT/vagrant`
    - [Host] `vagrant ssh`
    - [Guest] `cd VOWIFI_ROOT/settings/ims/pcscf`
    - [Guest] `sudo mkdir -p /var/run/kamailio_pcscf`
    - [Guest] `sudo kamailio -f kamailio.cfg -P /kamailio_pcscf.pid -DDeE`

  ### Terminal 2 - I-CSCF
    - [Host] `cd VOWIFI_ROOT/vagrant`
    - [Host] `vagrant ssh`
    - [Guest] `cd VOWIFI_ROOT/settings/ims/icscf`
    - [Guest] `sudo mkdir -p /var/run/kamailio_icscf`
    - [Guest] `sudo kamailio -f kamailio.cfg -P /kamailio_icscf.pid -DDeE`

  ### Terminal 3 - S-CSCF
    - [Host] `cd VOWIFI_ROOT/vagrant`
    - [Host] `vagrant ssh`
    - [Guest] `cd VOWIFI_ROOT/settings/ims/scscf`
    - [Guest] `sudo mkdir -p /var/run/kamailio_scscf`
    - [Guest] `sudo kamailio -f kamailio.cfg -P /kamailio_scscf.pid -DDeE`

  ### Terminal 4 - HSS
    - [Host] `cd VOWIFI_ROOT/vagrant`
    - [Host] `vagrant ssh`
    - [Guest] `cd FHoSS/deploy`
    - [Guest] vi hss.properties
    - [Guest] Change `host=127.0.0.1` to `host=0.0.0.0`
    - [Guest] `sudo ./startup.sh`

## Add IMS Subscription
  - [Host] Open the browser
  - http://localhost:8888/hss.web.console/ (the HSS setting page is running on the 8080 port of the guest VM. The port forwarding is set in the Vagrant setting file (from guest 8080 to host 8888))
  - Login with ID: hssAdmin / Password: hss
  
  ### Subscription with an example value
  - UE1: IMSI 310260123456781
    - Click [Create] under [IMS Subscription]
    - Name: 310260123456781
    - Capabilities Set: cap_set1
    - Preferred S-CSCF: scscf1
    - Click [Save]

    - Click [Create & Bind new IMPI]
    - Identity: 310260123456781@msg.pc.t-mobile.com
    - Secret Key: 11111111111111111111111111111111
    - Check Digest-AKAv1
    - Default: Digest-AKAv1-MD5
    - OPc: 99999999999999999999999999999999
    - Click [Save]

    - Click [Create & Bind new IMPU]
    - Identity: `sip:310260123456781@ims.mnc260.mcc310.3gppnetwork.org`
    - Service Profile: default_sp
    - Charging-Info Set: default_charging_set
    - Check "Can Register"
    - Click [Save]

    - Select "ims.mnc260.mcc310.3gppnetwork.org" from the dropdown menu under [Add Visitied-Networks] and Click [Add]
    - Check if the followings are correct
      - List of Visitied Networks: ims.mnc260.mcc310.3gppnetwork.org
      - List IMPUs from Implicit-Set: sip:310260123456781@ims.mnc260.mcc310.3gppnetwork.org
      - List of associated IMPIs: 310260123456781@msg.pc.t-mobile.com

  - UE2: IMSI 310260123456782
    - Click [Create] under [IMS Subscription]
    - Name: 310260123456782
    - Capabilities Set: cap_set1
    - Preferred S-CSCF: scscf1
    - Click [Save]

    - Click [Create & Bind new IMPI]
    - Identity: 310260123456782@msg.pc.t-mobile.com
    - Secret Key: 22222222222222222222222222222222
    - Check Digest-AKAv1
    - Default: Digest-AKAv1-MD5
    - OPc: 99999999999999999999999999999999
    - Click [Save]

    - Click [Create & Bind new IMPU]
    - Identity: `sip:310260123456782@ims.mnc260.mcc310.3gppnetwork.org`
    - Service Profile: default_sp
    - Charging-Info Set: default_charging_set
    - Check "Can Register"
    - Click [Save]

    - Select "ims.mnc260.mcc310.3gppnetwork.org" from the dropdown menu under [Add Visitied-Networks] and Click [Add]
    - Check if the followings are correct
      - List of Visitied Networks: ims.mnc260.mcc310.3gppnetwork.org
      - List IMPUs from Implicit-Set: sip:310260123456782@ims.mnc260.mcc310.3gppnetwork.org
      - List of associated IMPIs: 310260123456782@msg.pc.t-mobile.com

## Turn on WiFi on UEs

