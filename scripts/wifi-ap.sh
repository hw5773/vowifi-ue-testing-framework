#!/bin/bash

if [[ "$#" -ne 2 ]]; then
  echo "Illegal number of parameters"
  echo "./wifi-ap.sh <up/down> <wifi interface name>"
fi

if [[ "$1" == "up" ]]; then
  echo "The AP mode is up"
  echo "sudo ip address flush $2"
  sudo ip address flush $2
  echo "sudo wifi-ap.config set disabled=false"
  sudo wifi-ap.config set disabled=false
  echo "sudo wifi-ap.status restart-ap"
  sudo wifi-ap.status restart-ap
elif [[ "$1" == "down" ]]; then
  echo "The AP mode is down"
  echo "sudo ip address flush $2"
  sudo ip address flush $2
  echo "sudo wifi-ap.config set disabled=true"
  sudo wifi-ap.config set disabled=true
  echo "sudo dhclient -i $2"
  sudo dhclient -i $2
else
  echo "Invalid argument (The first one should be either of up or down)"
fi
