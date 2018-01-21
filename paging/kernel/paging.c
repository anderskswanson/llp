/*-------------------------------------------------------------------------
 * paging.c:
 * Mark P Jones, Portland State University
 *-----------------------------------------------------------------------*/
#include "simpleio.h"		// For printf (debugging output)
#include "paging.h"
#include "memory.h"

extern unsigned* allocPage();
extern void      fatal(char* msg);

/*-------------------------------------------------------------------------
 * Allocation of paging structures:
 */
struct Pdir* allocPdir() {
  // Allocate a fresh page directory:
  struct Pdir* pdir = (struct Pdir*)allocPage();

  // TODO (Step 5): Add superpage mappings to pdir for the first PHYSMAP
  // bytes of physical memory.  You should use a bitwise or
  // operation to ensure that the PERMS_KERNELSPACE bits are set
  // in every PDE that you create.
  
  for (unsigned i=0; i<(PHYSMAP >> SUPERSIZE); i++) {
    pdir->pde[(KERNEL_SPACE>>SUPERSIZE)+i] = (i << SUPERSIZE) | PERMS_KERNELSPACE;	  
  }
  
  return pdir;
}

struct Ptab* allocPtab() {
    return (struct Ptab*)allocPage();
}

/*-------------------------------------------------------------------------
 * Create a mapping in the specified page directory that will map the
 * virtual address (page) specified by virt to the physical address
 * (page) specified by phys.  Any nonzero offset in the least
 * significant 12 bits of either virt or phys will be ignored.
 * 
 */
void mapPage(struct Pdir* pdir, unsigned virt, unsigned phys) {
    // Mask out the least significant 12 bits of virt and phys.
    virt = alignTo(virt, PAGESIZE);
    phys = alignTo(phys, PAGESIZE);
    printf("virt %x, phys %x\n", virt, phys);

    // Make sure that the virtual address is in user space.
    if (virt>=KERNEL_SPACE) {
        fatal("virtual address is in kernel space");
    }

    // TODO (Step 7): Find the relevant entry in the page directory
    unsigned didx = virt >> SUPERSIZE;
    unsigned pde = pdir->pde[didx];

    // TODO (Step 7): report a fatal error if there is already a
    //       superpage mapped at that address (this shouldn't
    //       be possible at this stage, but we're programming
    //       defensively).

    if((pde&0x81) == 0x81)
        fatal("SUPERPAGE!");		

    // TODO (Step 7): If there is no page table (i.e., the PDE is
    //       empty), then allocate a new page table and update the
    //       pdir to point to it.   (use PERMS_USER_RW together with
    //       the new page table's *physical* address for this.)
    
    struct Ptab * ptab;		
    if((pde&0x1) == 0x1)
        ptab = fromPhys(struct Ptab *, alignTo(pde, PAGESIZE));
    else
    {
        ptab = allocPtab();
        pdir->pde[didx] = toPhys(ptab) | PERMS_USER_RW;
    }

    // TODO (Step 7): If there was an existing page table (i.e.,
    //       the PDE pointed to a page table), then report a fatal
    //       error if the specific entry we want is already in use.
    
    unsigned tidx = (virt >> PAGESIZE) & 0x3ff;
    unsigned pte = ptab->pte[tidx];

    if((pte&0x1) == 0x1)
        fatal("PAGE");

    // TODO (Step 7): Add an entry in the page table structure to
    //       complete the mapping of virt to phys.  (Use PERMS_USER_RW
    //       again, this time combined with the value of the
    //       phys parameter that was supplied as an input.)
    //
    ptab->pte[tidx] = phys | PERMS_USER_RW;
}

/*-------------------------------------------------------------------------
 * Print a description of a page directory (for debugging purposes).
 */
void showPdir(struct Pdir* pdir) {
  printf("  Page directory at %x\n", pdir);
  for (unsigned i=0; i<1024; i++) {
    if (pdir->pde[i]&1) {
      if (pdir->pde[i]&0x80) {
        printf("    %x: [%x-%x] => [%x-%x], superpage\n",
               i, (i<<SUPERSIZE), ((i+1)<<SUPERSIZE)-1,
               alignTo(pdir->pde[i], SUPERSIZE),
               alignTo(pdir->pde[i], SUPERSIZE) + 0x3fffff);
      } else {
        struct Ptab* ptab = fromPhys(struct Ptab*,
                                     alignTo(pdir->pde[i], PAGESIZE));
        unsigned base = (i<<SUPERSIZE);
        printf("    [%x-%x] => page table at %x (physical %x):\n",
               base, base + (1<<SUPERSIZE)-1,
               ptab, alignTo(pdir->pde[i], PAGESIZE));
        for (unsigned j=0; j<1024; j++) {
          if (ptab->pte[j] & 1) {
            printf("      %x: [%x-%x] => [%x-%x] page\n",
                   j, base+(j<<PAGESIZE), base + ((j+1)<<PAGESIZE) - 1,
                   alignTo(ptab->pte[j], PAGESIZE),
                   alignTo(ptab->pte[j], PAGESIZE) + 0xfff);
          }
        }
      }
    }
  }
}

/*-----------------------------------------------------------------------*/
