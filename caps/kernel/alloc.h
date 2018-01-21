/*-------------------------------------------------------------------------
 * alloc.h: Interface to capability based memory allocator
 * Mark P Jones, Portland State University
 *-----------------------------------------------------------------------*/
#ifndef ALLOC_H
#define ALLOC_H

/*-------------------------------------------------------------------------
 * Exported functions:
 */
extern void      initMemory(unsigned* hdrs, unsigned* mmap);
extern void      showUntyped();
extern unsigned* allocPage();
extern unsigned  copyPage(unsigned phys);

#endif
/*-----------------------------------------------------------------------*/
