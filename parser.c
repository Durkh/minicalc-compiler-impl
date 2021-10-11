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

    if (t->tipo != TOKEN_SYMBOL && (t->valor.longI != '(' && t->valor.longI != '[')) {
        fprintf(stderr, "Erro sintatico: '(' ou ']' esperado");
        exit(2);
    }

    char delim = (char)t->valor.longI;

    // primeiro operando
    res->op1 = AnaliseExpressao(); // Expressao*

    // operador
    t = ProximoToken();

    if (t->tipo == TOKEN_SYMBOL) {
        switch (t->valor.longI) {
            case '+':
                res->oper = OPER_SOMA;
                break;
            case '*':
                res->oper = OPER_MULT;
                break;
            case '-':
                res->oper = OPER_SUB;
                break;
            case '/':
                res->oper = OPER_DIV;
                break;
        }
    }else{
        fprintf(stderr, "Erro sintatico: operador esperado");
        exit(2);
    }

    // segundo operando
    res->op2 = AnaliseExpressao();

    // parentese fechando
    t = ProximoToken();

    if (t->tipo == TOKEN_SYMBOL) {
        switch (delim) {
        case '(':
            if ((char)t->valor.longI != ')') {
                fprintf(stderr, "Erro sintatico: ')' esperado");
                exit(2);
            }
            break;
        case '[':
            if ((char)t->valor.longI != ']') {
                fprintf(stderr, "Erro sintatico: ']' esperado");
                exit(2);
            }
            break;
        default:
           fprintf(stderr, "Erro sintatico: Símbolo desconhecido");
           exit(2);
        }
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

