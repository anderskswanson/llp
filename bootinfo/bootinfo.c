/* ------------------------------------------------------------------------
 * bootinfo.c:  display basic boot information from GRUB
 *
 * Mark P. Jones, April 2016
 */

#include "simpleio.h"

/* ------------------------------------------------------------------------
 * Multiboot data structures:
 */
extern struct MultibootInfo* mbi;
extern unsigned              mbi_magic;
#define MBI_MAGIC 0x2BADB002

struct MultibootInfo {
  unsigned                flags;
# define MBI_MEM_VALID    (1 << 0)
# define MBI_CMD_VALID    (1 << 2)
# define MBI_MODS_VALID   (1 << 3)
# define MBI_MMAP_VALID   (1 << 6)

  unsigned                memLower;
  unsigned                memUpper;
  unsigned                bootDevice;
  char*                   cmdline;
  unsigned                modsCount;
  struct MultibootModule* modsAddr;
  unsigned                syms[4];
  unsigned                mmapLength;
  unsigned                mmapAddr;
};

struct MultibootModule {
  unsigned modStart;
  unsigned modEnd;
  char*    modString;
  unsigned reserved;
};

struct MultibootMMap {
  unsigned size;
  unsigned baseLo;
  unsigned baseHi;
  unsigned lenLo;
  unsigned lenHi;
  unsigned type;
};

/*-------------------------------------------------------------------------
 * Main program:
 */
void bootinfo() {
  cls();
  puts("bootinfo is running!\n");
  printf("mbi_magic : 0x%x\n", mbi_magic);
  printf("mbi       : 0x%x\n", mbi);
  printf("->flags   : 0x%x\n", mbi->flags);
  printf("MEM_VALID : 0x%x\n", (mbi->flags)&MBI_MEM_VALID);
  printf("CMD_VALID : 0x%x\n", (mbi->flags)&MBI_CMD_VALID);
  printf("MODS_VALID: 0x%x\n", (mbi->flags)&MBI_MODS_VALID);
  printf("MMAP_VALID: 0x%x\n", (mbi->flags)&MBI_MMAP_VALID);
  printf("->memLower: %d\n", mbi->memLower);
  printf("->memUpper: %d\n", mbi->memUpper);
}

/* --------------------------------------------------------------------- */
