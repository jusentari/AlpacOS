
// IOAPIC is always at this memory location for every CPU.
#define IOAPIC  0xFEC00000

// We use this to get the id of the IOAPIC?
// Don't really know why we need this
#define REG_ID     0x00  // Register index: ID
// We use this to find how many interrupts we have access to
#define REG_VER    0x01  // Register index: version
// The redirection table starts at REG_TABLE and uses
// two registers to configure each interrupt.
#define REG_TABLE  0x10  // Redirection table base

// The first (low) register in a pair contains configuration bits.
// The second (high) register contains a bitmask telling which
// CPUs can serve that interrupt.
#define INT_DISABLED   0x00010000  // Interrupt disabled
#define INT_LEVEL      0x00008000  // Level-triggered (vs edge-)
#define INT_ACTIVELOW  0x00002000  // Active low (vs high)
#define INT_LOGICAL    0x00000800  // Destination is CPU id (vs APIC ID)

// Check IOAPIC docs for info on this
// https://wiki.osdev.org/IOAPIC
// http://web.archive.org/web/20161130153145/http://download.intel.com/design/chipsets/datashts/29056601.pdf


// IO APIC MMIO structure: write reg, then read or write data.
struct ioapic {
  uint reg;
  uint pad[3];
  uint data;
};