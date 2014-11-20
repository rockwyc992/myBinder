#include "dir.h"

const char* fullname(const char *path, const char *filename)
{
    return filename;
}

BOOLEAN cp (const char *src, const char *dst)
{

}

BOOLEAN cd (const char *dir)
{
    return SetCurrentDirectory(dir);
}

const char* pwd ()
{
    char *dir = malloc(1);
    DWORD size = 0;

    size = GetCurrentDirectory(1, dir);

    free(dir);
    dir = malloc(size);

    size = GetCurrentDirectory(size, dir);

    return dir;
}

Zip_file* list_dir (const char* dir)
{
    Zip_file *zip = NULL;

    WIN32_FIND_DATA *data = malloc(sizeof(WIN32_FIND_DATA));
    HANDLE handle = NULL;

    char *path = malloc(MAX_PATH);
    sprintf(path, "%s\\*.*", dir);

    handle = FindFirstFile(path, data);
    if(handle == INVALID_HANDLE_VALUE) {
        printf("not found [%s]\n", dir);
    } else {
        zip = new_Zip_file();
        do {
            if(strcmp(data->cFileName, ".") && strcmp(data->cFileName, "..")) {
                sprintf(path, "%s\\%s", dir, data->cFileName);
                if (data->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                    printf("Directory: %s\n", path);
                    add_zip_dir(zip, path);
                    Zip_file *dir_zip = list_dir(path);
                    merge_zip(zip, dir_zip);
                    free(dir_zip);
                } else {
                    printf("File: %s\n", path);
                    add_zip_file(zip, path, data);
                }
            }
        } while(FindNextFile(handle, data));
        FindClose(handle);
    }
    free(path);
    free(data);

    return zip;
}
