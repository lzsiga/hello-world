#!/bin/sh

SedProgr='s/\\C3\\A1/á/g
	  s/\\C3\\81/Á/g
	  s/\\C3\\A9/é/g
	  s/\\C3\\89/É/g
	  s/\\C3\\AD/í/g
	  s/\\C3\\8D/Í/g
	  s/\\C3\\B3/ó/g
	  s/\\C3\\93/Ó/g
	  s/\\C3\\B6/ö/g
	  s/\\C3\\96/Ö/g
	  s/\\C5\\91/ő/g
	  s/\\C5\\90/Ő/g
	  s/\\C3\\BA/ú/g
	  s/\\C3\\9A/Ú/g
	  s/\\C3\\BC/ü/g
	  s/\\C3\\9C/Ü/g
	  s/\\C5\\B1/ű/g
	  s/\\C5\\B0/Ű/g
	  s/\\C3\\A4/ä/g
	  s/\\C3\\84/Ä/g
	  s/\\xC3\\xA1/á/g
	  s/\\xC3\\x81/Á/g
	  s/\\xC3\\xA9/é/g
	  s/\\xC3\\x89/É/g
	  s/\\xC3\\xAD/í/g
	  s/\\xC3\\x8D/Í/g
	  s/\\xC3\\xB3/ó/g
	  s/\\xC3\\x93/Ó/g
	  s/\\xC3\\xB6/ö/g
	  s/\\xC3\\x96/Ö/g
	  s/\\xC5\\x91/ő/g
	  s/\\xC5\\x90/Ő/g
	  s/\\xC3\\xBA/ú/g
	  s/\\xC3\\x9A/Ú/g
	  s/\\xC3\\xBC/ü/g
	  s/\\xC3\\x9C/Ü/g
	  s/\\xC5\\xB1/ű/g
	  s/\\xC5\\xB0/Ű/g
	  s/\\xC3\\xA4/ä/g
	  s/\\xC3\\x84/Ä/g'

if [ -z "$1" ]; then # pipemode
    sed "$SedProgr"
else
    for i in "$@"; do
	sed "$SedProgr" "$i" >"$i.tmp"
	mv "$i.tmp" "$i"
    done
fi
