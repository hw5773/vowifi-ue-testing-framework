#/bin/sh

sudo ip link add sip1 type dummy
sudo ip link set sip1 up
sudo ip address add fdad:dabb:ed::1 dev sip1

sudo sysctl -w net.ipv4.ip_forward=1
sudo sysctl -w net.ipv6.conf.all.forwarding=1
