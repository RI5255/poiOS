#include "efi.h"
#include "common.h"
#include "shell.h"
#include "graphics.h"
#include "gui.h"
#include "file.h"

#define MAX_COMMAND_LEN 100

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
    unsigned char file_buf[MAX_FILE_BUF];
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

void shell(EFI_HANDLE image_handle) {
    UINT16 cmd[MAX_COMMAND_LEN];
    struct RECT r = {10, 10, 100, 200};
    while(TRUE) {
        puts(L"poiOS> ");
        if(gets(cmd, MAX_COMMAND_LEN) <= 0) // CRのみだった場合は何もしない
            continue;
        if(!strcmp(L"hello", cmd))
            puts(L"Hello UEFI!\r\n");
        else if(!strcmp(L"rect", cmd))
            draw_rect(r, white);
        else if(!strcmp(L"gui", cmd))
            gui();
        else if(!strcmp(L"pstat", cmd))
            pstat();
        else if(!strcmp(L"clear", cmd))
            ST->ConOut->ClearScreen(ST->ConOut);
        else if(!strcmp(L"ls", cmd))
            ls(image_handle);
        else 
            puts(L"Command not found.\r\n");
    }
}