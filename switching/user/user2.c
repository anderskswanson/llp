/* A simple program that we will run in user mode.
 */
#include "simpleio.h"

extern void kputc(unsigned);
extern void yield(void);

void kputs(char* s) {
  while (*s) {
    kputc(*s++);
  }
}


void cmain() {
  int i;

  setWindow(13, 11, 47, 32);   // user process on right hand side
  cls();
  puts("in user2 code\n");
  for (i=0; i<4; i++) {
    kputs("hello, kernel console from USER 2\n");
    puts("hello, user2 console\n");
    //yield();
  }

  unsigned* flagAddr = (unsigned*)0x402664;
  printf("flagAddr = 0x%x\n", flagAddr);
  *flagAddr = 1234;

  puts("\n\nUSER 2 code does not return\n");
  for (;;) { /* Don't return! */
  }
  puts("This message won't appear!\n");
}
