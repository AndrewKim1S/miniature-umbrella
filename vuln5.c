#include <stdio.h>

#define MAGIC 101010

void crash() {
        printf("[+] Program will intentionally segfault\n");
        int *ptr = NULL;
        *ptr = MAGIC;
}

int check_magic(int x) {
        if(x * 2 == MAGIC) {
                printf("[+] Input matched MAGIC\n");
                return 1;
        }
        printf("[+] Input does not match MAGIC\n");
        return 0;
}

int main(int argc, char **argv) {
        int x = 0;
        if(scanf("%d", &x) != 1) { return 0; }

        if(check_magic(x)) {
                crash();
        }
        return 0;
}
