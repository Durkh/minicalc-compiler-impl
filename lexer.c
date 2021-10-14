// Analisador léxico para MiniCalc

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "arq.h"
#include "lexer.h"

static Buffer *buffer;
static Token *tok = NULL;

static long pos = 0;

void InicializaLexer(char *arqFonte) {
    buffer = CriaBuffer(arqFonte);

    pos = 0;

    tok = (Token*) malloc(sizeof(Token));

    if (tok == NULL) {
        fprintf(stderr, "Erro alocando memoria para o token\n");
        exit(2);
    }

    tok->tipo = TOKEN_EOF;
    tok->valor.longI = 0;
}

bool inline static isEOF() {
    return pos > buffer->tam - 1;
}

static char* TextoToken(long ini, long fim) {
    char *str = (char*) malloc(fim - ini + 1);

    if (str == NULL) {
        fprintf(stderr, "Erro alocando memoria\n");
        exit(1);
    }

    for (int i = 0; i + ini < fim; ++i) {
        str[i] = buffer->cont[i + ini];
    }

    str[fim - ini] = '\0';

    return str;
}

bool inline static isValidSymbol(char c) {
    return (c == '(' || c == ')' || c == '+' || c == '*' ||
            c == '-' || c == '/' || c == '[' || c == ']' ||
            c == '%');
}

void inline static FetchNumber(){

    long initPos = pos;

    while (!isEOF() && isdigit(buffer->cont[pos]))
        pos++;

    char *texto = TextoToken(initPos, pos);

    //se não tem um ponto após o número, então é inteiro
    if(buffer->cont[pos] != '.'){

        tok->tipo = TOKEN_INT;
        tok->valor.longI = atol(texto);
       
    }else{

        //pula o ponto
        pos++;
        //coloca o pos no fim do número fracionário
        while (!isEOF() && isdigit(buffer->cont[pos]))
            pos++;

        //pega toda a string de ponto flutuante
        texto = TextoToken(initPos, pos);
        tok->tipo = TOKEN_FLOAT;
        tok->valor.doubleF = strtod(texto, NULL);
    }
    
    free(texto);
}

// função: ProximoToken
//
// Dado o arquivo-fonte, obtem e retorna o próximo token
Token* ProximoToken() {


    // pula espaços em branco
    while (!isEOF() && isspace(buffer->cont[pos]))
        pos++;

    if(buffer->cont[pos] == '#') {
        while (buffer->cont[pos++] != '\n');
    }

    if (isEOF()) {
        tok->tipo = TOKEN_EOF;
        tok->valor.longI = 0;

        //texto
    } else if (isalpha(buffer->cont[pos])) {

        long initPos = pos;
        while (!isEOF() && !isspace(buffer->cont[pos]))
            pos++;
        // texto do token: entre initPos e pos-1 no buffer
        char *texto = TextoToken(initPos, pos);
        if (strcmp(texto, "print") == 0)
        {
            tok->tipo = TOKEN_PRINT;
            tok->valor.longI = 0;
        } else {
            tok->tipo = TOKEN_ERRO;
            tok->valor.error = TEXT_ERROR;
        }
        free(texto);

        //número
    } else if (isdigit(buffer->cont[pos])) {

        //pega o número da string e o coloca em Tok
        FetchNumber();

        //erro de número literal
        //se junto do número tem algo que não seja um espaço, símbolo conhecido ou fim de string, trata como erro
        if(!isspace(buffer->cont[pos]) && !isValidSymbol(buffer->cont[pos]) && buffer->cont[pos] != '\0'){
            tok->tipo = TOKEN_ERRO;
            tok->valor.error = NUMBER_ERROR;
            //caractere junto do número é tratado junto do número
            pos++;
        }

        //símbolo
    } else if (isValidSymbol(buffer->cont[pos])) {

        tok->tipo = TOKEN_SYMBOL;
        tok->valor.longI = (int)buffer->cont[pos];

        pos++;

    } else {
        tok->tipo = TOKEN_ERRO;
        tok->valor.error = SYMBOL_ERROR;
        pos++;
    }

    return tok;
}

char proximoChar(){
    char buf = buffer->cont[pos];
    if(isValidSymbol(buf))
        return buf;
    else
        return '\0';
}

void FinalizaLexer() {
    DestroiBuffer(buffer);
    free(tok);
}
