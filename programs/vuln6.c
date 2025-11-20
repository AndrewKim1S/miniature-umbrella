#include <stdio.h>
#include <string.h>

#define MAGIC 101010


void crash() {
        printf("[+] Program will intentionally segfault\n");
        int *ptr = NULL;
        *ptr = MAGIC;
}

int check_magic(long x) {
        if(x * 2 == MAGIC) {
                printf("[+] Input matched MAGIC\n");
                return 1;
        }
        printf("[+] Input does not match MAGIC\n");
        return 0;
}

int main(int argc, char **argv) {
        if(argc < 2) {
		printf("[!] ERROR Usage: %s <input_file>\n", argv[0]);
		return 1;
	}

	FILE *f = fopen(argv[1], "rb");
	char numbuf[64];
	if(!fgets(numbuf, sizeof(numbuf), f)) {
		fclose(f);
		return 1;
	}
	numbuf[strcspn(numbuf, "\r\n")] = "\0";
	long x = strtol(numbuf, NULL, 10);

	fclose(f);
	
        if(check_magic(x)) {
                crash();
        }
        return 0;
}
