#ifndef __myBinder_STRUCT_H__
#define __myBinder_STRUCT_H__

typedef struct data_file
{
    char *path;
    DWORD size;
    FILE *data;
    struct data_file *next_file;
} Data_file;

typedef struct exe_file
{
    char *path;
    DWORD size;
    FILE *data;
    struct exe_file *next_exe;
} Exe_file;

typedef struct dir_file
{
    char *path;
    struct dir_file *next_dir;
} Dir_file;

typedef struct zip_file
{
    int num_dir;
    int num_exe;
    int num_file;

    Dir_file *dir;
    Dir_file *last_dir;

    Exe_file *exe;
    Exe_file *last_exe;

    Data_file *file;
    Data_file *last_file;
} Zip_file;

#endif // __myBinder_STRUCT_H__
