#ifndef PVM_UTIL_H
#define PVM_UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int dir_exists(const char *path);
int file_exists(const char *path);
int make_dir(const char *path);
int is_ignored(const char *filename);

#endif // PVM_UTIL_H
