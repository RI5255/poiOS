#include "efi.h"
#include "common.h"
#include "graphics.h"
#include "gui.h"
#include "shell.h"
#include "file.h"

#define WIDTH_PER_CH	8
#define HEIGHT 20

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

// TODO: 座標が決め打ちになっているところを何とかしたい
int ls_gui(EFI_HANDLE image_handle) {
    int file_num;
    struct RECT r;

    ST->ConOut->ClearScreen(ST->ConOut);
    file_num = ls(image_handle);

    r.x = 362;
    r.y = 148;
    r.w = (MAX_FILE_NAME_LEN - 1) * WIDTH_PER_CH;
    r.h = HEIGHT;

    for(int i = 0; i < file_num; i++) {
        file_list[i].rect.x = r.x;
        file_list[i].rect.y = r.y;
        file_list[i].rect.w = r.w;
        file_list[i].rect.h = r.h;
        draw_rect(file_list[i].rect, white);
        r.x += file_list[i].rect.w + WIDTH_PER_CH;

        file_list[i].is_highlighted = FALSE;
    }

    return file_num;
}


void gui(EFI_HANDLE image_handle) {
    int px = 0, py = 0, file_num;
    EFI_STATUS status;
    UINTN waitidx;
    EFI_SIMPLE_POINTER_STATE s;

    ST->ConOut->ClearScreen(ST->ConOut);
    SPP->Reset(SPP, FALSE);

    file_num = ls_gui(image_handle);

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

            // ファイルアイコンの処理
            for(int i = 0; i < file_num; i++) {
                if(is_in_rect(px, py, file_list[i].rect)) {
                    if(!file_list[i].is_highlighted) {
                        draw_rect(file_list[i].rect, yellow);
                        file_list[i].is_highlighted = TRUE;
                    }
                }else {
                    if(file_list[i].is_highlighted) {
                        draw_rect(file_list[i].rect, white);
                        file_list[i].is_highlighted = FALSE;
                    }
                }
            }
        }
    }
}