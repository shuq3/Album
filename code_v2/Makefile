CC      = arm-linux-gcc
LD      = arm-linux-ld
AR      = arm-linux-ar
OBJCOPY = arm-linux-objcopy
OBJDUMP = arm-linux-objdump

INCLUDEDIR 	:= $(shell pwd)/include
CFLAGS 		:= -Wall -Os -fno-builtin
CPPFLAGS   	:= -nostdinc -I$(INCLUDEDIR)

export 	CC AR LD OBJCOPY OBJDUMP INCLUDEDIR CFLAGS CPPFLAGS 

objs := start.o main.o uart.o clock.o irq.o lib/libc.a sdcard/sdcard.a\
		sdram.o led.o lcd.o nand.o font_8x8.o image.o

album.bin: $(objs)
	${LD} -T album.lds -o album.elf $^
	${OBJCOPY} -O binary -S album.elf $@
	${OBJDUMP} -D album.elf > album.dis

.PHONY : lib/libc.a
lib/libc.a:
	cd lib; make; cd ..
sdcard/sdcard.a:
	cd sdcard; make; cd ..

%.o:%.c
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

%.o:%.S
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

clean:
	make clean -C lib
	make clean -C sdcard
	rm -f *.bin *.elf *.dis *.o