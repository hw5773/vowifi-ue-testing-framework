#/bin/sh

sudo ip link add vowifi-310210 type bridge
sudo ip link set vowifi-310210 up
sudo ip address add fdad:dabb:ed::1 dev vowifi-310210
sudo route -A inet6 add fdad:dabb:ed::/64 dev vowifi-310210

sudo ip link add vowifi-310260 type bridge
sudo ip link set vowifi-310260 up
sudo ip address add fdad:dabb:ee::1 dev vowifi-310260
sudo route -A inet6 add fdad:dabb:ee::/64 dev vowifi-310260
