#!/bin/sh
time3=$(date "+%Y-%m-%d %H:%M:%S")
echo $time3 >> /home/pi/work/camre_capture_by_frame/log/everyday.txt
phddns restart >> /home/pi/work/camre_capture_by_frame/log/everyday.txt
cd /home/pi/nat123
mono  nat123linux.sh service >> /home/pi/work/camre_capture_by_frame/log/everyday.txt &
