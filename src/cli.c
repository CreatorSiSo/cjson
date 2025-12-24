#include <stdint.h>
#include <stdio.h>

#include "cjson.h"

int main(int argc, char* argv[]) {
    printf("Arguments: ");
    for (size_t argi = 0; argi < (size_t)argc; ++argi) {
        if (argi != 0) {
            printf(", ");
        }
        printf("%s", argv[argi]);
    }
    puts("\n");

    Arena arena = Arena_new();
    while (arena.regions < 16) {
        Arena_alloc(&arena, 8);
    }
    Arena_debug_print(&arena);

    Arena_destroy(&arena);
    Arena_debug_print(&arena);
}
