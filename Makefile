all: fs/EFI/BOOT/BOOTX64.EFI fs/hello.efi

fs/EFI/BOOT/BOOTX64.EFI: efi.c common.c file.c graphics.c gui.c mem.c shell.c main.c
	mkdir -p fs/EFI/BOOT
	x86_64-w64-mingw32-gcc -Wall -Wextra -e efi_main -nostdinc -nostdlib \
	-fno-builtin -Wl,--subsystem,10 -o $@ $+

fs/hello.efi: efi.c common.c hello.c 
	x86_64-w64-mingw32-gcc -Wall -Wextra -e efi_main -nostdinc -nostdlib \
	-fno-builtin -Wl,--subsystem,10 -shared -o $@ $+

run: fs/EFI/BOOT/BOOTX64.EFI
	qemu-system-x86_64 -drive if=pflash,format=raw,readonly,file=/home/riruoda/OVMF/OVMF_CODE.fd \
	-drive if=pflash,format=raw,file=/home/riruoda/OVMF/OVMF_VARS.fd \
	-drive dir=fs,driver=vvfat,rw=on

clean:
	rm -rf *~ fs

.PHONY: clean