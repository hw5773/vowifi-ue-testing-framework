#!/bin/bash

echo "Launching srsepc.sh"

echo "Killing any already running srsepc process"
ps -ef | grep srsepc | grep -v grep | awk '{print $2}' | xargs sudo kill -9

echo "Killiing the mme_statelearner server listening on port 60000"
sudo kill -9 $(lsof -t -i:60000)

echo "Killing done!"

source_dir=`pwd`

var=$(uname -a | grep -c rafiul)

if [ $var > 0 ];
then
    echo "Its my machine ..."
    #cd /home/rafiul/my-computer/research/lte/lte-github/lte-fuzzing/ue-state-fuzzing/srsLTE/build/srsepc/src
    cd /home/rafiul/my-computer/research/lte/code/LTEUE-State-Fuzzing/srsLTE/build/srsepc/src
    #cd /home/cyber2slab/LTEUE-State-Fuzzing/srsLTE/build/srsepc/src
else
    #cd /home/mitziu/LTE/LTEUE-State-Fuzzing/srsLTE/make/srsepc/src
    cd /home/cyber2slab/LTEUE-State-Fuzzing/srsLTE/build/srsepc/src
fi

sudo ./srsepc epc_for_srsue.conf >> /tmp/epc_fuzzing.log
cd "$source_dir"

echo "Finished launching srsepc.sh"
