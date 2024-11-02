#include "types.h"
#include "traps.h"
#include "functions.h"
#include "ioapic.h"

volatile struct ioapic *ioapic;
// Read & write to the IOAPIC by specifying a register
// in the IOAPIC struct, then read the data from
// that location in IOAPIC memory
static uint ioapicRead(int reg){
  ioapic->reg = reg;
  return ioapic->data;
}

static void ioapicWrite(int reg, uint data){
  ioapic->reg = reg;
  ioapic->data = data;
}

void ioapicInit(){
  // create pointer to IOAPIC location
ioapic = (volatile struct ioapic*)IOAPIC;

  int maxintr;
  // get the maximum number of interrupts
  maxintr = (ioapicRead(REG_VER) >> 16) & 0xFF;

  // Mark all interrupts edge-triggered, active high, disabled,
  // and not routed to any CPUs.
  for(int i = 0; i <= maxintr; i++){
    ioapicWrite(REG_TABLE+2*i, INT_DISABLED | (T_IRQ0 + i));
    ioapicWrite(REG_TABLE+2*i+1, 0);
  }
}

// Add interrupt request to IO APIC
// Keyboard is 1?
void ioapicEnable(int irq){
  ioapicWrite(REG_TABLE+2*irq, T_IRQ0 + irq);
  ioapicWrite(REG_TABLE+2*irq+1, 0);
}