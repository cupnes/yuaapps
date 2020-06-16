#!/bin/bash

# set -uex
set -ue

BIN=$1

NEW_SRC_IP=$2
NEW_DST_IP=$3
NEW_DST_MAC=$4

SRC_IP_OFS=6452
DST_IP_OFS=6462
IP_SZ=4
DST_MAC_OFS=6456
MAC_SZ=6

dump_hex() {
	ofs=$1
	sz=$2
	dd bs=1 if=${BIN} ibs=1 skip=${ofs} count=${sz} 2>/dev/null| od -An -tx1
}

dump() {
	echo -n 'SRC IP :'
	dump_hex ${SRC_IP_OFS} ${IP_SZ}

	echo -n 'DST IP :'
	dump_hex ${DST_IP_OFS} ${IP_SZ}

	echo -n 'DST MAC:'
	dump_hex ${DST_MAC_OFS} ${MAC_SZ}
}

replace() {
	echo -en ${NEW_SRC_IP} \
		| dd of=${BIN} bs=1 seek=${SRC_IP_OFS} conv=notrunc 2>/dev/null
	echo -en ${NEW_DST_IP} \
		| dd of=${BIN} bs=1 seek=${DST_IP_OFS} conv=notrunc 2>/dev/null
	echo -en ${NEW_DST_MAC} \
		| dd of=${BIN} bs=1 seek=${DST_MAC_OFS} conv=notrunc 2>/dev/null
}

dump
replace
dump
