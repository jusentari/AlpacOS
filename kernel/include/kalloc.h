#include "types.h"

// Initialization happens in two phases.
// 1. main() calls kInit() while still using entrypgdir to place just
// the pages mapped by entrypgdir on free list.
// 2. main() calls kInit2() with the rest of the physical pages
// after installing a full page table that maps them on all cores.
void kInit(void *vstart, void *vend);
void kInit2(void *vstart, void *vend);
void freeRange(void *vstart, void *vend);
void kFree(char *v);
char* kAlloc(void);

extern uint endkernel;


// struct that just contains a pointer to the next free page
struct run {
  struct run *next;
};