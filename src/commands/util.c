#include "util.h"

int dir_exists(const char *path) {
    struct stat st;
    return stat(path, &st) == 0 && S_ISDIR(st.st_mode);
}

int file_exists(const char *path) {
    struct stat st;
    return stat(path, &st) == 0 && S_ISREG(st.st_mode);
}

int make_dir(const char *path) {
    return mkdir(path, 0755);
}

int is_ignored(const char *filename) {
    FILE *ignore = fopen(".pvmginore", "r");
    if (!ignore) return 0;
    char line[256];
    int ignored = 0;
    while (fgets(line, sizeof(line), ignore)) {
        line[strcspn(line, "\r\n")] = 0;
        if (line[0] == '\0' || line[0] == '#') continue;
        if (strcmp(line, filename) == 0) {
            ignored = 1;
            break;
        }
    }
    fclose(ignore);
    return ignored;
}
