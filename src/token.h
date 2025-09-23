#ifndef TOKEN_H
#define TOKEN_H

// Enum com os tipos de tokens
typedef enum {
    // Palavras-chave
    TK_PROGRAM, TK_VAR, TK_INTEGER, TK_REAL,
    TK_BEGIN, TK_END, TK_IF, TK_THEN, TK_ELSE,
    TK_WHILE, TK_DO,

    // Identificadores e números
    TK_ID, TK_INT, TK_REALNUM,

    // Operadores
    TK_EQ,     // =
    TK_NE,     // <>
    TK_LT,     // <
    TK_LE,     // <=
    TK_GT,     // >
    TK_GE,     // >=
    TK_PLUS,   // +
    TK_MINUS,  // -
    TK_MUL,    // *
    TK_DIV,    // /
    TK_ASS,    // :=

    // Símbolos
    TK_SEM,    // ;
    TK_COMMA,  // ,
    TK_LPAREN, // (
    TK_RPAREN, // )
    TK_DOT,    // .

    // Erros
    TK_INVALID,
    TK_ERROR,
    TK_UNKNOWN,
    TK_EOF
} TokenType;

// Estrutura de um token
typedef struct {
    TokenType type;
    char lexema[64];
    int linha;
    int coluna;
} Token;

#endif
