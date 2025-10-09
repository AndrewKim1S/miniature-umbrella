#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAGIC_CRASH 100100
#define BUF_SIZE 64

static void do_copy(const char *payload) {
    char buf[BUF_SIZE];            // stack buffer
    // Vulnerable: no bounds check
    strcpy(buf, payload);
    printf("copied: %.10s\n", buf);
}

static void do_print_format(const char *payload) {
    // Vulnerable: direct format string
    printf(payload);
    printf("\n");
}

static void do_alloc_and_write(size_t n, const char *payload) {
    // Vulnerable: possible integer overflow when computing n * sizeof(int)
    size_t need = n * sizeof(int);
    int *arr = (int *)malloc(need);
    if (!arr) {
        printf("alloc failed\n");
        return;
    }
    // If integer overflow happened, we can write past the allocation
    for (size_t i = 0; i < n; i++) {
        arr[i] = 0x41414141;  // write pattern
    }
    printf("wrote %zu ints\n", n);
    free(arr);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "rb");
    if (!f) {
        perror("fopen");
        return 1;
    }

    // Read first two lines: command and a number
    char cmd[32];
    if (!fgets(cmd, sizeof(cmd), f)) {
        fclose(f);
        return 1;
    }
    // strip newline
    cmd[strcspn(cmd, "\r\n")] = '\0';

    char numbuf[64];
    if (!fgets(numbuf, sizeof(numbuf), f)) {
        fclose(f);
        return 1;
    }
    numbuf[strcspn(numbuf, "\r\n")] = '\0';

    long val = strtol(numbuf, NULL, 10);

    // read rest of file into payload buffer
    char payload[256] = {0};
    size_t r = fread(payload, 1, sizeof(payload)-1, f);
    // ensure null-terminated
    payload[r] = '\0';

    fclose(f);

    if (strcmp(cmd, "COPY") == 0) {
        // stack buffer overflow possible via payload
        do_copy(payload);
    } else if (strcmp(cmd, "PRINT") == 0) {
        // format string vulnerability
        do_print_format(payload);
    } else if (strcmp(cmd, "ALLOC") == 0) {
        // integer overflow / heap corruption route
        if (val <= 0) {
            printf("bad size\n");
        } else {
            do_alloc_and_write((size_t)val, payload);
        }
    } else if (strcmp(cmd, "MAGIC") == 0) {
        // magic-number triggered null deref if value matches
        if (val == MAGIC_CRASH) {
            printf("magic hit!\n");
            int *p = NULL;
            *p = 42; // intentional crash
        } else {
            printf("magic not hit: %ld\n", val);
        }
    } else {
        printf("unknown command: %s\n", cmd);
    }

    return 0;
}
