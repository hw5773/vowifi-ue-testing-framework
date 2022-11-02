#!/bin/sh
sudo mkdir -p /var/run/kamailio_pcscf
sudo kamailio -f kamailio.cfg -P /kamailio_pcscf.pid -DDeE
