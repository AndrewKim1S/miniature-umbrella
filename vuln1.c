#include <stdio.h>

int main(int argc, char **argv) {
        int x = 0;
        if(argc > 1) {
                printf("Attempt to read value from file\n");
                FILE *f = fopen(argv[1], "r");
                if(f) { 
                        fscanf(f, "%d", &x);
                        fclose(f);
                }
        } else {
                printf("No file\n");
                return -1;
        }
        if(x == 101010) {
                printf("ERROR!\n");
                int *null_pointer = NULL;
                *null_pointer = x;
        }
        return 0;
}
