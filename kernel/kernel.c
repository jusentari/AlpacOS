#include "types.h"
#include "multiboot.h"
#include <stdint.h>
#include "paging.h"
#include "gdt.h"
#include "kalloc.h"
#include "memlayout.h"
#include "console.h"
#include "functions.h"

multiboot_info_t *multiboot_header_ptr;

pde_t *page_dir;

int consoleTestProgMain();

void kernel_main(multiboot_info_t *multiboot_header){
    multiboot_header_ptr = multiboot_header;
    // okay so what to do for real.
    // gdt global descriptor table
    gdtInit();
    // paging
    pagingInit();
    // so now we have kernel between 2MB & 8MB, but it doesn't take up all that space
    // 0 - 0x8000 is mapped to 0 - 0x8000
    // 0x80000000 - 0x80008000 is mapped to 0 - 0x8000
    uint lastPageLoc = 0x00800000;
    // free range between end of kernel and 8MB (create that linked list of empty pages)
    freeRange(P2V((char*) &endkernel), P2V((char*) lastPageLoc));
    // sets up kernel virtual memory
    page_dir = setupKVM();

    idtInit();

    // PIC = Programmable Interrupt Controller
    // Disable the PIC
    picInit();
    // APIC = Advanced Programmable Interrupt Controller
    // enable the APIC
    apicInit();
    // IOAPIC
    ioapicInit();
    consoleInit(multiboot_header);
    // as part of paging i need to map hardware physical locations to virtual
    // like framebuffer
    // done!
    // set up interrupt table
    // set up hardware interrupts
    // set up traps for user space
    // set up filesystem access
    // added this because exiting this main function would disable interrupts, whic h idont
    // want bc im trying to test the keyboard
    consoleTestProgMain();
    for(;;) {
        asm("hlt");
    }
}
