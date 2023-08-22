/* naive_parser.c */

#include <stdio.h>
#include <stdlib.h>

typedef struct Exp Exp; /* this type is opaque */

static Exp *Exp_NewNum(double pvalue);
static Exp *Exp_New(char ptype, Exp *pleft, Exp *pright);
static double Exp_Eval(const Exp *e);
static void Exp_Delete(Exp *e);
static void Exp_Print(const Exp *e, FILE *to);

static void ExpTest(void);

int main (void) {
    ExpTest();
    return 0;
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
