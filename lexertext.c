//
// Created by egidio on 04/10/2021.
//

#include <stdio.h>

#include "arq.h"
#include "lexer.h"

void ImpBuffer(Buffer *b) {
    for (long c = 0; c < b->tam; ++c)
        putchar(b->cont[c]);
}

void ImprimeToken(Token* t) {
    switch(t->tipo) {
        case TOKEN_INT:
            printf("TOKEN_INT");
            printf(" - valor do token: %li\n", t->valor.longI);
            break;
        case TOKEN_FLOAT:
            printf("TOKEN_FLOAT");
            printf(" - valor do token: %lf\n", t->valor.doubleF);
            break;
        case TOKEN_PRINT:
            printf("TOKEN_PRINT\n");
            break;
        case TOKEN_EOF:
            printf("TOKEN_EOF\n");
            break;
        case TOKEN_SYMBOL:
            printf("TOKEN_SYMBOL");
            printf(" - valor do token: %c\n", (char)t->valor.longI);
            break;
        case TOKEN_ERRO:
            printf("TOKEN_ERRO");
            switch (t->valor.error) {
                case SYMBOL_ERROR:
                    printf(" - Erro de símbolo\n");
                    break;
                case NUMBER_ERROR:
                    printf(" - Erro de número\n");
                    break;
                default:
                    puts("- Erro desconhecido");
            }
            break;
        default:
            printf("Tipo desconhecido\n");
    }
}

int main() {
    InicializaLexer("../test/literal.mc");

    Token *t = ProximoToken();

    while (t->tipo != TOKEN_EOF) {
        printf("Tipo do token: ");
        ImprimeToken(t);

        t = ProximoToken();
    }

    FinalizaLexer();
    return 0;
}