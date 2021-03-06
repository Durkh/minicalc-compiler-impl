//
// Analisador lexico
//

#ifndef MINICALC_LEXER_H
#define MINICALC_LEXER_H

#define IDENT_TAM_MAX  60

typedef enum tagTipoToken {
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_VAR,
    TOKEN_IDENT,
    TOKEN_PRINT,
    TOKEN_ERRO,
    TOKEN_SYMBOL,
    TOKEN_EOF
} TipoToken;

typedef enum {
   SYMBOL_ERROR,
   NUMBER_ERROR,
   TEXT_ERROR
} ErrorType;

typedef struct tagToken {
    TipoToken   tipo;

    //união de 64 bits
    union {
        int     error;
        int64_t longI;
        double  doubleF;
    } valor;

    char        nome[IDENT_TAM_MAX];
} Token;


void InicializaLexer(char *arqFonte);
Token* ProximoToken();
char proximoChar();
void FinalizaLexer();


#endif //MINICALC_LEXER_H
