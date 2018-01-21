#include "video.h"

int main(int argc, char** argv) {
    int i, j;
    cls();
    video[2][38][0] = 'h';  video[2][40][0] = 'i';  video[2][42][0] = '!';
    video[2][38][1] = 0x51; video[2][40][1] = 0x43; video[2][42][1] = 0x35;

    display();
    return 0;
}

