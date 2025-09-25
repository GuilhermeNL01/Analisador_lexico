CC = cc
CFLAGS = -Wall -Wextra -std=c11

SRC_DIR := src
BIN_DIR := bin
OUTPUT_DIR := output

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BIN_DIR)/%.o)

TARGET := $(BIN_DIR)/bin_lexer

FILE ?= examples/exemplo1.pas

.PHONY: all run clean dirs

all: dirs $(TARGET)

dirs:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(OUTPUT_DIR)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

run: all
	./$(TARGET) $(FILE)

clean:
	rm -rf $(BIN_DIR) $(OUTPUT_DIR)