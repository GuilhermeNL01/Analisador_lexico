# Analisador Léxico de MicroPascal

## Descrição do Programa

Este programa implementa um **analisador léxico** para a linguagem MicroPascal. Ele lê um arquivo fonte `.pas`, reconhece tokens válidos da linguagem, identifica erros léxicos e gera um arquivo `.lex` com todos os tokens encontrados. Além disso, mantém uma **Tabela de Símbolos** com palavras reservadas e identificadores.

O programa foi desenvolvido em C e utiliza estruturas para representar tokens e a tabela de símbolos, bem como funções para inicializar, percorrer e manipular esses elementos.

---

## Estrutura de Arquivos

### `token.h`

- Define o **enum **``, com todos os tipos de tokens do MicroPascal (palavras-chave, operadores, símbolos, números, identificadores e erros).
- Define a struct `Token`, que contém:
  - `type`: tipo do token (TokenType)
  - `lexema`: texto correspondente
  - `linha` e `coluna`: posição do token no arquivo

### `symbol_table.h` / `symbol_table.c`

- Define a estrutura `Symbol` e `SymbolTable`.
  - `Symbol`: representa um símbolo da linguagem (lexema + tipo)
  - `SymbolTable`: array de símbolos + contador
- Funções:
  - `initSymbolTable`: inicializa a tabela e insere palavras reservadas
  - `lookupSymbol`: busca um lexema na tabela
  - `insertSymbol`: insere um novo símbolo (identificador) na tabela
  - `printSymbolTable`: imprime o conteúdo da tabela

### `lexer.h` / `lexer.c`

- Estrutura `Lexer` contém:

  - `source`: ponteiro para o arquivo fonte
  - `linha` e `coluna`: posição atual
  - `lastChar`: último caractere lido
  - `symbolTable`: referência à Tabela de Símbolos

- Funções principais:

  - `initLexer`: abre o arquivo fonte e inicializa o lexer
  - `closeLexer`: fecha o arquivo
  - `getNextToken`: lê o próximo token válido do arquivo

- Lógica de reconhecimento de tokens:

  - Ignora espaços, quebras de linha e tabulações
  - Reconhece **identificadores e palavras-chave**
  - Reconhece **números inteiros e reais**
  - Reconhece **operadores e símbolos** (`+`, `-`, `*`, `/`, `:=`, `;`, etc.)
  - Identifica **tokens desconhecidos ou inválidos** (`TK_UNKNOWN`, `TK_INVALID`)

### `main.c`

- Responsável por:
  - Receber o arquivo `.pas` via argumento de linha de comando
  - Inicializar a Tabela de Símbolos e o Lexer
  - Criar a pasta `output` se não existir
  - Executar o loop de reconhecimento de tokens:
    - Ignorar tokens inválidos
    - Imprimir tokens no console
    - Salvar tokens no arquivo `.lex`
    - Detectar e reportar erros léxicos (`TK_ERROR`)
  - Ao final, imprimir a Tabela de Símbolos

### Makefile

- Compila todos os arquivos `.c` em `src` e gera o executável em `bin/bin_lexer`
- Cria a pasta `output` automaticamente
- Alvo `run` permite passar qualquer arquivo `.pas`:
  ```bash
  make run FILE=examples/exemplo1.pas
  ```
- Alvo `clean` remove binário e pasta de saída

---

## Exemplos de Programas Testados

### `erro1.pas`

```pascal
program erro1;
var x: integer;
begin
  x := 1;
end.
```

- Sem erros
- Tokens reconhecidos: `program`, `erro1`, `;`, `var`, `x`, `:`, `integer`, `;`, `begin`, `x`, `:=`, `1`, `;`, `end`, `.`

### `erro2.pas`

```pascal
program erro2
var x: integer;
begin
  x := 1
end
```

- Erros léxicos: falta de `;` após `1`

### `erro3.pas`

```pascal
program erro3;
var x: integer;
begin
  x := 1 + ;
end.
```

- Erro léxico: operador `+` sem segundo operando

### `exemplo1.pas`

```pascal
program exemplo1;
var x, y: integer;
begin
  x := 10;
  y := x + 5;
end.
```

- Tokens corretos reconhecidos e inseridos na Tabela de Símbolos

### `exemplo2.pas`

```pascal
program exemplo2;
var a: integer;
begin
  a := 2 * (3 + 4);
end.
```

- Operações aritméticas com parênteses reconhecidas corretamente

### `exemplo3.pas`

```pascal
program exemplo3;
var r: real;
begin
  r := 3.14;
end.
```

- Reconhece números reais (`TK_REALNUM`)

---

## Funcionamento Geral

1. Compilar o projeto usando `make all` ou `make run`
2. Executar o analisador passando um arquivo `.pas` com `make run FILE=examples/<arquivo>.pas`
3. O analisador:
   - Cria/atualiza `output/<arquivo>.lex` com todos os tokens reconhecidos
   - Imprime tokens e Tabela de Símbolos no console
   - Reporta erros léxicos

Este setup permite testar múltiplos programas MicroPascal e registrar resultados de forma organizada.

Projeto Desenvolvido por:

- Guilherme Nunes Lobo UC22102052
- Samuel Mendonça Toledo de Lima UC22101656
- Victor Assis Oliveira UC22101365
- Talys Alexandre Nobre UC22101806