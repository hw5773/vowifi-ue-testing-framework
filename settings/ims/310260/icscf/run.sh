#!/bin/sh
sudo mkdir -p /var/run/kamailio_icscf
sudo kamailio -f kamailio.cfg -P /kamailio_icscf.pid -DDeE
