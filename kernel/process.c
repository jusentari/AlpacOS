#include "types.h"
#include "process.h"
pcb_t processes[16];
// process control block
typedef struct pcb {
    uint base;
    uint bounds;
    uint pid;
    enum process_status status;
} pcb_t;

struct pframe {
    uint eip; // instruction pointer
    uint esp; // stack pointer
    uint eax; // accumulator
    uint ebx; // base
    uint ecx; // counter
    uint edx; // data
    uint esi; // source index
    uint edi; // destination index
    uint ebp; // stack base pointer
    uint flags; // flags reg
    // may need to rearrange these ?
    // because of pushal
} pframe_t;

uint createProcess(){
    // allocate pages for the process, elf parsing time?
    // need to allocate enough space for code, static vars, etc.
    // need to make sure ro & rw are allocated correctly
    // set up the stack
    // clear the registers
    // call the main function pointer
    return 0;
}

// process purposefully yields to CPU
void yield(){

}

void killProcess(uint pid){
    // find process and set status to KILLED r smth
    processes[pid]->status = STOPPING;
}