#!/bin/bash
# root_gen.sh

# run this once to create the rootCA key/cert

set -eu
set -o pipefail

. config.sh
mkdir -p "$CA_DIR_ROOT_KEY" "$CA_DIR_ROOT_CERT"
chmod 0700 "$CA_DIR_ROOT_KEY"

openssl genrsa -aes256 -passout pass:"$CA_PWD_ROOT_KEY" -out "$CA_FILE_ROOT_KEY" 4096
echo "$CA_FILE_ROOT_KEY has been generated"

CA_FILE_ROOT_CERT="$CA_DIR_ROOT_CERT/ca.cert.pem"

sed 's;^dir\s*=.*$;dir='"$CA_DIR_ROOT;" \
    "$CA_CNF_OPENSSL" >"$CA_FILE_ROOT_CNF"

openssl req \
    -passin pass:"$CA_PWD_ROOT_KEY" \
    -config "$CA_FILE_ROOT_CNF" -subj "$CA_SUBJECT_ROOT" \
    -key "$CA_FILE_ROOT_KEY" \
    -new -x509 -days "$CA_DAYS_ROOT" -sha256 -extensions v3_ca \
    -out "$CA_FILE_ROOT_CERT"
"$CA_DIR_BIN/cert-addheader.sh" "$CA_FILE_ROOT_CERT"
echo "$CA_FILE_ROOT_CERT has been generated"

openssl x509 -in "$CA_FILE_ROOT_CERT" -out "$CA_FILE_ROOT_CERT_TXT" -noout -text
echo "$CA_FILE_ROOT_CERT_TXT has been generated"
