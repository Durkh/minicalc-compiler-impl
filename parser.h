//
// Created by egidio on 04/10/2021.
//

#ifndef MINICALC_PARSER_H
#define MINICALC_PARSER_H

#include <stdbool.h>
#include "lexer.h"

typedef enum tagTipoOper {
    OPER_SOMA,
    OPER_MULT,
    OPER_SUB,
    OPER_DIV,
    OPER_POW,
    OPER_MOD,
    OPER_CONST,
    OPER_VAR
} TipoOper;

typedef struct tagExpressao {
    struct tagExpressao *op1;
    struct tagExpressao *op2;

    union {
        int64_t longI;
        double  doubleF;
        char nomeIdent[IDENT_TAM_MAX];
    } valor;
    bool integer;

    TipoOper oper;

} Expressao;

typedef struct tagDeclaracao {
    char nomeIdent[IDENT_TAM_MAX];
    Expressao *e;
    struct tagDeclaracao* next;
} Declaracao;

typedef struct tagPrograma {
    Declaracao *decls;
    Expressao *e;
} Programa;

Programa* AnalisePrograma();
Expressao* AnaliseExpressao();
void DestroiPrograma(Programa *p);


#endif //MINICALC_PARSER_H
