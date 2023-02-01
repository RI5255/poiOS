all: fs/EFI/BOOT/BOOTX64.EFI

fs/EFI/BOOT/BOOTX64.EFI: main.c
	mkdir -p fs/EFI/BOOT
	x86_64-w64-mingw32-gcc -Wall -Wextra -e efi_main -nostdinc -nostdlib \
	-fno-builtin -Wl,--subsystem,10 -o $@ $<

run: fs/EFI/BOOT/BOOTX64.EFI
	qemu-system-x86_64 -drive if=pflash,format=raw,readonly,file=/home/riruoda/OVMF/OVMF_CODE.fd \
	-drive if=pflash,format=raw,file=/home/riruoda/OVMF/OVMF_VARS.fd \
	-drive dir=fs,driver=vvfat,rw=on

clean:
	rm -rf *~ fs

.PHONY: clean