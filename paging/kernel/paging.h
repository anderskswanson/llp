/*-------------------------------------------------------------------------
 * paging.h:
 * Mark P Jones, Portland State University
 *-----------------------------------------------------------------------*/
#ifndef PAGING_H
#define PAGING_H

/*-------------------------------------------------------------------------
 * Paging Structures:
 */
struct Pdir { unsigned pde[1024]; };
struct Ptab { unsigned pte[1024]; };

extern struct Pdir* allocPdir();
extern struct Ptab* allocPtab();

/*-------------------------------------------------------------------------
 * Set the page directory control register to a specific value.
 */
static inline void setPdir(unsigned pdir) {
  asm("  movl  %0, %%cr3\n" : : "r"(pdir));
}

/*-------------------------------------------------------------------------
 * Map a virtual address to a specific physical address:
 */
extern void mapPage(struct Pdir* pdir, unsigned virt, unsigned phys);

/*-------------------------------------------------------------------------
 * Display a description of a page directory (for debugging purposes):
 */
extern void showPdir(struct Pdir* pdir);

#endif
/*-----------------------------------------------------------------------*/
