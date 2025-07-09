#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "save.h"
#include "util.h"

int pvm_save(int argc, char *argv[]) {
    if (!dir_exists(".pvm")) {
        printf("Not a pvm repository. Run 'pvm init' first.\n");
        return 1;
    }
    FILE *index = fopen(".pvm/index", "a");
    if (!index) {
        perror("Failed to open index");
        return 1;
    }
    int saved_any = 0;
    for (int i = 2; i < argc; ++i) {
        if (strcmp(argv[i], ".") == 0) {
            DIR *d = opendir(".");
            if (!d) {
                perror("Failed to open current directory");
                continue;
            }
            struct dirent *entry;
            while ((entry = readdir(d)) != NULL) {
                if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".pvm") == 0)
                    continue;
                if (is_ignored(entry->d_name))
                    continue;
                struct stat st;
                if (stat(entry->d_name, &st) == 0 && S_ISREG(st.st_mode)) {
                    fprintf(index, "%s\n", entry->d_name);
                    printf("Saved %s\n", entry->d_name);
                    saved_any = 1;
                }
            }
            closedir(d);
        } else if (file_exists(argv[i])) {
            if (is_ignored(argv[i])) {
                printf("Ignored %s\n", argv[i]);
                continue;
            }
            fprintf(index, "%s\n", argv[i]);
            printf("Saved %s\n", argv[i]);
            saved_any = 1;
        } else {
            printf("File not found: %s\n", argv[i]);
        }
    }
    fclose(index);
    if (!saved_any) {
        printf("No files saved.\n");
        return 1;
    }
    return 0;
}
