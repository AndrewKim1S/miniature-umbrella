#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s FILE\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "rb");
    if (!f) {
        perror("fopen");
        return 1;
    }

    unsigned char buf[4];
    size_t n = fread(buf, 1, sizeof(buf), f);
    fclose(f);

    if (n < 4) {
        printf("File too short\n");
        return 1;
    }

    // Simple constraints
    if (buf[0] == 'A' && buf[1] == 'B') {
        if (buf[2] == 'C' && buf[3] == 'D') {
            printf("Crash!\n");
            int *p = NULL;
            *p = 123;   // segfault
        }
    }

    printf("No crash.\n");
    return 0;
}
