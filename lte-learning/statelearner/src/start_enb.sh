#!/bin/bash


echo "Launching srsenb.sh"

echo "Killing any already running srsenb process"
sudo pkill -9 -f srsenb
ps -ef | grep srsenb | grep -v grep | awk '{print $2}' | xargs sudo kill -9

echo "Kiliing the enodeb_statelearner server listening on port 60000"
#sudo kill $(lsof -t -i:60001)
sudo kill -9 $(lsof -t -i:60001)

source_dir=`pwd`

var=$(uname -a | grep -c rafiul)
if [ $var > 0 ];
then
    echo "Its my machine ..."
    /usr/local/lib/uhd/utils/b2xx_fx3_utils --reset-device
    #cd /home/cyber2slab/Documents/LTEUE-State-Fuzzing/srsLTE-release_19_03/srsLTE/build/srsenb/src
    cd /home/rafiul/my-computer/research/lte/code/LTEUE-State-Fuzzing/srsLTE-release_19_03/build/srsenb/src

else
    cd /home/cyber2slab/Documents/LTEUE-State-Fuzzing/srsLTE-release_19_03/srsLTE/build/srsenb/src
fi

#cd /home/rafiul/my-computer/research/lte/code/LTEUE-State-Fuzzing/srsLTE-release_19_03/build/srsenb/src

sudo ./srsenb enb.conf &> /tmp/enb_fuzzing.log
cd "$source_dir"

echo "Finished lauching srsenb.sh"
