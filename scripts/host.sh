#/bin/sh

sudo ip link add vowifi-gw type bridge
sudo ip link set vowifi-gw up
sudo ip address add fdad:dabb:ed::1 dev vowifi-gw
sudo route -A inet6 add fdad:dabb:ed::/64 dev vowifi-gw
