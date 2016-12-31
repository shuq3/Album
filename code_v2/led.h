#ifndef __LED_H__
#define __LED_H__

#include "s3c6410map.h"

#define led_init() do {\
	GPKCON0 &= ~0xffff0000;\
	GPKCON0 |= 0x11110000;\
	GPKDAT |= 0xf0;\
} while (0)

#define led_set(from0tof)	do {GPKDAT &= ~0xf0; GPKDAT |= ((~from0tof)&0xf) << 4;} while (0)

#define led_on() 		do {GPKDAT &= ~0xf0;} while (0)
#define led_off() 		do {GPKDAT |= 0xf0;} while (0)
#define led_flash() 	do {GPKDAT ^= 0xf0;} while (0)
//LED1 
#define led1_on()		do {GPKDAT &= ~0x10;} while (0)
#define led1_off()		do {GPKDAT |= 0x10;} while (0)
#define led1_flash()	do {GPKDAT ^= 0x10;} while (0)
//LED2
#define led2_on()		do {GPKDAT &= ~0x20;} while (0)
#define led2_off()		do {GPKDAT |= 0x20;} while (0)
#define led2_flash()	do {GPKDAT ^= 0x20;} while (0)
//LED3
#define led3_on()		do {GPKDAT &= ~0x40;} while (0)
#define led3_off()		do {GPKDAT |= 0x40;} while (0)
#define led3_flash()	do {GPKDAT ^= 0x40;} while (0)
//LED4
#define led4_on()		do {GPKDAT &= ~0x80;} while (0)
#define led4_off()		do {GPKDAT |= 0x80;} while (0)
#define led4_flash()	do {GPKDAT ^= 0x80;} while (0)

// #define led_on() 		do {GPKDAT |= 0xf0;} while (0)
// #define led_off() 		do {GPKDAT &= ~0xf0;} while (0)
// #define led_flash() 	do {GPKDAT ^= 0xf0;} while (0)
// //LED1 
// #define led1_on()		do {GPKDAT |= 0x10;} while (0)
// #define led1_off()		do {GPKDAT &= ~0x10;} while (0)
// #define led1_flash()	do {GPKDAT ^= 0x10;} while (0)
// //LED2
// #define led2_on()		do {GPKDAT |= 0x20;} while (0)
// #define led2_off()		do {GPKDAT &= ~0x20;} while (0)
// #define led2_flash()	do {GPKDAT ^= 0x20;} while (0)
// //LED3
// #define led3_on()		do {GPKDAT |= 0x40;} while (0)
// #define led3_off()		do {GPKDAT &= ~0x40;} while (0)
// #define led3_flash()	do {GPKDAT ^= 0x40;} while (0)
// //LED4
// #define led4_on()		do {GPKDAT |= 0x80;} while (0)
// #define led4_off()		do {GPKDAT &= ~0x80;} while (0)
// #define led4_flash()	do {GPKDAT ^= 0x80;} while (0)

// void led_init();
// void let_set(int n);
void led_set_ex(int n);
void led_hex_count_forever();
void led_cycle_forever();
void led_cycle_once();

#endif
