/* naive_parser.c */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

typedef struct LexToken {
    int type;     /* either 0..255 single-byte character) or LT_... */
    double value; /* if type==LT_NUM */
} LexToken;
#define LT_EOF -1  /* END OF STRING */
#define LT_NUM -2  /* number (actual value in 'value') */
static void LexToken_DebugPrint(const LexToken *token, FILE *to);

static LexData *LexInit(const char *from);
static int LexGet(LexData *ld, LexToken *into); /* returns into->type */
static void LexTerm(LexData *ld);

static void LexTest(void);

int main (void) {
    ExpTest();
    LexTest();
    return 0;
}

static LexData *LexInit(const char *from);
static int LexGet(LexData *ld, LexToken *into); /* returns into->type */
static void LexTerm(LexData *ld);

static void LexTest(void) {
    const char str[]= "1 -2- 3-(4 - 5)-6";
    LexData *ld= LexInit(str);
    LexToken token;

    printf("LexTest: reading \"%s\" token-wise\n", str);
    do {
        LexGet(ld, &token);
        LexToken_DebugPrint(&token, stdout);
    } while (token.type != LT_EOF);
    LexTerm(ld);
}

static void LexToken_DebugPrint(const LexToken *token, FILE *to) {
    if      (token->type==LT_EOF) fprintf(to, "token=EOF\n");
    else if (token->type==LT_NUM) fprintf(to, "token=NUMBER: %g\n", token->value);
    else                          fprintf(to, "token='%c'\n", (char)token->type);
}

struct LexData {
    char *from;
    char *lim;
    char *pos;
    int eof;      /* flag */
};

static LexData *LexInit(const char *pfrom) {
    LexData *ld= calloc(1, sizeof *ld);
    ld->from= strdup(pfrom);
    ld->pos= ld->from;
    ld->lim= ld->from + strlen(ld->from);
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
        return into->type= LT_EOF;
    }
    while (ld->pos < ld->lim && isspace((unsigned char)*ld->pos)) {
        ++ld->pos;
    }
    if (ld->pos >= ld->lim) {
        ld->eof= 1;
        return into->type= LT_EOF;
    }

    c= (unsigned char)*ld->pos;
    if (isdigit(c)) {
        into->value= strtod(ld->pos, &ld->pos);
        into->type= LT_NUM;

    } else {
        into->type= c;
        ++ld->pos;
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
    Exp *e= calloc(1, sizeof *e);
    e->type= 'N';
    e->value= pvalue;
    return e;
}

static Exp *Exp_New(char ptype, Exp *pleft, Exp *pright) {
    Exp *e= calloc(1, sizeof *e);
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

    } else {
        exit(13);
    }
}

static void Exp_Print(const Exp *e, FILE *to) {
    if (e==NULL) {
       exit (12);

    } else if (e->type=='N') {
        fprintf(to, "%g", e->value);

    } else if (e->type=='+' || e->type=='-'|| e->type=='*' || e->type=='/') {
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
