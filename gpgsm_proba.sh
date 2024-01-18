#!/bin/bash

# fajlok: proba.p12, proba.pwd, rootca.cert, intca.cert
# hivott: openssl-3.x.y

set -u
set -e
set -o pipefail

ConfDir=~/.gnupg
ConfFile="$ConfDir"/common.conf

mkdir -p -- "$ConfDir"
touch -- "$ConfFile" # don't autogenerate

if grep ^use-keyboxd "$ConfFile" 2>/dev/null; then
    mv -- "$ConfFile" "$ConfFile.bak"
    sed s'/^use-keyboxd/#use-keyboxd' <"$ConfFile.bak" >"$ConfFile"
else
    echo '#use-keyboxd' >>"$ConfFile"
fi

openssl pkcs12 -legacy -in proba.p12 -passin file:proba.pwd -nodes -nokeys  >proba.cert
openssl pkcs12 -legacy -in proba.p12 -passin file:proba.pwd -nodes -nocerts >proba.key

cat proba.cert intca.cert rootca.cert>proba_chain.cert
openssl pkcs12 -legacy -export -out proba_chain.p12 -in proba_chain.cert -inkey proba.key -passout file:proba.pwd
rm proba.key

STDOPT='--disable-crl-checks
        --disable-policy-checks
        --disable-ocsp
        --disable-trusted-cert-crl-check'

PWDOPT="$STDOPT --pinentry-mode loopback --passphrase-fd 0"

gpgsm $STDOPT --pinentry-mode loopback --passphrase-fd 0 --import proba_rebuilt.p12 <proba.pwd 2>&1 | tee proba_01.log
RC=$?
(gpgsm --dump-cert; gpgsm -K) >proba_rebuilt.dump
if [ $RC -eq 0 ]; then
    echo "ujraepitett proba.p12 importalasa sikerult"
else
    echo "ujraepitett proba.p12 importalasa NEM sikerult, probaljunk mast"
fi

echo 'Most kellene lecsapni a pgp-agent -et, dirmngrd-t, scdaemon-t, meg meg kitudja mi mast'
