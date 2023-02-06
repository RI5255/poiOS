#ifndef _FILE_H_
#define _FILE_H_

#define MAX_FILE_NAME_LEN 4
#define MAX_FILE_NUM 10
#define MAX_FILE_BUF 1024

#include "efi.h"
#include "graphics.h"

struct FILE {
    struct RECT rect;
    BOOLEAN is_highlighted;
    unsigned short name[MAX_FILE_NAME_LEN];

};

extern struct FILE file_list[MAX_FILE_NUM];

EFI_STATUS OpenRootDir(EFI_HANDLE image_handle, EFI_FILE_PROTOCOL ** root);

#endif 