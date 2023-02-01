#include "efi.h"
#include "shell.h"

// entry point
void efi_main(void *ImageHandle __attribute__((unused)), EFI_SYSTEM_TABLE *SystemTable) {
    efi_init(SystemTable);
    shell();
}