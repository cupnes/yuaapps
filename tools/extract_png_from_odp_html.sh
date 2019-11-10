#!/bin/bash

set -uex

if [ $# -ne 3 ]; then
	echo "$0 html_dir dst_dir prefix" >&2
	exit 1
fi

tools_dir=$(dirname $0)
html_dir=$1
dst_dir=$2
prefix=$3

if [ -e ${dst_dir} -a ! -d ${dst_dir} ]; then
	echo "${dst_dir} is not a directory." >&2
	exit 1
fi
mkdir -p ${dst_dir}

num_list=$(cd ${html_dir} && ls img*.png | cut -c4- | rev | cut -d'.' -f2- | rev | sort -n)
for n in ${num_list}; do
	cp ${html_dir}/img${n}.png ${dst_dir}/${prefix}_$(printf '%03d' ${n}).png
done
