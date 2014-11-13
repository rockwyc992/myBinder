#include <stdio.h>

#define SIZE 10000

int main()
{
    FILE *dot_rc = fopen("resource.rc", "w+");
    FILE *header = fopen("resource.h", "w+");
    FILE *txt = fopen("resource.txt", "w+");

    fputs("#include \"resource.h\"\n", dot_rc);
    fputs("#ifndef __myBinder_RESOURCE_H__\n", header);
    fputs("#define __myBinder_RESOURCE_H__\n\n", header);

    int i;
    for(i = 0; i < SIZE; i++){
        fprintf(dot_rc, "IDR_BIN%-5d    BIN    DISCARDABLE     \"resource.txt\"\n",i);
        fprintf(header, "#define IDR_BIN%-5d %d\n", i, i+SIZE);
    }

    fputs("\n#endif // __myBinder_RESOURCE_H__\n", header);

    fclose(dot_rc);
    fclose(header);
    fclose(txt);

    return 0;
}
