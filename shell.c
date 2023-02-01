#include "efi.h"
#include "common.h"

#define MAX_COMMAND_LEN 100

void shell(void) {
    UINT16 cmd[MAX_COMMAND_LEN];
    
    while(TRUE) {
        puts(L"poiOS> ");
        if(gets(cmd, MAX_COMMAND_LEN) <= 0) // CRのみだった場合は何もしない
            continue;
        if(!strcmp(L"hello", cmd))
            puts(L"Hello UEFI!\r\n");
        else 
            puts(L"Command not found.\r\n");
    }
}