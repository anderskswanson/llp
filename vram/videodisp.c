/* videodisp: simulate the display of a simple video frame
 * on a terminal that has support for ASCII escape code
 * sequences.  Support for blinking text is intentionally
 * not implemented.
 */
#include <stdio.h>

extern char video[25][80][2];

static void horizontal() {
    int i;
    for (i=0; i<80; i++) {
        putchar('-');
    }
    putchar('\n');
}

// Uses ANSI escape code sequences to generate appropriate colors
// (See http://en.wikipedia.org/wiki/ANSI_escape_code#Colors)
void display() {
    static char colorMap[8] = { 0, 4, 2, 6, 1, 5, 3, 7 };
    int row, col;
    horizontal();
    for (row=0; row<25; row++) {
        int lastAttr = 127;
        for (col=0; col<80; col++) {
            if (video[row][col][1] != lastAttr) {
                lastAttr = video[row][col][1];
                printf("\x1b[%s;3%d;4%dm",
                       ((lastAttr & 8) ? "1" : "0"),
                       colorMap[lastAttr & 7],
                       colorMap[(lastAttr >> 4) & 7]);
            }
            int c = video[row][col][0] & 255;
            putchar(c<32 ? ' ' : c);
        }
        puts("\x1b[39;49;0m");
    }
    horizontal();
}

