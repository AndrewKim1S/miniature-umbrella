#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc < 2) return 1;
    FILE *f = fopen(argv[1], "rb");
    unsigned char buf[32];
    fread(buf, 1, 32, f);
    fclose(f);

    unsigned long acc = 0;

    for (int i = 0; i < 32; i++) {
        // symbolic explosion: each step depends on ALL prior steps
        acc = (acc * 1315423911u) ^ buf[i];
    }

    if (acc == 0x41414141) {
        int *ptr = NULL;
        *ptr = 1; // crash
    }
    if(buf[2] == 'A') {
        int *ptr = NULL;
        *ptr = 1;
    }
}

