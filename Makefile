#----------------------------------------------------------------------------
include Makefile.common

.phony: all run libs clean

#BOOT = hello
#BOOT = bootinfo
#BOOT = example-mimg
BOOT = example-gdt
#BOOT = example-idt
#BOOT = switching

all:	libs
	make -C ${BOOT}

run:	libs
	make -C ${BOOT} run

libs:
	make -C simpleio
	make -C mimg

clean:
	make -C hello              clean
	make -C simpleio           clean
	make -C bootinfo           clean
	make -C mimg               clean
	make -C example-mimg       clean
	make -C example-gdt        clean
	make -C example-idt        clean
	make -C switching          clean

#----------------------------------------------------------------------------
