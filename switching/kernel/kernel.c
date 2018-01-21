#include "simpleio.h"
#include "mimguser.h"
#include "context.h"
#include "hardware.h"

extern void initPIC();

/* Initialize a context with specified eip and esp values.
 */
void initContext(struct Context* ctxt, unsigned eip, unsigned esp) {
  extern char USER_DS[];
  extern char USER_CS[];
  printf("user data segment is 0x%x\n", (unsigned)USER_DS);
  printf("user code segment is 0x%x\n", (unsigned)USER_CS);
  ctxt->segs.ds     = (unsigned)USER_DS;
  ctxt->segs.es     = (unsigned)USER_DS;
  ctxt->segs.fs     = (unsigned)USER_DS;
  ctxt->segs.gs     = (unsigned)USER_DS;
  ctxt->iret.ss     = (unsigned)USER_DS;
  ctxt->iret.esp    = esp;
  ctxt->iret.cs     = (unsigned)USER_CS;
  ctxt->iret.eip    = eip;
  ctxt->iret.eflags = INIT_USER_FLAGS;
}

//struct Context user;
struct Context user[2];
struct Context* Current;

void kernel() {
  struct BootData* bd = (struct BootData*)0x1000;
  unsigned* hdrs      = bd->headers;
  unsigned* mmap      = bd->mmap;
  unsigned  i;

  setAttr(0x2e);
  cls();
  setWindow(1, 23, 1, 45);   // kernel on left hand side
  setAttr(7);
  cls();
  printf("Protected kernel has booted!\n");

  printf("Headers:\n");
  for (i=0; i<hdrs[0]; i++) {
    printf(" header[%d]: [%x-%x], entry %x\n",
           i, hdrs[3*i+1], hdrs[3*i+2], hdrs[3*i+3]);
  }

  printf("Memory map:\n");
  for (i=0; i<mmap[0]; i++) {
    printf(" mmap[%d]: [%x-%x]\n",
           i, mmap[2*i+1], mmap[2*i+2]);
  }

  printf("Strings:\n");
  printf(" cmdline: %s [%x]\n", bd->cmdline, bd->cmdline);
  printf(" imgline: %s [%x]\n", bd->imgline, bd->imgline);

  printf("user code is at 0x%x\n", hdrs[9]);
  
  initContext(user, hdrs[9], 0);
  initContext((user+1), hdrs[12], 0);
  Current = user;
  printf("user is at %x\n", (unsigned)(Current)); 

  initPIC();
  startTimer();

  switchToUser(Current);
  printf("This message shouldn't appear!\n");
}

void kputc_imp() {  /* A trivial system call */
/*printf("We made it back into the kernel :-)\n");*/
/*printf("eax=%x, ebx=%x, retaddr=%x, esp=%x\n",
         user.regs.eax, user.regs.ebx, user.iret.eip, user.iret.esp);*/
  putchar(Current->regs.eax);
  switchToUser(Current);
}

void yield_imp()
{
  printf("Yielding ..");
  //Current = (Current==user) ? (user+1) : user;
  switchToUser(Current);
}

static void tick()
{
  static unsigned ticks = 0;
  ticks++;
  if((ticks&15) == 0)
    Current = (Current==user) ? (user+1) : user;
}

void timerInterrupt()
{
  maskAckIRQ(TIMERIRQ);
  enableIRQ(TIMERIRQ);
  tick();
  switchToUser(Current);
}
