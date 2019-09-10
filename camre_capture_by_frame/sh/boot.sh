#!/bin/sh
time3=$(date "+%Y-%m-%d %H:%M:%S")
echo $time3 >> /home/pi/work/camre_capture_by_frame/log/login.txt
cd /home/pi/work/camre_capture_by_frame/sh
sh timeout.sh >> /home/pi/work/camre_capture_by_frame/log/login.txt
phddns restart >>/home/pi/work/camre_capture_by_frame/log/login.txt
cd /home/pi/work/nat123
mono  nat123linux.sh service >> /home/pi/work/camre_capture_by_frame/log/login.txt & 
