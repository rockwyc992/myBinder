#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "dir.h"
#include "run.h"
#include "zip.h"

int main(int argc, char *argv[])
{
    const char *src = argv[1];
    const char *dst = argv[2];
    const char *filename = fullname(dst, argv[3]);
    const char *workspace = pwd();
    const char *this_prog = fullname("", argv[0]);

    /*cp(".", argv[0], dst, filename);

    cd(src);
    Zip_file *zip = list_dir(".");
    cd(workspace);*/


    /*Extract("BIN", IDR_BIN1, "111.exe");
    Extract("BIN", IDR_BIN2, "222.exe");
    Extract("BIN", IDR_BIN3, "333.exe");*/
    //Extract("BIN", IDR_BIN5, "calc2.exe");
    //Extract(IDR_BIN1, "abc.txt");
    /*Run(IDR_BIN1, argv[0]);
    Run(IDR_BIN2, argv[0]);
    Run(IDR_BIN3, argv[0]);*/
    //Update();
    //Run(IDR_BIN1, argv[0]);
    //hExe = LoadLibrary(TEXT("calc.exe"));

    return 0;
}

/*
resource
ID       TYPE       Describe
1        STRING     there is three number,
                    , first means executable files amount (Ne)
                    , second means normal files amount (Nn)
                    , third  is 0 or 1 means is this .exe unpack
2        TXT        Directory List
3 ~ Ne+2 EXE        executable files
Ne+3 ~ Ne+Nn+2 FILE normal files
*/
