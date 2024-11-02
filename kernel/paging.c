#include "paging.h"
#include "kalloc.h"
#include "memlayout.h"
#include "mmu.h"
#include "types.h"
extern char data[];
uint first_page_table[1024] __attribute__((aligned(4096)));
uint second_page_table[1024] __attribute__((aligned(4096)));
uint higher_half_page_table[1024] __attribute__((aligned(4096)));
uint second_higher_half_page_table[1024] __attribute__((aligned(4096)));

// Each process needs its own page dir, with each page dir
// entry having its own page table.
// Map the kernel here

// TODO: use something like "pde_t" instead
uint kernel_page_dir[1024] __attribute__((aligned(4096)));

void pageDirInit() {
  int i;
  for (i = 0; i < 1024; i++) {
    // This sets the following flags to the pages:
    //   Supervisor: Only kernel-mode can access them
    //   Write Enabled: It can be both read from and written to
    //   Not Present: The page table is not present
    kernel_page_dir[i] = 0x00000002;
  }
}
// TODO: make this take any pagedir
void mapKernel(uint *page_directory) {
  // holds the physical address where we want to start mapping these pages to.
  // in this case, we want to map these pages to the very beginning of memory.

  // we will fill all 1024 entries in the table, mapping 4 megabytes
  // identity mapped, so we use 0x1000
  // 0x1000 -> 0x1 + the first 12 bits for perms
  // page table entry 0 of page dir entry 0 is page frame 0
  // 0 << 22 + 0 << 12 == 0 << 12
  // page table entry 1 of page dir entry 0 is page frame 1
  // 0 << 22 + 1 << 12 == 1 << 12
  // etc.

  const uint second_page_start = 0x400000;
  for (uint i = 0; i < 1024; i++) {
    // As the address is page aligned, it will always leave 12 bits zeroed.
    // Those bits are used by the attributes ;)
    first_page_table[i] =
        (i * 0x1000) | 3; // attributes: supervisor level, read/write, present.
    higher_half_page_table[i] = (i * 0x1000) | 3;
    second_page_table[i] =
        (i * 0x1000 + second_page_start) |
        3; // attributes: supervisor level, read/write, present.
    second_higher_half_page_table[i] = (i * 0x1000 + second_page_start) | 3;
  }
  page_directory[0] = ((uint)first_page_table) | 3;
  page_directory[PDX(KERNBASE)] = ((uint)&higher_half_page_table) | 3;
  page_directory[PDX(second_page_start)] = ((uint)second_page_table) | 3;
  page_directory[PDX(KERNBASE + second_page_start)] =
      ((uint)&second_higher_half_page_table) | 3;
}

void pagingInit() {
  pageDirInit();
  mapKernel(kernel_page_dir);
  // loads address of kpd into cr3 register
  loadPageDirectory(kernel_page_dir);
  // sets paging bit to 1
  enablePaging();
}

// gets page table entry for a specific virtual address, use alloc to optionally
// create a new page table if one doesn't exist
static pte_t *getPageTableEntry(pde_t *page_directory,
                                const void *virtual_address, int alloc) {
  // get paged ir entry from first 10 bits
  uint dir_entry_from_va = PDX(virtual_address);
  // page_directory is a pointer to the actual page dir for this process
  // pde is the specific entry for the given virtual address, which we extract
  // the page dir index from with PDX
  pde_t *pde = &page_directory[dir_entry_from_va];

  // get the page table entry for the virtual address
  // importantly, we don't know if the page table exists at this point, so check
  // that first
  pte_t *page_table;
  // PTE_P is the flag to check if the Page Table Entry is Present
  if (*pde & PTE_P) {
    // get page table entry
    // figure out why the fuckfuckfuckfuckfuck this works
    // i still dont understand why the physical to virtual thing works or how it
    // works with paging enabled this works because it is memory mapped, kernel
    // is ALWAYS at 0x8000_0000 for EVERY process? this wasnt working before,
    // but that is because i had it masked with 0x000, which masks it with
    // 0x0000_0000, making it always 0
    uint pte_addr = PTE_ADDR(*pde);
    page_table = (pte_t *)(pte_addr + KERNBASE);
  } else {
    // if we don't allocate on a page miss, sorry dude
    if (!alloc)
      return 0;
    // otherwise, try to allocate some memory for that page table
    page_table = (pte_t *)kAlloc();
    // if page_table is null just gtfo
    if (page_table == 0) {
      return 0;
    }
    // clear info in the page table to avoid information slipping through (mfw
    // my ssh key aint erased) memset(page_table, 0, PGSIZE); get the physical
    // address for the page, then set the perms flags
    *pde = V2P(page_table) | PTE_P | PTE_W | PTE_U;
  }
  // page dir -> got page table (or made one) -> return page table entry
  return &page_table[PTX(virtual_address)];
}

// maps all of the pages for a virtual address + some size (with perms)
static int mapPages(pde_t *page_directory, void *virtual_address,
                    uint physical_address, uint size, uint permissions) {
  // snap the virtual address to a page edge
  uint cur_page = PGROUNDDOWN((uint)virtual_address);
  // get the virtual address of the page that is <size> bytes away
  uint last_page = PGROUNDDOWN((uint)virtual_address + size - 1);

  for (uint current_physical_address = physical_address;
       (uint)cur_page <= (uint)last_page;
       current_physical_address += PGSIZE, cur_page += PGSIZE) {
    pte_t *page_table_entry;
    // get page table for virtual address
    page_table_entry = getPageTableEntry(page_directory, (void *)cur_page, 1);
    // bruh memory is out
    if (page_table_entry == 0)
      return -1;
    // create a new page table entry w the perms & set page as present
    *page_table_entry = current_physical_address | permissions | PTE_P;
  }
  return 0;
}

// k, so this maps the kernel space
// first entry is physical 0 - 2M, maps that to KERNBASE (0x8000_0000) (kernel
// can write) second entry maps 2M - data, which is the kernel's code base (read
// only) third entry maps data - PHYSTOP, which is the space the kernel will
// take up (kernel can write)
// -- there is a huge gap here between PHYSTOP & DEVSPACE, PHYSTOP is stypicall
// like 300MB but in a 4GB system it can be DEVSPACE - 1 fourth entry maps
// DEVSPACE to DEVSPACE (DEVSPACE is typically higher mem spaces like
// 0xFE00_0000)

pde_t *setupKVM() {
  ulong virtual_data = (ulong)&data + KERNBASE;
  mapPages(kernel_page_dir, (char *)KERNBASE, (uint)0, EXTMEM, PTE_W);
  mapPages(kernel_page_dir, (char *)KERNLINK, (uint)V2P(KERNLINK),
           virtual_data - KERNLINK, 0);
  mapPages(kernel_page_dir, (char *)virtual_data, (uint)V2P(virtual_data),
           PHYSTOP - virtual_data, PTE_W);
  mapPages(kernel_page_dir, (char *)DEVSPACE, (uint)DEVSPACE,
           (uint)0xFF000000 - DEVSPACE, PTE_W);
  loadPageDirectory(kernel_page_dir);
  return kernel_page_dir;
}
