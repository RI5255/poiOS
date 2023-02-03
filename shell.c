#include "efi.h"
#include "common.h"
#include "shell.h"
#include "graphics.h"
#include "gui.h"

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

void shell(void) {
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
        else 
            puts(L"Command not found.\r\n");
    }
}