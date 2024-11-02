#include <types.h>
#include "traps.h"
#include "apic.h"

void apicWrite(uint index, uint value){
    apic[index] = value;
    apic[ID];  // wait for write to finish, by reading
}

// will want an apicRead at some point

// need to acknowledge interrupts when we are done with them
void apicAck(){
    apicWrite(EOI, 0);
}

void apicInit(){
  apic = (volatile uint*)APIC;
  // Enable local APIC; set spurious interrupt vector.
  // setting the spurious interrupt vector essentially "activates" the APIC
  apicWrite(SVR, ENABLE | (T_IRQ0 + IRQ_SPURIOUS));
  apicAck();
  apicWrite(TPR, 0);
}
