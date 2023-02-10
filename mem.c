#include "mem.h"
#include "efi.h"
#include "common.h"

#define MEMMAP_SIZE 4096 * 4

EFI_STATUS get_memmap(IN OUT UINT8 **memmap, IN OUT UINTN *num_ent, IN OUT UINTN *ent_size) {
    EFI_STATUS status;
    UINTN   memmap_size = MEMMAP_SIZE,
            map_key, descriptor_size;
    UINT32  descriptor_version;
    status = ST->BootServices->AllocatePool(
        EfiLoaderData,
        MEMMAP_SIZE,
        (VOID **)memmap
        );
   if(status) return status;

    status = ST->BootServices->GetMemoryMap(
        &memmap_size,
        (EFI_MEMORY_DESCRIPTOR*)*memmap,
        &map_key,
        &descriptor_size,
        &descriptor_version);
    if(status) return status;

    *num_ent = memmap_size / descriptor_size;
    *ent_size = descriptor_size;

    return EFI_SUCCESS;
}