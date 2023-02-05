#include "file.h"
#include "efi.h"
#include "common.h"

struct FILE file_list[MAX_FILE_NUM];

EFI_STATUS OpenRootDir(EFI_HANDLE image_handle, EFI_FILE_PROTOCOL ** root) {
    EFI_STATUS status;
    EFI_LOADED_IMAGE_PROTOCOL *loaded_image;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *sfsp;

    status = ST->BootServices->OpenProtocol(
        image_handle,
        &LIP_GUID,
        (VOID**)&loaded_image,
        image_handle,
        NULL,
        EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
    
    if(status)
        return status;

    status = ST->BootServices->OpenProtocol(
        loaded_image->DeviceHandle,
        &SFSP_GUID,
        (VOID**)&sfsp,
        image_handle,
        NULL,
        EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);

    if(status)
        return status;
    
    return sfsp->OpenVolume(sfsp, root);
}