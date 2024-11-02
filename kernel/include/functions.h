// blasphemy, but this is kinda like a list of "public" functions
// following the xv6 convention of putting these all in one place
// so i can have other files reference them w/o pulling along
// a ton of internal enums n shit

#include <types.h>
#include "multiboot.h"
#include "paging.h"

// apic.c
void apicInit();
void apicWrite();
void apicAck();

// console.c
void acceptCharacter(uchar character);
void drawCharToScreen(char character, uint32_t pixel_offset);
void drawChar(unsigned char character);
void deleteChar();
int strlen(const char* str);
void drawString(char* string);
void consoleInit(multiboot_info_t *d_info);
int getchar();

// gdt.c
void gdtInit();

// idt.c
void createIDTEntry(uchar vector, ulong isr);
void idtInit();


//ioapic.h
void ioapicInit();
void ioapicEnable(int irq);

// kalloc.h
void kInit(void *vstart, void *vend);
void freeRange(void *vstart, void *vend);
void kFree(char *v);
char* kAlloc();

// kbc.c
int kbdgetc();

// paging.c
void pageDirInit();
void mapKernel(uint *page_directory);
void pagingInit();
pde_t *setupKVM();

// pic.c
void picInit();
