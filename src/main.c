#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "symbol_table.h"
#include "token.h"

static void printToken(const Token *token) {
    printf("[%d:%d] type=%d lexema=\"%s\"\n", token->linha, token->coluna, token->type, token->lexema);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Uso: %s <arquivo.pas>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];

    SymbolTable table;
    initSymbolTable(&table);

    Lexer lexer;
    initLexer(&lexer, filename, &table);

    printf("Analisando: %s\n", filename);

    while (1) {
        Token token = getNextToken(&lexer);
        printToken(&token);
        if (token.type == TK_EOF) break;
    }

    printf("\n");
    printSymbolTable(&table);

    closeLexer(&lexer);
    return 0;
}


