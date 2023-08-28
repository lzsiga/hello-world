/* naive_parser.c */

#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* stack */

typedef struct Stack Stack; /* this type is opaque */

static Stack *Stk_New(size_t sizeOfBaseType);
static void   Stk_Delete(Stack *s);
static int    Stk_Count(Stack *s); /* number of elements */
/* the next two functions copy (memcpy) sizeOfBaseType bytes */
static int    Stk_Add(Stack *s, const void *from);     /* add to the end */
static void  *Stk_Get(Stack *s, void *to, int index);  /* index: 0 .. base-1 */

static void StackTest(void);

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

    StackTest();
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

/* original right-recursive grammar (except for 'pow'):
   start -> add
   add   -> mul    | add  '+' mul | add '-' mul
   mul   -> pow    | mul  '*' pow | mul '/' pow
   pow   -> elem   | elem '^' pow
   elem  -> NUMBER | '(' add ')'

   transformed to left-revursive:
   start -> add
   add   -> mul    | mul  '+' add | mul '-' add
   mul   -> pow    | pow  '*' mul | pow '/' mul
   pow   -> elem   | elem '^' pow
   elem  -> NUMBER | '(' add ')'

   non-recursive grammar:
   start -> add
   add   -> mul    [ {'+'|'-'} mul ] ...
   mul   -> pow    [ {'*'|'/'} pow ] ...
   pow   -> elem   [ '^' elem ] ...
   elem  -> NUMBER | '(' add ')'

   NB negative/positive sign is not handled,
   nor does it seem easy, here is some questions:
   --2   is -(-(2)) or -(-2) or 2
   -2*-3 is -(2*(-3)) or (-2)*(-3) -- they are equals anyways
   -2^-3 is -(2^(-3)) or (-2)^(-3) -- those are different
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
   NP_Add and NP_Mul, i.e. they aren't recursive any more,
   instead they use a stack of 'ParsTempElem'
 */
typedef struct ParsTempElem {
    int op; /* opetator as character: + - * / (pow is not relevant here) */
    Exp *exp;
} ParsTempElem;

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
                e= NULL;
            }
        }
    }
    return e;
}

static Exp *NP_Pow(ParseData *p) {
    Exp *first= NULL;
    Stack *stk= NULL;
    ParsTempElem pte;
    int err= 0;

    if (p->token.type==LT_EOF) {
        return NULL;

    } else if (p->token.type==LT_NUM || p->token.type=='(') {
        first= NP_Elem(p);
        if (!first) return NULL;

    } else {
        NP_PrintErr(p);
        return NULL;
    }

    stk= Stk_New (sizeof (ParsTempElem));
    pte.op= 0;
    pte.exp= first;
    Stk_Add(stk, &pte);

    while (err==0 && p->token.type=='^') {
        int op= p->token.type;
        Exp *next;

        LexGet (p->ld, &p->token);
        next= NP_Elem(p);
        if (next==NULL) {
            err= 1;
            continue;
        }
        pte.op= op;
        pte.exp= next;
        Stk_Add(stk, &pte);
    }

    if (err) {
        int i, n= Stk_Count(stk);
        for (i= 0; i<n; ++i) {
            Stk_Get(stk, &pte, i);
            Exp_Delete(pte.exp);
        }
        return NULL;
    } else {
        int i, n= Stk_Count(stk);
        Exp *e= NULL;

        if (n>0) {
            int op;
            Stk_Get(stk, &pte, n-1);
            e= pte.exp;
            op= pte.op;

            for (i=n-2; i>=0; --i) {
                Stk_Get(stk, &pte, i);
                e= Exp_New(op, pte.exp, e);
                op= pte.op;
            }
        }
        return e;
    }
}

/* NP_Mul and NP_Add are almost identical,
   they should be wrappers calling a common implementation
   Later: done and undone that, it was not beginner-friendly enough.
   Also I have doubts if handling of the positive/negative sign
   (should it ever happen) would be the same
 */

static Exp *NP_Mul(ParseData *p) {
    Exp *first= NULL;
    Stack *stk= NULL;
    ParsTempElem pte;
    int err= 0;

    if (p->token.type==LT_EOF) {
        return NULL;

    } else if (p->token.type==LT_NUM || p->token.type=='(') {
        first= NP_Pow (p);
        if (first==NULL) return NULL;

    } else {
        NP_PrintErr(p);
        return NULL;
    }

    stk= Stk_New (sizeof (ParsTempElem));
    pte.op= 0;
    pte.exp= first;
    Stk_Add(stk, &pte);

    while (err==0 && (p->token.type=='*' || p->token.type=='/')) {
        int op= p->token.type;
        Exp *next;

        LexGet (p->ld, &p->token);
        next= NP_Pow(p);
        if (next==NULL) {
            err= 1;
            continue;
        }
        pte.op= op;
        pte.exp= next;
        Stk_Add(stk, &pte);
    }

    if (err) {
        int i, n= Stk_Count(stk);
        for (i= 0; i<n; ++i) {
            Stk_Get(stk, &pte, i);
            Exp_Delete(pte.exp);
        }
        return NULL;
    } else {
        int i, n= Stk_Count(stk);
        Exp *e= NULL;

        if (n>0) {
            Stk_Get(stk, &pte, 0);
            e= pte.exp;

            for (i=1; i<n; ++i) {
                Stk_Get(stk, &pte, i);
                e= Exp_New(pte.op, e, pte.exp);
            }
        }
        return e;
    }
}

static Exp *NP_Add(ParseData *p) {
    Exp *first= NULL;
    Stack *stk= NULL;
    ParsTempElem pte;
    int err= 0;

    if (p->token.type==LT_EOF) {
        return NULL;

    } else if (p->token.type==LT_NUM || p->token.type=='(') {
        first= NP_Mul (p);
        if (first==NULL) return NULL;
    } else {
        NP_PrintErr(p);
        return NULL;
    }

    stk= Stk_New (sizeof (ParsTempElem));
    pte.op= 0;
    pte.exp= first;
    Stk_Add(stk, &pte);

    while (err==0 && (p->token.type=='+' || p->token.type=='-')) {
        int op= p->token.type;
        Exp *next;

        LexGet (p->ld, &p->token);
        next= NP_Mul(p);

        if (next==NULL) {
            err= 1;
            continue;
        }
        pte.op= op;
        pte.exp= next;
        Stk_Add(stk, &pte);
    }

    if (err) {
        int i, n= Stk_Count(stk);
        for (i= 0; i<n; ++i) {
            Stk_Get(stk, &pte, i);
            Exp_Delete(pte.exp);
        }
        return NULL;
    } else {
        int i, n= Stk_Count(stk);
        Exp *e= NULL;

        if (n>0) {
            Stk_Get(stk, &pte, 0);
            e= pte.exp;

            for (i=1; i<n; ++i) {
                Stk_Get(stk, &pte, i);
                e= Exp_New(pte.op, e, pte.exp);
            }
        }
        return e;
    }
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
    LexData *ld= (LexData *)calloc(1, sizeof *ld);
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
    Exp *e= (Exp *)calloc(1, sizeof *e);
    e->type= 'N';
    e->value= pvalue;
    return e;
}

static Exp *Exp_New(char ptype, Exp *pleft, Exp *pright) {
    Exp *e= (Exp *)calloc(1, sizeof *e);
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

static void StackTest(void) {
    typedef short StackTestType;
    int n= 7, i;
    Stack *s= Stk_New(sizeof (StackTestType));

    for (i=0; i<n; ++i) {
        StackTestType tmp= 2*i;
        Stk_Add(s, &tmp);
        if (Stk_Count(s) != (i+1)) exit(16);
    }
    for (i=n-1; i>=0; --i) {
        StackTestType tmp;
        Stk_Get(s, &tmp, i);
        printf("Stack[%d]=%d (expected %d)\n", i, tmp, 2*i);
    }
    Stk_Delete(s);
}

struct Stack {
    void *data;
    size_t sizeOfBaseType;
    size_t allocated;
    size_t used; /* allocated <= used, 'Stk_Count' returns this */
};

static Stack *Stk_New(size_t sizeOfBaseType) {
    Stack *s= (Stack *)calloc(1, sizeof *s);
    s->sizeOfBaseType= sizeOfBaseType;
    return s;
}

static void Stk_Delete(Stack *s) {
    if (s->data) free(s->data);
    free(s);
}

static int Stk_Count(Stack *s) {
    return (int)s->used;
}

static int Stk_Add(Stack *s, const void *from) {
    int toindex;

    if (s->used==s->allocated) {
       int newnum= 2 + 2*s->allocated;
       s->data= realloc (s->data, newnum * s->sizeOfBaseType);
       s->allocated= newnum;
    }
    toindex= s->used;
    ++s->used;
    memcpy ((char *)s->data + toindex*s->sizeOfBaseType, from, s->sizeOfBaseType);
    return toindex;
}

static void *Stk_Get(Stack *s, void *to, int index) {
    if (index<0 && (size_t)index >= s->used) exit(15);
    memcpy (to, (char *)s->data + index*s->sizeOfBaseType, s->sizeOfBaseType);
    return to;
}
