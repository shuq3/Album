#ifndef __COMMON_H
#define __COMMON_H

#include "s3c6410map.h"
#include "stdio.h"
#include "led.h"
#include "lcd.h"
#include "irq.h"
#include "image.h"
#include "irq.h"

#define vi *( volatile unsigned int * ) 

#define set_zero( addr, bit ) ( (vi addr) &= ( ~ ( 1 << (bit) ) ) )
#define set_one( addr, bit ) ( (vi addr) |= ( 1 << ( bit ) ) )

#define set_bit( addr, bit, val ) ( (vi addr) = (( vi addr)&=(~(1<<(bit))) ) | ( (val)<<(bit) ) )

#define set_2bit( addr, bit, val ) ( (vi addr) = (( vi addr)&(~(3<<(bit))) ) | ( (val)<<(bit) ) )

#define set_nbit( addr, bit, len,  val ) \
	( (vi addr) = ((( vi addr)&(~(( ((1<<(len))-1) )<<(bit))))  | ( (val)<<(bit) ) ))

#define get_bit( addr, bit ) ( (( vi addr ) & ( 1 << (bit) )) > 0  )

#define get_val( addr, val ) ( (val) = vi addr )
#define read_val( addr ) ( vi ( addr ) )
#define set_val( addr, val ) ( (vi addr) = (val) )
#define or_val( addr, val ) ( (vi addr) |= (val) ) 

///////////////////////////////

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

// function declare

void delay(int n);
void enter_begining_menu();

typedef enum {
	STOP,
	START,
	AUTO,
	EDIT
} STATUS;

extern STATUS status;

int nandll_read_page(unsigned char *buf, unsigned long addr);
int nandll_write_page(unsigned char *buf, unsigned long addr);

#endif /* __COMMON_H */
