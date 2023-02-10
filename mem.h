#ifndef _MEM_H_
#define _MEM_H_

#include "efi.h"

EFI_STATUS get_memmap( IN OUT UINT8 **memmap, IN OUT UINTN *num_ent, IN OUT UINTN *ent_size);

#endif