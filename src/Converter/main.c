#include "includes.h"
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    for (;;) {
        int opt = getopt(argc, argv, "o:");
        if (opt == -1)
            break;
        switch (opt) {
            case 'o':
                fprintf(stdout, "Got option: %s\n", optarg);
                break;
            default:
                /* Unexpected option */
                return 1;
        }
    }
    return 0;
}