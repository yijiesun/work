#!/bin/bash
cd /home/pi/work/frame
if [ ! -f "savedDirectorsList.txt" ];then
touch savedDirectorsList.txt
else
rm -f savedDirectorsList.txt
fi

cd /home/pi/work/frame
for dir in $(ls /home/pi/work/frame)
do
[ -d $dir ] && echo $dir >> /home/pi/work/frame/savedDirectorsList.txt
done
