#include "kalloc.h"
#include "types.h"
#include "paging.h"
#include "mmu.h"


// pointer to the first page in the freelist
struct {
  //struct spinlock lock;
  //int use_lock; use these later to lock memory when it is being freed/allocated
  struct run *freelist;
} kmem;

void kInit(void *vstart, void *vend){
  freeRange(vstart, vend);
}

void freeRange(void *vstart, void *vend){
  char *p;
  // PGROUNDUP rounds the first vstart to a page edge
  p = (char*) PGROUNDUP((uint)vstart);
  for(; p + PGSIZE <= (char*)vend; p += PGSIZE)
    kFree(p);
}

// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kAlloc().  (The exception is when
// initializing the allocator; see kInit above.)
void kFree(char *v){
  struct run *r;

  r = (struct run*)v;
  r->next = kmem.freelist;
  kmem.freelist = r;
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
char* kAlloc(){
  struct run *r;

  r = kmem.freelist;
  // if r is not null
  if(r)
    kmem.freelist = r->next;
  return (char*)r;
}

