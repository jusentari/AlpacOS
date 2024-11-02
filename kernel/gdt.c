// init the GDT here instead of relying on GRUB's GDT (RTFM!!!!!!!!)
// u should have read this first you stuuupid bitch : https://wiki.osdev.org/Global_Descriptor_Table
#include "mmu.h"
#include "x86.h"
#include "gdt.h"

// initialise kernel & userspace segments to protect kenerl code from userspace
struct segdesc gdt_ptr[NSEGS];
struct gdt_ptr_s gdtr;

void gdtInit(){
    // Look at mmu.h for an explanation here
    // Global Descriptor Table (GDT) is an array of segment descriptors that have a 
    // really convoluted structure dictated here (fig. 5-3): https://pdos.csail.mit.edu/6.828/2011/readings/i386/s05_01.htm
    // This section was lifted from xv6 and sets the permissions necessary for the GDT so that the kernel & user can use the whole memory
    gdt_ptr[SEG_KCODE] = SEG(STA_X|STA_R, 0, 0xffffffff, 0);
    gdt_ptr[SEG_KDATA] = SEG(STA_W, 0, 0xffffffff, 0);
    gdt_ptr[SEG_UCODE] = SEG(STA_X|STA_R, 0, 0xffffffff, DPL_USER);
    gdt_ptr[SEG_UDATA] = SEG(STA_W, 0, 0xffffffff, DPL_USER);
    //lgdt(gdt_ptr, sizeof(gdt_ptr));
    gdtr.base = (uint) &gdt_ptr[0];
    gdtr.limit = sizeof(gdt_ptr);
    asm volatile("lgdt %0" : : "m" (gdtr));
    flushGDT();
}