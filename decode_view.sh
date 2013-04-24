#!/bin/sh
for var in $@
do
	echo "decode view: $var"
	./ffmpeg -target_view_index $var -y -i ../../Dropbox/Masterarbeit/sequences/ballroom.h264 ../../Test/ballroom_$var.yuv > ../../Test/log_$var.txt 2>&1
done
