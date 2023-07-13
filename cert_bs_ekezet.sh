#!/bin/sh

export LC_ALL=C

SedProgr='s/\\C3\\A1/�/g
	  s/\\C3\\81/�/g
	  s/\\C3\\A9/�/g
	  s/\\C3\\89/�/g
	  s/\\C3\\AD/�/g
	  s/\\C3\\8D/�/g
	  s/\\C3\\B3/�/g
	  s/\\C3\\93/�/g
	  s/\\C3\\B6/�/g
	  s/\\C3\\96/�/g
	  s/\\C5\\91/�/g
	  s/\\C5\\90/�/g
	  s/\\C3\\BA/�/g
	  s/\\C3\\9A/�/g
	  s/\\C3\\BC/�/g
	  s/\\C3\\9C/�/g
	  s/\\C5\\B1/�/g
	  s/\\C5\\B0/�/g
	  s/\\C3\\A4/�/g
	  s/\\C3\\84/�/g
	  s/\\xC3\\xA1/�/g
	  s/\\xC3\\x81/�/g
	  s/\\xC3\\xA9/�/g
	  s/\\xC3\\x89/�/g
	  s/\\xC3\\xAD/�/g
	  s/\\xC3\\x8D/�/g
	  s/\\xC3\\xB3/�/g
	  s/\\xC3\\x93/�/g
	  s/\\xC3\\xB6/�/g
	  s/\\xC3\\x96/�/g
	  s/\\xC5\\x91/�/g
	  s/\\xC5\\x90/�/g
	  s/\\xC3\\xBA/�/g
	  s/\\xC3\\x9A/�/g
	  s/\\xC3\\xBC/�/g
	  s/\\xC3\\x9C/�/g
	  s/\\xC5\\xB1/�/g
	  s/\\xC5\\xB0/�/g
	  s/\\xC3\\xA4/�/g
	  s/\\xC3\\x84/�/g'

if [ -z "$1" ]; then # pipemode
    sed "$SedProgr"
else
    for i in "$@"; do
	sed "$SedProgr" "$i" >"$i.tmp"
	mv "$i.tmp" "$i"
    done
fi
