#!/bin/bash

echo "Killing srsEPC"

sudo pkill -9 -f srsepc
echo "Killing any already running srsepc process"
ps -ef | grep srsepc | grep -v grep | awk '{print $2}' | xargs sudo kill -9

echo "Kiliing the mme_statelearner server listening on port 60000"
sudo kill -9 $(lsof -t -i:60000)

echo "Killed srsEPC"
