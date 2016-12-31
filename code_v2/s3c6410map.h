#ifndef __S3C6410MAP_H__
#define __S3C6410MAP_H__

//寄存器地址映射
//映射成为变量
#define reg32(addr)		(*((volatile unsigned long *)(addr)))	//直接映射
#define reg16(addr)		(*((volatile unsigned short *)(addr)))	//直接映射
#define reg8(addr)		(*((volatile unsigned char *)(addr)))	//直接映射
//映射成为指针
#define reg32_p(addr)	((volatile unsigned long *)(addr))
#define reg16_p(addr)	((volatile unsigned short *)(addr))
#define reg8_p(addr)	((volatile unsigned char *)(addr))


//GPIO控制寄存器
//		寄存器 			地址 					读/写 	描述 						复位值
#define GPACON		reg32(0x7F008000)		//			uart

#define GPECON		reg32(0x7F008080)		//			lcd display
#define GPEDAT		reg32(0x7F008084)
#define GPEPUD		reg32(0x7F008088)
#define GPFCON		reg32(0x7F0080A0)
#define GPFDAT		reg32(0x7F0080A4)
#define GPFPUD		reg32(0x7F0080A8)
#define GPICON		reg32(0x7F008100)
#define GPIPUD		reg32(0x7F008108)
#define GPJCON		reg32(0x7F008120)
#define GPJPUD		reg32(0x7F008128)


#define GPKCON0		reg32(0x7F008800)
#define GPKDAT		reg32(0x7F008808)

#define GPMCON		reg32(0x7F008820)		// 读/写 	端口M 配置寄存器 			0x00222222
#define GPMDAT		reg32(0x7F008824)		// 读/写 	端口M 数据寄存器 			未定义
#define GPMPUD		reg32(0x7F008828)		// 读/写 	端口M 上拉/下拉寄存器 		0x000002AA

// uart
#define ULCON0		reg32(0x7F005000)
#define UCON0		reg32(0x7F005004)
#define UFCON0		reg32(0x7F005008)
#define UMCON0		reg32(0x7F00500C)
#define UTRSTAT0	reg32(0x7F005010)
#define UFSTAT0		reg32(0x7F005018)
#define UTXH0		reg32(0x7F005020)
#define URXH0		reg32(0x7F005024)
#define UBRDIV0		reg32(0x7F005028)
#define UDIVSLOT0	reg32(0x7F00502C)

// display controller
#define MIFPCON			reg32(0x7410800C)
#define SPCON			reg32(0x7F0081A0)
#define VIDCON0			reg32(0x77100000)
#define VIDCON1			reg32(0x77100004)
#define VIDTCON0		reg32(0x77100010)
#define VIDTCON1		reg32(0x77100014)
#define VIDTCON2		reg32(0x77100018)
#define WINCON0			reg32(0x77100020)
#define VIDOSD0A		reg32(0x77100040)
#define VIDOSD0B		reg32(0x77100044)
#define VIDOSD0C		reg32(0x77100048)
#define VIDW00ADD0B0	reg32(0x771000A0)
#define VIDW00ADD1B0	reg32(0x771000D0)
#define VIDW00ADD2		reg32(0x77100100)
#define DITHMODE		reg32(0x77100170)

// irq
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

// timer
#define		PWMTIMER_BASE			(0x7F006000)
#define		TCFG0    	( *((volatile unsigned long *)(PWMTIMER_BASE+0x00)) )
#define		TCFG1    	( *((volatile unsigned long *)(PWMTIMER_BASE+0x04)) )
#define		TCON      	( *((volatile unsigned long *)(PWMTIMER_BASE+0x08)) )
#define		TCNTB0    	( *((volatile unsigned long *)(PWMTIMER_BASE+0x0C)) )
#define		TCMPB0    	( *((volatile unsigned long *)(PWMTIMER_BASE+0x10)) )
#define		TCNTO0    	( *((volatile unsigned long *)(PWMTIMER_BASE+0x14)) )
#define		TCNTB1    	( *((volatile unsigned long *)(PWMTIMER_BASE+0x18)) )
#define		TCMPB1    	( *((volatile unsigned long *)(PWMTIMER_BASE+0x1C)) )
#define		TCNTO1    	( *((volatile unsigned long *)(PWMTIMER_BASE+0x20)) )
#define		TCNTB2    	( *((volatile unsigned long *)(PWMTIMER_BASE+0x24)) )
#define		TCMPB2    	( *((volatile unsigned long *)(PWMTIMER_BASE+0x28)) )
#define		TCNTO2    	( *((volatile unsigned long *)(PWMTIMER_BASE+0x2C)) )
#define		TCNTB3    	( *((volatile unsigned long *)(PWMTIMER_BASE+0x30)) )
#define		TCMPB3    	( *((volatile unsigned long *)(PWMTIMER_BASE+0x34)) )
#define		TCNTO3    	( *((volatile unsigned long *)(PWMTIMER_BASE+0x38)) )
#define		TCNTB4    	( *((volatile unsigned long *)(PWMTIMER_BASE+0x3C)) )
#define		TCNTO4    	( *((volatile unsigned long *)(PWMTIMER_BASE+0x40)) )
#define		TINT_CSTAT 	( *((volatile unsigned long *)(PWMTIMER_BASE+0x44)) )


#endif
