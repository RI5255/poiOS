#include "efi.h"
#include "common.h"

EFI_SYSTEM_TABLE *ST;
EFI_GRAPHICS_OUTPUT_PROTOCOL *GOP;
EFI_SIMPLE_POINTER_PROTOCOL *SPP;
EFI_DEVICE_PATH_TO_TEXT_PROTOCOL *DPTTP;
EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *STIEP;
EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL *DPFTP;
EFI_DEVICE_PATH_UTLITIES_PROTOCOL *DPUP;

static BOOLEAN is_exit = FALSE;

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
                            {0x90, 0xcc, 0x55, 0x1a, 0x4e, 0x4a, 0x7f, 0x1c}},
            STIEP_GUID  =   {0xdd9e7534, 0x7762, 0x4698, \
                            {0x8c, 0x14, 0xf5, 0x85, 0x17, 0xa6, 0x25, 0xaa}},
            DPFTP_GUID  =   {0x5c99a21, 0xc70f, 0x4ad2, \
                            {0x8a, 0x5f, 0x35, 0xdf, 0x33, 0x43, 0xf5, 0x1e}},
            DPUP_GUID   =   {0x379be4e, 0xd706, 0x437d, \
                            {0xb0, 0x37, 0xed, 0xb8, 0x2f, 0xb7, 0x72, 0xa4}};

// 'q'が押された時に実行される関数
EFI_STATUS key_notice(EFI_KEY_DATA *key_data __attribute__((unused))) {
    is_exit = TRUE;
    return EFI_SUCCESS;
}

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
        spp_handles[0],
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

EFI_STATUS OpenSITEP(EFI_HANDLE *image_handle, EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL **stiep) {
    EFI_STATUS status;
    UINTN num_stiep_handles = 0;
    EFI_HANDLE* stiep_handles = NULL;

    status = ST->BootServices->LocateHandleBuffer(
        ByProtocol,
        &STIEP_GUID,
        NULL,
        &num_stiep_handles,
        &stiep_handles);
    
    if(status)
        return status;
    
    // TODO: 数字が決め打ちなっている所を何とかしたい
    status = ST->BootServices->OpenProtocol(
        stiep_handles[0],
        &STIEP_GUID,
        (VOID**)stiep,
        image_handle,
        NULL,
        EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);

    if(status)
        return status;

    ST->BootServices->FreePool(stiep_handles);

    return 0;
}

void put_attribute(UINTN attribute) {
    UINT16  *c[]    =  {L"EFI_BLACK", L"EFI_BLUE", L"EFI_GREEN", L"EFI_CYAN", 
                        L"EFI_RED", L"EFI_MAGENTA", L"EFI_BROWN", L"EFI_LIGHTGRAY",
                        L"EFI_BRIGHT(DARKGRAY)", L"EFI_LIGHTBLUE", L"EFI_LIGHTGREEN", 
                        L"EFI_LIGHTCYAN", L"EFI_LIGHTRED", L"EFI_LIGHTMAGENTA", L"EFI_YELLOW", L"EFI_WHITE"},
            *bc[]   =  {L"EFI_BACKGROUND_BLACK", L"EFI_BACKGROUND_BLUE",
                        L"EFI_BACKGROUND_GREEN", L"EFI_BACKGROUND_CYAN",
                        L"EFI_BACKGROUND_RED", L"EFI_BACKGROUND_MAGENTA",
                        L"EFI_BACKGROUND_BROWN", L"EFI_BACKGROUND_LIGHTGRAY"};
    puts(c[attribute & 0x0f]);
    puts(L" ");
    puts(bc[(attribute & 0xf0) >> 4]);
    puts(L"\r\n");
}

void efi_init(EFI_HANDLE image_handle, EFI_SYSTEM_TABLE *system_table) {
    EFI_STATUS status;
    UINTN colums, rows;
    EFI_KEY_DATA key_data= {{0, L'q'}, {0, 0}};
    VOID *notify_handle;
    EFI_LOADED_IMAGE_PROTOCOL *lip;
    EFI_DEVICE_PATH_PROTOCOL *dev_path, *dev_node, *dev_path_merged;

    ST = system_table;
    ST->ConOut->ClearScreen(ST->ConOut);
    ST->BootServices->SetWatchdogTimer(0, 0, 0, NULL);

    // Locate Protocols
    status = OpenGOP(image_handle, &GOP);
    assert(status, L"OpenGOP");

    status = OpenSPP(image_handle, &SPP);
    assert(status, L"OpenSPP");

    status = OpenSITEP(image_handle, &STIEP);
    assert(status, L"OpenSTIEP");

    status = ST->BootServices->LocateProtocol(&DPTTP_GUID, NULL, (VOID**)&DPTTP);
    assert(status, L"Failed to locate DPTTP");

    status = ST->BootServices->LocateProtocol(&DPFTP_GUID, NULL, (VOID **)&DPFTP);
    assert(status, L"Failed to locate DPFTP");

    status = ST->BootServices->LocateProtocol(&DPUP_GUID, NULL, (VOID **)&DPUP);
    assert(status, L"Failed to locate DPUP");

    // test.efiのFullPathを作ってみる
    status = ST->BootServices->OpenProtocol(
            image_handle, 
            &LIP_GUID,
            (VOID **)&lip,
            image_handle,
            NULL,
            EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
    assert(status, L"Failed to Open LIP");

    status = ST->BootServices->OpenProtocol(
        lip->DeviceHandle,
        &DPP_GUID,
        (VOID **)&dev_path,
        image_handle,
        NULL,
        EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
    assert(status, L"Failed to Open DPP");
    
    dev_node = DPFTP->ConvertTextToDeviceNode(L"test.efi");
    dev_path_merged = DPUP->AppendDeviceNode(dev_path, dev_node);

    puts(L"dev_path_merged: ");
    puts(DPTTP->ConvertDevicePathToText(dev_path_merged, FALSE, FALSE));
    puts(L"\r\n");

    // 'q'を押した時に実行される関数を登録する
    status = STIEP->RegisterKeyNotify(STIEP, &key_data, key_notice, &notify_handle);
    assert(status, L"STIEP->RegisterKeyNotify");
    
    // EFI_SIMPLE_TEXT_OUTPUT_PROTOCOLの情報を表示する
    ST->ConOut->SetAttribute(ST->ConOut, EFI_LIGHTGREEN | EFI_BACKGROUND_BLACK);
    puts(L"EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL Modes:\r\n");
    for(UINT32 mode = 0; mode < ST->ConOut->Mode->MaxMode; mode++) {
        status = ST->ConOut->QueryMode(ST->ConOut, mode, &colums, &rows);
        switch(status) {
            case EFI_SUCCESS:
                puth(mode, sizeof(mode));
                puts(L": ");
                puth(colums, sizeof(colums));
                puts(L" x ");
                puth(rows, sizeof(rows));
                puts(L"\r\n");
                break;
            
            case EFI_UNSPPORTED:
                puth(mode, sizeof(mode));
                puts(L": unsupported\r\n");
                break;
            
            default:
                assert(status, L"QueryMode");
                break;
        }
    }
    puts(L"Current Mode: ");
    puth(ST->ConOut->Mode->Mode, sizeof(ST->ConOut->Mode->Mode));
    puts(L"\r\n");
    puts(L"Current Attribute: ");
    put_attribute(ST->ConOut->Mode->Attribute);
    
    ST->ConOut->SetAttribute(ST->ConOut, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);

    puts(L"push 'q' to exit\r\n");
    
    while(!is_exit);

    // 'q'を押した際に実行される関数の登録解除
    status = STIEP->UnregisterKeyNotify(STIEP, notify_handle);
    assert(status, L"STIEP->UnregisterNotify");

    // Modeを変えてみる
    puts(L"Switching Mode...\r\n");
    status = ST->ConOut->SetMode(ST->ConOut, 3);
    assert(status, L"ST->ConOut->SetMode");

    STIEP->Reset(STIEP, FALSE);
    ST->ConOut->ClearScreen(ST->ConOut);
}