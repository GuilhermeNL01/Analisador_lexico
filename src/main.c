#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "lexer.h"
#include "symbol_table.h"
#include "token.h"
#include "parser.h"

static void printToken(const Token *token) {
    printf("<%s, %s> [linha=%d, coluna=%d]\n", tokenTypeToName(token->type), token->lexema, token->linha, token->coluna);
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

    // Arquivo de produções sintáticas
    char synPath[1024];
    snprintf(synPath, sizeof(synPath), "output/%s.syn", outputFileName);
    FILE *synFile = fopen(synPath, "w");
    if (!synFile) {
        perror("Erro ao criar arquivo .syn");
        return 1;
    }

    SymbolTable table;
    initSymbolTable(&table);

    Lexer lexer;
    initLexer(&lexer, filename, &table);

    printf("Analisando (Sintático): %s\n", filename);

    // Parser: consome tokens e registra também no .lex
    Parser parser;
    initParser(&parser, &lexer, lexFile, synFile);
    parsePrograma(&parser);

    fclose(lexFile);
    fclose(synFile);

    printf("\nTabela de Símbolos:\n");
    printSymbolTable(&table);

    closeLexer(&lexer);
    return 0;
}