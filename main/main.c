#include <stdio.h>
#include <string.h>
#include "dir.h"
#include "zip.h"

const char* check_suffix_dotexe(const char *str);

int main(int argc, char *argv[])
{
    const char *src = argv[1];
    const char *dst = argv[2];
    const char *filename = check_suffix_dotexe(argv[3]);
    const char *workspace = pwd();
    cd(src);
    Zip_file *zip = list_dir(".");

    package(zip, dst, filename);
    cd(workspace);

    return 0;
}

const char* check_suffix_dotexe(const char *str)
{
    int len = strlen(str);

    if(strcmp(".exe", str+len-4)) {
        char* new_str = malloc(len+4);

        strcpy(new_str, str);
        strcat(new_str+len-4, ".exe");

        return new_str;
    }

    return str;
}
