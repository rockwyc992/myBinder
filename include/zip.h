#ifndef __myBinder_ZIP_H__
#define __myBinder_ZIP_H__

#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "run.h"
#include "struct.h"

extern HANDLE update;

/* main */
void Zip_All_File (const char *argv[]);

/* list_dir, package_dir */
Zip_file* list_dir (const char *dir);
void package (Zip_file *zip, const char *prog, const char *src);

/* Zip_file => new, free, merge */
Zip_file* new_Zip_file ();
void free_zip (Zip_file *zip);
void merge_zip (Zip_file *zip, const Zip_file *dir_zip);

/* Zip_file => add_file*/
void add_zip_dir (Zip_file *zip, const char *path);
void add_zip_exe (Zip_file *zip, const char *path, const WIN32_FIND_DATA *data);
void add_zip_file (Zip_file *zip, const char *path, const WIN32_FIND_DATA *data);

/* string */
const char* fullname (const char *dir, const char *file);
const char* check_suffix (const char *str, const char *prompt);

/* directory */
BOOLEAN is_dir_found(const char *dir);
const char* pwd ();
void cd (const char *dir);
void cp (const char *src, const char *dst);

#endif // __myBinder_ZIP_H__
