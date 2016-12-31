#include "led.h"

// void led_init() {
// 	GPKCON0 &= ~0xffff0000;
// 	GPKCON0 |= 0x11110000;
// 	GPKDAT |= 0xf0;
// }

// void led_set(int n) {
// 	GPKDAT &= ~0xf0;
// 	GPKDAT |= (~n & 0xf) << 4;
// 	GPKDAT = dat;
// }

void led_set_ex(int n) {
	int tmp = n >> 3;
	n &= ~0x8;
	n |= (n << 1) & 0x8;
	n &= ~0x4;
	n |= (tmp << 2) & 0x4;
	GPKDAT &= ~0xf0;
	GPKDAT |= (~n & 0xf) << 4;
}

void led_hex_count_forever() {
	int i = 0;
	while (1) {
		led_set_ex(i);
		i++;
		if (i == 16) i = 0;
		delay(0x200000);
	}
}

void led_cycle_forever() {
	led_off();
	led1_on();
	while (1) {
		delay(0x200000);
		led1_off();
		led2_on();
		delay(0x200000);
		led2_off();
		led4_on();
		delay(0x200000);
		led4_off();
		led3_on();
		delay(0x200000);
		led3_off();
		led1_on();
	}
}

void led_cycle_once() {
	led_off();
	led1_on();
	delay(0x10000);
	led1_off();
	led2_on();
	delay(0x10000);
	led2_off();
	led4_on();
	delay(0x10000);
	led4_off();
	led3_on();
	delay(0x10000);
	led3_off();
}
