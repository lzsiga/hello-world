/* naive_parser_lrec.c */

/* this version is copied from 'naive_parser.c',
   its only goal is comparing left- and right recursion
   (the former leads to infinite recursion in this naive implementation)

   The NP_Add functions has been duplicated into NP_Add_Lrec and NP_Add_Rrec
   the other functions are the same as in naive_parser.c

   #1 left recursion

     add       -> add_start mul
     add_start -> (empty) | add_strt {'+'|'-'}

   #2 right recursion

     add       -> mul add_rest
     add_rest  -> (empty) | {'+'|'-'} add_rest

   Expected result:

     * left recursion doesn't terminates, the program aborts when the stack overflows

     * right recursion terminates, but gives wrong result, e.g.
       1-2-3-4 becomes 1-(2-(3-4)) instead of ((1-2)-3)-4

   NB The working version of the recursive top-down parser is 'naive_parser.c'
 */

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

/* special values that differ from any valid pointer;
   they are used during the parsing
 */
#define Exp_ERR   ((Exp *)0)
#define Exp_EMPTY ((Exp *)1)

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

/* naive parser */

static Exp *NaiveParser(const char *from, int recFlag);

/* this flag only affect NP_Add,
   its purpose is demonstrating the problem with infinite recursion
 */
#define NP_FLAG_RIGHT_RECURSION 0 /* terminates */
#define NP_FLAG_LEFT_RECURSION  1 /* infinite recursion */

static void NaiveParsTest(const char *from);

static void DefaultTests(void) {
static const char Test_01[] = "1 + 2 - 3 - 4";

    NaiveParsTest(Test_01);
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
    printf("With right recursion (except wrong result)\n");
    e= NaiveParser(from, NP_FLAG_RIGHT_RECURSION);

    printf("Result: ");
    if (e==NULL) {
        printf("NULL\n");
    } else {
        Exp_Print(e, stdout);
        fputc('\n', stdout);
        fprintf(stdout, "Result: %g\n", Exp_Eval(e));
        Exp_Delete(e);
    }

    printf("With left recursion (except stack overflow on infinite recursion)\n");
    e= NaiveParser(from, NP_FLAG_LEFT_RECURSION);

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
    int recFlag;    /* NP_Add uses it */
} ParseData;

static Exp *NP_Root(ParseData *p);
static Exp *NP_Add(ParseData *p);
static Exp *NP_Mul(ParseData *p);
static Exp *NP_Pow(ParseData *p);

static Exp *NaiveParser(const char *from, int recFlag) {
    ParseData p;
    Exp *e;

    memset (&p, 0, sizeof p);
    p.ld= LexInit(from);
    p.recFlag= recFlag;
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

static Exp *NP_Add_Rrec(ParseData *p);
static Exp *NP_Add_Lrec(ParseData *p);

/* NP_Add calls NP_Add_Lrec or NP_Add_Rrec depending on p->recFlag */
static Exp *NP_Add(ParseData *p) {
    if (p->recFlag==NP_FLAG_LEFT_RECURSION) return NP_Add_Lrec(p);
    else                                    return NP_Add_Rrec(p);
}

/* NP_Add_Rrec_Rest returns two values so it happens via parameters
   also it may return Exp_EMPTY
 */
static void NP_Add_Rrec_Rest(ParseData *p, Exp **retExp, int *retOp);

static Exp *NP_Add_Rrec(ParseData *p) {
    Exp *e= Exp_ERR, *f= Exp_ERR, *ret= Exp_ERR;
    int op= 0;

    if (p->token.type!=LT_NUM && p->token.type!='(') {
        NP_PrintErr(p);
        return Exp_ERR;
    }

    e= NP_Mul (p);
    if (e==Exp_ERR) return Exp_ERR;

    NP_Add_Rrec_Rest(p, &f, &op);
    
    if (f==Exp_EMPTY) {
        ret= e;

    } else if (f==Exp_ERR) {
        Exp_Delete(e);
        ret= Exp_ERR;

    } else {
        ret= Exp_New (op, e, f);
    }

    return ret;
}

static void NP_Add_Rrec_Rest(ParseData *p, Exp **retExp, int *retOp) {
    int op;
    Exp *e;

    if (p->token.type!='+' && p->token.type!='-') {
        *retExp= Exp_EMPTY;
        *retOp= 0;
        return;
    }

    op= p->token.type;
    LexGet (p->ld, &p->token);

    e= NP_Add_Rrec(p);
    if (e==Exp_ERR) {
        *retExp= Exp_EMPTY;
        *retOp= 0;
        return;
    }

    *retExp= e;
    *retOp= op;
    return;
}

/* NP_Add_Lrec_Start returns two values so it happens via parameters
   also it may return Exp_EMPTY
   
   NB actually recursion never terminates, only the stack overflow stops the program
 */
static void NP_Add_Lrec_Start(ParseData *p, Exp **retExp, int *retOp);

static Exp *NP_Add_Lrec(ParseData *p) {
    Exp *e= Exp_ERR, *f= Exp_ERR, *ret= Exp_ERR;
    int op= 0;

    if (p->token.type!=LT_NUM && p->token.type!='(') {
        NP_PrintErr(p);
        return Exp_ERR;
    }

    NP_Add_Lrec_Start(p, &e, &op);
    if (e==Exp_ERR) return Exp_ERR;

    f= NP_Mul(p);

    if (f==Exp_ERR) {
        Exp_Delete(e);
        ret= f;

    } else if (e==Exp_EMPTY) {
        ret= f;

    } else {
        ret= Exp_New (op, e, f);
    }

    return ret;
}

static void NP_Add_Lrec_Start(ParseData *p, Exp **retExp, int *retOp) {
    Exp *e= Exp_ERR;
    int op= 0;

    if (p->token.type!=LT_NUM && p->token.type!='(') {
        *retExp= Exp_EMPTY;
        *retOp= 0;
        return;
    }

    e= NP_Add_Lrec(p);
    if (e==Exp_ERR) {
        *retExp= Exp_ERR;
        *retOp= 0;
        return;
    }

    if (p->token.type!='+' && p->token.type!='-') {
        *retExp= Exp_ERR;
        *retOp= 0;
        return;
    }

    op= p->token.type;
    LexGet (p->ld, &p->token);

    *retExp= e;
    *retOp= op;
    return;
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
