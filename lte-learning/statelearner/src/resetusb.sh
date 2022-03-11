#!/bin/bash

for i in /sys/bus/pci/drivers/xhci_hcd/*:*; do

  [ -e "$i" ] || continue

  echo "${i##*/}" > "${i%/*}/unbind"

  echo "${i##*/}" > "${i%/*}/bind"

done
