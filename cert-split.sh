#!/bin/sh

if [ $# -eq 0 ]; then
    echo "usge: $0 <certficate-bundle>"
    exit 1;
fi

if [ '!' -r "$1" ]; then
    echo "usage: file '$1' missing or unusable"
    exit 1;
fi

awk '
BEGIN { outfile=sprintf ("part%d.cert.pem", ++cnt); }
$0=="-----BEGIN CERTIFICATE-----" {
     state=1;
     print $0 >outfile;
     next; }
$0=="-----END CERTIFICATE-----" {
     state=0; print $0 >outfile;
     close (outfile);
     outfile=sprintf ("part%d.cert.pem", ++cnt);
     next; }
$0=="" { next; }
{} { print $0 >outfile; }
END { printf ("Totally %d parts have been written (\"part*.cert.pem\")\n", cnt-1); }' <"$1"
