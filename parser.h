//
// Created by egidio on 04/10/2021.
//

#ifndef MINICALC_PARSER_H
#define MINICALC_PARSER_H

#include <stdbool.h>

typedef enum tagTipoOper {
    OPER_SOMA,
    OPER_MULT,
    OPER_SUB,
    OPER_DIV,
    OPER_POW,
    OPER_MOD,
    OPER_CONST
} TipoOper;

typedef struct tagExpressao {
    struct tagExpressao *op1;
    struct tagExpressao *op2;

    union {
        int64_t longI;
        double  doubleF;
    } valor;
    bool integer;

    TipoOper oper;

} Expressao;

typedef struct tagPrograma {
    Expressao *e;
} Programa;

Programa* AnalisePrograma();
Expressao* AnaliseExpressao();
void DestroiPrograma(Programa *p);


#endif //MINICALC_PARSER_H
