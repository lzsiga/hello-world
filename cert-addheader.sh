#!/bin/sh

MyPath="$(cd $(dirname "$0"); pwd)"

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

({ openssl x509 -in "$1.tmp" -noout -text -serial -fingerprint -sha1;
   openssl x509 -in "$1.tmp" -noout -fingerprint -md5; } | awk '
    {
      if (store=="ski" || store=="aki") {
          sub("^keyid:", "", $1);
          if      (store=="ski") ski= $1;
          else if (store=="aki") aki= $1;
          store= "";
          next;
      } else if (match ($0, "X509v3 Subject Key Identifier:")) {
          store= "ski";
          next;
      } else if (match ($0, "X509v3 Authority Key Identifier:")) {
          store= "aki";
          next;
      } else {
          store= "";
          /* _not_ next */
      }
    }
    /^(MD5|md5) Fingerprint=/    { split ($0,arr,"="); fpmd5= arr[2]; }
    /^(SHA1|sha1) Fingerprint=/  { split ($0,arr,"="); fpsha1=arr[2]; }
    /^        Issuer:/           { i= substr($0,17);
                                   gsub(" = ", "=", i); }
    /^            Not Before:/   { nb= substr($0,25); }
    /^            Not After :/   { na= substr($0,25); }
    /^        Subject:/          { s= substr($0,17);
                                   gsub(" = ", "=", s); }
    /^serial=/                   { split ($0,arr,"="); sn= arr[2]; }
    END { print "s:" s "\ni: " i "\nnb: " nb "\nna: " na;
          if (ski!="")    print "SKI:  " ski;
          if (aki!="")    print "AKI:  " aki;
          if (sn!="")     print "sn:   " sn;
          if (fpmd5!="")  print "md5:  " fpmd5;
          if (fpsha1!="") print "sha1: " fpsha1;
          }' |
    $MyPath/cert_bs_ekezet.sh
cat "$1.tmp") >"$1"

rm "$1.tmp"
}

for i in "$@"; do
    SingleFile "$i"
done
