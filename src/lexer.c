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
static Token makeTokenAt(TokenType type, const char *lexema, int linha, int coluna) {
    Token token;
    token.type = type;
    strcpy(token.lexema, lexema);
    token.linha = linha;
    token.coluna = coluna;
    return token;
}

// Pega próximo token
Token getNextToken(Lexer *lexer) {
    char buffer[64];
    int i = 0;
    int c;
    int startLinha = 0;
    int startColuna = 0;

    // Ignorar espaços e quebras
    do {
        c = nextChar(lexer);
    } while (isspace(c));

    // posição inicial do token
    startLinha = lexer->linha;
    startColuna = lexer->coluna;

    if (c == EOF) {
        return makeTokenAt(TK_EOF, "EOF", startLinha, startColuna);
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
            return makeTokenAt(lexer->symbolTable->symbols[idx].type, buffer, startLinha, startColuna);
        } else {
            insertSymbol(lexer->symbolTable, buffer, TK_ID);
            return makeTokenAt(TK_ID, buffer, startLinha, startColuna);
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

        return makeTokenAt(isReal ? TK_REALNUM : TK_INT, buffer, startLinha, startColuna);
    }

    // Strings '...'
    if (c == '\'') {
        i = 0;
        while (1) {
            int d = nextChar(lexer);
            if (d == '\n' || d == EOF) {
                return makeTokenAt(TK_ERROR, "string nao-fechada", startLinha, startColuna);
            }
            if (d == '\'') {
                buffer[i] = '\0';
                return makeTokenAt(TK_STRING, buffer, startLinha, startColuna);
            }
            if (i < (int)sizeof(buffer) - 1) {
                buffer[i++] = (char)d;
            }
        }
    }

    // Operadores e símbolos
    switch (c) {
        case '+': return makeTokenAt(TK_PLUS, "+", startLinha, startColuna);
        case '-': return makeTokenAt(TK_MINUS, "-", startLinha, startColuna);
        case '*': return makeTokenAt(TK_MUL, "*", startLinha, startColuna);
        case '/': return makeTokenAt(TK_DIV, "/", startLinha, startColuna);
        case ';': return makeTokenAt(TK_SEM, ";", startLinha, startColuna);
        case ',': return makeTokenAt(TK_COMMA, ",", startLinha, startColuna);
        case '(': return makeTokenAt(TK_LPAREN, "(", startLinha, startColuna);
        case ')': return makeTokenAt(TK_RPAREN, ")", startLinha, startColuna);
        case '.': return makeTokenAt(TK_DOT, ".", startLinha, startColuna);
        case '{': return makeTokenAt(TK_LBRACE, "{", startLinha, startColuna);
        case '}': return makeTokenAt(TK_RBRACE, "}", startLinha, startColuna);
        case ':': {
            c = nextChar(lexer);
            if (c == '=') return makeTokenAt(TK_ASS, ":=", startLinha, startColuna);
            unreadChar(lexer, c);
            return makeTokenAt(TK_COLON, ":", startLinha, startColuna);
        }
        case '=': return makeTokenAt(TK_EQ, "=", startLinha, startColuna);
        case '<': {
            c = nextChar(lexer);
            if (c == '=') return makeTokenAt(TK_LE, "<=", startLinha, startColuna);
            if (c == '>') return makeTokenAt(TK_NE, "<>", startLinha, startColuna);
            unreadChar(lexer, c);
            return makeTokenAt(TK_LT, "<", startLinha, startColuna);
        }
        case '>': {
            c = nextChar(lexer);
            if (c == '=') return makeTokenAt(TK_GE, ">=", startLinha, startColuna);
            unreadChar(lexer, c);
            return makeTokenAt(TK_GT, ">", startLinha, startColuna);
        }
        default:
            buffer[0] = c;
            buffer[1] = '\0';
            return makeTokenAt(TK_ERROR, "caractere invalido", startLinha, startColuna);
    }
}
