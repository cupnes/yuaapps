#!/bin/bash

set -uex

if [ $# -ne 5 ]; then
	echo "$0 html_dir dst_dir width height slide_name" >&2
	exit 1
fi

tools_dir=$(dirname $0)
html_dir=$1
dst_dir=$2
width=$3
height=$4
slide_name=$5

img_dir=png

${tools_dir}/extract_png_from_odp_html.sh ${html_dir} ${img_dir} ${slide_name}

${tools_dir}/convert_images_to_slide.sh ${img_dir} ${dst_dir} ${width} ${height} ${slide_name}
