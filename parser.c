//
// Created by egidio on 04/10/2021.
//

//
// Analisador sintático para MiniCalc
//

// Analisador descendente recursivo

#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "lexer.h"

Token *t;

// Analisador sintático do programa
// Assume que o analisador léxico foi inicializado com o código-fonte
Programa* AnalisePrograma() {
    Programa *res = (Programa*) malloc(sizeof(Programa));

    if (res == NULL) {
        fprintf(stderr, "Erro de alocacao de memoria.");
        exit(1);
    }

    // verifica se o programa começa com palavra-chave 'print'
    t = ProximoToken();

    if (t->tipo != TOKEN_PRINT) {
        fprintf(stderr, "Erro sintatico: palavra-chave 'print' esperada no inicio do programa.");
        exit(2);
    }

    // analisa a expressao seguinte
    res->e = AnaliseExpressao();

//    t = ProximoToken();
//
//    if (t->tipo != TOKEN_EOF) {
//        fprintf(stderr, "Erro sintatico: entrada adicional apos fim do programa.");
//        exit(2);
//    }

    return res;
}

Expressao* AnaliseExpressao() {
    // analisa a expressao
    Expressao *res = (Expressao*) malloc(sizeof(Expressao));

    // parentese abrindo
    t = ProximoToken();

    // se proximo token for constante inteira, retorne expressao constante
    if (t->tipo == TOKEN_INT) {
        res->oper = OPER_CONST;
        res->valor = t->valor.longI;
        res->op1 = NULL;
        res->op2 = NULL;
        return res;
    }

    if (t->tipo != TOKEN_SYMBOL && t->valor.longI != '(') {
        fprintf(stderr, "Erro sintatico: '(' esperado");
        exit(2);
    }

    // primeiro operando
    res->op1 = AnaliseExpressao(); // Expressao*

    // operador
    t = ProximoToken();

    if (t->tipo != TOKEN_SYMBOL && (t->valor.longI != '+' && t->valor.longI != '*') ) {
        fprintf(stderr, "Erro sintatico: operador esperado");
        exit(2);
    }

    //TODO switch case
    res->oper = (t->tipo == TOKEN_SYMBOL && t->valor.longI == '+' ? OPER_SOMA : OPER_MULT);

    // segundo operando
    res->op2 = AnaliseExpressao();

    // parentese fechando
    t = ProximoToken();

    if (t->tipo != TOKEN_SYMBOL && t->valor.longI != ')') {
        fprintf(stderr, "Erro sintatico: ')' esperado");
        exit(2);
    }

    return res;
}

void DestroiExpressao(Expressao *e) {

    switch (e->oper) {
        case OPER_SOMA:
        case OPER_MULT:
        case OPER_DIV:
        case OPER_SUB:
            DestroiExpressao(e->op1);
            DestroiExpressao(e->op2);
            e->op1 = NULL;
            e->op2 = NULL;
        default:
            free(e);

    }

}

void DestroiPrograma(Programa *p) {
    DestroiExpressao(p->e);
    p->e = NULL;
    free(p);
}

