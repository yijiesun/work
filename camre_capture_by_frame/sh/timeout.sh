#!/bin/bash -v
export LD_PRELOAD=/usr/lib/arm-linux-gnueabihf/libv4l/v4l1compat.so

rmmod uvcvideo
modprobe uvcvideo nodrop=1 timeout=5000 quirks=0x80