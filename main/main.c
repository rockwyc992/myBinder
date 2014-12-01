#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "zip.h"

HANDLE update;

int main(int argc, const char *argv[])
{
    if(res_size(argv[0], ID_CONFIG)) {       //if CONFIG is not empty
        Extract_and_Run();
    } else if (argc == 4){
        cp("test\\src\\calc.exe", "test\\src\\notepad2.exe");
        Zip_All_File(argv);         //update resource
    } else {
        puts("argc is not 4");
        return 1;
    }

    return 0;
}
