#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
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

    // Cria pasta output se não existir
    #ifdef _WIN32
        mkdir("output");
    #else
        mkdir("output", 0777);
    #endif

    // Extrai nome base do arquivo (sem caminho)
    const char *baseName = strrchr(filename, '/');
    if (!baseName) baseName = filename;
    else baseName++; // pula '/'

    // Remove extensão
    char outputFileName[512];
    strncpy(outputFileName, baseName, sizeof(outputFileName));
    outputFileName[sizeof(outputFileName)-1] = '\0';
    char *dot = strrchr(outputFileName, '.');
    if (dot) *dot = '\0';

    // Caminho completo do arquivo de saída
    char outputPath[1024];
    snprintf(outputPath, sizeof(outputPath), "output/%s.lex", outputFileName);

    FILE *lexFile = fopen(outputPath, "w");
    if (!lexFile) {
        perror("Erro ao criar arquivo .lex");
        return 1;
    }

    SymbolTable table;
    initSymbolTable(&table);

    Lexer lexer;
    initLexer(&lexer, filename, &table);

    printf("Analisando: %s\n", filename);

    while (1) {
        Token token = getNextToken(&lexer);

        if (token.type == TK_INVALID) continue;

        printToken(&token);
        fprintf(lexFile, "[%d:%d] type=%d lexema=\"%s\"\n", token.linha, token.coluna, token.type, token.lexema);

        if (token.type == TK_ERROR) {
            fprintf(stderr, "Erro léxico na linha %d, coluna %d: %s\n", token.linha, token.coluna, token.lexema);
        }

        if (token.type == TK_EOF) break;
    }

    fclose(lexFile);

    printf("\nTabela de Símbolos:\n");
    printSymbolTable(&table);

    closeLexer(&lexer);
    return 0;
}