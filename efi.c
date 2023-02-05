#include "efi.h"
#include "common.h"

EFI_SYSTEM_TABLE *ST;
EFI_GRAPHICS_OUTPUT_PROTOCOL *GOP;
EFI_SIMPLE_POINTER_PROTOCOL *SPP;
EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SFSP;

void efi_init(EFI_SYSTEM_TABLE *SystemTable) {
    EFI_GUID    gop_guid =  {0x9042a9de, 0x23dc, 0x4a38, \
                            {0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a}},
                spp_guid =  {0x31878c87, 0xb75, 0x11d5, \
                            {0x9a, 0x4f, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d}},
                sfsp_guid = {0x0964e5b22, 0x6459, 0x11d2, \
                            {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};

    ST = SystemTable;
    ST->ConOut->ClearScreen(ST->ConOut);
    ST->BootServices->SetWatchdogTimer(0, 0, 0, NULL);
    ST->BootServices->LocateProtocol(&gop_guid, NULL, (VOID**)&GOP);
    ST->BootServices->LocateProtocol(&spp_guid, NULL, (VOID**)&SPP);
    ST->BootServices->LocateProtocol(&sfsp_guid, NULL, (VOID**)&SFSP);
}