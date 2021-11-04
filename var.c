//
// Created by egidio on 04/11/2021.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "var.h"
#include "lexer.h"

#define MAX_VARS      60

typedef struct tagItem {
    char nome[IDENT_TAM_MAX];

    bool isInteger;
    union {
      int64_t integer;
      double floating;
    } valor;

} Item;

Item tabela[MAX_VARS];
int tamanho = 0;

void AdicionaVar(char* nome, ExprRes valor) {
    if (tamanho >= MAX_VARS) {
        fprintf(stderr, "Tabela de variaveis esgotada.\n");
        exit(4);
    }

    if(valor.isInteger) {
        tabela[tamanho].valor.integer = valor.valor.integer;
    } else{
        tabela[tamanho].valor.floating = valor.valor.floating;
    }

    strcpy(tabela[tamanho].nome, nome);
    tamanho++;
}

bool ConsultaVar(char* nome, ExprRes* valor) {
    for (int i = 0; i < tamanho; i++) {
        if (strcmp(nome, tabela[i].nome) == 0) {
            if(valor->isInteger) {
            valor->valor.integer = tabela[i].valor.integer;
            } else{
                valor->valor.floating = tabela[i].valor.floating;
            }
            return true;
        }
    }
    return false;
}