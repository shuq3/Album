#include "common.h"

void timer_enable()
{
	/* 在中断控制器里使能timer0中断 */
	TCON &= ~(1<<1);
}

void timer_disable() {
	TCON |= 1<<1;
}

void irq_init() {
	/* 配置GPN0~5引脚为中断功能 */
	GPNCON &= ~(0xfff);
	GPNCON |= 0xaaa;
	/* 配置GPL11,GPL12引脚为中断功能 */
	GPLCON1 &= ~(0xff000);
	GPLCON1 |= 0x33000;

	/* 设置中断触发方式为: 下降沿触发 */
	EINT0CON1 &= ~(0xff0);
	EINT0CON1 |= 0x220;
	/* 设置中断触发方式为: 下降沿触发 */
	EINT0CON0 &= ~(0xfff);
	EINT0CON0 |= 0x222;

	EINT0FLTCON0 = (0xbf) | (0xbf<<8) | (0xbf<<16);
	EINT0FLTCON2 = (0xbf<<8) | (0xbf<<16);

	/* 禁止屏蔽中断 */
	EINT0MASK &= ~(0x18003f);

	/* 在中断控制器里使能这些中断 */
	// VIC0INTENABLE |= (1<<23);
	VIC0INTENABLE |= (0x800003); /* bit0: eint0~3, bit1: eint4~11 */
	VIC1INTENABLE |= (0x3); /* bit0: eint12~19,
							   bit1: eint20~27,
							   bit21:eint group1~9 */
}


void do_irq()
{
	if (EINT0PEND & (1<<0)) { // K1
		if (status == STOP) {
			status = START;
			lcd_clear_screen_lr(LCD_WHITE);
			image_show_lr();
		} else {
			timer_disable();
			status = STOP;
			enter_begining_menu();
		}
	} else if (EINT0PEND & (1<<1)) { // K2
		if (status == START) {
			status = AUTO;
			timer_enable();
		} else if (status == AUTO) {
			status = START;
			timer_disable();
		}
	} else if (EINT0PEND & (1<<2)) { // K3
		if (status != STOP) {
			if (status == EDIT) {
				image_move(0, 10);
				lcd_clear_screen_bu(LCD_WHITE);
				image_show();
			} else {
				next_image();
				lcd_clear_screen_lr(LCD_WHITE);
				image_show_lr();
			}
		}
	} else if (EINT0PEND & (1<<3)) { // K4
		if (status != STOP) {
			if (status == EDIT) {
				image_move(0, -10);
				lcd_clear_screen(LCD_WHITE);
				image_show();
			} else {
				previous_image();
				lcd_clear_screen_rl(LCD_WHITE);
				image_show_rl();
			}
		}
		
	} else if (EINT0PEND & (1<<4)) { // K5
		if (status == EDIT) {
			image_move(10, 0);
			lcd_clear_screen_lr(LCD_WHITE);
			image_show_lr();
		}
	} else if (EINT0PEND & (1<<5)) { // K6
		if (status == EDIT) {
			image_move(-10, 0);
			lcd_clear_screen_rl(LCD_WHITE);
			image_show_rl();
		}
	} else if (EINT0PEND & (1<<19)) { // K7
		if (status == START) {
			status = EDIT;
		} else if (status == EDIT) {
			status = START;
		}
		
	} else if (EINT0PEND & (1<<20)) { // K8
		
	} else { // timer0中断的中断处理函数
		// led_cycle_once();
		// static int i = 0;
		// printf("timer interrupt occurred %d times\n", i++);
		next_image();
		lcd_clear_screen_lr(LCD_WHITE);
		image_show_lr();
		delay(1000); // led计数（图片序号）显示延时

		unsigned long uTmp;
		//清timer0的中断状态寄存器
		uTmp = TINT_CSTAT;
		TINT_CSTAT = uTmp;
	}
	/* 清中断 */
	EINT0PEND   = 0x18003f;
	VIC0ADDRESS = 0;
	VIC1ADDRESS = 0;
}

// 初始化timer
void timer_init(
	unsigned long utimer,
	unsigned long uprescaler,
	unsigned long udivider,
	unsigned long utcntb,
	unsigned long utcmpb)
{
	unsigned long temp0;

	// 定时器的输入时钟 = PCLK / ( {prescaler value + 1} ) / {divider value} = PCLK/(65+1)/16=62500hz

	//设置预分频系数为66
	temp0 = TCFG0;
	temp0 = (temp0 & (~(0xff00ff))) | ((uprescaler-1)<<0);
	TCFG0 = temp0;

	// 16分频
	temp0 = TCFG1;
	temp0 = (temp0 & (~(0xf<<4*utimer))& (~(1<<20))) |(udivider<<4*utimer);
	TCFG1 = temp0;

	// 1s = 62500hz
	TCNTB0 = utcntb;
	TCMPB0 = utcmpb;

	// 手动更新
	TCON |= 1<<1;

	// 清手动更新位
	// TCON &= ~(1<<1);

	// 自动加载和启动timer0
	TCON |= (1<<0)|(1<<3);

	// 使能timer0中断
	temp0 = TINT_CSTAT;
	temp0 = (temp0 & (~(1<<utimer)))|(1<<(utimer));
	TINT_CSTAT = temp0;
}
