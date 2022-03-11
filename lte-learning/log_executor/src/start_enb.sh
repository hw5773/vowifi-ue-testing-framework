#!/bin/bash

echo "Launching srsenb.sh"

echo "Killing any already running srsenb process"
ps -ef | grep srsenb | grep -v grep | awk '{print $2}' | xargs sudo kill -9

echo "Killiing the enodeb_statelearner server listening on port 60001"
sudo kill -9 $(lsof -t -i:60001)

echo "Killing done!"

var=$(uname -a | grep -c rafiul)
if [ $var > 0 ];
then
    echo "Its my machine ..."
    /usr/local/lib/uhd/utils/b2xx_fx3_utils --reset-device
    cd /home/rafiul/my-computer/research/lte/code/LTEUE-State-Fuzzing/srsLTE/build/srsenb/src
    #cd /home/cyber2slab/LTEUE-State-Fuzzing/srsLTE/build/srsenb/src
else
    #cd /home/mitziu/LTE/LTEUE-State-Fuzzing/srsLTE/make/srsenb/src
    cd /home/cyber2slab/LTEUE-State-Fuzzing/srsLTE/build/srsenb/src
fi

sudo ./srsenb enb.conf >> /tmp/enb_fuzzing.log

echo "Finished lauching srsenb.sh"
