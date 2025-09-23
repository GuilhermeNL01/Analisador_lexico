#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"

static int nextChar(Lexer *lexer) {
    int c = fgetc(lexer->source);
    if (c == '\n') {
        lexer->linha++;
        lexer->coluna = 0;
    } else {
        lexer->coluna++;
    }
    lexer->lastChar = c;
    return c;
}

static void unreadChar(Lexer *lexer, int c) {
    if (c != EOF) {
        ungetc(c, lexer->source);
        lexer->coluna--;
    }
}

// Inicializa o lexer
void initLexer(Lexer *lexer, const char *filename, SymbolTable *table) {
    lexer->source = fopen(filename, "r");
    if (!lexer->source) {
        printf("Erro: não foi possível abrir %s\n", filename);
        exit(1);
    }
    lexer->linha = 1;
    lexer->coluna = 0;
    lexer->lastChar = ' ';
    lexer->symbolTable = table;
}

// Fecha o arquivo
void closeLexer(Lexer *lexer) {
    if (lexer->source) fclose(lexer->source);
}

// Cria token
static Token makeToken(Lexer *lexer, TokenType type, const char *lexema) {
    Token token;
    token.type = type;
    strcpy(token.lexema, lexema);
    token.linha = lexer->linha;
    token.coluna = lexer->coluna;
    return token;
}

// Pega próximo token
Token getNextToken(Lexer *lexer) {
    char buffer[64];
    int i = 0;
    int c;

    // Ignorar espaços e quebras
    do {
        c = nextChar(lexer);
    } while (isspace(c));

    if (c == EOF) {
        return makeToken(lexer, TK_EOF, "EOF");
    }

    // Identificadores ou palavras-chave
    if (isalpha(c)) {
        buffer[i++] = c;
        while (isalnum(c = nextChar(lexer))) {
            buffer[i++] = c;
        }
        buffer[i] = '\0';
        unreadChar(lexer, c);

        // minúsculo para case-insensitive
        for (int j = 0; buffer[j]; j++) buffer[j] = tolower(buffer[j]);

        int idx = lookupSymbol(lexer->symbolTable, buffer);
        if (idx != -1) {
            return makeToken(lexer, lexer->symbolTable->symbols[idx].type, buffer);
        } else {
            insertSymbol(lexer->symbolTable, buffer, TK_ID);
            return makeToken(lexer, TK_ID, buffer);
        }
    }

    // Números (int ou real)
    if (isdigit(c)) {
        buffer[i++] = c;
        int isReal = 0;

        while (isdigit(c = nextChar(lexer))) {
            buffer[i++] = c;
        }

        if (c == '.') {
            isReal = 1;
            buffer[i++] = c;
            while (isdigit(c = nextChar(lexer))) {
                buffer[i++] = c;
            }
        }

        buffer[i] = '\0';
        unreadChar(lexer, c);

        return makeToken(lexer, isReal ? TK_REALNUM : TK_INT, buffer);
    }

    // Operadores e símbolos
    switch (c) {
        case '+': return makeToken(lexer, TK_PLUS, "+");
        case '-': return makeToken(lexer, TK_MINUS, "-");
        case '*': return makeToken(lexer, TK_MUL, "*");
        case '/': return makeToken(lexer, TK_DIV, "/");
        case ';': return makeToken(lexer, TK_SEM, ";");
        case ',': return makeToken(lexer, TK_COMMA, ",");
        case '(': return makeToken(lexer, TK_LPAREN, "(");
        case ')': return makeToken(lexer, TK_RPAREN, ")");
        case '.': return makeToken(lexer, TK_DOT, ".");
        case ':': {
            c = nextChar(lexer);
            if (c == '=') return makeToken(lexer, TK_ASS, ":=");
            unreadChar(lexer, c);
            return makeToken(lexer, TK_UNKNOWN, ":");
        }
        case '=': return makeToken(lexer, TK_EQ, "=");
        case '<': {
            c = nextChar(lexer);
            if (c == '=') return makeToken(lexer, TK_LE, "<=");
            if (c == '>') return makeToken(lexer, TK_NE, "<>");
            unreadChar(lexer, c);
            return makeToken(lexer, TK_LT, "<");
        }
        case '>': {
            c = nextChar(lexer);
            if (c == '=') return makeToken(lexer, TK_GE, ">=");
            unreadChar(lexer, c);
            return makeToken(lexer, TK_GT, ">");
        }
        default:
            buffer[0] = c;
            buffer[1] = '\0';
            return makeToken(lexer, TK_UNKNOWN, buffer);
    }
}
