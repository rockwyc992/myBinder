#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "dir.h"

const char* check_suffix_dotexe(const char* str);
BOOLEAN list_dir(const char* dir);

struct file
{
    char* path;
    char* size;
    FILE *data;
};

struct zipfile
{
    int file_num;
    struct file* file;
};

void package()
{

}

int main(int argc, char* argv[])
{
    const char* src = argv[1];
    const char* dst = argv[2];
    const char* filename = check_suffix_dotexe(argv[3]);
    const char* workspace = pwd();

    cd(src);

    package();


    list_dir(".");

    return 0;
}

const char* check_suffix_dotexe(const char* str)
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

BOOLEAN list_dir(const char* dir)
{
    WIN32_FIND_DATA data;
    HANDLE find = NULL;

    char path[2048];
    sprintf(path, "%s\\*.*", dir);

    find = FindFirstFile(path, &data);
    if(find == INVALID_HANDLE_VALUE){
        printf("not found [%s]\n", dir);
        return FALSE;
    }

    do {
        if(strcmp(data.cFileName, ".") && strcmp(data.cFileName, "..")) {
            sprintf(path, "%s\\%s", dir, data.cFileName);
            if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                printf("Directory: %s\n", path);
                //list_dir(path);
            } else {
                printf("File: %s\n", path);
            }
        }
    } while(FindNextFile(find, &data));
    FindClose(find);
    return TRUE;
}
