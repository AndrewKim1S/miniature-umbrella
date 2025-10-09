#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void vuln_copy(const char *p) {
    char buf[64];
    // real overflow if p is long
    strcpy(buf, p);
    // use buf so optimizer doesn't remove it
    if (buf[0] == 'Z') {
        // some harmless action
        puts("Z starts buffer");
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <infile>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "rb");
    if (!f) { perror("fopen"); return 1; }

    char line[256];
    // read first line as "mode"
    if (!fgets(line, sizeof(line), f)) { fclose(f); return 1; }
    line[strcspn(line, "\r\n")] = 0;

    // read rest as payload
    char payload[512] = {0};
    size_t r = fread(payload, 1, sizeof(payload)-1, f);
    payload[r] = 0;
    fclose(f);

    // *Special explicit check that is unrelated to overflow*
    if (strcmp(line, "MAGIC") == 0 && strstr(payload, "EXPLOIT") != NULL) {
        // we observed crashes on inputs that had this check true
        // but the overflow is actually possible for any long payload
        printf("special magic observed\n");
        // intentionally do something that might amplify the crash's visibility
        // (but not necessary for overflow)
        // e.g., touch something
    }

    // vulnerable copy happens regardless of the magic check
    vuln_copy(payload);

    return 0;
}
