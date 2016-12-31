#ifndef __TIMER_H__
#define __TIMER_H__
#include "s3c6410map.h"

void irq_init();
void timer_enable();
void timer_disable();
void do_irq();
void timer_init(
	unsigned long utimer,
	unsigned long uprescaler,
	unsigned long udivider,
	unsigned long utcntb,
	unsigned long utcmpb
);


#endif
