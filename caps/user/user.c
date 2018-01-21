/*-------------------------------------------------------------------------
 * user.c: a simple user-level program
 * Mark P. Jones, Portland State University, May 2016
 *-----------------------------------------------------------------------*/
#include "capio.h"
#include "syscalls.h"

void kputs(unsigned cap, char* s) {
  while (*s) {
    kputc(cap, *s++);
  }
}

unsigned printbuff(unsigned cap)
{
  return printBuffer(cap);	
}

unsigned makebuff(unsigned cap, unsigned addr)
{
  return allocBuffer(cap, addr);	
}

unsigned currentticks(unsigned cap) {
  return getTicks(cap);	
}

unsigned remainingmemory(unsigned cap)
{
  return remaining(cap);	
}

unsigned rtype(unsigned cap, unsigned type, unsigned bits, unsigned slot, unsigned num)
{
  return retype(cap, type, bits, slot, num);	
}


void strcpy(char* src, char* dst) {
  do {
    *dst++ = *src;
  } while (*src++);    
}

void cmain() {
  int i;

  // Basic tests of console and window output: ----------------------------
  cls();
  unsigned myid = kputc(CONSOLE, '!');
  printf("My process id is %x\n", myid);
  puts("in user code\n");

  for (i=0; i<4; i++) {
    kputs(CONSOLE, "hello, kernel console\n");
    puts("hello, user console\n");
    setAttr(i&0xf);
  }

  // Test operations for manipulating capability spaces: ------------------
  if (myid) {
    printf("My process id is %x\n", kputc(CONSOLE, 'A'));
    capmove(1, 4, MOVE);
    printf("My process id is %x\n", kputc(CONSOLE, 'B'));
    capmove(4, 1, COPY);
    printf("My process id is %x\n", kputc(CONSOLE, 'C'));
    capclear(1);
    printf("My process id is %x\n", kputc(CONSOLE, 'D'));
    capmove(4, 1, MOVE);
    printf("My process id is %x\n", kputc(CONSOLE, 'E'));
  } else {
    printf("I don't have a capability for the console");
  }

  // Allocate memory for this process without a capability: ---------------
  kmapPage(0x600000);
  kmapPage(0x601000);
  kmapPage(0x603000);
  unsigned stomp = 0x700000;
  for (int j=0; j<8; j++) {
    kmapPage(stomp);
    *((unsigned*)stomp) = stomp;
    stomp += (1<<12);
  }

  // Allocate memory for this process using a capability: -----------------
  unsigned memoryPool = 3;   // Capability number for untyped object
  //alloc a page
  rtype(UNTYPED, PAGE_TYPE, 0, 12, 1);
  rtype(UNTYPED, CSPACE_TYPE, 0, 14, 1);
  //allocPage(memoryPool,    /*slot*/12);
  //allocCspace(memoryPool,  /*slot*/14);

  // Use capabilities to add memory to this address space: ----------------
  stomp = 0x80000000;        // Let's allocate a page here
  //lets allocate two pages!
  rtype(UNTYPED, PAGETABLE_TYPE, 0, 21, 2);
//  allocPageTable(memoryPool, 21/*slot*/);
  mapPageTable(21, stomp);
  mapPageTable(21, stomp+0x800000); // 8MB further
  rtype(UNTYPED, PAGE_TYPE, 0, 20, 1);
  //allocPage(memoryPool,      20/*slot*/);
  mapPage(20, stomp);
  dump();

  /*//test to print ticks
  unsigned time = currentticks(TIMER);
  if(time)
    printf("TICKS = %d\n", time);
  else
    printf("I don't have a capability to the timer!\n");	 

  printf("remaining memory = %d bytes\n", remainingmemory(UNTYPED));
*/
  char * buffer = (char*)0x100000;
  //allocPageTable(memoryPool, 22);
  mapPageTable(22, buffer);
  printf("did my buffer get made: %d\n", makebuff(UNTYPED, buffer));   
  strcpy("Hello from buffer!\n", buffer);
  printf("did my buffer print: %d\n", printbuff(WINDOW));
  // Loop to avoid terminating user program: ------------------------------
  puts("\n\nUser code does not return\n");
  for (;;) { /* Don't return! */
  }
  puts("This message won't appear!\n");
}

/*-----------------------------------------------------------------------*/
