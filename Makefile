cflags := -Wall -Wextra -Wpedantic -std=c23
lib_headers := src/cjson.h src/alloc.h
lib_sources := src/cjson.c src/alloc.c
lib_objects := $(patsubst src/%.c, build/%.o, $(lib_sources))

cli: build/cli.o $(lib_objects)
	cc $(cflags) $^ -o build/cjson

static: $(lib_objects)
	ar rcs build/cjson.a $^

build/%.o: src/%.c $(lib_headers)
	@mkdir -p build
	cc $(cflags) -c $< -o $@

build/cli.o: src/cli.c $(lib_headers)
	@mkdir -p build
	cc $(cflags) -c src/cli.c -o build/cli.o

clean:
	rm -rf build
