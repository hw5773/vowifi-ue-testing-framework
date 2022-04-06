# How to setup the VoWiFi testbed

0. Overview
 - Operating System: Ubuntu 20.04 (bento)
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

1. VoWiFi settings and source codes of the ePDG/IMS server
 - git clone https://github.com/hw5773/vowifi-core-network.git
 - Let's say the absolute path of the vowifi-core-network directory VOWIFI_ROOT

2. VoWiFi network topology
 -      UE -------- [WiFi AP/DNS] ------ [ePDG] -- [IMS]
 - (Smartphone) ----- (laptop) ------- (desktop) - (VM)

3. VoWiFi SIM card setting
 - H/W: sysmoISIM-SJA2 (ISIM card), SIM card reader/writer
 - S/W: pysim
 
 1) Clone the pysim tool to set up the VoWiFi sim card
  - git clone https://github.com/osmocom/pysim.git
  
 2) SIM writing and setting
  - sudo python3 pySim-prog.py -p 0 -a <ADM value> -n "T-Mobile" -x 310 -y 260 --imsi=<IMSI> --msisdn=<Telephone Number> --ims-hdomain=msg.pc.t-mobile.com --impi=<IMSI>@msg.pc.t-mobile.com --impu=sip:<IMSI>@ims.mnc260.mcc310.3gppnetwork.org --iccid=8901260245784161215 --smsp 542d4d6f62696c65fffffffffffffffff1ffffffffffffffffffffffff07912160130300f4ffffffff0000ff --smsc 12063130004 --opmode 80 --acc 0010 -k <key> -o <OPc>
  - sudo python3 pySim-shell.py -p 0 -a <ADM value> --script VOWIFI_ROOT/scripts/vowifi-setting.script
  
  * Commands with an example value (you can just copy & paste the following command for the two SIM cards and only revise the ADM values)
  a) SIM 1
  - sudo python3 pySim-prog.py -p 0 -a <ADM value> -n "T-Mobile" -x 310 -y 260 --imsi=310260123456780 --msisdn=17657751234 --ims-hdomain=msg.pc.t-mobile.com --impi=310260123456780@msg.pc.t-mobile.com --impu=sip:310260123456780@ims.mnc260.mcc310.3gppnetwork.org --iccid=8901260245784161215 --smsp 542d4d6f62696c65fffffffffffffffff1ffffffffffffffffffffffff07912160130300f4ffffffff0000ff --smsc 12063130004 --opmode 80 --acc 0010 -k 0000000000000000 -o 9999999999999999
  - sudo python3 pySim-shell.py -p 0 -a <ADM value> --script VOWIFI_ROOT/scripts/vowifi-setting.script
  
  b) SIM 2
  - sudo python3 pySim-prog.py -p 0 -a <ADM value> -n "T-Mobile" -x 310 -y 260 --imsi=310260123456781 --msisdn=17657751235 --ims-hdomain=msg.pc.t-mobile.com --impi=310260123456781@msg.pc.t-mobile.com --impu=sip:310260123456781@ims.mnc260.mcc310.3gppnetwork.org --iccid=8901260245784161215 --smsp 542d4d6f62696c65fffffffffffffffff1ffffffffffffffffffffffff07912160130300f4ffffffff0000ff --smsc 12063130004 --opmode 80 --acc 0010 -k 1111111111111111 -o 9999999999999999
  - sudo python3 pySim-shell.py -p 0 -a <ADM value> --script VOWIFI_ROOT/scripts/vowifi-setting.script
  
4. WiFi AP
 - S/W: wifi-ap (or hostapd + dnsmasq)
 - snap install wifi-ap
 - (to be updated)
 
5. ePDG installation and configuration
 1) Installation
 - S/W: Strongswan
 - cd VOWIFI_ROOT/epdg
 - ./configure --prefix=/usr/local --enable-eap-aka --enable-eap-aka-3gpp --enable-eap-aka-3gpp2
 - (Please check if eap-aka, eap-aka-3gpp, and eap-aka-3gpp2 modules are all enabled.)
 - make && sudo make install 
 
 2) Configuration - ipsec.conf
 - The important attributes in this configuration file are as follows:
 - (to be updated)
 
 3) Configuration - strongswan.conf
 - The values in "attr" are provided to UE for the networking information. T-mobile uses 16386 to indicate the IPv6 address of the IMS server (in detail, it is the address of P-CSCF).
 
 * Settings with an example value (you can revise the addresses)
 - attr {
     16386 = fdad:dabb:ed::2
   }
 
 4) Configuration - ipsec.secrets
 - The format (syntax) of the configuration file for the EAP secrets
 - EAP <ID> : <K> <OPc> (The key value and the OPc value are concatenated without any space in-between them)
 
6. IMS server installation and configuration
 1) Install VirtualBox and Vagrant
 - sudo apt-get install virtualbox
 - sudo apt-get install vagrant
 
 2) Create the bridge
 - sudo ip link add vowifi-gw type bridge
 - sudo ip link set vowifi-gw up
 - sudo ip address add fdad:dabb:ed::1 dev vowifi-gw
 - sudo route -A inet6 add fdad:dabb:ed::/64 dev vowifi-gw
 
 3) Initiate the VM
 - cd VOWIFI_ROOT/vagrant
 - vagrant up
 - select vowifi-gw for the bridge
 - vagrant ssh (to get the shell of VM)
 
 4) Network setting in the VM
 - sudo route -A inet6 add default gw via fdad:dabb:ed::1 dev eth1 (the interface name might be different)
 
 1) IMS Installation
 - S/W: Kamailio
 - cd VOWIFI_ROOT/ims
 - 
