# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -std=c11

# Source and build directories
SRC_DIR := src
BIN_DIR := bin
OUTPUT_DIR := output

# Find all .c files in src
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BIN_DIR)/%.o)

# Executable name
TARGET := $(BIN_DIR)/bin_lexer

# Default Pascal file to run
FILE ?= examples/exemplo1.pas

.PHONY: all run clean dirs

# Build all
all: dirs $(TARGET)

# Create required directories
dirs:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(OUTPUT_DIR)

# Compile each .c to .o
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link all objects into the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Run the analyzer with any file
run: all
	./$(TARGET) $(FILE)

# Clean build
clean:
	rm -rf $(BIN_DIR) $(OUTPUT_DIR)