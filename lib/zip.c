#include "zip.h"

void Zip_All_File (const char *argv[])
{
    const char *src = check_suffix(argv[1], "\\");
    const char *dst = check_suffix(argv[2], "\\");
    const char *filename = check_suffix(argv[3], ".exe");

    const char *workspace = pwd();

    const char *this_prog = argv[0];
    extern const char *final_prog;
    final_prog = cat_string(dst, filename);

    FreeLibrary(dst_exe);               //free handle
    cp(this_prog, final_prog);          //copy this program to dst
    dst_exe = LoadLibrary(final_prog);  //load dst program

    if (is_dir_found(src)) {
        cd(src);
        package();
        cd(workspace);
    }
}

void package ()
{
    Zip_file *zip;
    const char *config;

    change_icon();
    zip = list_dir(".");

    config = make_config(zip->num_dir, zip->num_exe, zip->num_file);
    update_config(config);

    update_dir(zip->dir);
    update_exe(zip->exe);
    update_file(zip->file);

    free_zip(zip);
}

BOOLEAN is_exe (char *filename)
{
    int len = strlen(filename);
    if (filename[len-4] != '.') {
        return FALSE;
    } else if (filename[len-3] != 'e') {
        return FALSE;
    } else if (filename[len-2] != 'x') {
        return FALSE;
    } else if (filename[len-1] != 'e') {
        return FALSE;
    }
    return TRUE;
}

Zip_file* new_Zip_file ()
{
    Zip_file *zip = malloc(sizeof(Zip_file));

    zip->num_dir = 0;
    zip->dir = NULL;
    zip->last_dir = zip->dir;

    zip->num_exe = 0;
    zip->exe = NULL;
    zip->last_exe = zip->exe;

    zip->num_file = 0;
    zip->file = NULL;
    zip->last_file = zip->file;

    return zip;
}

void free_zip(Zip_file *zip)
{
    Dir_file *dir = zip->dir;
    while (dir != NULL) {
        Dir_file *now = dir;
        dir = now->next_dir;
        free(now);
    }

    Exe_file *exe = zip->exe;
    while (exe != NULL) {
        Exe_file *now = exe;
        exe = now->next_exe;
        free(now);
    }

    Data_file *file = zip->file;
    while (file != NULL) {
        Data_file *now = file;
        file = now->next_file;
        free(now);
    }

    free(zip);
}

void merge_zip (Zip_file *zip, const Zip_file *dir_zip)
{
    if(zip->file == NULL) {
        zip->file = dir_zip->file;
        zip->last_file = dir_zip->last_file;
    } else if(dir_zip->file != NULL){
        zip->last_file->next_file = dir_zip->file;
        zip->last_file = dir_zip->last_file;
    }

    if(zip->exe == NULL) {
        zip->exe = dir_zip->exe;
        zip->last_exe = dir_zip->last_exe;
    } else if(dir_zip->exe != NULL) {
        zip->last_exe->next_exe = dir_zip->exe;
        zip->last_exe = dir_zip->last_exe;
    }

    if(zip->dir == NULL) {
        zip->dir = dir_zip->dir;
        zip->last_dir = dir_zip->last_dir;
    } else if(dir_zip->dir != NULL){
        zip->last_dir->next_dir = dir_zip->dir;
        zip->last_dir = dir_zip->last_dir;
    }

    zip->num_dir += dir_zip->num_dir;
    zip->num_exe += dir_zip->num_exe;
    zip->num_file += dir_zip->num_file;
}

void add_zip_file (Zip_file *zip, const char *path, const WIN32_FIND_DATA *data)
{
    Data_file *file = malloc(sizeof(Data_file));

    file->path = malloc(MAX_PATH);
    memcpy(file->path, path, MAX_PATH);

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

void add_zip_exe (Zip_file *zip, const char *path, const WIN32_FIND_DATA *data)
{
    Exe_file *exe = malloc(sizeof(Exe_file));

    exe->path = malloc(MAX_PATH);
    memcpy(exe->path, path, MAX_PATH);

    exe->size = /*(data->nFileSizeHigh * (MAXDWORD+1)) + */data->nFileSizeLow; //max file size only 4GB
    exe->next_exe = NULL;

    if (zip->exe == NULL) {
        zip->exe = exe;
        zip->last_exe = zip->exe;
    } else {
        zip->last_exe->next_exe = exe;
        zip->last_exe = exe;
    }
    zip->num_exe++;
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

const char* fullname(const char *dir, const char *filename)
{
    const char *path;

    path = cat_string(dir, filename);
    path = check_suffix(path, ".exe");

    return path;
}

const char* check_suffix (const char *str, const char *prompt)
{
    int len_prompt = strlen(prompt);
    int len = strlen(str);

    if (len < len_prompt) {
        return cat_string(str, prompt);
    } else if (strcmp(str+len-len_prompt, prompt)) {
        return cat_string(str, prompt);
    } else {
        return str;
    }
}

void cp (const char *src, const char *dst)
{
    FILE *in = fopen(src, "rb");
    FILE *out = fopen(dst, "wb+");

    while (!feof(in)) {
        #define BUF_SIZE 10000
        char buf[BUF_SIZE];
        size_t count = fread(buf, sizeof(char), BUF_SIZE, in);
        fwrite(buf, sizeof(char), count, out);
        #undef BUF_SIZE
    }

    fclose(in);
    fclose(out);
}

void cd (const char *dir)
{
    SetCurrentDirectory(dir);
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
    Zip_file *zip = new_Zip_file();

    WIN32_FIND_DATA *data = malloc(sizeof(WIN32_FIND_DATA));
    HANDLE handle = NULL;

    char *path = malloc(MAX_PATH);
    sprintf(path, "%s\\*.*", dir);

    handle = FindFirstFile(path, data);
    do {
        if(strcmp(data->cFileName, ".") && strcmp(data->cFileName, "..")) {
            sprintf(path, "%s\\%s", dir, data->cFileName);
            if (data->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                //printf("Directory: %s\n", path);
                add_zip_dir(zip, path);
                Zip_file *dir_zip = list_dir(path);
                merge_zip(zip, dir_zip);
                free(dir_zip);
            } else if (is_exe(path)) {
                //printf("ExE: %s\n", path);
                add_zip_exe(zip, path, data);
            } else {
                //printf("File: %s\n", path);
                add_zip_file(zip, path, data);
            }
        }
    } while(FindNextFile(handle, data));
    FindClose(handle);
    free(path);
    free(data);

    return zip;
}

BOOLEAN is_dir_found(const char *dir)
{
    WIN32_FIND_DATA *data = malloc(sizeof(WIN32_FIND_DATA));
    HANDLE handle = NULL;
    BOOLEAN result;

    char *path = malloc(MAX_PATH);
    sprintf(path, "%s\\*.*", dir);

    handle = FindFirstFile(path, data);
    if(handle == INVALID_HANDLE_VALUE) {
        printf("not found [%s]\n", dir);
        result = FALSE;
    } else {
        FindClose(handle);
        result = TRUE;
    }
    free(path);
    free(data);

    return result;
}

