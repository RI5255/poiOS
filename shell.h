#ifndef _SHELL_H_
#define _SHELL_H 

#include "efi.h"

void pstat(void);
int ls(EFI_HANDLE image_handle);
void cat(EFI_HANDLE image_handle, UINT16 *file_name);
void edit(EFI_HANDLE image_handle, CHAR16 *file_name);
void shell(EFI_HANDLE image_handle);

#endif 