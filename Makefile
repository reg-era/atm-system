CC = gcc
CFLAGS = -Wall -g -I./include

SOURCES = $(wildcard src/**/*.c src/*.c)
OBJECTS = $(patsubst src/%.c, bin/%.o, $(SOURCES))

build: bin/atm

bin/atm: $(OBJECTS)
	$(CC) $^ -o $@ -lsqlite3 -lm

bin/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

run: build
	./bin/atm

prepare:
	@mkdir bin bin/cmd bin/lib

clean:
	rm -rf bin

all: clean prepare build run