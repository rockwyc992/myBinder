#include "dir.h"

BOOLEAN cd(const char* dir)
{
    return SetCurrentDirectory(dir);
}

const char* pwd()
{
    char* dir = malloc(1);
    DWORD size = 0;

    size = GetCurrentDirectory(1, dir);

    free(dir);
    dir = malloc(size);

    size = GetCurrentDirectory(size, dir);

    return dir;
}
