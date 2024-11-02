#include "types.h"
// Gate descriptors for interrupts and traps
typedef struct idt_entry_t {
  uint lower_offset : 16;   // low 16 bits of offset in segment
  uint segment_selector : 16;         // code segment selector
  uint args : 5;        // # args, 0 for interrupt/trap gates
  uint zero1 : 3;        // reserved(should be zero I guess)
  uint type : 4;        // type(STS_{IG32,TG32})
  uint zero2 : 1;           // must be 0 (system)
  uint dpl : 2;         // descriptor(meaning new) privilege level
  uint present : 1;           // Present
  uint higher_offset : 16;  // high bits of offset in segment
} __attribute__((packed)) idt_entry_t;

// okay i spent 15 fucking hours on this.
// i SPECIFIED PACKED. limit was 16 a uint (16 bits), base was a ulong (32 bits).
// but fucking limit
// HAD 16 BITS OF PADDING AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAa
// it took me so long to figure this shit out ughhh
typedef struct idtr {
    uint limit : 16; // ufck you gcc & gdb, ligma nuts
    ulong base : 32;
} __attribute__((packed)) idtr_t;
#define IDT_ENTRIES 256

extern void loadIDT();