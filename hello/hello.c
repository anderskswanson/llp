/* ------------------------------------------------------------------------
 * hello.c:  hello, kernel world
 *
 * Mark P. Jones, February 2008
 */

/*-------------------------------------------------------------------------
 * Video RAM:
 */

#define COLUMNS         80
#define LINES           25
#define ATTRIBUTE       12
#define VIDEO           0xB8000

typedef unsigned char single[2];
typedef single        row[COLUMNS];
typedef row           screen[LINES];


extern void clear();
int numdigits(unsigned int);
static screen* video = (screen*)VIDEO;

/*-------------------------------------------------------------------------
 * Cursor coordinates:
 */
static int xpos = 0;
static int ypos = 0;

/*-------------------------------------------------------------------------
 * Clear the screen.
 */
void cls(void) {
    int i, j;
    for (i=0; i<LINES; ++i) {
      for (j=0; j<COLUMNS; ++j) {
        (*video)[i][j][0] = ' ';
        (*video)[i][j][1] = ATTRIBUTE+j+i;
      }
    }
    ypos = 0;
    xpos = 0;
}

/*
 * Returns base 10 number as one digit of base 16
 */
char hexify_digit(unsigned int x)
{
    if(x < 10)
        return x + '0';
    if(x < 17)
	return x + 'A' - 10;
    return -1;    
}

void scroll_up()
{
  int i, j;
  for(i = 0; i < LINES - 1; ++i)
  {	  
     for(j = 0; j < COLUMNS; ++j)
     {
       (*video)[i][j][0] = (*video)[i+1][j][0];	     
       (*video)[i][j][1] = (*video)[i+1][j][1];
     }	     
  }

  ++i;
  for(j = 0; j < COLUMNS; ++j)
  {
    (*video)[i][j][0] = ' ';
    (*video)[i][j][1] = ATTRIBUTE;    
  }
}  

/*-------------------------------------------------------------------------
 * Output a single character.
 */
void putchar(char c) {
    int i, j;

    if (c!='\n' && c!='\r') {
        (*video)[ypos][xpos][0] = c & 0xFF;
        (*video)[ypos][xpos][1] = ATTRIBUTE;
        if (++xpos < COLUMNS) {
            return;
        }
    }

    xpos = 0;               // Perform a newline
    if (++ypos >= LINES) {  // scroll up top lines of screen ... 
        ypos = LINES-1;
        for (i=0; i<ypos; ++i) {
          for (j=0; j<COLUMNS; ++j) {
            (*video)[i][j][0] = (*video)[i+1][j][0];
            (*video)[i][j][1] = (*video)[i+1][j][1];
          }
        }
        for (j=0; j<COLUMNS; ++j) { // fill in new blank line
          (*video)[ypos][j][0] = ' ';
          (*video)[ypos][j][1] = ATTRIBUTE;
        }
    }
}

/*-------------------------------------------------------------------------
 * Output a zero-terminated string.
 */
void puts(char *msg) {
  while (*msg) {
    putchar(*msg++);
  }
}

/*-------------------------------------------------------------------------
 * Output an eight character string with the hexadecimal representation of 
 * argument n. E.g., 10 would be displayed as 0000000A and 1024 as 00000400
 */
void outhex(unsigned int num)	
{
  char hexes[9] = "";
  unsigned int temp = 0;
  int i = 0;
  
  while(num && i < 8)
  {
    temp = num & 0xf;
    hexes[i] = hexify_digit(temp);
    num >>= 4;
    ++i;    
  }

  if(i > 0) 
  {
    --i;	  
    for(; i > -1; --i)
      putchar(hexes[i]);	  
  }
}

/*
 * Convert an integer into a char array
 */
void outint(unsigned int num)
{
  int x;
  int digs = numdigits(num);  
  char output[digs + 1];
  output[digs] = '\0';

  while(num > 0 && digs > 0)
  {
    --digs;
    x = (num % 10) + '0';
    output[digs] = x;    
    num /= 10;	     
  }	  

  puts(output);
}

/*
 * count number of digits in an unsigned int
 * dumb but fast 
 */
int numdigits(unsigned int num)
{
  if(num < 10) return 1;
  if(num < 100) return 2;
  if(num < 1000) return 3;
  if(num < 10000) return 4;
  if(num < 100000) return 5;
  if(num < 1000000) return 6;
  if(num < 10000000) return 7;
  if(num < 100000000) return 8;
  if(num < 1000000000) return 9;
  return 10; //max digit count for 32 bit unsigned int
}
/*-------------------------------------------------------------------------
 * Main program:
 */
void hello() {
  int i;
  clear();

  for (i=0; i<2; i++) {
    puts("hhhh   hhhh\n");
    puts(" hh    hhh        lll lll\n");
    puts(" hh    hh   eeee  ll  ll   oooo\n");
    puts(" hhhhhhhh  ee  ee ll  ll  oo  oo\n");
    puts(" hh    hh eeeeeee ll  ll oo   oo\n");
    puts(" hh    hh  ee     ll  ll oo  oo\n");
    puts("hhh   hhhh  eeee  ll  ll  oooo\n");
    puts("\n");
    puts("    K e r n e l   W o r l d\n");
    puts("    October 13th 2017\n");
  }

  outhex(120);
  putchar('\n');
  outhex(123456789);
  putchar('\n');
  outint(987654321);
  scroll_up();
  scroll_up();
}
/* --------------------------------------------------------------------- */
