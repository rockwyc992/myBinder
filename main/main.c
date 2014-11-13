#include <stdio.h>
#include <string.h>
#include "dir.h"
#include "zip.h"

int main(int argc, char *argv[])
{
    const char *src = argv[1];
    const char *dst = argv[2];
    const char *filename = argv[3];
    const char *workspace = pwd();
    cd(src);
    Zip_file *zip = list_dir(".");
    cd(workspace);
    package(zip, dst, filename);

    return 0;
}
