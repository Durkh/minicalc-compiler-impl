#include <stdio.h>
#include <stdlib.h>

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
            printf(" - Valor do token: %li\n", t->valor);
            break;
        case TOKEN_PRINT:
            printf("TOKEN_PRINT\n");
            break;
        case TOKEN_EOF:
            printf("TOKEN_EOF\n");
            break;
        case TOKEN_SYMBOL:
            printf("TOKEN_SYMBOL");
            printf(" - Valor do token: %c\n", (char)t->valor);
            break;
        case TOKEN_ERRO:
            printf("TOKEN_ERRO\n");
            switch (t->valor) {
                case SYMBOL_ERROR:
                    printf("- Erro de símbolo\n");
                    break;
                case NUMBER_ERROR:
                    printf("- Erro de número\n");
                    break;
                default:
                    puts("Erro desconhecido");
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
