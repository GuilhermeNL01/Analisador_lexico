# Analisador Léxico de MicroPascal

## Descrição do Programa

Este programa implementa um **analisador léxico** para a linguagem MicroPascal. Ele lê um arquivo fonte `.pas`, reconhece tokens válidos da linguagem, identifica erros léxicos e gera um arquivo `.lex` com todos os tokens encontrados. Além disso, mantém uma **Tabela de Símbolos** com palavras reservadas e identificadores.

O programa foi desenvolvido em C e utiliza estruturas para representar tokens e a tabela de símbolos, bem como funções para inicializar, percorrer e manipular esses elementos.

---

## Funcionalidades Principais

- **Reconhecimento de tokens** (case-insensitive):
  - Palavras-chave: `program`, `var`, `integer`, `real`, `begin`, `end`, `if`, `then`, `else`, `while`, `do`.
  - Identificadores: `ID` (letra seguida de letras/dígitos).
  - Números: inteiros (`INTEGER`) e reais com ponto (`REAL`, ex.: `2.5`).
  - Operadores: `OP_AD(+)`, `OP_MIN(-)`, `OP_MUL(*)`, `OP_DIV(/)`, `OP_ASS(:=)`, `OP_EQ(=)`, `OP_NE( <>)`, `OP_LT(<)`, `OP_LE(<=)`, `OP_GT(>)`, `OP_GE(>=)`.
  - Símbolos: `SMB_SEM(;)`, `SMB_COM(,)`, `SMB_OPA(()`, `SMB_CPA())`, `SMB_DOT(.)`, `SMB_COL(:)`, `SMB_OBC({)`, `SMB_CBC(})`.
  - Literais de string: `STRING` entre aspas simples `'...'`.

- **Posição exata dos tokens**: cada token é emitido com a posição de início `[linha, coluna]`.

- **Tabela de Símbolos (TS)**:
  - Iniciada com todas as palavras-chave.
  - Apenas palavras-chave e identificadores são cadastrados; sem duplicatas.
  - Impressão legível mostrando `lexema` e o nome do token.

- **Erros léxicos tratados**:
  - Caractere inválido: reportado como `LEX_ERROR` com linha e coluna.
  - String não-fechada antes da quebra de linha/EOF: `LEX_ERROR`.
  - Comentários não são parte da linguagem alvo (MicroPascal desta atividade), então `{` e `}` são tratados como símbolos.

- **Formato do arquivo de saída `.lex`**:
  - Cada linha segue o formato: `<nome, lexema> [linha=X, coluna=Y]`.
  - Ex.: `<OP_AD, +> [linha=5, coluna=12]` ou `<ID, soma> [linha=3, coluna=1]`.

## Estrutura de Arquivos

### `token.h`

- Define o enum `TokenType`, com todos os tipos de tokens do MicroPascal (palavras-chave, operadores, símbolos, números, identificadores, literais e erros).
- Define a struct `Token`, que contém:
  - `type`: tipo do token (TokenType)
  - `lexema`: texto correspondente
  - `linha` e `coluna`: posição do token no arquivo
 - Define `tokenTypeToName(TokenType)`: converte o tipo para o nome exibido no `.lex` (ex.: `OP_AD`, `SMB_SEM`, `ID`, etc.).

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
  - `getNextToken`: lê o próximo token válido do arquivo e retorna também a posição de início do token

- Lógica de reconhecimento de tokens (AFD implementado em código):

  - Ignora espaços, quebras de linha e tabulações
  - Reconhece **identificadores e palavras-chave**
  - Reconhece **números inteiros e reais**
  - Reconhece **operadores e símbolos** (`+`, `-`, `*`, `/`, `:=`, `;`, etc.)
  - Reconhece literais de **string** `'...'` e reporta **string não-fechada** como erro
  - Reporta **caracteres inválidos** como `LEX_ERROR`

### `main.c`

- Responsável por:
  - Receber o arquivo `.pas` via argumento de linha de comando
  - Inicializar a Tabela de Símbolos e o Lexer
  - Criar a pasta `output` se não existir
  - Executar o loop de reconhecimento de tokens:
    - Imprimir tokens no console no formato `<nome, lexema> [linha=X, coluna=Y]`
    - Salvar tokens no arquivo `.lex` com o mesmo formato
    - Detectar e reportar erros léxicos (`LEX_ERROR`) tanto no console quanto no `.lex`
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
var x: integer
begin
  x := 1;
end.
```

- Falta da ; 
- Tokens reconhecidos: `program`, `erro1`, `;`, `var`, `x`, `:`, `integer`, `;`, `begin`, `x`, `:=`, `1`, `;`, `end`, `.`

### `erro2.pas`

```pascal
program erro2;
var x: integer;
begin
  x := 1;
  y := 2;  { uso de identificador não declarado é erro semântico/sintático, não léxico }
end.
```

- Observação: erros de sintaxe/semântica não são detectados nesta etapa léxica. O `.lex` ainda é gerado normalmente.

### `erro3.pas`

```pascal
program erro3;
begin
  s := 'string sem fim
end.
```

- Erro léxico: string não-fechada antes da quebra de linha

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

- Reconhece números reais (`REAL`)

---

## Funcionamento Geral

1. Compilar o projeto usando `make all` ou `make run`
2. Executar o analisador passando um arquivo `.pas` com `make run FILE=examples/<arquivo>.pas`
3. O analisador:
   - Cria/atualiza `output/<arquivo>.lex` com todos os tokens reconhecidos
   - Imprime tokens e Tabela de Símbolos no console
   - Reporta erros léxicos (caractere inválido, string não-fechada)

---

## Nomes dos Tokens (convenção de saída)

- Operadores: `OP_EQ`, `OP_NE`, `OP_LT`, `OP_LE`, `OP_GT`, `OP_GE`, `OP_AD`, `OP_MIN`, `OP_MUL`, `OP_DIV`, `OP_ASS`
- Símbolos: `SMB_SEM`, `SMB_COM`, `SMB_OPA`, `SMB_CPA`, `SMB_DOT`, `SMB_COL`, `SMB_OBC`, `SMB_CBC`
- Palavras-chave: `program`, `var`, `integer`, `real`, `begin`, `end`, `if`, `then`, `else`, `while`, `do`
- Identificador: `ID`
- Números: `INTEGER`, `REAL`
- Strings: `STRING`
- Erros: `LEX_ERROR`

---

## Analisador Sintático (Parser)

- Implementado como parser recursivo descendente, baseado na gramática fornecida.
- Função principal: `parsePrograma`, que chama os procedimentos para cada não-terminal:
  - `programa`, `bloco`, `parte_declaracoes`, `declaracao_variaveis`, `lista_identificadores`, `tipo`,
    `comando_composto`, `comando`, `atribuicao`, `comando_condicional`, `comando_repetitivo`,
    `expressao`, `expressao_simples`, `termo`, `fator`.
- Procedimento `CasaToken` compara o token corrente ao esperado; em caso de sucesso consome o token; em caso de falha, emite erro e encerra.
- Mensagens de erro (formato exigido):
  - `nn:token nao esperado [lex].`
  - `nn:fim de arquivo não esperado.`
- Saída das produções: a sequência das regras escolhidas é escrita em `output/<arquivo>.syn` e também exibida no console.
- O parser consome os tokens do analisador léxico (mantendo a geração de `output/<arquivo>.lex`).

---

## Entregáveis (atividade)

- Figura do AFD (JFLAP) em `docs/`.
- Código-fonte completo.
- Relatório técnico (este README) descrevendo as structs e funções, além de testes (mín. 3 corretos e 3 com erro), com as respectivas saídas `.lex` e a Tabela de Símbolos.

Este setup permite testar múltiplos programas MicroPascal e registrar resultados de forma organizada.

Projeto Desenvolvido por:

- Guilherme Nunes Lobo UC22102052
- Samuel Mendonça Toledo de Lima UC22101656
- Victor Assis Oliveira UC22101365
- Talys Alexandre Nobre UC22101806
