#!/bin/bash

if [[ "$#" -ne 1 ]]; then
  echo "Illegal number of parameters"
  echo "./wifi-ap.sh <wifi interface name>"
  exit 1
fi

echo "The AP mode is up"
echo "sudo ip address flush $1"
sudo ip address flush $1
echo "sudo ifconfig $1 172.24.1.1 netmask 255.255.255.0"
sudo ifconfig $1 172.24.1.1 netmask 255.255.255.0
echo "sudo dnsmasq -C /etc/dnsmasq.conf"
sudo dnsmasq -C /etc/dnsmasq.conf
echo "sudo hostapd -B /etc/hostapd/hostapd.conf"
sudo hostapd -B /etc/hostapd/hostapd.conf
