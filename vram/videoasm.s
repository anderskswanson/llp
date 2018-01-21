
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
	.globl	cls
cls:	pushl	%ebp
	movl	%esp, %ebp
        
        movl    $(DEFAULT_ATTR<<8 | SPACE), %eax
        movl    %eax, %edx
        shl     $16, %edx
        orl     %eax, %edx
        movl    $video, %eax
        movl    $1000, %ecx
L1:     movl    %edx, (%eax)
        add     $4, %eax
        decl    %ecx
        jnz     L1

        movl	%ebp, %esp
	popl	%ebp
	ret

	# Set the video attribute for characters output using outc.
	.globl	setAttr
setAttr:pushl	%ebp
	movl	%esp, %ebp

        movl    8(%ebp), %eax
        movb    %al, attr

	movl	%ebp, %esp
	popl	%ebp
	ret

	# Output a single character at the current row and col position
	# on screen, advancing the cursor coordinates and scrolling the
	# screen as appropriate.  Special treatment is provided for
        # NEWLINE characters, moving the "cursor" to the start of the
	# "next line".
	.globl	outc
outc:	pushl	%ebp
	movl	%esp, %ebp

#calculate the number of bytes over we are
        movl    col, %eax
        imull   $2,  %eax        
        movl    row, %ebx
        imull   $80, %ebx
        add     %eax, %ebx

#move into video space and output character
        movl    $video, %eax
        add     %ebx, %eax #add to video addr
        add     $1, col 
        movl    8(%ebp), %ecx
#        cmpl    NEWLINE, %ecx
#        jnz     L6
#        add     $1, row
L6:     movl    %ecx, (%eax) #move arg into video

#if at 80th col, reset col count and increment row count
        cmpl    $80, col 
        jnz     L4
        add     $1, row
        movl    $0, col
#if at 25th row, reset col count and scroll
        cmpl    $25, row
        jnz     L4
        movl    $0, row

#shift existing characters up one line
        movl    $video+160, %eax
        movl    $video, %ecx
        movl    $960, %edx        
L2:     movl    (%eax), %ebx
        movl    %ebx, (%ecx) 
        add     $4, %eax
        add     $4, %ecx
        decl    %edx
        jnz     L2

#clear bottom row
        movl    $(DEFAULT_ATTR<<8 | SPACE), %eax
        movl    %eax, %edx
        shl     $16, %edx
        orl     %eax, %edx
        movl    $40, %ebx
L3:     movl    %edx, (%ecx)
        add     $4, %ecx
        decl    %ebx
        jnz     L3

L4:	movl	%ebp, %esp
	popl	%ebp
	ret

	# Output an unsigned numeric value as a sequence of 8 hex digits.
	.globl	outhex
outhex:	pushl	%ebp
	movl	%esp, %ebp
	# Fill me in!
	movl	%ebp, %esp
	popl	%ebp
	ret

