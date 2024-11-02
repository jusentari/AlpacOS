#include "types.h"

#define PGSIZE 4096

// page table entry type
typedef uint pte_t;
// page directory entry type
typedef uint pde_t;

void pagingInit();
extern void loadPageDirectory(unsigned int*);
extern void enablePaging();
pde_t *setupKVM(void);
