//
// Analisador lexico
//

#ifndef MINICALC_LEXER_H
#define MINICALC_LEXER_H

typedef enum tagTipoToken {
    TOKEN_INT,
    TOKEN_FLOAT,
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

} Token;


void InicializaLexer(char *arqFonte);
Token* ProximoToken();
void FinalizaLexer();


#endif //MINICALC_LEXER_H
