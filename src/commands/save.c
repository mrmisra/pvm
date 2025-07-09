#ifdef _WIN32
#include <windows.h>
#define popen _popen
#define pclose _pclose
#else
#define _POSIX_C_SOURCE 200809L
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
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

int pvm_index_git(void) {
    // Import tracked files
    FILE *fp = popen("git ls-files", "r");
    if (!fp) {
        printf("Failed to run git ls-files\n");
        return 1;
    }
    FILE *index = fopen(".pvm/index", "w");
    if (!index) {
        perror("Failed to open .pvm/index");
        pclose(fp);
        return 1;
    }
    char line[512];
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\r\n")] = 0;
        fprintf(index, "%s\n", line);
    }
    fclose(index);
    pclose(fp);

    // Import log
    fp = popen("git log --pretty=format:'commit %H%n    %s'", "r");
    if (!fp) {
        printf("Failed to run git log\n");
        return 1;
    }
    FILE *log = fopen(".pvm/log", "w");
    if (!log) {
        perror("Failed to open .pvm/log");
        pclose(fp);
        return 1;
    }
    while (fgets(line, sizeof(line), fp)) {
        fputs(line, log);
    }
    fclose(log);
    pclose(fp);

    // Import branches
    fp = popen("git branch --format='%%(refname:short)'", "r");
    if (!fp) {
        printf("Failed to run git branch\n");
        return 1;
    }
    FILE *branches = fopen(".pvm/branches", "w");
    if (!branches) {
        perror("Failed to open .pvm/branches");
        pclose(fp);
        return 1;
    }
    while (fgets(line, sizeof(line), fp)) {
        fputs(line, branches);
    }
    fclose(branches);
    pclose(fp);

    printf("Imported index, log, and branches from git.\n");
    return 0;
}
