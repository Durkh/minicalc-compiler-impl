//
// Analisador lexico
//

#ifndef MINICALC_LEXER_H
#define MINICALC_LEXER_H

typedef enum tagTipoToken {
    TOKEN_INT,
    TOKEN_PRINT,
    TOKEN_ERRO,
    TOKEN_SYMBOL,
    TOKEN_EOF
} TipoToken;

typedef enum {
   SYMBOL_ERROR,
   NUMBER_ERROR
} ErrorType;

typedef struct tagToken {
    TipoToken   tipo;
    long        valor;
} Token;


void InicializaLexer(char *arqFonte);
Token* ProximoToken();
void FinalizaLexer();


#endif //MINICALC_LEXER_H
