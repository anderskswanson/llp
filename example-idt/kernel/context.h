/* Describes the stack frame that is created when we execute pusha.
 */
struct Registers {
  unsigned edi;
  unsigned esi;
  unsigned ebp;
  unsigned esp;
  unsigned ebx;
  unsigned edx;
  unsigned ecx;
  unsigned eax;
};

/* Describes a stack frame that holds values for user segment registers.
 */
struct Segments {
  unsigned ds;
  unsigned es;
  unsigned fs;
  unsigned gs;
};

/* Describes the stack frame that is created when an interrupt
 * occurs and there is a privilege level change (e.g., user to
 * kernel).
 */
struct Iret {
  unsigned error;   /* fake this for interrupts/exceptions that
                       don't generate an error code ... */
  unsigned eip;
  unsigned cs;
  unsigned eflags;
  unsigned esp;
  unsigned ss;
};

#define IOPL_SHIFT       12   /* shift for IO protection level in eflags */
#define IF_SHIFT          9   /* shift for interrupt flags in eflags     */
#define FLAGS_RESERVED  0x2   /* Intel reserved bits in eflags           */
/*#define INIT_USER_FLAGS (3<<IOPL_SHIFT | 1<<IF_SHIFT | FLAGS_RESERVED)*/
#define INIT_USER_FLAGS (3<<IOPL_SHIFT | FLAGS_RESERVED)

/* The complete frame that is created to capture the context of user
 * code when an interrupt occurs.
 */
struct Context {
  struct Registers regs;
  struct Segments  segs;
  struct Iret      iret;
};

/* Enter user code for a given context.
 */
extern int switchToUser(struct Context* ctxt);

