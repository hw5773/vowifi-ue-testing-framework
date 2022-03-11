#!/bin/bash

echo "Launching srsepc.sh"

echo "Killing any already running srsepc process"
sudo pkill -9 -f srsepc
ps -ef | grep srsepc | grep -v grep | awk '{print $2}' | xargs sudo kill -9

echo "Kiliing the mme_statelearner server listening on port 60000"
#sudo kill $(lsof -t -i:60000)
sudo kill -9 $(lsof -t -i:60000)

echo "Killing done!"

source_dir=`pwd`

var=$(uname -a | grep -c rafiul)

if [ $var > 0 ];
then
    echo "Its my machine ..."
    #cd /home/cyber2slab/Documents/LTEUE-State-Fuzzing/srsLTE-release_19_03/srsLTE/build/srsepc/src
    cd /home/rafiul/my-computer/research/lte/code/LTEUE-State-Fuzzing/srsLTE-release_19_03/build/srsepc/src

else
    cd /home/cyber2slab/Documents/LTEUE-State-Fuzzing/srsLTE-release_19_03/srsLTE/build/srsepc/src
fi
#cd /home/rafiul/my-computer/research/lte/code/LTEUE-State-Fuzzing/srsLTE-release_19_03/build/srsepc/src

sudo ./srsepc epc.conf &> /tmp/epc_fuzzing.log
cd "$source_dir"

echo "Finished launching srsepc.sh"
