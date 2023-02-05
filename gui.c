#include "efi.h"
#include "common.h"
#include "graphics.h"
#include "gui.h"

EFI_GRAPHICS_OUTPUT_BLT_PIXEL tmp = {0, 0, 0, 0};
unsigned int cursor_old_x;
unsigned int cursor_old_y;

void draw_cursor(unsigned int x, unsigned int y) {
    draw_pixel(x, y, white);
}

void save_cursor_area(unsigned int x, unsigned int y) {
    tmp = get_pixel(x ,y);
    tmp.Reserved = 0xff;
}

void load_cursor_area(unsigned int x, unsigned int y) {
    draw_pixel(x, y, tmp);
}

void put_cursor(unsigned int x, unsigned int y) {
    if(tmp.Reserved)
        load_cursor_area(cursor_old_x, cursor_old_y);

    save_cursor_area(x, y);

    draw_cursor(x, y);

    cursor_old_x = x;
    cursor_old_y = y;
}

void gui(void) {
    struct RECT r = {10, 10, 20, 20};
    int px = 0, py = 0;
    BOOLEAN is_highlighted = FALSE;
    EFI_STATUS status;
    UINTN waitidx;
    EFI_SIMPLE_POINTER_STATE s;

    ST->ConOut->ClearScreen(ST->ConOut);
    SPP->Reset(SPP, FALSE);

    draw_rect(r, white);
    
    while(TRUE) {
        ST->BootServices->WaitForEvent(1, &(SPP->WaitForInput), &waitidx);
        status = SPP->GetState(SPP, &s);

        if(!status) {
            px += s.RelativeMovementX;
            if(px < 0)
                px = 0;
            else if(GOP->Mode->Info->HorizontalResolution <= (unsigned int)px)
                px = GOP->Mode->Info->HorizontalResolution - 1;
            
            py += s.RelativeMovementY;
            if(py < 0)
                 py = 0;
            else if(GOP->Mode->Info->VerticalResolution <= (unsigned int)py)
                py = GOP->Mode->Info->VerticalResolution - 1;

            put_cursor(px, py);

            if(is_in_rect(px, py, r)) {
                if(!is_highlighted) {
                    draw_rect(r, yellow);
                    is_highlighted = TRUE;
                }
            } else {
                if(is_highlighted) {
                    draw_rect(r, white);
                    is_highlighted = FALSE;
                }
            }
        }
    }
}