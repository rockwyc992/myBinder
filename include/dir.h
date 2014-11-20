#ifndef __myBinder_DIR_H__
#define __myBinder_DIR_H__

#include <stdio.h>
#include <windows.h>
#include "zip.h"

const char* fullname(const char *path, const char *filename);

const char* pwd ();

BOOLEAN cd (const char *dir);

BOOLEAN cp (const char *src, const char *dst);

Zip_file* list_dir (const char *dir);

#endif // __myBinder_DIR_H__
