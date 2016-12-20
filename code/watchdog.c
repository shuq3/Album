#include "stdio.h"
#include "Watchdog.h"

void irq_init(void)
{
  /* 在中断控制器里使能这些中断 */
  VIC0INTENABLE |= (1<<26);
}

void wtd_operate(unsigned long uenreset, unsigned long uenint, unsigned long uselectclk, unsigned long uenwtd, unsigned long uprescaler,unsigned long uwtdat, unsigned long uwtcnt)
{
  WTDAT = uwtdat; // 定时周期
  WTCNT = uwtcnt; // 定时器启动后，WTDAT的值会传入WTCNT

  /*uenreset: 是否使能reset
    *uenint: 是否使能中断
    *uselectclk: 分频系数
    *uenwtd:是否启动定时器
    *uprescaler:预分频系数
    */
  WTCON = (uenreset<<0)|(uenint<<2)|(uselectclk<<3)|(uenwtd<<5)|((uprescaler)<<8);
}

//中断处理函数
void do_irq()
{
  // 判断中断类型

  // 相关中断清除
  // WTCLRINT = 1;

  // 看门狗reset
  // wtd_operate(1,1,0,1,100,100000,100000);
}
