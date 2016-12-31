#include "irq.h"
#include "led.h"
#include "lcd.h"
#include "image.h"
#include "stdio.h"

void timer_enable()
{
	/* 在中断控制器里使能timer0中断 */
	VIC0INTENABLE |= (1<<23);
}

void timer_disable() {
	VIC0INTENABLE &= ~(1<<23);
}

void irq_init() { }


void do_irq()
{
	// timer0中断的中断处理函数
	{
		// led_cycle_once();
		static int i = 0;
		printf("timer interrupt occurred %d times\n", i++);
		next_image();
		lcd_clear_screen_lr(LCD_WHITE);
		image_show_lr();
		delay(100); // led计数（图片序号）显示延时

		unsigned long uTmp;
		//清timer0的中断状态寄存器
		uTmp = TINT_CSTAT;
		TINT_CSTAT = uTmp;
	}
}

// 初始化timer
void timer_init(unsigned long utimer,unsigned long uprescaler,unsigned long udivider,unsigned long utcntb,unsigned long utcmpb)
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
	TCON &= ~(1<<1);

	// 自动加载和启动timer0
	TCON |= (1<<0)|(1<<3);

	// 使能timer0中断
	temp0 = TINT_CSTAT;
	temp0 = (temp0 & (~(1<<utimer)))|(1<<(utimer));
	TINT_CSTAT = temp0;
}
