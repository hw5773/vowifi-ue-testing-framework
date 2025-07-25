# Open-source VoWiFi UE Testing Framework
- Includes a separate control architecture
- Includes a JSON-based testing format for describing any testcase
- Tested with 24 UEs
- Includes capability to generate adversarial and out-of-order messages in testcases

# Overview
   ![Overview of the VoWiFi-UE-testing framework](figure/vwattacker.png?raw=true)
# Directories
- control: includes the source files of the controllers
- epdg: includes the source files of the ePDG (StrongSwan)
- hss: includes the source files of the HSS
- ims: includes the source files of the IMS (Kamailio)
- scripts: includes useful Python scripts
- settings: includes configuration files in running modules in the VoWiFi testbed
- simcard: includes setting/configuration files to make SIM cards
- testcases: includes example testcases
- vagrant: includes VM configuration files
- vowifi-property-extraction-with-rag: includes rag-based vowifi property extraction framework

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

## Machines
  - I recommend having **a laptop with two network interfaces**: one is to communicate with the VoWiFi phones and the other is used to access the Internet. Note that some VoWiFi phones do not initiate the VoWiFi protocol if there is no Internet connection.
  - I will say the **wlan** interface for the interface used to communicate with the phones and the **other** interface for the interface accessed to the Internet. Note that the other interface can be any type (e.g., WiFi, LTE, ethernet, and so on) of an interface.

## Kernel settings
  - forwarding ipv6 packets should be enabled
  - `sudo sysctl -w net.ipv6.conf.all.forwarding=1`

## VoWiFi settings and source codes of the ePDG/IMS server
  - `git clone https://github.com/hw5773/vowifi-core-network.git`
  - Let's say the absolute path of the vowifi-core-network directory VOWIFI_ROOT

## VoWiFi network topology
  -      UE -------- [WiFi AP/DNS] ------ [ePDG] -- [IMS]
  -  (Smartphone) ---(laptop/desktop) ---(laptop/desktop) - (VM)

## VoWiFi SIM card setting
  - H/W: sysmoISIM-SJA2 (ISIM card), SIM card reader/writer
  - S/W: pysim
 
  - Clone the pysim tool to set up the VoWiFi sim card
    - `git clone https://github.com/osmocom/pysim.git`
  
  - SIM writing and setting
    - `sudo python3 pySim-prog.py -p 0 -a <ADM value> -n "T-Mobile" -x 310 -y 210 --imsi=<IMSI> --msisdn=<Telephone Number> --ims-hdomain=msg.pc.t-mobile.com --impi=<IMSI>@msg.pc.t-mobile.com --impu=sip:<IMSI>@ims.mnc210.mcc310.3gppnetwork.org --iccid=8901260245784161215 --smsp 542d4d6f62696c65fffffffffffffffff1ffffffffffffffffffffffff07912160130300f4ffffffff0000ff --smsc 12063130004 --opmode 80 --acc 0010 -k <key> -o <OPc>`
    - `sudo python3 pySim-shell.py -p 0 -a <ADM value> --script VOWIFI_ROOT/simcard/scripts/vowifi-setting.script`

  - USIM/ISIM service change
    - `sudo python3 pySim-shell.py -p 0 -a <ADM value>`
    - (pySim-shell) `select ADF.ISIM`
    - (pySim-shell) `select EF.IST`
    - (pySim-shell) `edit_binary_decoded` (enable Service 2 - Generic Boostrapping Architecture (GBA))
    - (pySim-shell) `select ADF.USIM`
    - (pySim-shell) `select EF.UST`
    - (pySim-shell) `edit_binary_decoded` (enable Service 68 - Generic Boostrapping Architecture (GBA))
    - (pySim-shell) `select EF.ISIM_SQN` (disable sqn_check)
    - (pySim-shell) `select ADF.USIM`
    - (pySim-shell) `select EF.USIM_SQN` (disable sqn_check)
    - (pySim-shell) `select ADF.USIM` (change the length of MNC in IMSI to 3)
    - (pySim-shell) `select EF.AD`
    - (pySim-shell) `update_binary_decoded '{"ms_operation_mode": "type_approval", "additional_info": {"ciphering_indicator": false, "csg_display_control": false, "prose_services": false, "extended_drx": false}, "rfu": 0, "mnc_len": 3, "extensions": ""}'`
    - (pySim-shell) `select DF.GSM` (change the length of MNC in IMSI to 3)
    - (pySim-shell) `select EF.AD`
    - (pySim-shell) `update_binary_decoded '{"ms_operation_mode": "type_approval", "rfu1": 0, "rfu2": 0, "ofm": false, "extensions": {"rfu3": 0, "mnc_len": 3, "extensions": ""}}'`
  
  ### Commands with an example value (you can just copy & paste the following command for the two SIM cards and only revise the ADM values)
    - SIM 1
      - `sudo python3 pySim-prog.py -p 0 -a <ADM value> -n "T-Mobile" -x 310 -y 210 --imsi=310210123456781 --msisdn=17657751234 --ims-hdomain=msg.pc.t-mobile.com --impi=310210123456781@msg.pc.t-mobile.com --impu=sip:310210123456781@ims.mnc210.mcc310.3gppnetwork.org --iccid=8901260245784161215 --smsp 542d4d6f62696c65fffffffffffffffff1ffffffffffffffffffffffff07912160130300f4ffffffff0000ff --smsc 12063130004 --opmode 80 --acc 0010 -k 11111111111111111111111111111111 -o 99999999999999999999999999999999`
      - `sudo python3 pySim-shell.py -p 0 -a <ADM value> --script VOWIFI_ROOT/simcard/scripts/vowifi-setting.script`
  
    - SIM 2
      - `sudo python3 pySim-prog.py -p 0 -a <ADM value> -n "T-Mobile" -x 310 -y 210 --imsi=310210123456782 --msisdn=17657751235 --ims-hdomain=msg.pc.t-mobile.com --impi=310210123456782@msg.pc.t-mobile.com --impu=sip:310210123456782@ims.mnc210.mcc310.3gppnetwork.org --iccid=8901260245784161215 --smsp 542d4d6f62696c65fffffffffffffffff1ffffffffffffffffffffffff07912160130300f4ffffffff0000ff --smsc 12063130004 --opmode 80 --acc 0010 -k 22222222222222222222222222222222 -o 99999999999999999999999999999999`
      - `sudo python3 pySim-shell.py -p 0 -a <ADM value> --script VOWIFI_ROOT/scripts/vowifi-setting.script`

  ### List of SIM card settings preset in the repository (within the sql file for the HSS)
  | No. | IMSI | Ki | OP | OPc | 
  | --- | --- | --- | --- | --- |
  | 0 | 310260123456780 | 00000000000000001111111111111111 | 0123456789abcdef0123456789abcdef | 39e98f41b6d0ed76f89f1be1db95d6ca |
  | 1 | 310260123456781 | 11111111111111110000000000000000 | 0123456789abcdef0123456789abcdef | cae66d4110513d5f71e7ae0986d9c3d9 |
  | 2 | 310260123456782 | 22222222222222223333333333333333 | 0123456789abcdef0123456789abcdef | 05ac79b5bb02b825a5cda8dfcc09439e |
  | 3 | 310260123456783 | 33333333333333332222222222222222 | 0123456789abcdef0123456789abcdef | 3b907342af465290c3250e2e750e91fe |
  | 4 | 310260123456784 | 44444444444444445555555555555555 | 0123456789abcdef0123456789abcdef | d8206cd5ed008a739fafe3d5d4d9e5c7 |
  | 5 | 310260123456785 | 55555555555555554444444444444444 | 0123456789abcdef0123456789abcdef | a7eb0198858cc008985ae4fce81abff5 |
  | 6 | 310260123456786 | 66666666666666667777777777777777 | 0123456789abcdef0123456789abcdef | af1ecb112aa057c15765ad96de61d9cc |
  | 7 | 310260123456787 | 77777777777777776666666666666666 | 0123456789abcdef0123456789abcdef | 26321ae82e97a5b51128fa6ccedc932d |
  | 8 | 310260123456788 | 88888888888888889999999999999999 | 0123456789abcdef0123456789abcdef | b292d947b09e60301f601b4faf22697f |
  | 9 | 310260123456789 | 99999999999999998888888888888888 | 0123456789abcdef0123456789abcdef | 987b215969f494bb8c8380e38fb6c107 |
  
## WiFi AP
  - I will assume that the laptop (or the desktop) has two network interfaces: the wlan interface and the other interface
  - `sudo apt-get update && sudo apt-get install git dnsmasq hostapd net-tools`
  - `sudo ifconfig <the name of the wlan interface (e.g., wlan0)> 172.24.1.1 netmask 255.255.255.0`
  - (Please make sure that 172.24.1.1 does not collide with other IP networks around the laptop)
  - `sudo vi /etc/hostapd/hostapd.conf`
  - Type (or copy) the following in (or to) the hostapd.conf file
  ```
  interface=<the name of the wlan interface>
  driver=nl80211
  ssid=vowifi
  hw_mode=g
  channel=11
  ieee80211n=1
  wmm_enabled=1
  ht_capab=[HT40][SHORT-GI-20][DSSS_CKK-40]
  macaddr_acl=0
  auth_algs=1
  ignore_broadcast_ssid=0
  wpa=2
  wpa_key_mgmt=WPA-PSK
  wpa_passphrase=vowifipassword
  rsn_pairwise=CCMP
  ```
  - `sudo rm /etc/dnsmasq.conf`
  - `sudo vi /etc/dnsmasq.conf`
  - Type (or copy) the following in (or to)
  ```
  interface=<the name of the wlan interface>
  listen-address=172.24.1.1
  bind-interfaces
  server=1.1.1.1
  domain-needed
  bogus-priv
  dhcp-range=172.24.1.2,172.24.1.254,12h
  ```
  - `sudo dnsmasq -C /etc/dnsmasq.conf`
  - `sudo hostapd -B /etc/hostapd/hostapd.conf`
  - `sudo sysctl -w net.ipv4.ip_forward=1`
  - `sudo iptables -t nat -A POSTROUTING -o <the name of the other interface> -j MASQUERADE`
  - `sudo iptables -A FORWARD -i <the name of the other interface> -o <the name of the wlan interface> -m state --state RELATED,ESTABLISHED -j ACCEPT`
  - `sudo iptables -A FORWARD -i <the name of the wlan interface> -o <the name of the other interface> -j ACCEPT`

## ePDG address setting
  - sudo vi /etc/hosts (on the WiFi AP machine)
  - add the following record
    ```
    <ePDG's IP address> epdg.epc.mnc210.mcc310.pub.3gppnetwork.org
    <ePDG's IP address> epdg.epc.mnc260.mcc310.pub.3gppnetwork.org
    ```

  ### With one laptop
  Add the record `172.24.1.1 epdg.epc.mnc210.mcc310.pub.3gppnetwork.org` and `172.24.1.1 epdg.epc.mnc260.mcc310.pub.3gppnetwork.org` to /etc/hosts
 
## ePDG installation and configuration
  - Installation
    - S/W: Strongswan
    - `cd VOWIFI_ROOT/epdg`
    - `autoreconf -f -i` (or `autoreconf --install`)
    - `./configure --prefix=/usr/local --enable-eap-aka --enable-eap-aka-3gpp --enable-eap-aka-3gpp2`
    - (Please check if eap-aka, eap-aka-3gpp, and eap-aka-3gpp2 modules are all enabled.)
    - `make && sudo make install`

  - Copy and paste all the configuration files
    - `cd VOWIFI_ROOT/settings/epdg`
    - `sudo cp -r * /usr/local/etc`

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
    - `<ID> : EAP <K> <OPc>` (The key value and the OPc value are concatenated without any space in-between them)

  ### Settings with an example value
    - `310210123456781@nai.epc.mnc210.mcc310.3gppnetwork.org : EAP 0x1111111111111111111111111111111199999999999999999999999999999999`
    - `310210123456782@nai.epc.mnc210.mcc310.3gppnetwork.org : EAP 0x2222222222222222222222222222222299999999999999999999999999999999`

  ### Running Strongswan to do our experiment
    - `cd VOWIFI_ROOT/epdg`
    - `./310260.sh` or `./310210.sh` (depending on the MCC/MNC that you want to run)
 
## IMS server installation and configuration
  - We are going to make two different core networks, one with MCC 310 and MNC 260, and the other with MCC 310 and MNC 210
  - The following example is for 310260. You should do the same things for 310210 as well.
  - The reason why we set up two different IMS network is because there are some VoWiFi phones that work in only one of them (there are some mutual exclusions, unfortunately).

  - [Host] Install VirtualBox and Vagrant
    - `sudo apt-get install virtualbox`
    - `sudo apt-get install vagrant`
 
  - [Host] Create the bridge
    - `cd VOWIFI_ROOT/scripts`
    - `./host.sh`
    - The above script generates two bridge interfaces, called vowifi-310210 and vowifi-310260, each of which connects its corresponding IMS network.
 
  - [Host] Initiate the VM
    - `cd VOWIFI_ROOT/vagrant/310260`
    - `vagrant up or (First run this --> vagrant init <box-name> e.g:, in our case it is <bento/ubuntu-20.04> so command will be --> vagrant init bento/ubuntu-20.04)`
    - select vowifi-310260 for the bridge
    - `vagrant ssh` (to get the shell of VM)
 
  - [Guest] Download the Setting files for the IMS server and setting the network interface in the VM
    - `git clone https://github.com/hw5773/vowifi-core-network.git`
    - `sudo apt-get install net-tools`
    - `cd VOWIFI_ROOT/scripts`
    - `./guest-310260.sh`
 
  - [Guest] mysql Installation
    - `sudo apt-get install mysql-client mysql-server`
    - `sudo mysql`
    - `alter user 'root'@'localhost' identified with mysql_native_password by '<password>';`
    - `quit;`

  - [Guest] IMS Server Installation and Configuration
    - S/W: Kamailio
    - `cd VOWIFI_ROOT/ims`
    - `sudo apt-get install automake autoconf build-essential bison flex libxml2-dev libcurl4-gnutls-dev libssl-dev libmysqlclient-dev libpcre3-dev g++ pkg-config libsctp-dev libmnl-dev`
    - `make include_modules="ims_usrloc_pcscf ims_registrar_pcscf xmlrpc db_mysql cdp cdp_avp ims_icscf presence ims_usrloc_scscf ims_registrar_scscf ims_auth ims_isc ims_charging ims_dialog ims_ipsec_pcscf" cfg`
    - `make && sudo make install`

  - [Guest] Kamailio mysql Configuration
    - `sudo vi /usr/local/etc/kamailio/kamctlrc`
    - uncomment DBENGINE=MYSQL
    - uncomment and revise the `SIP_DOMAIN` to `SIP_DOMAIN=ims.mnc260.mcc310.3gppnetwork.org`
    - `kamdbctl create` (Enter 'ascii' for the charater set name if you are asked)

  - [Guest] Database for P-CSCF, I-CSCF, and S-CSCF
    - `mysql -u root -p`
    - `create database 'pcscf';`
    - `create database 'icscf';`
    - `create database 'scscf';`
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
    - `insert into 'nds_trusted_domains' values (1, 'ims.mnc210.mcc310.3gppnetwork.org');`
    - `insert into 's_cscf' values (1, 'First and only S-CSCF', 'sip:scscf.ims.mnc210.mcc310.3gppnetwork.org:6060');`
    - `insert into 's_cscf_capabilities' values (1,1,0),(2,1,1);`

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
    - `cp VOWIFI_ROOT/settings/hss/310260/DiameterPeerHSS.xml ~/FHoSS/deploy`

  - [Guest] Change the domain names
    - `chmod +x configurator.sh`
    - `vi webapps/hss.web.console/WEB-INF/web.xml`
    - Change all the "open-ims" to "ims.mnc260.mcc310.3gppnetwork.org" (check them by grep -r -n "open-ims")

    - `cp configurator.sh VOWIFI_ROOT/settings/hss`
    - `cd VOWIFI_ROOT/settings/hss`
    - `./configurator.sh`
    - Domain Name: ims.mnc210.mcc310.3gppnetwork.org
    - IP Address: 127.0.0.1
    - Change all the "open-ims" to "ims.mnc260.mcc310.3gppnetwork.org" (check them by grep -r -n "open-ims")

    - `cp configurator.sh ~/FHoSS/config/`
    - `cd ~/FHoSS/config`
    - `./configurator.sh`
    - Domain Name: ims.mnc260.mcc310.3gppnetwork.org
    - IP Address: 127.0.0.1
    - Change all the "open-ims" to "ims.mnc210.mcc310.3gppnetwork.org" (check them by grep -r -n "open-ims")

    - `cd ../src-web`
    - `vi WEB-INF/web.xml`
    - Change "open-ims.test" to "ims.mnc210.mcc310.3gppnetwork.org"

  - [Guest] Prepare the database for HSS
    - `mysql -u root -p`
    - `drop database hss_db;`
    - `create database hss_db;`
    - `quit;`

    - `cd VOWIFI_ROOT/settings/hss/310260`
    - `mysql -u root -p < 310260.sql`

## Running P-CSCF, I-CSCF, S-CSCF, and HSS
  - Open four terminals

  ### Terminal 1 - P-CSCF
    - [Host] `cd VOWIFI_ROOT/vagrant/310260`
    - [Host] `vagrant ssh`
    - [Guest] `cd VOWIFI_ROOT/settings/ims/310260/pcscf`
    - [Guest] `./run.sh`

  ### Terminal 2 - I-CSCF
    - [Host] `cd VOWIFI_ROOT/vagrant/310260`
    - [Host] `vagrant ssh`
    - [Guest] `cd VOWIFI_ROOT/settings/ims/310260/icscf`
    - [Guest] `./run.sh`

  ### Terminal 3 - S-CSCF
    - [Host] `cd VOWIFI_ROOT/vagrant/310260`
    - [Host] `vagrant ssh`
    - [Guest] `cd VOWIFI_ROOT/settings/ims/310260/scscf`
    - [Guest] `./run.sh`

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
  - UE1: IMSI 310260123456781 (with the phone number 17657751111)
    - Click [Create] under [IMS Subscription]
    - Name: `310260123456781`
    - Capabilities Set: cap_set1
    - Preferred S-CSCF: scscf1
    - Click [Save]

    - Click [Create & Bind new IMPI]
    - Identity: `310260123456781@msg.pc.t-mobile.com`
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

    - Click [Search] under [IMS Subscription]
    - Name: 310260123456781
    - Click [Create & Bind new IMPI]
    - Identity: `310260123456781@ims.mnc260.mcc310.3gppnetwork.org`
    - Secret Key: 11111111111111111111111111111111
    - Check Digest-AKAv1
    - Default: Digest-AKAv1-MD5
    - OPc: 99999999999999999999999999999999
    - Associate IMPU: `sip:310260123456781@ims.mnc260.mcc310.3gppnetwork.org`
    - Click [Add]
    - Click [Save]
   
    - Select "ims.mnc260.mcc310.3gppnetwork.org" from the dropdown menu under [Add Visitied-Networks] and Click [Add]
    - Check if the followings are correct
      - List of Visitied Networks: ims.mnc260.mcc310.3gppnetwork.org
      - List IMPUs from Implicit-Set: sip:310260123456781@ims.mnc260.mcc310.3gppnetwork.org
      - List of associated IMPIs: 310260123456781@msg.pc.t-mobile.com / 310260123456781@ims.mnc260.mcc310.3gppnetwork.org
    - Bind the IMPIs with sip:17657751111@msg.pc.t-mobile.com, sip:+17657751111@ims.mnc260.mcc310.3gppnetwork.org, sip:310260123456781@ims.mnc260.mcc310.3gppnetwork.org, and sip:+17657751111@msg.pr.t-mobile.com

  - UE2: IMSI 310260123456782 (with the phone number 17657752222)
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

    - Click [Search] under [IMS Subscription]
    - Name: 310260123456782
    - Click [Create & Bind new IMPI]
    - Identity: `310260123456782@ims.mnc260.mcc310.3gppnetwork.org`
    - Secret Key: 11111111111111111111111111111111
    - Check Digest-AKAv1
    - Default: Digest-AKAv1-MD5
    - OPc: 99999999999999999999999999999999
    - Associate IMPU: `sip:310260123456782@ims.mnc260.mcc310.3gppnetwork.org`
    - Click [Add]
    - Click [Save]

    - Select "ims.mnc260.mcc310.3gppnetwork.org" from the dropdown menu under [Add Visitied-Networks] and Click [Add]
    - Check if the followings are correct
      - List of Visitied Networks: ims.mnc260.mcc310.3gppnetwork.org
      - List IMPUs from Implicit-Set: sip:310260123456782@ims.mnc260.mcc310.3gppnetwork.org
      - List of associated IMPIs: 310260123456782@msg.pc.t-mobile.com / 310260123456782@ims.mnc260.mcc310.3gppnetwork.org
    - Bind the IMPIs with sip:17657752222@msg.pc.t-mobile.com, sip:+17657752222@ims.mnc260.mcc310.3gppnetwork.org, sip:310260123456782@ims.mnc260.mcc310.3gppnetwork.org, and sip:+17657752222@msg.pr.t-mobile.com

## Turn on WiFi on UEs

## Control Architecture
 - To conduct a UE testing, we implement the control architecture for VoWiFi to controll all the modules in the VoWiFi network including a UE, an ePDG, an IMS, and an HSS.
   ![The control architecture of the VoWiFi-UE-testing framework](figure/controller.png?raw=true)

## Testcase Samples
 - The following JSON object is a testcase to test a UE on adversarial messages sent by ePDG and IMS according to the testcase. The testcase includes a scenario, where the encryption algorithm is set to null in the ealg attribute of the 401 unauthorized message.
 - ```json
   {
     "testcases": [
       {
         "testcase": [
           {
             "receiver": "ue",
             "name": "enable_vowifi",
             "reporter": "epdg",
             "id": "update-401_unauthorized-security_server-ealg-null"
           },
           {
             "receiver": "epdg",
             "name": "ike_sa_init_response",
             "reporter": "epdg"
           },
           {
             "receiver": "epdg",
             "name": "ike_auth_1_response",
             "reporter": "epdg"
           },
           {
             "receiver": "epdg",
             "name": "ike_auth_2_response",
             "reporter": "epdg"
           },
           {
             "receiver": "epdg",
             "name": "ike_auth_3_response",
             "reporter": "ims"
           },
           {
             "receiver": "ims",
             "name": "401_unauthorized",
             "reporter": "ims",
             "sub": [
               {
                 "name": "security_server",
                 "sub": [
                   {
                     "name": "ealg",
                     "op": "update",
                     "type": "string",
                     "value": "null"
                   }
                 ]
               }
             ]
           },
         ]
       }
     ]
   }
   ```

A testcase (TC), shown in the follwoing Figure, is an interface provided for a tester to describe the scenario of their interest encoded in a simple JSON format. 


![VoWiFi-UE-testcases json format](figure/testcase-examples.png?raw=true)

# VoWiFi Property Extraction using RAG

## Prompt to extract properties
```
<|system|>  
% Role Declaration  
You are a Vo-WiFi expert. Your task is to extract properties of Vo-WiFi from the contexts given from specifications.  

Below you will find the basic structure of Properties in a Vo-WiFi specification.  

% Rules  
Properties often  
1. Use "shall", "must", or "should" to indicate mandatory actions or strong recommendations.  
2. Describe specific actions, such as taking input, sending parameters, or generating output.  
3. Include terms like Input, Output, Parameter etc.  
4. Specify interactions between network components (e.g., UE, ePDG, AAA Server) and the data exchanged (e.g., AUTH parameter, Notify payload).  
5. Describe steps in a process and the dependencies between them, specify conditions or contexts for actions to occur.  

% Instructions  
1. Be concise while generating; only give the extracted properties as a response, and don't add anything on your own.  

% Example Properties  
Some example properties are:  
1. The UE shall take its own copy of the MSK as input to generate the AUTH parameter to authenticate the first IKE_SA_INIT message.  
2. The AUTH parameter is sent to the ePDG. The UE includes a Notify payload ANOTHER_AUTH_FOLLOWS indicating to the ePDG that another authentication and authorization round will follow.  
3. The UE sends the identity in the private network in IDi payload that is used for the next authentication and authorization with the External AAA Server and without an AUTH payload.  

% Context Block  
You will find the required information about vo-wifi properties in the following context:  

{context} % retrieved from the specifications  

<|assistant|>  % Assistant Output Section
```
##  List of properties extracted using LLM

| Sl | Property |
|----|----------|
| 1 | The UE shall take its own copy of the MSK as input to generate the AUTH parameter to authenticate the first IKE_SA_INIT message. |
| 2 | The UE takes its own copy of the MSK (Master Session Key) as input to generate the AUTH parameter to authenticate the first IKE_SA_INIT message. |
| 3 | The AUTH parameter is sent to the ePDG, and the UE includes a Notify payload ANOTHER_AUTH_FOLLOWS to indicate that another authentication and authorization round will follow. |
| 4 | The UE sends its identity in the private network in the IDi payload for the next authentication and authorization with the External AAA Server and without an AUTH payload. |
| 5 | The UE shall take its own copy of the MSK as input to generate the AUTH parameter to authenticate the first IKE_SA_INIT message. |
| 6 | EAP-AKA, as specified in RFC 4187, within IKEv2, as specified in RFC 5996, shall be used to authenticate UEs, and certificates used for authentication of the ePDG shall meet the certificate profiles given in TS 33.310. |
| 7 | The ePDG shall authenticate itself to the UE with an identity that is the same as the FQDN of the ePDG determined by the ePDG selection procedures defined in TS 23.402, and this identity shall be contained in the IKEv2 ID_FQDN payload and shall match a dNSName SubjectAltName component in the ePDG's certificate. |
| 8 | The UE shall use the Configuration Payload of IKEv2 to obtain the Remote IP address. |
| 9 | Replay protection is provided in IKEv2 as the UE and ePDG generate nonces as input to derive the encryption and authentication keys, preventing intermediate nodes from modifying or changing the user identity. |
| 10 | The UE omits the AUTH parameter in order to indicate to the ePDG that it wants to use EAP over IKEv2. |
| 11 | When the UE requests with a CERTREQ payload, the ePDG responds by sending the certificates requested by the UE in the CERT payload. To protect the previous message in the IKE_SA_INIT exchange, the ePDG includes an AUTH payload in the response. |
| 12 | The UE checks the authentication parameters and responds to the authentication challenge, and the IKE_AUTH request message includes the EAP message (EAP-Response/AKA-Challenge) containing the UE's response to the authentication challenge. |
| 13 | The UE takes its own copy of the MSK as input to generate the AUTH parameter, and the AUTH parameter is sent to the ePDG. |
| 14 | The UE shall send X.509 certificate - Signature payloads with encoding value 4. |
| 15 | The UE shall not assume that any except the first IKEv2 CERT payload is ordered in any way. |
| 16 | The UE shall be able to support certificate paths containing up to four certificates, where the intermediate CA certificates and the ePDG certificate are obtained from the IKEv2 CERT payload and the self-signed CA certificate is obtained from a UE local store of trusted root certificates. |
| 17 | The UE shall be prepared to receive irrelevant certificates, or certificates it does not understand. |
| 18 | The UE shall be able to process certificates even if naming attributes are unknown. |
| 19 | The UE shall support both UTCTime and GeneralizedTime encoding for validity time. |
| 20 | The UE shall check the validity time, and reject certificates that are either not yet valid or are expired. |
| 21 | The UE shall support processing of the BasicConstraints, NameConstraints, and KeyUsage extensions. |
| 22 | Support for OCSP is mandatory in the UE. |
| 23 | The UE should send an OCSP request message to the OCSP server after the tunnel is established, and before user data is transmitted, to check the certificate status of the ePDG. |
| 24 | The UE shall establish a new IPsec tunnel with the new ePDG as described in subclause 8.2.2. |
| 25 | The UE receives an IKE_AUTH Response message from the ePDG, containing its identity, a certificate, and the AUTH parameter to protect the previous message it sent to the UE. |
| 26 | The UE shall re-establish the IPsec Tunnel for the corresponding PDN connection after its release. |
| 27 | The first certificate provided MUST contain the public key used to verify the AUTH field. |
| 28 | The responder might use some other IDr to finish the exchange. |
| 29 | If the initiator guesses the wrong Diffie-Hellman group during the IKE_SA_INIT, it must retry the IKE_SA_INIT with the corrected Diffie-Hellman group, and it should again propose its full supported set of groups, while picking an element of the selected group for its KE value. |
| 30 | The IKE SA is still created as usual, and the Notify message types that do not prevent an IKE SA from being set up include at least NO_PROPOSAL_CHOSEN, TS_UNACCEPTABLE, SINGLE_PAIR_REQUIRED, INTERNAL_ADDRESS_FAILURE, and FAILED_CP_REQUIRED. |
| 31 | If the failure is related to creating the IKE SA, the IKE SA is not created. The information needs to be treated with caution, assuming the peer receiving the Notify error message has not yet authenticated the other end, or if the peer fails to authenticate the other end for some reason. |
| 32 | The responder MUST reject the request and indicate its preferred Diffie-Hellman group in the response. |
| 33 | INFORMATIONAL exchanges MUST ONLY occur after the initial exchanges and are cryptographically protected with the negotiated keys. |
| 34 | The IKE SA MUST be closed or rekeyed. |
| 35 | An endpoint MUST NOT conclude that the other endpoint has failed based on any routing information (e.g., ICMP messages) or IKE messages that arrive without cryptographic protection (e.g., Notify messages complaining about unknown SPIs), because these messages can be forged or sent by attackers. |


## Tested UEs
- Our framework supports the following 26 UEs:
 
  | No. | Device Vendor | Device Model | Android Version | Baseband Vendor | Baseband Model |
  | --- | --- | --- | --- | --- | --- |
  | 1 | Blackcyber | I14 Pro Max | 13  | Qualcomm | Snapdragon 888 |
  | 2 | Blackcyber | I15 Pro Max | 13 | Qualcomm | Snapdragon 8 Gen2 |
  | 3 | Blackview | A55 | 11 | MediaTek | Helio A22 (MT6761) |
  | 4 | Google | Pixel 4a | 13 | Qualcomm | Snapdragon 765G |
  | 5 | Google | Pixel 6a | 12 | Google | Tensor |
  | 6 | HTC | U11 life | 8 | Qualcomm | Snapdragon 630 (SDM630) |
  | 7 | Huawei | Nexus 6P | 8 | Qualcomm | Snapdragon 810 (MSM8994) |
  | 8 | LG | Stylo 6 | 10 | MediaTek | Helio P35 (MT6765) |
  | 9 | LG | Velvet 5G | 10 | Qualcomm | Snapdragon 765G |
  | 10 | Motorola | Moto e5 Plus | 8 | Qualcomm | Snapdragon 430 (MSM8937) |
  | 11 | Motorola | G Power 2021 | 10 | Qualcomm | Snapdragon 662 (SM6115) |
  | 12 | Nokia | G100 | 12 | Qualcomm | Snapdragon 665 |
  | 13 | NUU | B15 (S6701L) | 11 | MediaTek | Helio G80 (MT6768) |
  | 14 | OnePlus | Nord N20 (CPH2459) | 12 | Qualcomm | Snapdragon 695 (SM6375) |
  | 15 | OnePlus | 7T | 11 | Qualcomm | Snapdragon 855 (SM8150) |
  | 16 | Samsung | Galaxy S6 (G920T) | 7 | Samsung | Exynos 7 Octa 7420 |
  | 17 | Samsung | Galaxy A21 | 10 | MediaTek | Helio P35 (MT6765) |
  | 18 | Samsung | Galaxy A21s | 12 | Samsung | Exynos 850 |
  | 19 | Samsung | Galaxy A23 | 12 | Qualcomm | Snapdragon 665 (SM6225) |
  | 20 | Samsung | Galaxy A33 | 13 | Samsung | Exynos 1280 |
  | 21 | Samsung | Galaxy A34 5G | 14 | Samsung | Exynos 1280 |
  | 22 | TCL | 40XL (T608M) | 13 | MediaTek | Helio P35 (MT6765V/CA) |
  | 23 | T-Mobile | Revvl4+ | 10 | MediaTek | Helio A22 (MT6761) |
  | 24 | Ulefone | Note 14 | 12 | MediaTek | Helio A22 (MT6761) |
  | 25 | UMIDIGI | A13 Pro | 11 | Unisoc | T610 |
  | 26 | ZTE | Stage 5G (A2020N3) | 9 | Qualcomm | Snapdragon 855 (SM8150) |

## How to run the testbed
 - UE controller
   - Attach the target UE to a laptop (or a desktop) through a USB cable (UE must be in a USB debugging mode)
   - Run the script ue_controller.py under the control/ue_controller directory by `python3 ue_controller.py`
 - ePDG controller
   - Run the script 310210.sh or 310260.sh according to the IMSI of the target UE
 - IMS controller
   - Run the ims components in the order of P-CSCF, I-CSCF, and S-CSCF
 - Main controller
   - Run the main controller by `java -cp build/libs -jar build/libs/main_controller.jar --config ../config/310210.properties --file ../../testcases/epdg.json


## VoWiFi Property Extraction with RAG

This framework provides a streamlined framework for extracting VoWiFi (Voice over Wi-Fi) properties from RFC documents using Retrieval-Augmented Generation (RAG).


### Prerequisites

- Python 3.8 or higher
- At least 8GB RAM (16GB recommended)
- CUDA-compatible GPU (optional, for faster processing)

## Quick Start

### Installation

1. **Clone the repository**:
   ```bash
   git clone <repository-url>
   cd vowifi-property-extraction-with-rag
   ```

2. **Install dependencies**:
   ```bash
   # Create virtual environment (recommended)
   python -m venv venv
   source venv/bin/activate  # On Windows: venv\Scripts\activate
   
   # Install dependencies
   pip install -r requirements.txt
   ```

## Usage

### Property Extraction from RFCs

The framework automatically extracts VoWiFi properties from RFC documents in the `rfcs/` folder.

#### Basic Usage

```bash
python vowifi_rag_framework.py
```

This will:
- Process all documents in the `rfcs/` folder
- Build a vector knowledge base
- Extract all VoWiFi properties automatically
- Save results to `extracted_properties/vowifi_properties.txt`

## Note
We have been working on the development of an open-source VoWiFi testbed and testing framework for the last 3 years. Recently and concurrently Osmocom (https://osmocom.org/projects/osmo-epdg) open-sourced their Osmo-epdg for creating open-source VoWiFi networks. We are really happy that the community is finally aiming to fill the gap in VoWiFi testbeds and testing. 
