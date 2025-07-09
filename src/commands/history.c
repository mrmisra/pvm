#include <stdio.h>
#include "history.h"

int pvm_history(int argc, char *argv[]) {
    FILE *log = fopen(".pvm/log", "r");
    if (!log) {
        printf("No commit history found.\n");
        return 1;
    }
    char line[512];
    printf("Commit history:\n");
    while (fgets(line, sizeof(line), log)) {
        printf("%s", line);
    }
    fclose(log);
    return 0;
}
