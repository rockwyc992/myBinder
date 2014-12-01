#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "zip.h"
#include "run.h"

int main(int argc, char *argv[])
{
    dst_exe = LoadLibrary(argv[0]);
    if(res_size(ID_CONFIG)) {                               //if CONFIG is not empty
     //   Extract_and_Run();
    } else if (argc == 4){
        Zip_All_File(argv);  //update resource
    } else {
        puts("argc is not 4");
        return 1;
    }

    return 0;
}
