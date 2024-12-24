CC = gcc
CFLAGS = -Wall -g -I./include

SRC_DIR = src
BIN_DIR = bin
EXEC = atm

SOURCES = $(wildcard $(SRC_DIR)/*.c)

OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BIN_DIR)/%.o)

build: clean $(BIN_DIR)/$(EXEC)

$(BIN_DIR)/$(EXEC): $(OBJECTS)
	$(CC) $^ -o $@ -lsqlite3

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

run: build
	./$(BIN_DIR)/$(EXEC)

clean:
	rm -rf $(BIN_DIR)/*

all: clean build run

.PHONY: build run clean
