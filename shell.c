#include "common.h"
#include "shell.h"
#include "graphics.h"
#include "gui.h"

#define MAX_COMMAND_LEN 100

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
        else 
            puts(L"Command not found.\r\n");
    }
}