#!/bin/sh
sudo mkdir -p /var/run/kamailio_scscf
sudo kamailio -f kamailio.cfg -P /kamailio_scscf.pid -DDeE
