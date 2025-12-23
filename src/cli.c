#include <stdint.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    printf("Arguments: ");
    for (size_t argi = 0; argi < (size_t)argc; ++argi) {
        if (argi != 0) {
            printf(", ");
        }
        printf("%s", argv[argi]);
    }
    putchar('\n');
}
