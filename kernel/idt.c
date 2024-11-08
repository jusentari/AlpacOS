#include "types.h"
#include "idt.h"
#include "gdt.h"
// must be aligned to 16-byte interval
__attribute__((aligned(0x10)))
static idt_entry_t idt[IDT_ENTRIES];
static idtr_t idtr;

extern ulong vectors[];

void createIDTEntry(uchar vector, ulong isr){
    idt_entry_t* descriptor = &idt[vector];
    descriptor->lower_offset = isr & 0xFFFF;
    descriptor->segment_selector = 0x08;
    descriptor->args = 0;
    descriptor->zero1 = 0;
    // this shouldn't be hardcoded. 0xE for interrupts, 0xF for traps (system call)
    // afaik, traps don't set interrupt flag to 0, so they can be interrupted
    // actual interrupts do tho
    descriptor->type = 0xE;
    descriptor->zero2 = 0;
    // descriptor privilege level
    descriptor->dpl = 0;
    descriptor->present = 1; // duh
    descriptor->higher_offset = isr >> 16 & 0xFFFF;
    
}

void idtInit(){
    // idtr points to base of idt table
    idtr.base = (ulong) &idt[0];
    // limit is length of idt table
    idtr.limit = (uint) (sizeof(idt_entry_t) * IDT_ENTRIES) - 1;
    for(int i = 0; i < IDT_ENTRIES; i++){
        // pass in vector number & location of vector code
        createIDTEntry(i, vectors[i]);
    }
    idt[64].type = 0xF;

    asm volatile ("lidt %0" :: "m"(idtr));
    asm volatile ("sti");
}

















/*
#include <kernel/idt.h>

#include <stdbool.h>
#include <stdint.h>

extern void *isr_stub_table[];

__attribute__((aligned(0x10)))
static idt_entry_t idt[256];
static idtr_t idtr;

void init_idt() {
    idtr.base = (uintptr_t) &idt[0];
    idtr.limit = (uint16_t) sizeof(idt_entry_t) * IDT_ENTRIES - 1;

    for (uint8_t vector = 0; vector < IDT_ENTRIES; vector++) {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
    }

    asm volatile ("lidt %0" :: "m" (idtr));
    asm volatile ("sti");
}

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    idt_entry_t* descriptor = &idt[vector];
    descriptor->isr_low = (uint32_t) isr & 0xFFFF;
    descriptor->kernel_cs = 0x08;
    descriptor->attributes = flags;
    descriptor->isr_high = (uint32_t) isr >> 16;
    descriptor->reserved = 0;
}*/