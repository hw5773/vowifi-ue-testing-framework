#!/bin/bash

echo "Killing srseNodeB"

sudo pkill -9 -f srsenb
ps -ef | grep srsenb | grep -v grep | awk '{print $2}' | xargs sudo kill -9

echo "Killing the enodeb_statelearner server listening on port 60001"
sudo kill -9 $(lsof -t -i:60001)
/usr/local/lib/uhd/utils/b2xx_fx3_utils --reset-device

echo "Killed srseNodeB"
