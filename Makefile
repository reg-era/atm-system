CC = gcc
CFLAGS = -Wall -g -I./include

SOURCES = $(wildcard src/**/*.c src/*.c)
OBJECTS = $(patsubst src/%.c, bin/%.o, $(SOURCES))

build: clean prepare bin/atm

bin/atm: $(OBJECTS)
	$(CC) $^ -o $@ -lsqlite3

bin/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

run:
	./bin/atm

prepare:
	@mkdir bin bin/cmd bin/lib

clean:
	rm -rf bin

all: build run