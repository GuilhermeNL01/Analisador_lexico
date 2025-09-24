#include "token.h"

const char* tokenTypeToName(TokenType type) {
    switch (type) {
        // Palavras-chave (exibir o próprio lexema)
        case TK_PROGRAM: return "program";
        case TK_VAR: return "var";
        case TK_INTEGER: return "integer";
        case TK_REAL: return "real";
        case TK_BEGIN: return "begin";
        case TK_END: return "end";
        case TK_IF: return "if";
        case TK_THEN: return "then";
        case TK_ELSE: return "else";
        case TK_WHILE: return "while";
        case TK_DO: return "do";

        // Identificadores e números
        case TK_ID: return "ID";
        case TK_INT: return "INTEGER";
        case TK_REALNUM: return "REAL";
        case TK_STRING: return "STRING";

        // Operadores
        case TK_EQ: return "OP_EQ";
        case TK_NE: return "OP_NE";
        case TK_LT: return "OP_LT";
        case TK_LE: return "OP_LE";
        case TK_GT: return "OP_GT";
        case TK_GE: return "OP_GE";
        case TK_PLUS: return "OP_AD";
        case TK_MINUS: return "OP_MIN";
        case TK_MUL: return "OP_MUL";
        case TK_DIV: return "OP_DIV";
        case TK_ASS: return "OP_ASS";

        // Símbolos
        case TK_SEM: return "SMB_SEM";
        case TK_COMMA: return "SMB_COM";
        case TK_LPAREN: return "SMB_OPA";
        case TK_RPAREN: return "SMB_CPA";
        case TK_DOT: return "SMB_DOT";
        case TK_COLON: return "SMB_COL";
        case TK_LBRACE: return "SMB_OBC";
        case TK_RBRACE: return "SMB_CBC";

        // Erros e especiais
        case TK_ERROR: return "LEX_ERROR";
        case TK_INVALID: return "INVALID";
        case TK_UNKNOWN: return "UNKNOWN";
        case TK_EOF: return "EOF";
    }
    return "UNKNOWN";
}


