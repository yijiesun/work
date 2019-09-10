#!/bin/bash
cd /home/pi/work/camre_capture_by_frame/log
if [ ! -f "percent.txt" ];then
touch percent.txt
else
rm -f percent.txt
fi
all=`df -h`
echo $all | sed 's/.*T \([0-9]*\)% .*/\1/g' >> /home/pi/work/camre_capture_by_frame/log/percent.txt