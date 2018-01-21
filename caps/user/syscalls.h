/*-------------------------------------------------------------------------
 * syscalls.h:
 * Mark P. Jones, Portland State University, May 2016
 *-----------------------------------------------------------------------*/
#ifndef SYSCALLS_H
#define SYSCALLS_H


#define CSPACE_TYPE 0
#define UNTYPED_TYPE 1
#define PAGE_TYPE 2
#define PAGETABLE_TYPE 3
/*-------------------------------------------------------------------------
 * System calls not requiring a capability argument:
 */
extern unsigned kmapPage(unsigned addr);
extern void     dump();

/*
 * Print the buffer
 */
#define WINDOW 2
extern unsigned printBuffer(unsigned cap);

/*
 * Access to untyped memory
 */

#define UNTYPED 3
extern unsigned retype(unsigned, unsigned, unsigned, unsigned, unsigned);
extern unsigned remaining(unsigned cap);
extern unsigned allocBuffer(unsigned cap, unsigned addr);

/*
 * Access to the ticks
 */
#define TIMER 7
extern unsigned getTicks(unsigned cap);


/*-------------------------------------------------------------------------
 * Access to the console window:
 */
#define CONSOLE  1    // default capability slot for the console

extern unsigned kputc(unsigned cap, unsigned ch);

/*-------------------------------------------------------------------------
 * Capability space manipulation:
 */
enum Capmove { MOVE=0, COPY=1 };
extern unsigned capmove(unsigned src, unsigned dst, enum Capmove copy);
extern unsigned capclear(unsigned src);

/*-------------------------------------------------------------------------
 * Allocate memory from an untyped memory area:
 */
extern unsigned allocUntyped(unsigned ucap, unsigned slot, unsigned bits);
extern unsigned allocCspace(unsigned ucap, unsigned slot);
extern unsigned allocPage(unsigned ucap, unsigned slot);
extern unsigned allocPageTable(unsigned ucap, unsigned slot);

/*-------------------------------------------------------------------------
 * Mapping objects in to an address space:
 */
extern unsigned mapPage(unsigned cap, unsigned addr);
extern unsigned mapPageTable(unsigned cap, unsigned addr);

#endif
/*-----------------------------------------------------------------------*/
