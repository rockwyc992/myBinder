#ifndef __myBinder_ZIP_H__
#define __myBinder_ZIP_H__

#include <stdio.h>
#include <string.h>
#include <windows.h>

typedef struct data_file
{
    char *path;
    DWORD size;
    FILE *data;
    struct data_file *next_file;
} Data_file;

typedef struct dir_file
{
    char *path;
    struct dir_file *next_dir;
} Dir_file;

typedef struct zip_file
{
    int num_dir;
    int num_file;
    Dir_file *dir;
    Dir_file *last_dir;
    Data_file *file;
    Data_file *last_file;
} Zip_file;

BOOLEAN package (Zip_file *zip, const char *dst, const char *filename);

Zip_file* new_Zip_file();
void merge_zip (Zip_file *zip, const Zip_file *dir_zip);
void add_zip_file (Zip_file *zip, const char *path, const WIN32_FIND_DATA *data);
void add_zip_dir (Zip_file *zip, const char *path);

#endif // __myBinder_ZIP_H__
