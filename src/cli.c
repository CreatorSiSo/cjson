#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "alloc.h"
#include "cjson.h"

int main(int argc, char* argv[]) {
    printf("Arguments: ");
    for (size_t argi = 0; argi < (size_t)argc; ++argi) {
        if (argi != 0) {
            printf(", ");
        }
        printf("%s", argv[argi]);
    }
    puts("");

    Arena arena = Arena_new();

    constexpr size_t CHUNK_SIZE = 64;
    ByteBuffer input = Buffer_new();
    while (true) {
        Buffer_grow((AnyBuffer*)&input, &arena, CHUNK_SIZE);

        const size_t bytes_read =
            fread(input.data + input.length, 1, CHUNK_SIZE, stdin);
        if (ferror(stdin)) return -1;

        input.length += bytes_read;
        if (feof(stdin)) break;
    }

    printf("Input (%zu bytes):\n", input.length);
    fwrite(input.data, 1, input.length, stdout);
    puts("");

    Json value = Json_parse(ByteSlice_new(input.data, input.length), &arena);

    ByteBuffer pretty_printed = Json_to_string(&value, true);
    fwrite(pretty_printed.data, 1, pretty_printed.length, stdout);

    Arena_debug_print(&arena);
    Arena_destroy(&arena);
}
