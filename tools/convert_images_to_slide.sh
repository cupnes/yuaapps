#!/bin/bash

set -uex

if [ $# -ne 5 ]; then
	echo "$0 img_dir dst_dir width height slide_name" >&2
	exit 1
fi

tools_dir=$(dirname $0)
img_dir=$1
dst_dir=$2
width=$3
height=$4
slide_name=$5

if [ -e ${dst_dir} -a ! -d ${dst_dir} ]; then
	echo "${dst_dir} is not a directory." >&2
	exit 1
fi
mkdir -p ${dst_dir}

for img in $(ls ${img_dir}); do
	${tools_dir}/convert_image_force_resl.sh ${img_dir}/${img} ${dst_dir} ${width} ${height}
done

ls ${dst_dir} > s.${slide_name}
mv s.${slide_name} ${dst_dir}
