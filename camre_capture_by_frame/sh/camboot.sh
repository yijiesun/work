#!/bin/bash
mount -a
cd /home/pi/work/camre_capture_by_frame/sh
./dirname.sh
cd /home/pi/work/camre_capture_by_frame/build
./cam_cap /home/pi/work/westdata/ 1 1920 1080 10 1 0.005 15 30 jpg |tee log.txt
#                  1              2   3    4  5  6   7    8  9  10
:<<!
* argv[1] 抓取的图片保存的地址
* argv[2] 间隔多少秒抓一帧
* argv[3] 分辨率width
* argv[4] 分辨率height
* argv[5] 低于多少内存的时候开始删除最早的数据（用线程）
* argv[6] 低于警戒线内存的时候开始疯狂删除最早的数据
* argv[7] 画面中运动检测超过多少则视为有运动物体（总数为1）
* argv[8] 检测到运动后会保存一个视频，缓存运动之前之后多少帧
* argv[9] 保存的运动视频最高不超过多少秒
* argv[10] 保存的图片格式jpg/bmp/png
!