#pragma once

#include <stdlib.h>
#include <string.h>
#include <errno.h>

int read_sysfs_int(const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f) {
        printf("%s(%s): %s\n", __func__, path, strerror(errno));
        exit(1); 
    }

    int v;
    fscanf(f, "%d", &v);
    fclose(f);

    return v; 
}

const char *read_sysfs_str(const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f) {
        printf("%s(%s): %s\n", __func__, path, strerror(errno));
        exit(1); 
    }

    static char buf[4096] = {0};
    fscanf(f, "%s", buf);
    fclose(f);

    return buf;
}
