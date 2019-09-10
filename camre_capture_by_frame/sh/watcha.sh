#!/bin/bash
all=`df -h`
#echo $all | sed 's/.*T \([0-9]*\)% .*/\1/g'
echo -n  "mem total used remain percent: "
echo $all | sed 's/.*\/dev\/sda2 \(.*\) \/home\/pi\/work\/westdata.*/\1/g' 