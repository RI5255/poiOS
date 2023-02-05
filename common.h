#ifndef _LIB_H_
#define _LIB_H_

#define NULL	(void *)0
#define TRUE	1
#define FALSE	0

#include "efi.h"

VOID putc(UINT16 c);
VOID puts(UINT16 *s);
VOID puth(UINT64 val, UINT8 num_digits);
UINT16 getc(void);
UINT32 gets(UINT16 *buf, UINT32 buf_size);
INT32 strcmp(const UINT16 *s1, const UINT16 *s2);
VOID strncpy(UINT16 *dst, UINT16 *src, UINT64 n);
VOID assert(UINTN status, UINT16 *message);

#endif