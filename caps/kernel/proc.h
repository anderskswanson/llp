/*-------------------------------------------------------------------------
 * proc.h:
 * Mark P Jones + YOUR NAME HERE, Portland State University
 *-----------------------------------------------------------------------*/
#ifndef PROC_H
#define PROC_H
#include "context.h"

/*-------------------------------------------------------------------------
 * User-level processes:
 */
struct Process {
  struct Context ctxt;
  struct Pdir*   pdir;
  struct Cspace* cspace;
  unsigned*      buffer;
};

extern struct Process proc[];     // The set of all user-level processes
extern struct Process* current;   // Identifies the current process

extern void initProcess(struct Process* proc,
                        unsigned lo,
                        unsigned hi,
                        unsigned entry);
extern void reschedule();

#endif
/*-----------------------------------------------------------------------*/
