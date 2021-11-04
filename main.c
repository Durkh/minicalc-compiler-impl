#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "arq.h"
#include "lexer.h"
#include "parser.h"
#include "var.h"

ExprRes AvaliaExpressao(Expressao* e) {
    ExprRes res;
    ExprRes v1, v2;

    switch (e->oper) {
        case OPER_VAR:
            if(ConsultaVar(e->valor.nomeIdent, &v1)){
                res = v1;
            } else{
                fprintf(stderr, "Variavel nao declarada: %s\n", e->valor.nomeIdent);
                exit(3);
            }
            break;
        case OPER_CONST:
            if(res.isInteger) {
                res.valor.integer = e->valor.longI;
                res.isInteger = true;
            }else {
                res.valor.floating = e->valor.doubleF;
            }
            break;
        case OPER_SOMA:
            v1 = AvaliaExpressao(e->op1);
            v2 = AvaliaExpressao(e->op2);
            if (v1.isInteger && v2.isInteger){
                res.valor.integer = v1.valor.integer + v2.valor.integer;
                res.isInteger = true;
            }else{
                res.valor.floating = v1.valor.floating + v2.valor.floating;
            }
            break;
        case OPER_MULT:
            v1 = AvaliaExpressao(e->op1);
            v2 = AvaliaExpressao(e->op2);
            if (v1.isInteger && v2.isInteger){
                res.valor.integer = v1.valor.integer * v2.valor.integer;
                res.isInteger = true;
            }else{
                res.valor.floating = v1.valor.floating * v2.valor.floating;
            }
            break;
        case OPER_SUB:
            v1 = AvaliaExpressao(e->op1);
            v2 = AvaliaExpressao(e->op2);
            if (v1.isInteger && v2.isInteger){
                res.valor.integer = v1.valor.integer - v2.valor.integer;
                res.isInteger = true;
            }else{
                res.valor.floating = v1.valor.floating - v2.valor.floating;
            }
            break;
        case OPER_DIV:
            v1 = AvaliaExpressao(e->op1);
            v2 = AvaliaExpressao(e->op2);
            if (v1.isInteger && v2.isInteger){
                res.valor.integer = v1.valor.integer / v2.valor.integer;
                res.isInteger = true;
            }else{
                res.valor.floating = v1.valor.floating / v2.valor.floating;
            }
            break;
        case OPER_MOD:
            res.isInteger = e->integer;

            assert(res.isInteger);

            v1 = AvaliaExpressao(e->op1);
            v2 = AvaliaExpressao(e->op2);

            res.valor.integer = v1.valor.integer % v2.valor.integer;
            break;
        case OPER_POW:
            res.isInteger = false;
            v1 = AvaliaExpressao(e->op1);
            v2 = AvaliaExpressao(e->op2);
            if (v1.isInteger && v2.isInteger){
                res.valor.floating = pow(v1.valor.integer, v2.valor.integer);
            }else{
                res.valor.floating = pow(v1.valor.floating, v2.valor.floating);
            }
            break;
        default:
            printf("Operador nao reconhecido.\n");
    }

    return res;
}

void ImprimeDeclaracoes(Declaracao *d) {
    while (d != NULL) {
        printf("Declaracao - ident: %s\n", d->nomeIdent);
        d = d->next;
    }
}

void ProcessaDeclaracoes(Declaracao *d) {
    while (d != NULL) {
        ExprRes val = AvaliaExpressao(d->e);
        AdicionaVar(d->nomeIdent, val);
        d = d->next;
    }
}


int main() {
    InicializaLexer("../test/expcomplexa.mc");

    // arvore sintatica do programa
    Programa *p = AnalisePrograma();

    ProcessaDeclaracoes(p->decls);

    ExprRes resultado = AvaliaExpressao(p->e);

    if(resultado.isInteger)
        printf("%li\n", resultado.valor.integer);
    else
        printf("%lf\n", resultado.valor.floating);

    DestroiPrograma(p);
    FinalizaLexer();
    return 0;
}