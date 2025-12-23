cflags := -Wall -Wextra -Wpedantic

cli: build/cli.o build/cjson.o
	cc $(cflags) $^ -o build/cjson

static: build/cjson.o
	ar rcs build/cjson.a $^

build/cjson.o: src/cjson.c
	cc $(cflags) -c src/cjson.c -o build/cjson.o

build/cli.o: src/cli.c src/cjson.h
	cc $(cflags) -c src/cli.c -o build/cli.o

clean:
	rm -rf build/*
