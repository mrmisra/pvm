#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "save.h"
#include "util.h"

static void save_recursive(const char *path, FILE *index, int *saved_any) {
    DIR *d = opendir(path);
    if (!d) return;
    struct dirent *entry;
    char fullpath[512];
    while ((entry = readdir(d)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".pvm") == 0)
            continue;
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);
        if (is_ignored(fullpath))
            continue;
        struct stat st;
        if (stat(fullpath, &st) == 0) {
            if (S_ISREG(st.st_mode)) {
                fprintf(index, "%s\n", fullpath);
                printf("Saved %s\n", fullpath);
                *saved_any = 1;
            } else if (S_ISDIR(st.st_mode)) {
                save_recursive(fullpath, index, saved_any);
            }
        }
    }
    closedir(d);
}

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
            save_recursive(".", index, &saved_any);
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
