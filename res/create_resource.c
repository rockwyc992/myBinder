#include <stdio.h>

#define ID_START    10000
#define CONFIG_SIZE     1
#define DIR_SIZE     1000
#define EXE_SIZE     1000
#define FILE_SIZE   10000

#define ID_CONFIG       (ID_START)
#define ID_DIR          (ID_START + CONFIG_SIZE)
#define ID_EXE_PATH     (ID_START + CONFIG_SIZE + DIR_SIZE)
#define ID_EXE_DATA     (ID_START + CONFIG_SIZE + DIR_SIZE + EXE_SIZE)
#define ID_FILE_PATH    (ID_START + CONFIG_SIZE + DIR_SIZE + EXE_SIZE + EXE_SIZE)
#define ID_FILE_DATA    (ID_START + CONFIG_SIZE + DIR_SIZE + EXE_SIZE + EXE_SIZE + FILE_SIZE)
#define ID_END          (ID_START + CONFIG_SIZE + DIR_SIZE + EXE_SIZE + EXE_SIZE + FILE_SIZE + FILE_SIZE)
/*
    0          CONFIG
    1 ~  1000  DIR
 1001 ~  2000  EXE_PATH
 2001 ~  3000  EXE_DATA
 3001 ~ 13000 FILE_PATH
13001 ~ 23000 FILE_DATA
*/

void create_txt()
{
    FILE *txt = fopen("resource.txt", "w+");
    fclose(txt);
}

void create_dotrc()
{

    FILE *dot_rc = fopen("resource.rc", "w+");
    int i;

    fputs("#include \"resource.h\"\n", dot_rc);
    fputs("1 ICON \"ico/mybinder.ico\"\n", dot_rc);
    //fputs("2 ICON \"ico/mybinder_bigger.ico\"\n", dot_rc);

    for(i = ID_START; i < ID_END; i++){
        fprintf(dot_rc, "%d    RCDATA    DISCARDABLE     \"resource.txt\"\n",i);
    }

    fclose(dot_rc);
}

void create_header()
{
    FILE *header = fopen("resource.h", "w+");
    fputs("#ifndef __myBinder_RESOURCE_H__\n", header);
    fputs("#define __myBinder_RESOURCE_H__\n\n", header);

    fprintf(header, "#define ID_CONFIG      %d\n", ID_CONFIG);
    fprintf(header, "#define ID_DIR         %d\n", ID_DIR);
    fprintf(header, "#define ID_EXE_PATH    %d\n", ID_EXE_PATH);
    fprintf(header, "#define ID_EXE_DATA    %d\n", ID_EXE_DATA);
    fprintf(header, "#define ID_FILE_PATH   %d\n", ID_FILE_PATH);
    fprintf(header, "#define ID_FILE_DATA   %d\n", ID_FILE_DATA);

    fputs("\n#endif // __myBinder_RESOURCE_H__\n", header);
    fclose(header);
}

int main()
{
    create_txt();
    create_dotrc();
    create_header();

    return 0;
}
