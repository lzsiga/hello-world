#!/bin/sh

MyPath="$(realpath $(dirname "$0"))"

SingleFile () {

if [ '!' -f "$1" ]; then
    echo "usage: $0 <certfile>"
    exit 1
fi

if [ '!' -w "$1" ]; then
    echo "$1 should be writable"
    exit 1
fi

export LC_ALL=C

set -e

cp -p "$1" "$1.tmp"

(openssl x509 -in "$1.tmp" -noout -text -fingerprint | awk '
    /^.* Fingerprint=/           { split ($0,arr,"="); fp= arr[2]; }
    /^        Issuer:/           { i= substr($0,17);
                                   gsub(" = ", "=", i); }
    /^            Not Before:/   { nb= substr($0,25); }
    /^            Not After :/   { na= substr($0,25); }
    /^        Subject:/          { s= substr($0,17);
                                   gsub(" = ", "=", s); }
    END { print "s:" s "\ni: " i "\nnb: " nb "\nna: " na"\nfp: " fp; }' |
    $MyPath/cert_bs_ekezet.sh
cat "$1.tmp") >"$1"

rm "$1.tmp"
}

for i in "$@"; do
    SingleFile "$i"
done
