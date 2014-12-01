#ifndef __myBinder_RUN_H__
#define __myBinder_RUN_H__

#include <stdio.h>
#include <windows.h>
#include "struct.h"
#include "resource.h"

extern HMODULE dst_exe;

/* main */
void Extract_and_Run();

/* Run and Extract */
void Run(int id, char *path);
void Extract(int id, const char *filename);
char* Load_file(const char *path, int size);

void RunFromMemory(char*, char*);
void Update(int id, const char* data, int size);

/* size of resource */
DWORD res_size(int id);

/* what is this? */
SECURITY_ATTRIBUTES secAttrib;

/* change icon */
void change_icon ();

/* make config, string*/
const char* make_config (int dir, int exe, int file);
const char* make_string (int num);

/* merge string */
const char* cat_string (const char *first, const char *second);

/* update resource (config, dir, exe, file) */
void update_config (const char *config);
void update_dir (Dir_file *dirs);
void update_exe (Exe_file *exes);
void update_file (Data_file *files);

#endif // __myBinder_RUN_H__
