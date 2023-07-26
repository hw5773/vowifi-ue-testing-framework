#!/bin/sh

sudo mkdir -p /var/run/kamailio_pcscf

if [ $# -eq 0 ]; then
  sudo kamailio -f kamailio.cfg -P /kamailio_pcscf.pid -DDeE
elif [ $# -eq 1 ]; then
  if [ "$1" = "pixel" ]; then
    sudo kamailio -f kamailio-pixel.cfg -P /kamailio_pcscf.pid -DDeE
  else
    sudo kamailio -f kamailio.cfg -P /kamailio_pcscf.pid -DDeE
  fi
else
  echo "invalid command"
fi
