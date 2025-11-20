// Testing random input
// Crashing input is <0.....>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc < 2) return 1;
    FILE *f = fopen(argv[1], "rb");
    unsigned char buf[32];
    fread(buf, 1, 32, f);
    fclose(f);

    int seed = atoi(buf);
    srand(seed);
    int x  = rand();
    if(x != 0 || x++ > 0) {
        if(x != x*2) {
            printf("[+] Reached random conditions");
        }
    }

    int y = x;
    for(int i = 0; i < x; i++) {
        y = y ^ (i * x);
    }
    if(y == 0) {
        printf("[+] Reached y condition");
    }

    if(buf[0] == '1') {
        int *ptr = NULL;
        *ptr = 1;
    }
}

