TOOLPREFIX = ~/opt/cross/bin/i686-elf-
CC = $(TOOLPREFIX)gcc
AS = $(TOOLPREFIX)gas
LD = $(TOOLPREFIX)ld
OBJCOPY = $(TOOLPREFIX)objcopy
OBJDUMP = $(TOOLPREFIX)objdump

main.img:
	(cd bootloader && make)
	(cd kernel && make)
	mv bootloader/main.img .
	dd if=kernel/alpacos.bin of=main.img seek=1 conv=notrunc

alpacos.iso:
	(cd kernel && make)
	(cd grub-boot && make)
	cp grub-boot/alpacos.iso .

clean:
	(cd bootloader && make clean)
	(cd kernel && make clean)
	(cd grub-boot && make clean)
	rm -rf alpacos.iso

qemu: main.img
	qemu-system-i386 -drive file=main.img,index=0,media=disk,format=raw

qemu-gdb: alpacos.iso
	qemu-system-i386 -M q35,accel=tcg -d int -serial mon:stdio -drive file=alpacos.iso,index=0,media=disk,format=raw -S -gdb tcp::1234

qemu-grub: alpacos.iso
	qemu-system-i386 -drive file=alpacos.iso,index=0,media=disk,format=raw
