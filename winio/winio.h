#ifndef WINIO_H
#define WINIO_H

// Set the base address of the video frame buffer:
extern void setVideo(unsigned);

// Data structure describing a single window:
struct Window {
    int top, bottom, left, right;   // Defines shape of window
    int xpos, ypos;                 // Current position
    int attr;                       // Default attribute
};

// A default console window:
extern struct Window console[];

// General operations on an arbitrary window:
extern void wsetWindow(struct Window* win, int t, int h, int l, int w);
extern void wsetAttr(struct Window* win, int a);
extern void wcls(struct Window* win);
extern void wputchar(struct Window* win, int c);
extern void wputs(struct Window* win, char* s);
extern void wprintf(struct Window* win, const char *format, ...);

// Operations on the console window:
#define setWindow(args...) wsetWindow(console, args)
#define setAttr(a)         wsetAttr(console, a)
#define cls()              wcls(console)
#define putchar(c)         wputchar(console, c)
#define puts(s)            wputs(console, s)
#define printf(args...)    wprintf(console, args)

#endif
