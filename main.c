#include "efi.h"
#include "shell.h"
#include "common.h"

// entry point
void efi_main(void *image_handle, EFI_SYSTEM_TABLE *system_table) {
    efi_init(image_handle, system_table);
    shell(image_handle);
}