#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "token.h"

#define MAX_SYMBOLS 100

typedef struct {
    char lexema[64];
    TokenType type;
} Symbol;

typedef struct {
    Symbol symbols[MAX_SYMBOLS];
    int count;
} SymbolTable;

void initSymbolTable(SymbolTable *table);
int lookupSymbol(SymbolTable *table, const char *lexema);
int insertSymbol(SymbolTable *table, const char *lexema, TokenType type);
void printSymbolTable(SymbolTable *table);

#endif
