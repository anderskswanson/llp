/*-------------------------------------------------------------------------
 * capio.h: A version of the simpleio library using capabilities.
 * Mark P Jones, Portland State University
 *-----------------------------------------------------------------------*/
#ifndef CAPIO_H
#define CAPIO_H

// General operations that allow us to specify a window capability.
extern void capsetAttr(unsigned cap, int a);
extern void capcls(unsigned cap);
extern void capputchar(unsigned cap, int c);
extern void capputs(unsigned cap, char* s);
extern void capprintf(unsigned cap, const char *format, ...);

// By default, we assume that our window capability is in slot 2.
#define DEFAULT_WINDOW_CAP  2

#define setAttr(a)         capsetAttr(DEFAULT_WINDOW_CAP, a)
#define cls()              capcls(DEFAULT_WINDOW_CAP)
#define putchar(c)         capputchar(DEFAULT_WINDOW_CAP, c)
#define puts(s)            capputs(DEFAULT_WINDOW_CAP, s)
#define printf(args...)    capprintf(DEFAULT_WINDOW_CAP, args)

#endif
/*-----------------------------------------------------------------------*/
