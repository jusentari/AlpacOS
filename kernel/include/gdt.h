#include "types.h"

#define GDT_ENTRIES 5

struct gdt_entry_s {
    ushort limit_low;
    ushort base_low;
    uchar base_middle;
    uchar access;
    uchar flags;
    uchar base_high;
} __attribute__((packed));

struct gdt_ptr_s {
    ushort limit;
    uint base;
} __attribute__((packed));

extern void flushGDT();
void set_gdt_entry(ushort number, uint base, uint limit, ushort access, ushort flags);
void gdtInit();