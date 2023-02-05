#include "efi.h"
#include "common.h"

EFI_SYSTEM_TABLE *ST;
EFI_GRAPHICS_OUTPUT_PROTOCOL *GOP;
EFI_SIMPLE_POINTER_PROTOCOL *SPP;
EFI_DEVICE_PATH_TO_TEXT_PROTOCOL *DPTTP;

EFI_GUID    GOP_GUID    =   {0x9042a9de, 0x23dc, 0x4a38, \
                            {0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a}},
            SPP_GUID    =   {0x31878c87, 0xb75, 0x11d5, \
                            {0x9a, 0x4f, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d}},
            SFSP_GUID   =   {0x0964e5b22, 0x6459, 0x11d2, \
                            {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}},
            LIP_GUID    =   {0x5B1B31A1, 0x9562, 0x11d2, \
                            {0x8E, 0x3F, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B}},
            DPP_GUID    =   {0x09576e91, 0x6d3f, 0x11d2, \
                            {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}},  
            DPTTP_GUID  =   {0x8b843e20, 0x8132, 0x4852, \
                            {0x90, 0xcc, 0x55, 0x1a, 0x4e, 0x4a, 0x7f, 0x1c}};

EFI_STATUS OpenGOP(EFI_HANDLE image_handle, EFI_GRAPHICS_OUTPUT_PROTOCOL **gop) {
    EFI_STATUS status;
    UINTN num_gop_handles = 0;
    EFI_HANDLE* gop_handles = NULL;

    status = ST->BootServices->LocateHandleBuffer(
        ByProtocol,
        &GOP_GUID,
        NULL,
        &num_gop_handles,
        &gop_handles);
    
    if(status)
        return status;

    status = ST->BootServices->OpenProtocol(
        gop_handles[0],
        &GOP_GUID,
        (VOID**)gop,
        image_handle,
        NULL,
        EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
    
    if(status)
        return status;

    ST->BootServices->FreePool(gop_handles);

    return 0;
}

EFI_STATUS OpenSPP(EFI_HANDLE image_handle, EFI_SIMPLE_POINTER_PROTOCOL **spp) {
    EFI_STATUS status;
    UINTN num_spp_handles = 0;
    EFI_HANDLE* spp_handles = NULL;

    status = ST->BootServices->LocateHandleBuffer(
        ByProtocol,
        &SPP_GUID,
        NULL,
        &num_spp_handles,
        &spp_handles);
    
    if(status)
        return status;
    
    // TODO: 数字が決め打ちなっている所を何とかしたい
    status = ST->BootServices->OpenProtocol(
        spp_handles[2],
        &SPP_GUID,
        (VOID**)spp,
        image_handle,
        NULL,
        EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);

    if(status)
        return status;

    ST->BootServices->FreePool(spp_handles);

    return 0;
}

void efi_init(EFI_HANDLE image_handle, EFI_SYSTEM_TABLE *system_table) {
    EFI_STATUS status;

    ST = system_table;
    ST->ConOut->ClearScreen(ST->ConOut);
    ST->BootServices->SetWatchdogTimer(0, 0, 0, NULL);

    status = OpenGOP(image_handle, &GOP);
    assert(status, L"OpenGOP");

    status = OpenSPP(image_handle, &SPP);
    assert(status, L"OpenSPP");

    status = ST->BootServices->LocateProtocol(&DPTTP_GUID, NULL, (VOID**)&DPTTP);
    assert(status, L"Failed to locate DPTTP");

}