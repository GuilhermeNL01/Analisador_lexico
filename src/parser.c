#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

static void casaToken(Parser *p, TokenType esperado);

// Helpers para conjuntos FIRST
static int isRelOp(TokenType t) {
    return t == TK_EQ || t == TK_NE || t == TK_LT || t == TK_LE || t == TK_GE || t == TK_GT;
}

static void emitProd(Parser *p, const char *texto) {
    if (p->synOut) {
        fprintf(p->synOut, "%s\n", texto);
    }
    printf("%s\n", texto);
}

static void emitLex(Parser *p, const Token *tok) {
    if (p->lexOut) {
        fprintf(p->lexOut, "<%s, %s> [linha=%d, coluna=%d]\n", tokenTypeToName(tok->type), tok->lexema, tok->linha, tok->coluna);
    }
}

void initParser(Parser *parser, Lexer *lexer, FILE *lexOut, FILE *synOut) {
    parser->lexer = lexer;
    parser->lexOut = lexOut;
    parser->synOut = synOut;
    parser->hadError = 0;
    parser->current = getNextToken(lexer);
}

// Encaminha mensagens de erro conforme formato exigido
static void erroTokenNaoEsperado(Parser *p, const Token *tok) {
    fprintf(stderr, "%d:token nao esperado [%s].\n", tok->linha, tok->lexema);
    p->hadError = 1;
    exit(1);
}

static void erroFimDeArquivo(Parser *p) {
    fprintf(stderr, "%d:fim de arquivo não esperado.\n", p->current.linha);
    p->hadError = 1;
    exit(1);
}

static void avancar(Parser *p) {
    p->current = getNextToken(p->lexer);
    if (p->current.type == TK_ERROR) {
        // Propaga erro léxico como erro sintático para encerrar de imediato
        erroTokenNaoEsperado(p, &p->current);
    }
}

static void casaToken(Parser *p, TokenType esperado) {
    if (p->current.type == TK_EOF) {
        erroFimDeArquivo(p);
    }
    if (p->current.type == esperado) {
        emitLex(p, &p->current);
        avancar(p);
    } else {
        erroTokenNaoEsperado(p, &p->current);
    }
}

// Protótipos das funções de não-terminais
static void bloco(Parser *p);
static void parteDeclVars(Parser *p);
static void declaracaoVariaveis(Parser *p);
static void listaIdentificadores(Parser *p);
static void tipo(Parser *p);
static void comandoComposto(Parser *p);
static void comando(Parser *p);
static void atribuicao(Parser *p);
static void comandoCondicional(Parser *p);
static void comandoRepetitivo(Parser *p);
static void expressao(Parser *p);
static void expressaoSimples(Parser *p);
static void termo(Parser *p);
static void fator(Parser *p);

void parsePrograma(Parser *p) {
    emitProd(p, "programa -> program ID ; bloco .");
    casaToken(p, TK_PROGRAM);
    casaToken(p, TK_ID);
    casaToken(p, TK_SEM);
    bloco(p);
    casaToken(p, TK_DOT);
    if (p->current.type != TK_EOF) {
        // tokens após '.'
        erroTokenNaoEsperado(p, &p->current);
    }
}

static void bloco(Parser *p) {
    emitProd(p, "bloco -> parte_declaracoes comando_composto");
    parteDeclVars(p);
    comandoComposto(p);
}

static void parteDeclVars(Parser *p) {
    // { var <decl> {; <decl>} ; }
    // zero ou mais blocos de var ... ;
    while (p->current.type == TK_VAR) {
        emitProd(p, "parte_declaracoes -> var declaracao_variaveis { ; declaracao_variaveis } ;");
        casaToken(p, TK_VAR);
        declaracaoVariaveis(p);
        // Após declaração, sempre há um ; (que pode ser seguido de outra decl ou é o final)
        casaToken(p, TK_SEM);
        // Pode haver mais declarações na mesma seção var
        while (p->current.type == TK_ID) {
            declaracaoVariaveis(p);
            casaToken(p, TK_SEM);
        }
    }
}

static void declaracaoVariaveis(Parser *p) {
    emitProd(p, "declaracao_variaveis -> lista_identificadores : tipo");
    listaIdentificadores(p);
    casaToken(p, TK_COLON);
    tipo(p);
}

static void listaIdentificadores(Parser *p) {
    emitProd(p, "lista_identificadores -> ID { , ID }");
    casaToken(p, TK_ID);
    while (p->current.type == TK_COMMA) {
        casaToken(p, TK_COMMA);
        casaToken(p, TK_ID);
    }
}

static void tipo(Parser *p) {
    if (p->current.type == TK_INTEGER) {
        emitProd(p, "tipo -> integer");
        casaToken(p, TK_INTEGER);
    } else if (p->current.type == TK_REAL) {
        emitProd(p, "tipo -> real");
        casaToken(p, TK_REAL);
    } else {
        erroTokenNaoEsperado(p, &p->current);
    }
}

static void comandoComposto(Parser *p) {
    emitProd(p, "comando_composto -> begin comando ; { comando ; } end");
    casaToken(p, TK_BEGIN);
    comando(p);
    casaToken(p, TK_SEM);
    while (p->current.type != TK_END) {
        comando(p);
        casaToken(p, TK_SEM);
    }
    casaToken(p, TK_END);
}

static void comando(Parser *p) {
    if (p->current.type == TK_ID) {
        emitProd(p, "comando -> atribuicao");
        atribuicao(p);
    } else if (p->current.type == TK_BEGIN) {
        emitProd(p, "comando -> comando_composto");
        comandoComposto(p);
    } else if (p->current.type == TK_IF) {
        emitProd(p, "comando -> comando_condicional");
        comandoCondicional(p);
    } else if (p->current.type == TK_WHILE) {
        emitProd(p, "comando -> comando_repetitivo");
        comandoRepetitivo(p);
    } else {
        erroTokenNaoEsperado(p, &p->current);
    }
}

static void atribuicao(Parser *p) {
    emitProd(p, "atribuicao -> variavel := expressao");
    // variavel -> ID
    casaToken(p, TK_ID);
    casaToken(p, TK_ASS);
    expressao(p);
}

static void comandoCondicional(Parser *p) {
    emitProd(p, "comando_condicional -> if expressao then comando [ else comando ]");
    casaToken(p, TK_IF);
    expressao(p);
    casaToken(p, TK_THEN);
    comando(p);
    if (p->current.type == TK_ELSE) {
        casaToken(p, TK_ELSE);
        comando(p);
    }
}

static void comandoRepetitivo(Parser *p) {
    emitProd(p, "comando_repetitivo -> while expressao do comando");
    casaToken(p, TK_WHILE);
    expressao(p);
    casaToken(p, TK_DO);
    comando(p);
}

static void expressao(Parser *p) {
    emitProd(p, "expressao -> expressao_simples [ relacao expressao_simples ]");
    expressaoSimples(p);
    if (isRelOp(p->current.type)) {
        // relacao
        emitProd(p, "relacao -> = | <> | < | <= | >= | >");
        // consome o operador relacional atual
        TokenType op = p->current.type;
        casaToken(p, op);
        expressaoSimples(p);
    }
}

static void expressaoSimples(Parser *p) {
    emitProd(p, "expressao_simples -> [ + | - ] termo { ( + | - ) termo }");
    if (p->current.type == TK_PLUS || p->current.type == TK_MINUS) {
        TokenType un = p->current.type;
        casaToken(p, un);
    }
    termo(p);
    while (p->current.type == TK_PLUS || p->current.type == TK_MINUS) {
        TokenType op = p->current.type;
        casaToken(p, op);
        termo(p);
    }
}

static void termo(Parser *p) {
    emitProd(p, "termo -> fator { ( * | / ) fator }");
    fator(p);
    while (p->current.type == TK_MUL || p->current.type == TK_DIV) {
        TokenType op = p->current.type;
        casaToken(p, op);
        fator(p);
    }
}

static void fator(Parser *p) {
    if (p->current.type == TK_ID) {
        emitProd(p, "fator -> variavel");
        // variavel -> ID
        casaToken(p, TK_ID);
    } else if (p->current.type == TK_INT || p->current.type == TK_REALNUM) {
        emitProd(p, "fator -> numero");
        TokenType t = p->current.type;
        casaToken(p, t);
    } else if (p->current.type == TK_LPAREN) {
        emitProd(p, "fator -> ( expressao )");
        casaToken(p, TK_LPAREN);
        expressao(p);
        casaToken(p, TK_RPAREN);
    } else {
        erroTokenNaoEsperado(p, &p->current);
    }
}


