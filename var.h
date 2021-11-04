//
// Created by egidio on 04/11/2021.
//

#ifndef MINICALC_VAR_H
#define MINICALC_VAR_H

#include <stdbool.h>

typedef struct {
    bool isInteger;
    union {
        int64_t integer;
        double floating;
    }valor;
}ExprRes;


void AdicionaVar(char* nome, ExprRes valor);
bool ConsultaVar(char* nome, ExprRes* valor);

#endif //MINICALC_VAR_H
