#include "s3c6410map.h"
#include "lcd.h"

// 功能:初始化串口
void init_uart(void)
{
	/* 1. 配置引脚 */
	GPACON &= ~0xff;
	GPACON |= 0x22;
	
	/* 2. 设置数据格式等 */
	ULCON0 = 0x3;  					// 数据位:8, 无校验, 停止位: 1, 8n1 
	UCON0  = 0x5;  					// 时钟：PCLK，禁止中断，使能UART发送、接收 
	UFCON0 = 0x01; 					// FIFO ENABLE
	UMCON0 = 0;						// 无流控
	
	/* 3. 设置波特率 */
	// DIV_VAL = (PCLK / (bps x 16 ) ) - 1 = (66500000/(115200x16))-1 = 35.08
	// DIV_VAL = 35.08 = UBRDIVn + (num of 1’s in UDIVSLOTn)/16 
	UBRDIV0   = 35;
	UDIVSLOT0 = 0x1;
	
}

/* 接收一个字符 */
char getc(void)
{
	while ((UFSTAT0 & 0x7f) == 0);  // 如果RX FIFO空，等待 
	return URXH0;                   // 取数据 
}

/* 发送一个字符 */
void putc(char c)
{
	while (UFSTAT0 & (1<<14)); 		// 如果TX FIFO满，等待 
	UTXH0 = c;                      // 写数据 
	lcd_draw_char(c);
}


