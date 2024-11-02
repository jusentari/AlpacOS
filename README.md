# AlpacOS - A thrown-together OS for learning OS principles

Mainly inspired by (or even lifted from) xv6  

### To compile:  
Run `make qemu-grub` to run it normally  
Run `make qemu-gdb` to run it with a gdb hook, then type `target remote localhost:1234` in gdb to start debugging  

### Features:  
lol  
`x<mem location in hex>` to see what is at a memory location. That's it rn

Plan:  
Basic text display on screen *  
Enable paging *  
Set up kernel page allocation *  
Memory API  

Interrupt table *  
Keyboard input *  

Start on userspace:  
Processes  
Basic scheduling  
Sys calls  
File system/persistence  

Networking?  

Threading? I don't think so because I'll be porting this to a single core RISC-V platform later  

xv6 init:  
  kInit(end, P2V(4*1024*1024)); // phys page allocator  
  kvmalloc();      // kernel page table  
  --mpinit();        // detect other processors  
  lapicinit();     // interrupt controller  
  seginit();       // segment descriptors  
  picinit();       // disable pic  
  ioapicinit();    // another interrupt controller  
  consoleinit();   // console hardware  
  uartinit();      // serial port  
  pinit();         // process table  
  tvinit();        // trap vectors  
  binit();         // buffer cache  
  fileinit();      // file table  
  ideinit();       // disk   
  --startothers();   // start other processors  
  kInit2(P2V(4*1024*1024), P2V(PHYSTOP)); // must come after startothers()  
  userinit();      // first user process  
  mpmain();        // finish this processor's setup  


References:  
https://github.com/palladian1/xv6-annotated/  
https://pages.cs.wisc.edu/~remzi/OSTEP/  
https://wiki.osdev.org/Main_Page  
https://csapp.cs.cmu.edu/3e/home.html  
https://github.com/ry755/fennecos/  
https://github.com/mit-pdos/xv6-public  