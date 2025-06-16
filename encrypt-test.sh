#!/usr/local/bin/bash

Subj='/C=HU/L=Budapest/O=Initech/CN=TestCN/emailAddress=recipient@email.com'

DoOne() {
    local Name="$1"
    local Alg="$2"
    local Opts="$3"
    local KeyFile="encrypt-test.$Name.key"
    local CertFile="encrypt-test.$Name.cert"
    local MailFile="encrypt-test.$Name.mail.enc"
    printf '\nTrying %s algorithm %s\n' "$Name" "$Alg"
    openssl genpkey -quiet -out "$KeyFile" -algorithm "$Alg" $Opts
    printf 'Key generated %s\n' "$KeyFile"
    if openssl req -x509 -key "$KeyFile" -out "$CertFile" -days 365 -nodes \
        -subj "$Subj" \
        -addext "keyUsage=digitalSignature, keyEncipherment, dataEncipherment" \
        -addext "extendedKeyUsage=emailProtection" \
        -addext "basicConstraints=critical, CA:false";
    then
        printf 'Cert generated %s\n' "$CertFile"
    else
        printf 'Cert not generated\n'
        return
    fi
    openssl x509 -in "$CertFile" -text -noout -out "$CertFile.txt"
    if openssl smime -encrypt -in ../encrypt-test.sh \
                -from testuser@email.com \
                -to recipient@email.com \
                -subject "$Alg Encrypted message" \
                -AES128 -out "$MailFile" "$CertFile";
    then
        printf 'Encrypted file %s created\n' "$MailFile"
    else
        printf 'Encryption did not work\n'
        return
    fi
}

  mkdir -p encrypt-test
  rm -rf excrypt-test/* 2>/dev/null
  cd encrypt-test/

  DoOne rsa     RSA     "-pkeyopt rsa_keygen_bits:2048"
  DoOne rsa-pss RSA-PSS "-pkeyopt rsa_keygen_bits:2048"
  DoOne ec      EC      "-pkeyopt ec_paramgen_curve:P-256"
  DoOne ec-sm2  EC      "-pkeyopt ec_paramgen_curve:SM2"
  DoOne x255919 X25519  " "
  DoOne x448    X448    " "
  DoOne ed25519 ED25519 " "
  DoOne ed448   ED448   " "
# DoOne ml-dsa  ML-DSA  " "
# DoOne ml-kem  ML-KEM  " "

exit

4037C5FDE97F0000:error:10800096:PKCS7 routines:PKCS7_RECIP_INFO_set:
encryption not supported for this key type:crypto/pkcs7/pk7_lib.c:653

asn1.h:
     23 struct evp_pkey_asn1_method_st {
     53     int (*pkey_ctrl) (EVP_PKEY *pkey, int op, long arg1, void *arg2);

evp.h:
    668 struct evp_pkey_st {
    678     const EVP_PKEY_ASN1_METHOD *ameth;

   1607 # define ASN1_PKEY_CTRL_PKCS7_SIGN       0x1
   1608 # define ASN1_PKEY_CTRL_PKCS7_ENCRYPT    0x2
   1609 # define ASN1_PKEY_CTRL_DEFAULT_MD_NID   0x3
   1610 # define ASN1_PKEY_CTRL_CMS_SIGN         0x5
   1611 # define ASN1_PKEY_CTRL_CMS_ENVELOPE     0x7
   1612 # define ASN1_PKEY_CTRL_CMS_RI_TYPE      0x8

pk7_lib.c:
    628     EVP_PKEY *pkey = NULL;

    640     pkey = X509_get0_pubkey(x509);

    652     if (pkey->ameth == NULL || pkey->ameth->pkey_ctrl == NULL) {
    653         ERR_raise(ERR_LIB_PKCS7,
    654                   PKCS7_R_ENCRYPTION_NOT_SUPPORTED_FOR_THIS_KEY_TYPE);
    655         goto err;
    656     }
    657
    658     ret = pkey->ameth->pkey_ctrl(pkey, ASN1_PKEY_CTRL_PKCS7_ENCRYPT, 0, p7i);
    659     if (ret == -2) {
    660         ERR_raise(ERR_LIB_PKCS7,
    661                   PKCS7_R_ENCRYPTION_NOT_SUPPORTED_FOR_THIS_KEY_TYPE);
    662         goto err;
    663     }

rsa_ossl.c:
     41     rsa_ossl_public_encrypt,
     42     rsa_ossl_public_decrypt,     /* signature verification */
     43     rsa_ossl_private_encrypt,    /* signing */
     44     rsa_ossl_private_decrypt,
