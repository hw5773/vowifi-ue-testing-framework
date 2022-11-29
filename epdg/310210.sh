#!/bin/sh
sudo cp /usr/local/etc/strongswan-310210.conf /usr/local/etc/strongswan.conf
sudo src/starter/starter --nofork
