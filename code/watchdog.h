#ifndef __WATCHDOG_H
#define __WATCHDOG_H

#define EINT0CON0  			(*((volatile unsigned long *)0x7F008900))
#define EINT0MASK  			(*((volatile unsigned long *)0x7F008920))
#define EINT0PEND  			(*((volatile unsigned long *)0x7F008924))
#define PRIORITY 	    	(*((volatile unsigned long *)0x7F008280))
#define SERVICE     		(*((volatile unsigned long *)0x7F008284))
#define SERVICEPEND 		(*((volatile unsigned long *)0x7F008288))
#define VIC0IRQSTATUS  		(*((volatile unsigned long *)0x71200000))
#define VIC0FIQSTATUS  		(*((volatile unsigned long *)0x71200004))
#define VIC0RAWINTR    		(*((volatile unsigned long *)0x71200008))
#define VIC0INTSELECT  		(*((volatile unsigned long *)0x7120000c))
#define VIC0INTENABLE  		(*((volatile unsigned long *)0x71200010))
#define VIC0INTENCLEAR 		(*((volatile unsigned long *)0x71200014))
#define VIC0PROTECTION 		(*((volatile unsigned long *)0x71200020))
#define VIC0SWPRIORITYMASK 	(*((volatile unsigned long *)0x71200024))
#define VIC0PRIORITYDAISY  	(*((volatile unsigned long *)0x71200028))
#define VIC0ADDRESS        	(*((volatile unsigned long *)0x71200f00))

#define WDT_BASE			(0x7E004000)
#define WTCON				( *((volatile unsigned long *)(WDT_BASE+0x00)) )
#define WTDAT				( *((volatile unsigned long *)(WDT_BASE+0x04)) )
#define WTCNT				( *((volatile unsigned long *)(WDT_BASE+0x08)) )
#define WTCLRINT			( *((volatile unsigned long *)(WDT_BASE+0x0C)) )

void do_irq();
void irq_init(void);
void wtd_operate(unsigned long uenreset, unsigned long uenint, unsigned long uselectclk, unsigned long uenwtd, unsigned long uprescaler,unsigned long uwtdat, unsigned long uwtcnt);
