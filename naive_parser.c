/* naive_parser.c */

#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* memory management with error handling */

static void *emalloc(size_t s);
static void *ecalloc(size_t s1, size_t s2);
static void *erealloc(void *p, size_t s);

/* expression tree */

typedef struct Exp Exp; /* this type is opaque */

static Exp *Exp_NewNum(double pvalue);
static Exp *Exp_New(char ptype, Exp *pleft, Exp *pright);
static double Exp_Eval(const Exp *e);
static void Exp_Delete(Exp *e);
static void Exp_Print(const Exp *e, FILE *to);

static void ExpTest(void);

/* lexical parser */

typedef struct LexData LexData; /* this type is opaque */

typedef struct LexPos {
    int line;     /* line:   1.. */
    int offset;   /* offset: 0.. */
} LexPos;

typedef struct LexToken {
    int type;     /* either 0..255 single-byte character) or LT_... */
    double value; /* if type==LT_NUM */
    LexPos pos;   /* parsing position */
} LexToken;
#define LT_EOF -1  /* END OF STRING */
#define LT_NUM -2  /* number (actual value in 'value') */
static void LexToken_DebugPrint(const LexToken *token, FILE *to);

static LexData *LexInit(const char *from);
static int LexGet(LexData *ld, LexToken *into); /* returns into->type */
static void LexTerm(LexData *ld);

static void LexTest(const char *from);

/* naive parser */

static Exp *NaiveParser(const char *from);

static void NaiveParsTest(const char *from);

static void DefaultTests(void) {
static const char Test_01[] = "10.4 -2- 3-(4 - 5)-6";
static const char Test_10[] = "123";
static const char Test_11[] = "123 / 3";
static const char Test_12[] = "12 * 5.3 * 8";
static const char Test_13[] = "96 / 2 / 3 / 4"; /* naive interpretation: (96/(2/(3/4))); correct interpretaion: (((96/2)/3)/4) */
static const char Test_14[] = "((96/2)/3)/4";   /* no problem here */
static const char Test_15[] = "96/(2/(3/4))";   /* or here */
static const char Test_16[] = "6 - 7";
static const char Test_17[] = "1 * 2 - 3 / 4";
static const char Test_20[] = "(1-3)*(4-7)";
static const char Test_21[] = "(1-(3))*((4)/7)";
static const char Test_22[] = "(1";             /* syntax error */
static const char Test_23[] = "1+(2+(3";        /* syntax error */
static const char Test_30[] = "2^3^4";          /* NB pow is right associative, so (2^(3^4)) is the right answer */

    ExpTest();
    LexTest(Test_01);
    NaiveParsTest(Test_10);
    NaiveParsTest(Test_11);
    NaiveParsTest(Test_12);
    NaiveParsTest(Test_13);
    NaiveParsTest(Test_14);
    NaiveParsTest(Test_15);
    NaiveParsTest(Test_16);
    NaiveParsTest(Test_17);
    NaiveParsTest(Test_20);
    NaiveParsTest(Test_21);
    NaiveParsTest(Test_22);
    NaiveParsTest(Test_23);
    NaiveParsTest(Test_30);
}

int main (int argc, char **argv) {
   (void)emalloc;  /* for some reason it is never used */
   (void)erealloc; /* for some reason it is never used */

   if (argc==1) {
       DefaultTests();
   } else {
       int i;
       for (i=1; i<argc; ++i) {
           NaiveParsTest(argv[i]);
       }
       return 0;
    }
    return 0;
}

static void NaiveParsTest(const char *from) {
    Exp *e;

    printf("NaiveParsTest: input=\"%s\"\n", from);
    e= NaiveParser(from);
    printf("Result: ");
    if (e==NULL) {
        printf("NULL\n");
    } else {
        Exp_Print(e, stdout);
        fputc('\n', stdout);
        fprintf(stdout, "Result: %g\n", Exp_Eval(e));
        Exp_Delete(e);
    }
}

typedef struct ParseData {
    LexData *ld;
    LexToken token; /* we keep reading ahead one token */
} ParseData;

static Exp *NP_Root(ParseData *p);
static Exp *NP_Add(ParseData *p);
static Exp *NP_Mul(ParseData *p);
static Exp *NP_Pow(ParseData *p);

/* #1 original right-recursive grammar (except for 'pow'):
   start -> add
   add   -> mul    | add  '+' mul | add '-' mul
   mul   -> pow    | mul  '*' pow | mul '/' pow
   pow   -> elem   | elem '^' pow
   elem  -> NUMBER | '(' add ')'

   #2 transformed to left-recursive:
   start -> add
   add   -> mul    | mul  '+' add | mul '-' add
   mul   -> pow    | pow  '*' mul | pow '/' mul
   pow   -> elem   | elem '^' pow
   elem  -> NUMBER | '(' add ')'

   #3 non-recursive grammar:
   start -> add
   add   -> mul    [ {'+'|'-'} mul ] ...
   mul   -> pow    [ {'*'|'/'} pow ] ...
   pow   -> elem   [ '^' elem ] ...
   elem  -> NUMBER | '(' add ')'

   NB negative/positive sign is not handled,
   nor does it seem easy, here is some questions:
   --2   is -(-(2)) or -(-2) or 2
   -3*-2 is -(3*(-2)) or (-3)*(-2) -- they are equals anyways
   -3^-2 is -(3^(-2)) or (-3)^(-2) -- those are different
 */

static Exp *NaiveParser(const char *from) {
    ParseData p;
    Exp *e;

    memset (&p, 0, sizeof p);
    p.ld= LexInit(from);
    LexGet (p.ld, &p.token);
    e= NP_Root(&p);
    LexTerm(p.ld);
    return e;
}

static void NP_PrintErr(ParseData *p) {
    fprintf(stderr, "*** failed at ");
    LexToken_DebugPrint(&p->token, stderr);
}

static void NP_PrintErrF(ParseData *p, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    LexToken_DebugPrint(&p->token, stderr);
}

static Exp *NP_Root(ParseData *p) {
    Exp *e= NP_Add(p);
    if (p->token.type != LT_EOF) {
        NP_PrintErr(p);
    }
    return e;
}

/* to solve the left associativity problem, we had to improve
   NP_Add and NP_Mul, i.e. they aren't recursive any more
   see grammar #3
 */

static Exp *NP_Elem (ParseData *p) {
    Exp *e= NULL;

    if (p->token.type==LT_EOF) {
        e= NULL;

    } else if (p->token.type==LT_NUM) {
        e= Exp_NewNum (p->token.value);
        LexGet(p->ld, &p->token);

    } else if (p->token.type=='(') {
        LexGet(p->ld, &p->token);
        e= NP_Add(p);
        if (e!=NULL) {
            if (p->token.type==')') {
                LexGet(p->ld, &p->token);
            } else {
                NP_PrintErrF(p, "*** Missing right parentheses near ");
                Exp_Delete(e);
                e= NULL;
            }
        }
    }
    return e;
}

/* NP_Pow right-associative, implemented via recursion (grammar #2) */

static Exp *NP_Pow(ParseData *p) {
    Exp *e= NULL;

    if (p->token.type==LT_EOF) {
        return NULL;

    } else if (p->token.type==LT_NUM || p->token.type=='(') {
        e= NP_Elem(p);
        if (!e) return NULL;

    } else {
        NP_PrintErr(p);
        return NULL;
    }

    if (p->token.type=='^') {
        int op= p->token.type;
        Exp *next;

        LexGet (p->ld, &p->token);
        next= NP_Pow(p);
        if (next==NULL) {
            Exp_Delete(e);
            e= NULL;
        } else {
            e= Exp_New(op, e, next);
        }
    }
    return e;
}

/* NP_Mul and NP_Add are almost identical,
   they should be wrappers calling a common implementation
   Later: done and undone that, it was not beginner-friendly enough.
   Also I have doubts if handling of the positive/negative sign
   (should it ever happen) would be the same
 */

static Exp *NP_Mul(ParseData *p) {
    Exp *e= NULL;
    int err= 0;

    if (p->token.type==LT_EOF) {
        return NULL;

    } else if (p->token.type==LT_NUM || p->token.type=='(') {
        e= NP_Pow (p);
        if (e==NULL) return NULL;

    } else {
        NP_PrintErr(p);
        return NULL;
    }

    while (err==0 && (p->token.type=='*' || p->token.type=='/')) {
        int op= p->token.type;
        Exp *next;

        LexGet (p->ld, &p->token);
        next= NP_Pow(p);
        if (next==NULL) {
            err= 1;
        } else {
            e= Exp_New(op, e, next);
        }
    }

    if (err) {
        Exp_Delete(e);
        e= NULL;
    }
    return e;
}

static Exp *NP_Add(ParseData *p) {
    Exp *e= NULL;
    int err= 0;

    if (p->token.type==LT_EOF) {
        return NULL;

    } else if (p->token.type==LT_NUM || p->token.type=='(') {
        e= NP_Mul (p);
        if (e==NULL) return NULL;
    } else {
        NP_PrintErr(p);
        return NULL;
    }

    while (err==0 && (p->token.type=='+' || p->token.type=='-')) {
        int op= p->token.type;
        Exp *next;

        LexGet (p->ld, &p->token);
        next= NP_Mul(p);

        if (next==NULL) {
            err= 1;
        } else {
            e= Exp_New(op, e, next);
        }
    }

    if (err) {
        Exp_Delete(e);
        e= NULL;
    }
    return e;
}

static void LexTest(const char *from) {
    LexData *ld= LexInit(from);
    LexToken token;

    printf("LexTest: reading \"%s\" token-wise\n", from);
    do {
        LexGet(ld, &token);
        LexToken_DebugPrint(&token, stdout);
    } while (token.type != LT_EOF);
    LexTerm(ld);
}

static void LexToken_DebugPrint(const LexToken *token, FILE *to) {
    fprintf(to, "token at %d:%d ", token->pos.line, token->pos.offset);
    if      (token->type==LT_EOF) fprintf(to, "EOF\n");
    else if (token->type==LT_NUM) fprintf(to, "NUMBER: %g\n", token->value);
    else                          fprintf(to, "'%c'\n", (char)token->type);
}

struct LexData {
    char *from;
    char *lim;
    char *ptr;    /* parsing position as pointer */
    LexPos pos;   /* parsing position as line+offset */
    int eof;      /* flag */
};

static LexData *LexInit(const char *pfrom) {
    LexData *ld= (LexData *)ecalloc(1, sizeof *ld);
    ld->from= strdup(pfrom);
    ld->ptr= ld->from;
    ld->lim= ld->from + strlen(ld->from);
    ld->pos.line= 1;
    ld->pos.offset= 0;
    return ld;
}

static void LexTerm(LexData *ld) {
    free(ld->from);
    memset(ld, 0, sizeof *ld);
    free(ld);
}

static int LexGet(LexData *ld, LexToken *into) {
    int c;

    memset(into, 0, sizeof *into);
    if (ld==NULL) {
        exit(14);

    } else if (ld->eof) {
        into->type= LT_EOF;
        into->pos= ld->pos;
        return into->type;
    }
    while (ld->ptr < ld->lim && isspace(c= (unsigned char)*ld->ptr)) {
        ++ld->ptr;
        if (c=='\n') {
            ++ld->pos.line;
            ld->pos.offset= 0;
        } else {
            ++ld->pos.offset;
        }
    }

    into->pos= ld->pos;
    if (ld->ptr >= ld->lim) {
        ld->eof= 1;
        into->type= LT_EOF;
        return into->type;
    }

    c= (unsigned char)*ld->ptr;
    if (isdigit(c)) {
        char *endptr= NULL;
        into->value= strtod(ld->ptr, &endptr);
        into->type= LT_NUM;
        ld->pos.offset += (endptr - ld->ptr);
        ld->ptr= endptr;

    } else {
        into->type= c;
        ++ld->ptr;
        ++ld->pos.offset;
    }
    return into->type;
}

static void ExpTest(void) {
    Exp *e= Exp_New ('-', Exp_New ('+', Exp_NewNum (2), Exp_NewNum(3)),
                          Exp_New ('*', Exp_NewNum (4), Exp_NewNum(5)));

    printf("Test expression: ");
    Exp_Print(e, stdout);
    fputc('\n', stdout);

    printf("Evaluate: %g\n", Exp_Eval(e));

    Exp_Delete(e);
}

struct Exp {
    char type;         /* 'N', '+', '-', '*', '/' = number(leaf), add/sub/mul/div */
    double value;      /* used only if leaf */
    struct Exp *left;  /* must be null if leaf */
    struct Exp *right; /* must be null if leaf */
};

static Exp *Exp_NewNum(double pvalue) {
    Exp *e= (Exp *)ecalloc(1, sizeof *e);
    e->type= 'N';
    e->value= pvalue;
    return e;
}

static Exp *Exp_New(char ptype, Exp *pleft, Exp *pright) {
    Exp *e= (Exp *)ecalloc(1, sizeof *e);
    e->type= ptype;
    e->left= pleft;
    e->right= pright;
    return e;
}

static double Exp_Eval(const Exp *e) {
    if (e==NULL) {
       exit (12);

    } else if (e->type=='N') {
        return e->value;

    } else if (e->type=='+') {
        return Exp_Eval(e->left) + Exp_Eval(e->right);

    } else if (e->type=='-') {
        return Exp_Eval(e->left) - Exp_Eval(e->right);

    } else if (e->type=='*') {
        return Exp_Eval(e->left) * Exp_Eval(e->right);

    } else if (e->type=='/') {
        return Exp_Eval(e->left) / Exp_Eval(e->right);

    } else if (e->type=='^') {
        double l= Exp_Eval(e->left);
        double r= Exp_Eval(e->right);
        double res= pow(l,r);
        return res;

    } else {
        exit(13);
    }
}

static void Exp_Print(const Exp *e, FILE *to) {
    if (e==NULL) {
       exit (12);

    } else if (e->type=='N') {
        fprintf(to, "%g", e->value);

    } else if (e->type=='+' || e->type=='-'|| e->type=='*' || e->type=='/' || e->type=='^') {
        fputc('(', to);
        Exp_Print(e->left, to);
        fputc((char)e->type, to);
        Exp_Print(e->right, to);
        fputc(')', to);

    } else {
        exit(13);
    }
}

static void Exp_Delete(Exp *e) {
    if (e==NULL) return;
    Exp_Delete(e->left);
    Exp_Delete(e->right);
    free(e);
}

/* memory management with error handling */

void *emalloc(size_t s) {
    void *p= malloc(s);
    if (!p && s!=0) {
        fprintf(stderr, "*** Out of memory in malloc(%ld)\n", (long)s);
        exit(20);
    }
    return p;
}

void *ecalloc(size_t s1, size_t s2) {
    void *p= calloc(s1, s2);
    if (!p && s1!=0 && s2!=0)  {
        fprintf(stderr, "*** Out of memory in calloc(%ld, %ld)\n", (long)s1, (long)s2);
        exit(20);
    }
    return p;
}

void *erealloc(void *p, size_t s) {
    void *q= realloc(p, s);
    if (!q && s!=0) {
        fprintf(stderr, "*** Out of memory in realloc(%ld)\n", (long)s);
        exit(20);
    }
    return q;
}
