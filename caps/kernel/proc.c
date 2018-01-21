/*-------------------------------------------------------------------------
 * proc.c:
 * Mark P Jones + Anders K Swanson, Portland State University
 *-----------------------------------------------------------------------*/
#include "winio.h"
#include "util.h"
#include "context.h"
#include "proc.h"
#include "paging.h"
#include "caps.h"

/*-------------------------------------------------------------------------
 * User-level processes:
 */
struct Process proc[2];    // The set of all user-level processes
struct Process* current;   // Identifies the current process

void initProcess(struct Process* proc,
                 unsigned lo,
                 unsigned hi,
                 unsigned entry) {
  proc->pdir   = newUserPdir(lo, hi);
  proc->cspace = allocCspace();
  proc->buffer = 0;
  showPdir(proc->pdir);
  printf("user code is at 0x%x\n", entry);
  initContext(&proc->ctxt, entry, 0);
}

/*-------------------------------------------------------------------------
 * Switch to the next available process:
 */
void reschedule() {
  current = (current==proc) ? (proc+1) : proc;
  setPdir(toPhys(current->pdir));
}

/*-----------------------------------------------------------------------*/
