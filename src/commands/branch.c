#include <stdio.h>
#include <string.h>
#include "branch.h"
#include "util.h"

int pvm_branch(int argc, char *argv[]) {
    if (!dir_exists(".pvm")) {
        printf("Not a pvm repository. Run 'pvm init' first.\n");
        return 1;
    }
    char branches_file[] = ".pvm/branches";
    if (argc == 2) {
        FILE *f = fopen(branches_file, "r");
        if (!f) {
            printf("No branches found.\n");
            return 0;
        }
        char line[128];
        printf("Branches:\n");
        while (fgets(line, sizeof(line), f)) {
            printf("  %s", line);
        }
        fclose(f);
    } else if (argc == 3) {
        FILE *f = fopen(branches_file, "a");
        if (!f) {
            perror("Failed to open branches file");
            return 1;
        }
        fprintf(f, "%s\n", argv[2]);
        fclose(f);
        printf("Created branch %s\n", argv[2]);
    } else {
        printf("Usage: pvm branch [branchname]\n");
    }
    return 0;
}
