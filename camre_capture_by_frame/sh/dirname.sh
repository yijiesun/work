#!/bin/bash -v
cd /home/pi/work/westdata
if [ ! -f "savedDirectorsList.txt" ];then
touch savedDirectorsList.txt
else
rm -f savedDirectorsList.txt
fi

cd /home/pi/work/westdata
for dir in $(ls /home/pi/work/westdata)
do
[ -d $dir ] && echo $dir >> /home/pi/work/westdata/savedDirectorsList.txt
done
