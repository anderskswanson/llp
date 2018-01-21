/* A simple program that we will run in user mode.
 */
#include "simpleio.h"

extern void kputc(unsigned);
extern void yield(void);
volatile unsigned flag = 0;

void kputs(char* s) {
  while (*s) {
    kputc(*s++);
  }
}


void cmain() {
  int i;

  setWindow( 1, 11, 47, 32);   // user process on right hand side
  cls();
  puts("in user code\n");
  for (i=0; i<4; i++) {
    kputs("hello, kernel1 console\n");
    puts("hello, user1 console\n");
    //yield();
  }

  printf("My flag is at 0x%x\n", &flag);
  while(flag == 0)
  {
	  //nothing to see here
  }

  printf("Someone set the user1 flag to %d\n!!!", flag);

  puts("\n\nUser code does not return\n");
  for (;;) { /* Don't return! */
  }
  puts("This message won't appear!\n");
}
