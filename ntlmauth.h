/* ntlmauth.h */

#ifndef NTLMAUTH_H
#define NTLMAUTH_H

#include "buffdata.h"

#define NTLM_PROTOCOL "NTLMSSP"
#define NTLM_TYPE1    1
#define NTLM_TYPE2    2
#define NTLM_TYPE3    3
/*
#define NTLM_FLAGS1   ((short)0xb203)
#define NTLM_FLAGS2   ((short)0x8201)
#define NTLM_FLAGS3   ((short)0x8201)
*/
#define NTLM_RESPLEN  0x18

#define NTLM_FLAGS1 (NTLMFLAG_NEGOTIATE_UNICODE|NTLMFLAG_REQUEST_TARGET|\
		     NTLMFLAG_NEGOTIATE_NTLM_KEY|NTLMFLAG_NEGOTIATE_ALWAYS_SIGN)
#define NTLM_FLAGS3 (NTLMFLAG_NEGOTIATE_UNICODE|NTLMFLAG_NEGOTIATE_NTLM_KEY)

/* Flag bits definitions available at on 
   http://davenport.sourceforge.net/ntlm.html */

#define NTLMFLAG_NEGOTIATE_UNICODE               (1<<0) /* 0x001 */
#define NTLMFLAG_NEGOTIATE_OEM                   (1<<1) /* 0x002 */
#define NTLMFLAG_REQUEST_TARGET                  (1<<2) /* 0x004 */
/* unknown (1<<3) */
#define NTLMFLAG_NEGOTIATE_SIGN                  (1<<4) /* 0x010 */
#define NTLMFLAG_NEGOTIATE_SEAL                  (1<<5) /* 0x020 */
#define NTLMFLAG_NEGOTIATE_DATAGRAM_STYLE        (1<<6) /* 0x040 */
#define NTLMFLAG_NEGOTIATE_LM_KEY                (1<<7) /* 0x080 */
#define NTLMFLAG_NEGOTIATE_NETWARE               (1<<8) /* 0x100 */
#define NTLMFLAG_NEGOTIATE_NTLM_KEY              (1<<9) /* 0x200 */
/* unknown (1<<10) */
/* unknown (1<<11) */
#define NTLMFLAG_NEGOTIATE_DOMAIN_SUPPLIED       (1<<12) /* 0x00001000 */
#define NTLMFLAG_NEGOTIATE_WORKSTATION_SUPPLIED  (1<<13) /* 0x00002000 */
#define NTLMFLAG_NEGOTIATE_LOCAL_CALL            (1<<14) /* 0x00004000 */
#define NTLMFLAG_NEGOTIATE_ALWAYS_SIGN           (1<<15) /* 0x00008000 */
#define NTLMFLAG_TARGET_TYPE_DOMAIN              (1<<16) /* 0x00010000 */
#define NTLMFLAG_TARGET_TYPE_SERVER              (1<<17) /* 0x00020000 */
#define NTLMFLAG_TARGET_TYPE_SHARE               (1<<18) /* 0x00040000 */
#define NTLMFLAG_NEGOTIATE_NTLM2_KEY             (1<<19) /* 0x00080000 */
#define NTLMFLAG_REQUEST_INIT_RESPONSE           (1<<20) /* 0x00100000 */
#define NTLMFLAG_REQUEST_ACCEPT_RESPONSE         (1<<21) /* 0x00200000 */
#define NTLMFLAG_REQUEST_NONNT_SESSION_KEY       (1<<22) /* 0x00400000 */
#define NTLMFLAG_NEGOTIATE_TARGET_INFO           (1<<23) /* 0x00800000 */
/* unknown (1<24) */					 /* 0x01000000 */
/* unknown (1<25) */					 /* 0x02000000 */
/* unknown (1<26) */					 /* 0x04000000 */
/* unknown (1<27) */					 /* 0x08000000 */
/* unknown (1<28) */					 /* 0x10000000 */
#define NTLMFLAG_NEGOTIATE_128                   (1<<29) /* 0x20000000 */
#define NTLMFLAG_NEGOTIATE_KEY_EXCHANGE          (1<<30) /* 0x40000000 */
#define NTLMFLAG_NEGOTIATE_56                    (1<<31) /* 0x80000000 */

/* all numbers are unsigned, little endian byte order !!! */

typedef struct NtlmBuff {
    short len;		/* length */
    short len2;		/* repeat length */
    int   off;		/* offset */
} NtlmBuff;

typedef struct NTLM1 {
    char protocol [8];		/* NTLM_PROTOCOL */
    int msgtype;		/* NTLM_TYPE1    */
    unsigned int flags;		/* NTLM_FLAGS1 */

    NtlmBuff dom;		/* buffer at 0x20 + host_len */
    NtlmBuff host;		/* buffer at 0x20 */
} NTLM1;

typedef struct NTLM2 {
    char protocol [8];		/* NTLM_PROTOCOL */
    int  msgtype;		/* NTLM_TYPE2    */
    NtlmBuff target;

    unsigned int flags;		/* NTLM_FLAGS2 */
    char  nonce [8];		/* nonce */
    char  context [8];		/* something */
} NTLM2;

typedef struct NTLM3 {
    char protocol [8];		/* NTLM_PROTOCOL */
    int  msgtype;		/* NTLM_TYPE3    */

    NtlmBuff lm;		/* LM response */
    NtlmBuff nt;		/* NT response */
    NtlmBuff dom;		/* DOMAIN */
    NtlmBuff user;		/* USER */
    NtlmBuff host;		/* HOST */
    NtlmBuff session;		/* session key */

    unsigned int flags;		/* NTLM_FLAGS3 */

    /* the domain string   - UTF-16LE */
    /* the username string - UTF-16LE */
    /* the host string     - UTF-16LE */
    /* LanManager response 0x18 bytes */
    /* NT response 0x18 bytes */

} NTLM3;

#define NTLMO_DEFAULT  0 /* only the structure */
#define NTLMO_BASE64   1 /* convert output to BASE64 */
#define NTLMO_HTTP     2 /* create line: WWW-Authenticate: NTLM <base64>\n */
#define NTLMO_HTTPCRLF 3 /* create line: WWW-Authenticate: NTLM <base64>\r\n */

/* 'host' and 'domain' should be in uppercase */
/* 'user' and 'passwd' are case-sensitive */

extern int CreateNtlm1 (int opt, Buffer *into, 
                        const char *host, const char *domain);

extern int ParseNtlm2 (int opt, char nonce[8], const ConstBuffData *from);

extern int CreateNtlm3 (int opt, Buffer *into, const char nonce [8],
                        const char *host, const char *domain,
			const char *user, const char *passwd);

#endif
