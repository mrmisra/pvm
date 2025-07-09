#include <stdio.h>
#include "init.h"
#include "util.h"

int pvm_init() {
    if (dir_exists(".pvm")) {
        printf("Repository already initialized.\n");
        return 1;
    }
    if (make_dir(".pvm") != 0) {
        perror("Failed to create .pvm directory");
        return 1;
    }
    printf("Initialized empty pvm repository in .pvm\n");
    return 0;
}
