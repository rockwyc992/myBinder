#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "zip.h"

HMODULE dst_exe;
const char *final_prog;

int main(int argc, const char *argv[])
{
    dst_exe = LoadLibrary(argv[0]); //Load this program

    if(res_size(ID_CONFIG)) {       //if CONFIG is not empty
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
