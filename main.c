#include <stdio.h>
#include <string.h>
#include "src/headers/init.h"
#include "src/headers/save.h"
#include "src/headers/commit.h"
#include "src/headers/branch.h"
#include "src/headers/history.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: pvm <command> [args]\n");
        return 1;
    }
    if (strcmp(argv[1], "init") == 0) {
        return pvm_init();
    } else if (strcmp(argv[1], "save") == 0) {
        return pvm_save(argc, argv);
    } else if (strcmp(argv[1], "commit") == 0) {
        return pvm_commit(argc, argv);
    } else if (strcmp(argv[1], "branch") == 0) {
        return pvm_branch(argc, argv);
    } else if (strcmp(argv[1], "history") == 0) {
        return pvm_history(argc, argv);
    } else if (strcmp(argv[1], "index") == 0 && argc > 2 && strcmp(argv[2], "--git") == 0) {
        return pvm_index_git();
    } else {
        printf("Unknown command: %s\n", argv[1]);
        return 1;
    }
}
