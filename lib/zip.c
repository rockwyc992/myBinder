#include "zip.h"

BOOLEAN package (Zip_file *zip, const char *dst, const char *filename)
{
    char *full_name = malloc(MAX_PATH);
    sprintf(full_name, "%s\\%s", dst, filename);
    //FILE *goal = fopen(full_name, "w+");
    //printf("%s %d", full_name, goal);
    //fprintf(goal, "%d %d", zip->num_file, zip->num_dir);
    Data_file *file = zip->file;
    while(file != NULL) {
        printf("%s %d\n", file->path, file->size);
        file = file->next_file;
    }
    return TRUE;
}

Zip_file* new_Zip_file ()
{
    Zip_file *zip = malloc(sizeof(Zip_file));

    zip->num_dir = 0;
    zip->dir = NULL;
    zip->last_dir = zip->dir;

    zip->num_file = 0;
    zip->file = NULL;
    zip->last_file = zip->file;

    return zip;
}

void merge_zip (Zip_file *zip, const Zip_file *dir_zip)
{
    if(zip->file == NULL) {
        zip->file = dir_zip->file;
        zip->last_file = dir_zip->last_file;
    } else {
        zip->last_file->next_file = dir_zip->file;
        zip->last_file = dir_zip->last_file;
    }

    if(zip->dir == NULL) {
        zip->dir = dir_zip->dir;
        zip->last_dir = dir_zip->last_dir;
    } else {
        zip->last_dir->next_dir = dir_zip->dir;
        zip->last_dir = dir_zip->last_dir;
    }

    zip->num_dir += dir_zip->num_dir;
    zip->num_file += dir_zip->num_file;
}

void add_zip_file (Zip_file *zip, const char *path, const WIN32_FIND_DATA *data)
{
    Data_file *file = malloc(sizeof(Data_file));

    file->path = malloc(MAX_PATH);
    memcpy(file->path, path, MAX_PATH);

    file->data = NULL;
    file->size = /*(data->nFileSizeHigh * (MAXDWORD+1)) + */data->nFileSizeLow; //max file size only 4GB
    file->next_file = NULL;

    if (zip->file == NULL) {
        zip->file = file;
        zip->last_file = zip->file;
    } else {
        zip->last_file->next_file = file;
        zip->last_file = file;
    }
    zip->num_file++;
}

void add_zip_dir (Zip_file *zip, const char *path)
{
    Dir_file *dir = malloc(sizeof(Dir_file));

    dir->path = malloc(MAX_PATH);
    memcpy(dir->path, path, MAX_PATH);

    dir->next_dir = NULL;

    if (zip->dir == NULL) {
        zip->dir = dir;
        zip->last_dir = zip->dir;
    } else {
        zip->last_dir->next_dir = dir;
        zip->last_dir = dir;
    }
    zip->num_dir++;
}
