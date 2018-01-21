/*-------------------------------------------------------------------------
 * util.h:
 * Mark P Jones, Portland State University
 *-----------------------------------------------------------------------*/
#ifndef UTIL_H
#define UTIL_H

/*-------------------------------------------------------------------------
 * Basic code for halting the processor, reporting a fatal error, or
 * checking an assertion:
 */
extern void halt();

static inline void fatal(char* msg) {
  printf("FATAL ERROR: %s\n", msg);
  halt();
}

static inline void assert(int cond, char* msg) {
  if (!cond) {
    printf("ASSERTION FAILED: %s\n", msg);
    halt();
  }
}

#endif
/*-----------------------------------------------------------------------*/
