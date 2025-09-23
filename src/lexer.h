#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include "token.h"
#include "symbol_table.h"

// Estrutura do analisador léxico
typedef struct {
    FILE *source;
    int linha;
    int coluna;
    int lastChar;
    SymbolTable *symbolTable;
} Lexer;

void initLexer(Lexer *lexer, const char *filename, SymbolTable *table);
Token getNextToken(Lexer *lexer);
void closeLexer(Lexer *lexer);

#endif
