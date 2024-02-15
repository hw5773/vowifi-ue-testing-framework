#!/bin/bash

if [[ "$#" -ne 2 ]]; then
  echo "Illegal number of parameters"
  echo "./ap_setting.sh <wifi interface name> <internet interface name>"
  exit 1
fi

echo "sudo sysctl -w net.ipv4.ip_forward=1"
sudo sysctl -w net.ipv4.ip_forward=1
echo "sudo sysctl -w net.ipv6.conf.all.forwarding=1"
sudo sysctl -w net.ipv6.conf.all.forwarding=1
echo "sudo iptables -t nat -A POSTROUTING -o $2 -j MASQUERADE"
sudo iptables -t nat -A POSTROUTING -o $2 -j MASQUERADE
echo "sudo iptables -A FORWARD -i $2 -o $1 -m state --state RELATED,ESTABLISHED -j ACCEPT"
sudo iptables -A FORWARD -i $2 -o $1 -m state --state RELATED,ESTABLISHED -j ACCEPT
echo "sudo iptables -A FORWARD -i $1 -o $2 -j ACCEPT"
sudo iptables -A FORWARD -i $1 -o $2 -j ACCEPT
