#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include "efi.h"

struct RECT {
	unsigned int x, y;
	unsigned int w, h;
};

extern const EFI_GRAPHICS_OUTPUT_BLT_PIXEL white;
extern const EFI_GRAPHICS_OUTPUT_BLT_PIXEL yellow;

void draw_pixel(unsigned int x, unsigned int y,
                EFI_GRAPHICS_OUTPUT_BLT_PIXEL color);
void draw_rect(struct RECT r, EFI_GRAPHICS_OUTPUT_BLT_PIXEL c);
BOOLEAN is_in_rect(unsigned int x, unsigned int y, struct RECT r);
EFI_GRAPHICS_OUTPUT_BLT_PIXEL get_pixel(unsigned int x, unsigned int y);

#endif