#include "efi.h"
#include "common.h"

EFI_SYSTEM_TABLE *ST;
EFI_GRAPHICS_OUTPUT_PROTOCOL *GOP;
EFI_SIMPLE_POINTER_PROTOCOL *SPP;

void efi_init(EFI_SYSTEM_TABLE *SystemTable) {
    EFI_GUID    gop_guid =  {0x9042a9de, 0x23dc, 0x4a38, \
                            {0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a}},
                spp_guid =  {0x31878c87, 0xb75, 0x11d5, \
                            {0x9a, 0x4f, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d}};
    ST = SystemTable;
    ST->ConOut->ClearScreen(ST->ConOut);
    ST->BootServices->SetWatchdogTimer(0, 0, 0, NULL);
    ST->BootServices->LocateProtocol(&gop_guid, NULL, (VOID**)&GOP);
    ST->BootServices->LocateProtocol(&spp_guid, NULL, (VOID**)&SPP);
}