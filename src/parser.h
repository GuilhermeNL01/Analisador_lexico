#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "lexer.h"

typedef struct {
    Lexer *lexer;
    Token current;
    FILE *lexOut;   // opcional: registra tokens consumidos
    FILE *synOut;   // registra produções selecionadas
    int hadError;
} Parser;

void initParser(Parser *parser, Lexer *lexer, FILE *lexOut, FILE *synOut);
void parsePrograma(Parser *parser);

#endif


