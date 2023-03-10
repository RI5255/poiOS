#include "efi.h"
#include "common.h"
#include "shell.h"
#include "graphics.h"
#include "gui.h"
#include "file.h"
#include "mem.h"

#define MAX_COMMAND_LEN 100

void dialogue_get_file_name(int idx) {
    int i;

    ST->ConOut->ClearScreen(ST->ConOut);

    puts(L"New File Name: ");
    
    for(i = 0; i < MAX_FILE_NAME_LEN; i++) {
        file_list[idx].name[i] = getc();
        if (file_list[idx].name[i] != L'\r')
			putc(file_list[idx].name[i]);
        else 
            break;
    }

    file_list[idx].name[i] = L'\0';
}

void pstat(void) {
    UINTN status, waitidx;
    EFI_SIMPLE_POINTER_STATE s;
    
    SPP->Reset(SPP, FALSE);

    while(1) {
        ST->BootServices->WaitForEvent(1, &(SPP->WaitForInput), &waitidx);

        status = SPP->GetState(SPP, &s);
        
        if(!status) {
            puth(s.RelativeMovementX, sizeof(INT32));
            puts(L" ");
            puth(s.RelativeMovementY, sizeof(INT32));
            puts(L" ");
            puth(s.RelativeMovementZ, sizeof(INT32));
            puts(L" ");
            puth(s.LeftButton, sizeof(BOOLEAN));
            puts(L" ");
            puth(s.RightButton, sizeof(BOOLEAN));
            puts(L"\r\n");
        }
    }
}

int ls(EFI_HANDLE image_handle) {
    EFI_STATUS status;
    UINTN buf_size;
    EFI_FILE_INFO *file_info;
    EFI_FILE_PROTOCOL *root;
    UINT16 file_buf[MAX_FILE_BUF / 2];
    int idx = 0;
    int file_num;

    status = OpenRootDir(image_handle, &root);
    assert(status, L"OpenRootDir");

    while(1) {
        buf_size = MAX_FILE_BUF;
        status = root->Read(root, &buf_size, (VOID *)file_buf);
        assert(status, L"root->Read");
        if(!buf_size) break; // Read関数はentryが無くなると0を返す。

        file_info = (EFI_FILE_INFO *)file_buf;
        strncpy(file_list[idx].name, file_info->FileName, MAX_FILE_NAME_LEN - 1);
        file_list[idx].name[MAX_FILE_NAME_LEN - 1] = L'\0';
        puts(file_list[idx].name);
        puts(L" ");
        
        idx++;
    }
    puts(L"\r\n");
    file_num = idx;

    root->Close(root);

    return file_num;
}

void cat(EFI_HANDLE image_handle, UINT16 *file_name) {
    EFI_STATUS status;
    EFI_FILE_PROTOCOL *root, *file;
    UINTN buf_size = MAX_FILE_BUF;
    UINT16 file_buf[MAX_FILE_BUF / 2];

    status = OpenRootDir(image_handle, &root);
    assert(status, L"OpenRootDir");

    status = root->Open(root, &file, file_name, EFI_FILE_MODE_READ, 0);
    assert(status, L"root->Open");

    status = file->Read(file, &buf_size, (VOID *)file_buf);
    assert(status, L"file->Read");

    puts(file_buf);
    puts(L"\r");

    file->Close(file);
    root->Close(root);

}

void edit(EFI_HANDLE image_handle, CHAR16 *file_name) {
    EFI_STATUS status;
    EFI_FILE_PROTOCOL *root, *file;
    UINTN buf_size = MAX_FILE_BUF;
    UINT16 c, file_buf[MAX_FILE_BUF / 2];
    int i = 0;

    ST->ConOut->ClearScreen(ST->ConOut);

    while(TRUE) {
        c = getc();
        
        if(c == SC_ESC) break;

        putc(c);
        file_buf[i++] = c;

        if(c == L'\r') {
            putc(L'\n');
            file_buf[i++] = L'\n';
        }
    }

    file_buf[i] = L'\0';

    status = OpenRootDir(image_handle, &root);
    assert(status, L"OpenRootDir");

    status = root->Open(root, &file, file_name, EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE, 0);
    assert(status, L"root->Open");

    status = file->Write(file, &buf_size, (VOID *)file_buf);
    assert(status, L"file->Write");

    file->Flush(file);

    file->Close(file);
    root->Close(root);

}

void run(EFI_HANDLE image_handle) {
    EFI_STATUS status;
    EFI_LOADED_IMAGE_PROTOCOL *lip;
    EFI_DEVICE_PATH_PROTOCOL *dev_path, *dev_node, *dev_path_merged;
    EFI_HANDLE image;

    // hello.efiをロードして実行してみる
    status = ST->BootServices->OpenProtocol(
            image_handle, 
            &LIP_GUID,
            (VOID **)&lip,
            image_handle,
            NULL,
            EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
    assert(status, L"Failed to Open LIP");

    status = ST->BootServices->OpenProtocol(
        lip->DeviceHandle,
        &DPP_GUID,
        (VOID **)&dev_path,
        image_handle,
        NULL,
        EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
    assert(status, L"Failed to Open DPP");
    
    dev_node = DPFTP->ConvertTextToDeviceNode(L"hello.efi");
    dev_path_merged = DPUP->AppendDeviceNode(dev_path, dev_node);

    status = ST->BootServices->LoadImage(
            FALSE, 
            image_handle, 
            dev_path_merged, 
            NULL, 
            0,
            &image);
    assert(status, L"LoadImage");

    puts(L"Loaded image: ");
    puts(DPTTP->ConvertDevicePathToText(dev_path_merged, FALSE, FALSE));
    puts(L"\r\n");
    puts(L"push 'e' to execute");
    puts(L"\r\n");

    while(getc() != L'e');

    status = ST->BootServices->StartImage(image, NULL, NULL);
    assert(status, L"StartImage");
}

void memmap(void) {
    EFI_STATUS status;
    UINT8 *memmap;
    UINTN num_ent, ent_size;

    status = get_memmap(&memmap, &num_ent, &ent_size);
    assert(status, L"get_memmap");

    puts(L"memmap has ");
    puth(num_ent, sizeof(num_ent));
    puts(L" entries\r\n");
    
    // とりあえずTypeがEfiConventionalMemoryのもののみを表示する。
    puts(L"Type PhysicalStart NumberOfPages\r\n");
    for(UINTN i = 0; i < num_ent; i++) {
        EFI_MEMORY_DESCRIPTOR *d = (EFI_MEMORY_DESCRIPTOR*)(memmap + ent_size * i);
        if(d->Type != EfiConventionalMemory) continue;
        puth(d->Type, sizeof(d->Type));
        puts(L" 0x");
        puth(d->PhysicalStart, sizeof(d->PhysicalStart));
        puts(L" ");
        puth(d->NumberOfPages, sizeof(d->NumberOfPages));
        puts(L"\r\n");
    }

    status = ST->BootServices->FreePool((VOID*)memmap);
    assert(status, L"FreePool");
}

void shell(EFI_HANDLE image_handle) {
    UINT16 cmd[MAX_COMMAND_LEN];
    struct RECT r = {10, 10, 100, 200};
    BOOLEAN is_exit = FALSE;

    while(!is_exit) {
        puts(L"poiOS> ");
        if(gets(cmd, MAX_COMMAND_LEN) <= 0) // CRのみだった場合は何もしない
            continue;
        if(!strcmp(L"hello", cmd))
            puts(L"Hello UEFI!\r\n");
        else if(!strcmp(L"rect", cmd))
            draw_rect(r, white);
        else if(!strcmp(L"gui", cmd)){
            gui(image_handle);
            ST->ConOut->ClearScreen(ST->ConOut);
        }
        else if(!strcmp(L"pstat", cmd))
            pstat();
        else if(!strcmp(L"clear", cmd))
            ST->ConOut->ClearScreen(ST->ConOut);
        else if(!strcmp(L"ls", cmd))
            ls(image_handle);
        else if(!strcmp(L"cat", cmd))
            cat(image_handle, L"abc");
        else if(!strcmp(L"edit", cmd))
            edit(image_handle, L"abc");
        else if(!strcmp(L"run", cmd)){
            run(image_handle);
        }
        else if(!strcmp(L"memmap", cmd))
            memmap();
        else if(!strcmp(L"exit", cmd))
            is_exit = TRUE;
        else if(!strcmp(L"shutdown", cmd))
            ST->RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);
        else 
            puts(L"Command not found.\r\n");
    }
}