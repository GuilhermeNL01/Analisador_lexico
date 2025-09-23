#include <stdio.h>
#include <string.h>
#include "symbol_table.h"

void initSymbolTable(SymbolTable *table) {
    table->count = 0;

    // Palavras-chave
    insertSymbol(table, "program", TK_PROGRAM);
    insertSymbol(table, "var", TK_VAR);
    insertSymbol(table, "integer", TK_INTEGER);
    insertSymbol(table, "real", TK_REAL);
    insertSymbol(table, "begin", TK_BEGIN);
    insertSymbol(table, "end", TK_END);
    insertSymbol(table, "if", TK_IF);
    insertSymbol(table, "then", TK_THEN);
    insertSymbol(table, "else", TK_ELSE);
    insertSymbol(table, "while", TK_WHILE);
    insertSymbol(table, "do", TK_DO);
}

int lookupSymbol(SymbolTable *table, const char *lexema) {
    for (int i = 0; i < table->count; i++) {
        if (strcmp(table->symbols[i].lexema, lexema) == 0) {
            return i;
        }
    }
    return -1;
}

int insertSymbol(SymbolTable *table, const char *lexema, TokenType type) {
    if (lookupSymbol(table, lexema) != -1) {
        return -1; // já existe
    }
    if (table->count >= MAX_SYMBOLS) {
        printf("Erro: tabela de símbolos cheia!\n");
        return -1;
    }
    strcpy(table->symbols[table->count].lexema, lexema);
    table->symbols[table->count].type = type;
    table->count++;
    return table->count - 1;
}

void printSymbolTable(SymbolTable *table) {
    printf("\n=== Tabela de Símbolos ===\n");
    for (int i = 0; i < table->count; i++) {
        printf("%d: %s (%d)\n", i, table->symbols[i].lexema, table->symbols[i].type);
    }
}
