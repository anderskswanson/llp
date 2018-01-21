/* Compile using: gcc -o twiddletests twiddletests.c 
 *
 * Expected output:
 *
 *   pageStart(0x1234)      = 0x1000
 *   firstPageAfter(0x1234) = 0x2000
 *   endPageBefore(0x1234)  = 0xfff
 *   pageEnd(0x1234)        = 0x1fff
 *   pageStart(0x2000)      = 0x2000
 *   firstPageAfter(0x2000) = 0x2000
 *   endPageBefore(0x2000)  = 0x1fff
 *   pageEnd(0x2000)        = 0x2fff
 */
#include <stdio.h>
#include "memory.h"

void test(unsigned val) {
 //   printf("pageStart(0x%x)      = 0x%x\n", val, pageStart(val));
//    printf("firstPageAfter(0x%x) = 0x%x\n", val, firstPageAfter(val));
    printf("endPageBefore(0x%x)  = 0x%x\n", val, endPageBefore(val));
//    printf("pageEnd(0x%x)        = 0x%x\n", val, pageEnd(val));
}

int main(int argc, char** argv) {
    test(0x1fff);
    test(0x3000);
}

