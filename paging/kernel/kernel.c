/*-------------------------------------------------------------------------
 * kernel.c:
 * Mark P Jones + Anders K Swanson, Portland State University
 * I helped make this!
 *-----------------------------------------------------------------------*/
#include "simpleio.h"
#include "mimguser.h"
#include "context.h"
#include "paging.h"
#include "memory.h"

/*-------------------------------------------------------------------------
 * Basic code for halting the processor and reporting a fatal error:
 */
extern void halt();

void fatal(char* msg) {
    printf("FATAL ERROR: %s\n", msg);
    halt();
}

/*-------------------------------------------------------------------------
 * Memory management: simple functionality for allocating pages of
 * memory for use in constructing page tables, etc.
 */
unsigned physStart;  // Set during initialization to start of memory pool
unsigned physEnd;    // Set during initialization to end of memory pool

unsigned* allocPage() {
    
    if(physEnd - physStart < 0x1000)
        fatal("insufficient data for page allocation");


    unsigned * page = fromPhys(unsigned*, alignTo(physStart, PAGESIZE));
    for(int i = 0; i < 1024; ++i)
        page[i] = 0;	    
    physStart = pageNext(physStart);

    return page;

}

/*-------------------------------------------------------------------------
 * Context data structures: a place holder for when we get back to
 * context switching ...
 */
struct Context user;

/*-------------------------------------------------------------------------
 * The main "kernel" code:
 */
void kernel() {
  struct BootData* bd = (struct BootData*)0x1000;
  unsigned* hdrs      = bd->headers;
  unsigned* mmap      = bd->mmap;
  unsigned  i;

  setAttr(0x2e);
  cls();
  setAttr(7);
  setWindow(1, 23, 1, 45);   // kernel on left hand side
  cls();
  printf("Paging kernel has booted!\n");

  printf("Headers:\n");
  for (i=0; i<hdrs[0]; i++) {
    printf(" header[%d]: [%x-%x], entry %x\n",
           i, hdrs[3*i+1], hdrs[3*i+2], hdrs[3*i+3]);
                  //start      end          entry
  }

  printf("Memory map:\n");
  for (i=0; i<mmap[0]; i++) {
    printf(" mmap[%d]: [%x-%x]\n",
           i, mmap[2*i+1], mmap[2*i+2]);
  }

  printf("Strings:\n");
  printf(" cmdline: %s [%x]\n", bd->cmdline, bd->cmdline);
  printf(" imgline: %s [%x]\n", bd->imgline, bd->imgline);

  extern struct Pdir initdir[];
  printf("initial page directory is at 0x%x\n", initdir);
  showPdir(initdir);

  printf("kernel code is at 0x%x\n", kernel);


  physStart = 0;
  physEnd = 0;

  for(i = 0; i < mmap[0]; ++i)
  {
    if(mmap[2*i+1] >= KERNEL_LOAD && physStart < mmap[2*i+1])
    {
      if(mmap[2*i+2] < PHYSMAP && (mmap[2*i+2] - mmap[2*i+1] > physEnd - physStart))
      {
        physStart = mmap[2*i+1];	    
        physEnd = mmap[2*i+2];	    
      }
    }
  }

  physStart = firstPageAfter(physStart);
  physEnd = endPageBefore(physEnd);

  // TODO (Step 3): Report a fatal error if there is no suitable
  // region of memory.
  
  if((physEnd - physStart) <= 0)
  {
     printf("Error: no suitable region of memory\n");
     halt();
  }     


  // TODO (Step 3): Scan the list of headers for loaded regions of
  // memory to look for conflicts with the [physStart..physEnd]
  // region.  If you find a conflict, increase physStart to point
  // to the start of the first page after the conflicting region.

  for (i=0; i<hdrs[0]; i++) 
  {
    if(physStart >= hdrs[3*i+1] && physStart <= hdrs[3*i+2])
    {
      physStart = firstPageAfter(hdrs[3*i+2]);	    
    }
  }


  // TODO (Step 3): Report a fatal error if this process ends with
  // an empty region of physical memory.

  if(physStart - physEnd <= 0)
  {
    printf("Error: empty region of physical memory\n");	  
    halt();
  }	  

  // Display the upper and lower bounds of the chosen memory
  // region, as well as the total number of bytes that it
  // contains.

  printf("Will allocate from region [%x-%x], %d bytes\n",
               physStart, physEnd, 1 + physEnd - physStart);

  // Now we will build a new page directory:
  //
  printf("%x\n", allocPage());
  printf("%x\n", allocPage());
  printf("%x\n", allocPage());
  printf("%x\n", allocPage());
  printf("%x\n", allocPage());
  struct Pdir* newpdir = allocPdir();

  // TODO (Step 8): You might need to do something more to the
  // newly created page directory, but who knows what that might
  // be ... ?   :-)

  mapPage(newpdir, 0x0, 0);
  showPdir(newpdir);

  printf("This message should appear on the screen!\n");

  printf("hdrs[7] = %x, hdrs[8] = %x\n", hdrs[7], hdrs[8]);
  /*printf("user code is at 0x%x\n", hdrs[9]);
  initContext(&user, hdrs[9], 0);
  printf("user is at %x\n", (unsigned)(&user));
  switchToUser(&user);*/

  printf("The kernel will now halt!\n");
}

void kputc_imp() {  /* A trivial system call */
  putchar(user.regs.eax);
  switchToUser(&user);
}

/*-----------------------------------------------------------------------*/
