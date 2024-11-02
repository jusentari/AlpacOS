#include "functions.h"
#include "types.h"

// Layout of the trap frame built on the stack by the
// hardware and by trapasm.S, and passed to trap().
// Read it bottom up from this webpage:
// https://pdos.csail.mit.edu/6.828/2011/readings/i386/s09_06.htm
struct trapframe {
  // registers as pushed by pushal
  uint edi;
  uint esi;
  uint ebp;
  uint oesp; // useless & ignored
  uint ebx;
  uint edx;
  uint ecx;
  uint eax;

  // rest of trap frame
  ushort gs;
  ushort padding1;
  ushort fs;
  ushort padding2;
  ushort es;
  ushort padding3;
  ushort ds;
  ushort padding4;
  uint trapno;

  // below here defined by x86 hardware
  uint err;
  uint eip;
  ushort cs;
  ushort padding5;
  uint eflags;

  // below here only when crossing rings, such as from user to kernel
  uint esp;
  ushort ss;
  ushort padding6;
};

void trap(struct trapframe *trapframe) {
  if (trapframe->trapno == 64) {
    // this be a syscall
  }

  // 33 is keyboard interrupt
  // need to set up ioapic first
  if (trapframe->trapno == 33) {
    int character = kbdgetc();
    apicAck();
    if (character > 0) {
      acceptCharacter((uchar)character);
    }
  }
}
