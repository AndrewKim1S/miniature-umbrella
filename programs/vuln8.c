#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <file>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "rb");
    if (!f) {
        perror("fopen");
        return 1;
    }

    unsigned char buf[8];
    size_t n = fread(buf, 1, sizeof(buf), f);
    fclose(f);

    if (n < 8) {
        printf("Input too short\n");
        return 1;
    }

    // ===========================================
    // Deep nested conditions (symbolic nightmare)
    // ===========================================

    if (buf[0] == 'A') {
        if (buf[1] == 'B') {
            if (buf[2] == 'C') {
                if (buf[3] == 'D') {
                    if (buf[4] == 'E') {
                        if (buf[5] == 'F') {
                            if (buf[6] == 'G') {
                                if (buf[7] == 'H') {
                                    // Final leaf of 8 nested branches
                                    printf("Deep crash!\n");
                                    int *p = NULL;
                                    *p = 0x1234;   // segfault
                                } else {
                                    printf("Fail at 7\n");
                                }
                            } else {
                                printf("Fail at 6\n");
                            }
                        } else {
                            printf("Fail at 5\n");
                        }
                    } else {
                        printf("Fail at 4\n");
                    }
                } else {
                    printf("Fail at 3\n");
                }
            } else {
                printf("Fail at 2\n");
            }
        } else {
            printf("Fail at 1\n");
        }
    } else {
        printf("Fail at 0\n");
    }

    return 0;
}

