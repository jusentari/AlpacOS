TOOLPREFIX = ~/opt/cross/bin/i686-elf-
CCARGS=-ffreestanding -fno-pic -static -fno-builtin -fno-strict-aliasing -O0 -Wall -MD -g -m32 -Werror -fno-omit-frame-pointer -mno-ms-bitfields -Iinclude
CC = $(TOOLPREFIX)gcc
AS = $(TOOLPREFIX)gas
LD = $(TOOLPREFIX)ld
OBJCOPY = $(TOOLPREFIX)objcopy
OBJDUMP = $(TOOLPREFIX)objdump


kernel.bin:
	$(CC) $(CCARGS) -nostdinc -c boot.S -o boot.o
	$(CC) $(CCARGS) -nostdinc -c vectors.S -o vectors.o
	$(CC) $(CCARGS) -nostdinc -c trapasm.S -o trapasm.o
	$(CC) $(CCARGS) -c kernel.c -o kernel.o
	$(CC) $(CCARGS) -c gdt.c -o gdt.o
	$(CC) $(CCARGS) -c paging.c -o paging.o
	$(CC) $(CCARGS) -c kalloc.c -o kalloc.o
	$(CC) $(CCARGS) -c console.c -o console.o
	$(CC) $(CCARGS) -c idt.c -o idt.o
	$(CC) $(CCARGS) -c ioapic.c -o ioapic.o
	$(CC) $(CCARGS) -c kbd.c -o kbd.o
	$(CC) $(CCARGS) -c traps.c -o traps.o
	$(CC) $(CCARGS) -c pic.c -o pic.o
	$(CC) $(CCARGS) -c apic.c -o apic.o

	$(CC) $(CCARGS) -c bin/consoleTestProg.c -o consoleTestProg.o

	$(LD) -m elf_i386  -T linker.ld -o alpacos.bin -Og -nostdlib boot.o kernel.o paging.o gdt.o kalloc.o console.o idt.o traps.o kbd.o vectors.o trapasm.o pic.o apic.o ioapic.o consoleTestProg.o

clean:
	rm -rf *.o *.d alpacos.bin
