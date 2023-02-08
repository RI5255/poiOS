#include "efi.h"
#include "shell.h"
#include "common.h"

// entry point
void efi_main(void *image_handle, EFI_SYSTEM_TABLE *system_table) {
    EFI_STATUS status;
    EFI_LOADED_IMAGE_PROTOCOL *lip;
    EFI_DEVICE_PATH_PROTOCOL *dev_path, *dev_node, *dev_path_merged;
    EFI_HANDLE image;
    efi_init(image_handle, system_table);

    // hello.efiをロードしてみる
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
    
    dev_node = DPFTP->ConvertTextToDeviceNode(L"hello.efi");
    dev_path_merged = DPUP->AppendDeviceNode(dev_path, dev_node);

    puts(L"dev_path_merged: ");
    puts(DPTTP->ConvertDevicePathToText(dev_path_merged, FALSE, FALSE));
    puts(L"\r\n");

    status = ST->BootServices->LoadImage(
            FALSE, 
            image_handle, 
            dev_path_merged, 
            NULL, 
            0,
            &image);
    assert(status, L"LoadImage");
    puts(L"LoadImage: Success");

    while(TRUE);
    
    shell(image_handle);
}