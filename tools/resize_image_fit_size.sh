#!/bin/bash

set -ue
# set -uex

if [ $# -ne 4 ]; then
	echo "$0 src_img dst_img width height" > /dev/stderr
	exit 1
fi

src_img=$1
dst_img=$2
width=$3
height=$4

width_height=$(identify $src_img | cut -d' ' -f3)
src_width=$(echo $width_height | cut -d'x' -f1)
src_height=$(echo $width_height | cut -d'x' -f2)

# echo "${src_img} is ${src_width}x${src_height}"

if [ $src_height -le $src_width ]; then
	convert $src_img -strip -resize ${width}x $dst_img
else
	convert $src_img -strip -resize x${height} $dst_img
fi
