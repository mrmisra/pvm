#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <zlib.h>
#include "commit.h"
#include "util.h"

static int compress_file(const char *src, const char *dst) {
    FILE *in = fopen(src, "rb");
    if (!in) return -1;
    gzFile out = gzopen(dst, "wb");
    if (!out) { fclose(in); return -1; }
    char buf[4096];
    int bytes;
    while ((bytes = fread(buf, 1, sizeof(buf), in)) > 0) {
        if (gzwrite(out, buf, bytes) != bytes) {
            fclose(in); gzclose(out); return -1;
        }
    }
    fclose(in);
    gzclose(out);
    return 0;
}

int pvm_commit(int argc, char *argv[]) {
    if (!dir_exists(".pvm")) {
        printf("Not a pvm repository. Run 'pvm init' first.\n");
        return 1;
    }
    FILE *index = fopen(".pvm/index", "r");
    if (!index) {
        printf("Nothing to commit. Use 'pvm save <file>' first.\n");
        return 1;
    }
    char commit_dir[256];
    time_t now = time(NULL);
    snprintf(commit_dir, sizeof(commit_dir), ".pvm/commit_%ld", now);
    if (make_dir(commit_dir) != 0) {
        perror("Failed to create commit directory");
        fclose(index);
        return 1;
    }
    char file[256];
    while (fgets(file, sizeof(file), index)) {
        file[strcspn(file, "\n")] = 0;
        char dst[512];
        snprintf(dst, sizeof(dst), "%s/%s.gz", commit_dir, strrchr(file, '/') ? strrchr(file, '/')+1 : file);
        if (compress_file(file, dst) == 0) {
            printf("Compressed and saved %s\n", file);
        } else {
            printf("Failed to compress %s\n", file);
        }
    }
    fclose(index);
    remove(".pvm/index");
    FILE *log = fopen(".pvm/log", "a");
    if (log) {
        fprintf(log, "commit %ld\n", now);
        for (int i = 2; i < argc; ++i) {
            if (strcmp(argv[i], "-m") == 0 && i + 1 < argc) {
                fprintf(log, "    %s\n", argv[i+1]);
                break;
            }
        }
        fclose(log);
    }
    printf("Committed as %s\n", commit_dir);
    return 0;
}
