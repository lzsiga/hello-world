/* ntlmauth.c */

#include <alloca.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <openssl/md4.h>
#include <openssl/des.h>

#include "buffdata.h"
#include "base64.h"
#include "translay.h"
#include "byteord.h"

#include "ntlmauth.h"

size_t strnlen (const char *, size_t);

static const char AuthS [] = "WWW-Authenticate: NTLM ";
static const char AuthC [] = "Authorization: NTLM ";

static size_t UniCopy (int funi, char *to, const char *from, size_t len)
{
    const char *p = from;
    char *q = to;

    for (; len>0; --len) {
	*q++ = *p++;
	if (funi) *q++ = '\0';
    }
    return (size_t)(q-to);
}

int CreateNtlm1 (int opt, Buffer *into,
		 const char *host, const char *domain)
{
    size_t hlen, dlen, nlen, reqlen, blen;
    NTLM1 *ntlm1;
    char *p, *p0;
    int funi= 1;
    int flags;

    if (sizeof (NTLM1) != 0x20) {
	fprintf (stderr, "CreateNtlm1: Invalid \"ntlmauth.h\" header\n");
	exit (32);
    }

    hlen = strlen (host);
    dlen = strlen (domain);

    flags = NTLMFLAG_REQUEST_TARGET|
	    NTLMFLAG_NEGOTIATE_NTLM_KEY|NTLMFLAG_NEGOTIATE_ALWAYS_SIGN;

    if (funi) {
	nlen = sizeof (NTLM1) + 2*hlen + 2*dlen;
	flags |= NTLMFLAG_NEGOTIATE_UNICODE;
    } else {
	nlen = sizeof (NTLM1) + hlen + dlen;
	flags |= NTLMFLAG_NEGOTIATE_OEM;
    }

    reqlen = nlen;
    if (opt & (NTLMO_BASE64|NTLMO_HTTP)) {
	reqlen = (reqlen+2) / 3 * 4;
    }
    if (opt & NTLMO_HTTP) {
	reqlen += sizeof (AuthC) -1 +1;
    }
    if ((opt & NTLMO_HTTPCRLF)==NTLMO_HTTPCRLF) {
	++reqlen;
    }
    if (into->maxlen - into->len < reqlen) return -1;

    ntlm1 = alloca (nlen);
    if (ntlm1==NULL) return -1;
    memset (ntlm1, 0, nlen);

    strcpy (ntlm1->protocol, NTLM_PROTOCOL);
#if 'A' != 0x41
    Translay (lat_e2a, sizeof (ntlm1->protocol), ntlm1->protocol);
#endif
    ntlm1->msgtype = host2litl (NTLM_TYPE1);
    ntlm1->flags = (uint32_t)host2litl (flags);
    ntlm1->dom.len = host2lits (funi? 2*dlen: dlen);
    ntlm1->dom.len2 = ntlm1->dom.len;
    ntlm1->host.len = host2lits (funi? 2*hlen: hlen);
    ntlm1->host.len2 = ntlm1->host.len;

    p0 = (char *)ntlm1 + sizeof (NTLM1);
    p = p0;

    ntlm1->host.off = host2litl (p-(char *)ntlm1);
    p += UniCopy (funi, p, host, hlen);
    ntlm1->dom.off = host2litl (p-(char *)ntlm1);
    p += UniCopy (funi, p, domain, dlen);

#if 'A' != 0x41
    Translay (lat_e2a, (size_t)(p-p0), p0);
#endif

    p = into->ptr + into->len;

    if (opt & NTLMO_HTTP) {
	memcpy (p, AuthC, sizeof (AuthC) -1);
	p += sizeof (AuthC) -1;
    }
    if (opt & (NTLMO_BASE64|NTLMO_HTTP)) {
	tobase64 (nlen, ntlm1, &blen, p, 1);
	p += blen;
    } else {
	memcpy (p, ntlm1, nlen);
	p += nlen;
    }
    if ((opt & NTLMO_HTTPCRLF)==NTLMO_HTTPCRLF) {
	*p++ = '\r';
    }
    if (opt & NTLMO_HTTP) {
	*p++ = '\n';
    }

    into->len = (size_t)(p - into->ptr);

    return 0;
}

int ParseNtlm2 (int opt, char nonce[8], const ConstBuffData *from)
{
    ConstBuffData f = *from;
    NTLM2 *ntlm2= NULL;
    int rc;
    size_t nlen;
    size_t ntlm2_b64len = (sizeof (NTLM2)+2)/3*4;

    (void)opt;

    if (sizeof (NTLM2) != 0x28) {
	fprintf (stderr, "ParseNtlm2: Invalid \"ntlmauth.h\" header"
		 " %d <> %d\n", (int)sizeof (NTLM2), 0x28);
	exit (32);
    }
    ntlm2 = alloca (sizeof (NTLM2)+2); /* base64 :( */

    memset (nonce, 0, sizeof (ntlm2->nonce));

    if (f.len >= sizeof (AuthS)-1 &&
	memcmp (f.ptr, AuthS, sizeof (AuthS)-1)==0) {
	f.len -= sizeof (AuthS)-1;
	f.ptr += sizeof (AuthS)-1;
	if (f.len>0 && f.ptr[f.len-1]=='\n') --f.len;
    }

    if (f.len >= ntlm2_b64len &&
	memcmp (f.ptr, "TlRMTVNT", 8)==0) {
	rc = frombase64 (ntlm2_b64len, f.ptr, &nlen, ntlm2);
	if (rc || nlen < sizeof (NTLM2) ||
		  nlen > sizeof (NTLM2)+2) return -1;

    } else if (f.len >= sizeof (NTLM2) &&
	memcmp (f.ptr, "NTLMSSP", 8)==0) {
	memcpy (&ntlm2, f.ptr, sizeof (ntlm2));

    } else return -1;

    if (strcmp (ntlm2->protocol, NTLM_PROTOCOL) != 0) {
#if 'A' != 0x41
	Translay (lat_a2e, sizeof (ntlm2->protocol), ntlm2->protocol);
	if (strcmp (ntlm2->protocol, NTLM_PROTOCOL) != 0)
#endif
	    return -1;
    }

    memcpy (nonce, ntlm2->nonce, sizeof (ntlm2->nonce));
    return 0;
}

static void Respond (size_t pwlen, const char *passw, const char *upassw,
		     const char nonce[8],
		     char lm_resp [NTLM_RESPLEN],
		     char nt_resp [NTLM_RESPLEN]);

int CreateNtlm3 (int opt, Buffer *into, const char nonce[8],
		 const char *host, const char *domain,
		 const char *user, const char *passwd)
{
    size_t hlen, dlen, ulen, nlen, reqlen, blen, pwlen;
    NTLM3 *ntlm3;
    char *p, *p0;
    char localpw [14+1], ulocalpw [14+1];
    char lmresp [NTLM_RESPLEN];
    char ntresp [NTLM_RESPLEN];
    int funi = 1;
    int flags;
    const char *sect;

    if (sizeof (NTLM3) != 0x40) {
	fprintf (stderr, "CreateNtlm3: Invalid \"ntlmauth.h\" header\n");
	exit (32);
    }
    hlen = strlen (host);
    dlen = strlen (domain);
    ulen = strlen (user);

    if (dlen==0) {
	sect = strchr (user, '\\');
	if (sect==NULL) sect = strchr (user, '/');
	if (sect) {
	    domain = user;
	    dlen = (size_t)(sect-user);
	    ulen -= (size_t)(sect+1 - user);
	    user = sect+1;
	}
    }

    flags = NTLMFLAG_NEGOTIATE_NTLM_KEY;
    if (funi) {
	nlen = sizeof (NTLM3) + 2*dlen + 2*ulen + 2*hlen +2*NTLM_RESPLEN;
	flags |= NTLMFLAG_NEGOTIATE_UNICODE;
    } else {
	nlen = sizeof (NTLM3) + dlen + ulen + hlen +2*NTLM_RESPLEN;
	flags |= NTLMFLAG_NEGOTIATE_OEM;
    }

    reqlen = nlen;
    if (opt & (NTLMO_BASE64|NTLMO_HTTP)) {
	reqlen = (reqlen+2) / 3 * 4;
    }
    if (opt & NTLMO_HTTP) {
	reqlen += sizeof (AuthC) -1 +1;
    }
    if ((opt & NTLMO_HTTPCRLF)==NTLMO_HTTPCRLF) {
	++reqlen;
    }

    if (into->maxlen - into->len < reqlen) return -1;

    pwlen = strnlen (passwd, 14);
    memset (localpw, 0, sizeof (localpw));
    memset (ulocalpw, 0, sizeof (ulocalpw));
    memcpy (localpw,  passwd, pwlen);
    memcpy (ulocalpw, passwd, pwlen);
    Translay (lat_l2u, pwlen, ulocalpw);

#if 'A' != 0x41
    Translay (lat_e2a, pwlen, localpw);
    Translay (lat_e2a, pwlen, ulocalpw);
#endif

    Respond (pwlen, localpw, ulocalpw, nonce, lmresp, ntresp);

    ntlm3 = alloca (nlen);
    if (ntlm3==NULL) return -1;
    memset (ntlm3, 0, nlen);

    strcpy (ntlm3->protocol, NTLM_PROTOCOL);
#if 'A' != 0x41
    Translay (lat_e2a, sizeof (ntlm3->protocol), ntlm3->protocol);
#endif

    ntlm3->msgtype = host2litl (NTLM_TYPE3);
    ntlm3->flags = (uint32_t)host2litl (flags);

    ntlm3->lm.len  = host2lits (NTLM_RESPLEN);
    ntlm3->lm.len2 = ntlm3->lm.len;
    ntlm3->nt.len  = host2lits (NTLM_RESPLEN);
    ntlm3->nt.len2 = ntlm3->nt.len;
    ntlm3->dom.len	= host2lits (funi? 2*dlen: dlen);
    ntlm3->dom.len2	= ntlm3->dom.len;
    ntlm3->user.len	= host2lits (funi? 2*ulen: ulen);
    ntlm3->user.len2	= ntlm3->user.len;
    ntlm3->host.len	= host2lits (funi? 2*hlen: hlen);
    ntlm3->host.len2	= ntlm3->host.len;
    ntlm3->session.len	= 0;
    ntlm3->session.len2 = 0;

    p0 = (char *)ntlm3 + sizeof (NTLM3);
    p = p0;
    ntlm3->dom.off = host2litl (p-(char *)ntlm3);
    p += UniCopy (funi, p, domain, dlen);
    ntlm3->user.off = host2litl (p-(char *)ntlm3);
    p += UniCopy (funi, p, user, ulen);
    ntlm3->host.off = host2litl (p-(char *)ntlm3);
    p += UniCopy (funi, p, host, hlen);

#if 'A' != 0x41
    Translay (lat_e2a, (size_t)(p-p0), p0);
#endif

    ntlm3->lm.off = host2litl (p-(char *)ntlm3);
    memcpy (p, lmresp, NTLM_RESPLEN);
    p += NTLM_RESPLEN;
    ntlm3->nt.off = host2litl (p-(char *)ntlm3);
    memcpy (p, ntresp, NTLM_RESPLEN);
    p += NTLM_RESPLEN;
    ntlm3->session.off = host2litl (p-(char *)ntlm3);

    p = into->ptr + into->len;

    if (opt & NTLMO_HTTP) {
	memcpy (p, AuthC, sizeof (AuthC) -1);
	p += sizeof (AuthC) -1;
    }
    if (opt & (NTLMO_BASE64|NTLMO_HTTP)) {
	tobase64 (nlen, ntlm3, &blen, p, 1);
	p += blen;
    } else {
	memcpy (p, ntlm3, nlen);
	p += nlen;
    }
    if ((opt & NTLMO_HTTPCRLF)==NTLMO_HTTPCRLF) {
	*p++ = '\r';
    }
    if (opt & NTLMO_HTTP) {
	*p++ = '\n';
    }

    into->len = (size_t)(p - into->ptr);

    return 0;
}

static void calc_resp (const unsigned char *keys,
		       const unsigned char *plaintext,
		       unsigned char *results);
static void setup_des_key (const unsigned char key_56[], DES_key_schedule *ks);

static void Respond (size_t pwlen, const char *passw, const char *upassw,
		     const char nonce[8],
		     char lm_resp [NTLM_RESPLEN],
		     char nt_resp [NTLM_RESPLEN])
{
    char  lm_pw [14];
    char  nt_pw [28];
    size_t idx;
    static const_DES_cblock magic =
    { 0x4B, 0x47, 0x53, 0x21, 0x40, 0x23, 0x24, 0x25 };
    unsigned char lm_hpw[21];
    unsigned char nt_hpw[21];
    DES_key_schedule ks;
    MD4_CTX context;

    /* setup LanManager password */

    if (pwlen > 14)  pwlen = 14;

    memset (lm_pw, 0, sizeof (lm_pw));
    memcpy (lm_pw, upassw, pwlen);

    /* create LanManager hashed password */

    setup_des_key ((void *)lm_pw, &ks);
    DES_ecb_encrypt ((void *)magic, (void *)lm_hpw, &ks, DES_ENCRYPT);

    setup_des_key ((void *)(lm_pw+7), &ks);
    DES_ecb_encrypt ((void *)magic, (void *)(lm_hpw+8), &ks, DES_ENCRYPT);

    memset(lm_hpw+16, 0, 5);

    /* create NT hashed password */

    for (idx=0; idx<pwlen; idx++)
    {
	nt_pw[2*idx]   = passw[idx];
	nt_pw[2*idx+1] = 0;
    }

    MD4_Init (&context);
    MD4_Update (&context, nt_pw, 2*pwlen);
    MD4_Final (nt_hpw, &context);

    memset (nt_hpw+16, 0, 5);

    /* create responses */

    calc_resp (lm_hpw, (unsigned char *)nonce, (unsigned char *)lm_resp);
    calc_resp (nt_hpw, (unsigned char *)nonce, (unsigned char *)nt_resp);
}

    /*
     * takes a 21 byte array and treats it as 3 56-bit DES keys. The
     * 8 byte plaintext is encrypted with each key and the resulting 24
     * bytes are stored in the results array.
     */
static void calc_resp (const unsigned char *keys, const unsigned char *plaintext,
		       unsigned char *results)
    {
	DES_key_schedule ks;

	setup_des_key (keys, &ks);
	DES_ecb_encrypt ((DES_cblock*) plaintext, (DES_cblock*) results, &ks, DES_ENCRYPT);

	setup_des_key (keys+7, &ks);
	DES_ecb_encrypt ((DES_cblock*) plaintext, (DES_cblock*) (results+8), &ks, DES_ENCRYPT);

	setup_des_key (keys+14, &ks);
	DES_ecb_encrypt ((DES_cblock*) plaintext, (DES_cblock*) (results+16), &ks, DES_ENCRYPT);
    }

/*
 * turns a 56 bit key into the 64 bit, odd parity key and sets the key.
 * The key schedule ks is also set.
 */
static void setup_des_key (const unsigned char key_56[], DES_key_schedule *ks)
{
	DES_cblock key;

	key[0] = key_56[0];
	key[1] = ((key_56[0] << 7) & 0xFF) | (key_56[1] >> 1);
	key[2] = ((key_56[1] << 6) & 0xFF) | (key_56[2] >> 2);
	key[3] = ((key_56[2] << 5) & 0xFF) | (key_56[3] >> 3);
	key[4] = ((key_56[3] << 4) & 0xFF) | (key_56[4] >> 4);
	key[5] = ((key_56[4] << 3) & 0xFF) | (key_56[5] >> 5);
	key[6] = ((key_56[5] << 2) & 0xFF) | (key_56[6] >> 6);
	key[7] =  (key_56[6] << 1) & 0xFF;

	DES_set_odd_parity (&key);
	DES_set_key_unchecked (&key, ks);
}
