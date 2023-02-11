#include "efi.h"
#include "common.h"

void efi_main(void *ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
	SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Hello UEFI!\r\n");
    //return
    SystemTable->BootServices->Exit(ImageHandle, EFI_SUCCESS, 0, NULL);
}