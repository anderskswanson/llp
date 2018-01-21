

	.equ	ROWS, 		25		# number of rows
	.equ	COLS, 		80		# number of columns

	.equ	CHARBYTES,	2		# total #bytes per char
	.equ	ROWBYTES,	COLS*CHARBYTES	# total #bytes per row
	.equ	SCREENBYTES,	ROWS*ROWBYTES	# total #bytes per screen

	.equ	SPACE,		32		# blank space
	.equ	NEWLINE,	'\n'		# newline character

	.equ	DEFAULT_ATTR, 	0x2e		# PSU Green

	.data

        # Reserve space for a video ram frame buffer with
        # 25 rows; 80 columns; and one code and one attribute
        # byte per character.
	.globl  video
	.align	4
video:	.space	SCREENBYTES

        # Some variables to hold the current row, column, and
	# video attribute:
	.align	4
row:	.long	0		# we will only use the least significant
col:	.long	0		# bytes of these variables
attr:	.long	0

	.text

	# Clear the screen, setting all characters to SPACE
        # using the DEFAULT_ATTR attribute.
	.globl	clear
clear:	pushl	%ebp
	movl	%esp, %ebp
        
        movl    $(DEFAULT_ATTR<<8 | SPACE), %eax
        movl    %eax, %edx
        shl     $16, %edx
        orl     %eax, %edx
        movl    $0xB8000, %eax
        movl    $1000, %ecx
L1:     movl    %edx, (%eax)
        add     $4, %eax
        decl    %ecx
        jnz     L1

        movl	%ebp, %esp
	popl	%ebp
        ret
