CC = cc
CFLAGS = -Wall -Wextra -std=c11
SRC = src/main.c src/lexer.c src/symbol_table.c
BIN = bin_lexer

.PHONY: all run clean

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -o $(BIN) $(SRC)

run: $(BIN)
	./$(BIN) examples/exemplo1.pas

clean:
	rm -f $(BIN)


