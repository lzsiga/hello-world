# config.sh

CA_DIR_BIN=$(cd $(dirname $0); pwd)

CA_BIN_OPENSSL="$(which openssl)"
CA_DIR_OPENSSL="$($CA_BIN_OPENSSL version -d | sed 's/^.*"\(.*\)"$/\1/')"
CA_CNF_OPENSSL="$CA_DIR_OPENSSL/openssl.cnf"

CA_DIR='/home/projects/TropaCA'
CA_DIR_ROOT="$CA_DIR/RootCA"
CA_DIR_ROOT_KEY="$CA_DIR_ROOT/private"
CA_FILE_ROOT_KEY="$CA_DIR_ROOT_KEY/ca.key.pem"
CA_PWD_ROOT_KEY='InRealApplicationDontEnterHere'

CA_DIR_ROOT_CERT="$CA_DIR_ROOT/mycert"
CA_FILE_ROOT_CERT="$CA_DIR_ROOT_CERT/ca.cert.pem"
CA_FILE_ROOT_CERT_TXT="$CA_DIR_ROOT_CERT/ca.cert.txt"

CA_FILE_ROOT_CNF="$CA_DIR_ROOT/openssl.cnf"

CA_SUBJECT_ROOT='/C=HU/L=Budapest/O=TropaKft/CN=TropaRootCA/emailAddress=ssladmin@tropa.hu'
CA_DAYS_ROOT='9000'
