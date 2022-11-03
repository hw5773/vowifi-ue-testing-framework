#!/bin/sh
sudo cp /usr/local/etc/strongswan-310260.conf /usr/local/etc/strongswan.conf
sudo src/starter/starter --nofork
