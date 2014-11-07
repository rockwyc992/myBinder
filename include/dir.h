#ifndef __myBinder_DIR_H__
#define __myBinder_DIR_H__

#include <stdio.h>
#include <windows.h>
#include "zip.h"

const char* pwd ();

BOOLEAN cd (const char *dir);

Zip_file* list_dir (const char *dir);

#endif // __myBinder_DIR_H__
