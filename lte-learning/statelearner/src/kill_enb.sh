#!/bin/bash

source_dir=`pwd`

echo "Killing srseNodeB"

sudo pkill -9 -f srsenb
ps -ef | grep srsenb | grep -v grep | awk '{print $2}' | xargs sudo kill -9

/usr/local/lib/uhd/utils/b2xx_fx3_utils --reset-device

for i in /sys/bus/pci/drivers/xhci_hcd/*:*; do

  [ -e "$i" ] || continue

  echo "${i##*/}" > "${i%/*}/unbind"

  echo "${i##*/}" > "${i%/*}/bind"

done

echo "Kiliing the enodeb_statelearner server listening on port 60001"
sudo kill -9 $(lsof -t -i:60001)

echo "Killed srseNodeB"

echo "Restarting Networking..."
sudo /etc/init.d/networking restart

