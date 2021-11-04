//
// Created by egidio on 04/10/2021.
//

//
// Analisador sintático para MiniCalc
//

// Analisador descendente recursivo

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "lexer.h"

Token *t;

Declaracao* AnaliseDeclaracao() {
    t = ProximoToken();

    if (t->tipo != TOKEN_VAR)
        return NULL;

    Declaracao *res = (Declaracao*) malloc(sizeof(Declaracao));

    t = ProximoToken();

    if (t->tipo != TOKEN_IDENT) {
        fprintf(stderr, "Erro de sintaxe: identificador esperado\n");
        free(res);
        exit(2);
    }

    strcpy(res->nomeIdent, t->nome);

    t = ProximoToken();

    if (t->tipo != TOKEN_SYMBOL && t->valor.longI != '=') {
        fprintf(stderr, "Erro de sintaxe: '=' esperado\n");
        free(res);
        exit(2);
    }

    res->e = AnaliseExpressao();

    t = ProximoToken();

    if (t->tipo != TOKEN_SYMBOL && t->valor.longI != ';') {
        fprintf(stderr, "Erro de sintaxe: ';' esperado no final da declaracao\n");
        free(res);
        exit(2);
    }

    return res;
}

// Analisador sintático do programa
// Assume que o analisador léxico foi inicializado com o código-fonte
Programa* AnalisePrograma() {
    Programa *res = (Programa*) malloc(sizeof(Programa));

    if (res == NULL) {
        fprintf(stderr, "Erro de alocacao de memoria.");
        exit(1);
    }

    Declaracao *orig = AnaliseDeclaracao();   // origem da lista encadeada
    Declaracao *d = orig;
    if (d != NULL) {
        Declaracao *d2 = AnaliseDeclaracao();
        while (d2 != NULL) {
            d->next = d2;
            d = d2;
            d2 = AnaliseDeclaracao();
        }
        d->next = NULL;
    }

    res->decls = orig;

    if (t->tipo != TOKEN_PRINT) {
        fprintf(stderr, "Erro sintatico: palavra-chave 'print' esperada no inicio do programa.");
        exit(2);
    }

    // analisa a expressao seguinte
    res->e = AnaliseExpressao();


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
        res->valor.longI = t->valor.longI;
        res->integer = true;
        res->op1 = NULL;
        res->op2 = NULL;
        return res;
    }

    if(t->tipo == TOKEN_IDENT){
        res->oper = OPER_VAR;
        res->valor.longI = 0;
        strcpy(res->valor.nomeIdent, t->nome);
        res->op1 = NULL;
        res->op2 = NULL;
        return res;
    }

    if(t->tipo == TOKEN_FLOAT){
        res->oper = OPER_CONST;
        res->valor.doubleF = t->valor.doubleF;
        res->integer = false;
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

    char buf = '\0';
    if (t->tipo == TOKEN_SYMBOL) {
        switch (t->valor.longI) {
            case '+':
                res->oper = OPER_SOMA;
                break;
            case '*':
                buf = proximoChar();
                if(buf == '*'){
                    res->oper = OPER_POW;
                    t = ProximoToken();
                }else {
                    res->oper = OPER_MULT;
                }
                break;
            case '-':
                res->oper = OPER_SUB;
                break;
            case '/':
                res->oper = OPER_DIV;
                break;
            case '%':
                res->oper = OPER_MOD;
                break;
        }
    }else{
        fprintf(stderr, "Erro sintatico: operador esperado");
        exit(2);
    }

    // segundo operando
    res->op2 = AnaliseExpressao();

    if(res->oper == OPER_MOD && !res->op2->integer){
        fprintf(stderr, "Erro sintático: módulo entre ponto flutuante não suportado");
        exit(2);
    }

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
        case OPER_MOD:
        case OPER_POW:
            DestroiExpressao(e->op1);
            DestroiExpressao(e->op2);
            e->op1 = NULL;
            e->op2 = NULL;
        default:
            free(e);

    }

}

void DestroiDeclaracoes(Declaracao *d) {
    Declaracao *d2;
    while (d != NULL) {
        d2 = d->next;
        free(d);
        d = d2;
    }
}

void DestroiPrograma(Programa *p) {
    DestroiExpressao(p->e);
    p->e = NULL;
    free(p);
}

