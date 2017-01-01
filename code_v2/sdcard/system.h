/*************************************************************************************************************
 * 文件名:	system.h
 * 功能:		S3C6410相关系统函数
 * 作者:		cp1300@139.com
 * 创建时间:	2012年3月4日11:25
 * 最后修改时间:2012年3月4日
 * 详细:		相关系统操作宏定义
*************************************************************************************************************/

#ifndef	_SYSTEM_H_
#define	_SYSTEM_H_ 

#include "sys_types.h"
#include "s3c6410_map.h"
#include "../include/stdio.h"

#define Debug printf	//调试支持

#define nop __nop()		//空指令延时一个系统时钟周期



//IO模式宏定义
#define	IO_IN_PUT		0	//输入模式
#define IO_OUT_PUT		1	//输出模式
#define IO_SF_MODE		2	//特殊模式,复用功能
#define IO_EXT_INT		7	//外部中断输入模式
#define IO_NO_UP		0	//禁止上拉,下拉
#define IO_DROP_DOWM	1	//下拉
#define IO_PULL_UP		2	//上拉

//外部中断触发模式定义
#define EXT_LowLevel	0	//低电平触发
#define EXT_HighLevel	1	//高电平触发
#define EXT_NegEdge		2	//下降沿触发
#define EXT_PosEdge		4	//上升沿触发
#define EXT_Edge		6	//边沿触发


//中断组0编号定义
//	 外部中断组0的IO	偏移+标示	中断组0中的编号
#define EINT0_GPN0		0			//0
#define EINT0_GPN1		0			//1
#define EINT0_GPN2		4			//2
#define EINT0_GPN3		4			//3
#define EINT0_GPN4		8			//4
#define EINT0_GPN5		8			//5
#define EINT0_GPN6		12			//6	
#define EINT0_GPN7		12			//7
#define EINT0_GPN8		16			//8
#define EINT0_GPN9		16			//9
#define EINT0_GPN10		20			//10
#define EINT0_GPN11		20			//11
#define EINT0_GPN12		24			//12
#define EINT0_GPN13		24			//13
#define EINT0_GPN14		28			//14
#define EINT0_GPN15		28			//15
#define EINT0_GPL8		(0x80 + 0)	//16
#define EINT0_GPL9		(0x80 + 0)	//17
#define EINT0_GPL10		(0x80 + 4)	//18
#define EINT0_GPL11		(0x80 + 4)	//19
#define EINT0_GPL12		(0x80 + 8)	//20
#define EINT0_GPL13		(0x80 + 8)	//21
#define EINT0_GPL14		(0x80 + 12)	//22
#define EINT0_GPM0		(0x80 + 12)	//23
#define EINT0_GPM1		(0x80 + 16)	//24
#define EINT0_GPM2		(0x80 + 16)	//25
#define EINT0_GPM3		(0x80 + 20)	//26
#define EINT0_GPM4		(0x80 + 20)	//27




//外部中断分组定义
//		组名			//组号			//范围
#define EINT_Group0    	0				//GPN0--->GPN15        GPL8--->GPL14         GPM0--->GPM4
#define EINT_Group01    1				//GPA0--->GPA7         GPB0--->GPB6
#define EINT_Group02    1				//GPC0--->GPC7
#define EINT_Group03    2				//GPD0--->GPD5
#define EINT_Group04    2				//GPF0--->GPF14
#define EINT_Group05   	3				//GPG0--->GPG7
#define EINT_Group06    3				//GPH0--->GPH9
#define EINT_Group07    4				//GPO0--->GPO15
#define EINT_Group08    4				//GPP0--->GPP14
#define EINT_Group09    5				//GPQ0--->GPQ9


//HCLK门控时钟定义
/*HCLK_GATE控制所有Ips的HCLK，如果区域为‘1’，则HCLK被提供，否则，HCLK被屏蔽。当S3C6410
转换成掉电模式时，系统控制器检查一些模块（IROM，MEM0，MEM1和MFC模块）的状态。因此，位25，22，
21，0必须为‘1’，以符合掉电的要求。
		HCLK_GATE 		位 		描述 																初始状态*/
#define HCLK_UHOST		29		//为UHOST 选通HCLK(0：屏蔽，1：通过)。 								1
#define HCLK_SECUR		28		//为安全子系统选通HCLK(0：屏蔽，1：通过)。 							1
#define HCLK_SDMA1		27		//为SDMA1 选通HCLK(0：屏蔽，1：通过)。 								1
#define HCLK_SDMA0		26		//为SDMA0 选通HCLK(0：屏蔽，1：通过)。 								1
#define HCLK_IROM		25		//为IROM 选通HCLK(0：屏蔽，1：通过)。 								1
#define HCLK_DDR1		24		//为DDR1 选通HCLK(0：屏蔽，1：通过)。 								1
#define HCLK_DDR0		23		//为DDR0 选通HCLK(0：屏蔽，1：通过)。 								1
#define HCLK_MEM1		22		//为DMC1 选通HCLK(0：屏蔽，1：通过)。 								1
#define HCLK_MEM0		21		//为DMC0，SROM，OneNAND，NFCON 和CFCON 选通HCLK(0：屏蔽，1：通过)。	1
#define HCLK_USB		20		//为USB OTG 选通HCLK(0：屏蔽，1：通过)。 							1
#define HCLK_HSMMC2		19		//为HSMMC2 选通HCLK(0：屏蔽，1：通过)。 							1
#define HCLK_HSMMC1		18		//为HSMMC1 选通HCLK(0：屏蔽，1：通过)。 							1
#define HCLK_HSMMC0		17		//为HSMMC0 选通HCLK(0：屏蔽，1：通过)。 							1
#define HCLK_MDP		16		//为MDP 选通HCLK(0：屏蔽，1：通过)。 								1
#define HCLK_DHOST		15		//为直接HOST 接口选通HCLK(0：屏蔽，1：通过)。 						1
#define HCLK_IHOST		14		//为间接HOST 接口选通HCLK(0：屏蔽，1：通过)。 						1
#define HCLK_DMA1		13		//为DMA1 选通HCLK(0：屏蔽，1：通过)。 								1
#define HCLK_DMA0		12		//为DMA0 选通HCLK(0：屏蔽，1：通过)。 								1
#define HCLK_JPEG		11		//为JPEG 选通HCLK(0：屏蔽，1：通过)。 								1
#define HCLK_CAMIF		10		//为相机接口选通HCLK(0：屏蔽，1：通过)。 							1
#define HCLK_SCALER		9		//为定标器选通HCLK(0：屏蔽，1：通过)。 								1
#define HCLK_2D			8		//为2D 选通HCLK(0：屏蔽，1：通过)。 								1
#define HCLK_TV			7		//为TV 译码器选通HCLK(0：屏蔽，1：通过)。 							1
#define HCLK_POST0		5		//为POST0 选通HCLK(0：屏蔽，1：通过)。 								1
#define HCLK_ROT		4		//为旋转器选通HCLK(0：屏蔽，1：通过)。 								1
#define HCLK_LCD		3		//为LCD 控制器选通HCLK(0：屏蔽，1：通过)。 							1
#define HCLK_TZIC		2		//为中断控制器选通HCLK(0：屏蔽，1：通过)。 							1
#define HCLK_INTC		1		//为向量中断控制器选通HCLK(0：屏蔽，1：通过)。 						1
#define HCLK_MFC		0		//为MFC 选通HCLK(0：屏蔽，1：通过)。 								1


//PCLK门控时钟定义
//		PCLK_GATE 		位 				描述 											初始状态
#define PCLK_SKEY		(0x40 | 24)		// 为安全键选通PCLK(0：屏蔽，1：通过)。 		1
#define PCLK_CHIPID		(0x40 | 23)		// 为片上ID 选通PCLK(0：屏蔽，1：通过)。 		1
#define PCLK_SPI1		(0x40 | 22)		// 为SPI1 选通PCLK(0：屏蔽，1：通过)。 			1
#define PCLK_SPI0		(0x40 | 21)		// 为SPI0 选通PCLK(0：屏蔽，1：通过)。 			1
#define PCLK_HSIRX		(0x40 | 20)		// 为HSI 接收器选通PCLK(0：屏蔽，1：通过)。 	1
#define PCLK_HSITX		(0x40 | 19)		// 为HIS 发送器选通PCLK(0：屏蔽，1：通过)。 	1
#define PCLK_GPIO		(0x40 | 18)		// 为GPIO 选通PCLK(0：屏蔽，1：通过)。 			1
#define PCLK_IIC		(0x40 | 17)		// 为IIC 选通PCLK(0：屏蔽，1：通过)。 			1
#define PCLK_IIS1		(0x40 | 16)		// 为IIS1 选通PCLK(0：屏蔽，1：通过)。 			1
#define PCLK_IIS0		(0x40 | 15)		// 为IIS0 选通PCLK(0：屏蔽，1：通过)。			1
#define PCLK_AC97		(0x40 | 14)		// 为AC97 选通PCLK(0：屏蔽，1：通过)。 			1
#define PCLK_TZPC		(0x40 | 13)		// 为TZPC 选通PCLK(0：屏蔽，1：通过)。 			1
#define PCLK_TSADC		(0x40 | 12)		// 为触摸屏ADC 选通PCLK(0：屏蔽，1：通过。 		1
#define PCLK_KEYPAD		(0x40 | 11)		// 为Key PAD 选通PCLK(0：屏蔽，1：通过)。		1
#define PCLK_IRDA		(0x40 | 10)		// 为IRDA 选通PCLK(0：屏蔽，1：通过)。 			1
#define PCLK_PCM1		(0x40 | 9)		// 为PCM1 选通PCLK(0：屏蔽，1：通过)。 			1
#define PCLK_PCM0		(0x40 | 8)		// 为PCM0 选通PCLK(0：屏蔽，1：通过)。 			1
#define PCLK_PWM		(0x40 | 7)		// 为PWM 选通PCLK(0：屏蔽，1：通过)。 			1
#define PCLK_RTC		(0x40 | 6)		// 为RTC 选通PCLK(0：屏蔽，1：通过)。 			1
#define PCLK_WDT		(0x40 | 5)		// 为看门狗定时器选通PCLK(0：屏蔽，1：通过)。 	1
#define PCLK_UART3		(0x40 | 4)		// 为UART3 选通PCLK(0：屏蔽，1：通过)。 		1
#define PCLK_UART2		(0x40 | 3)		// 为UART2 选通PCLK(0：屏蔽，1：通过)。 		1
#define PCLK_UART1		(0x40 | 2)		// 为UART1 选通PCLK(0：屏蔽，1：通过)。 		1
#define PCLK_UART0		(0x40 | 1)		// 为UART0 选通PCLK(0：屏蔽，1：通过)。 		1
#define PCLK_MFC		(0x40 | 0)		// 为MFC 选通PCLK(0：屏蔽，1：通过)。 			1


//PCLK门控时钟定义
//PCLK_GATE 位 描述 初始状态
#define SCLK_UHOST		(0x80 | 30) 	// 为USB-HOST 选通特殊时钟 (0：屏蔽，1：通过)。 1
#define SCLK_MMC2_48	(0x80 | 29)		// 为MMC2 选通特殊时钟 (0：屏蔽，1：通过)。 1
#define SCLK_MMC1_48	(0x80 | 28)		// 为MMC1 选通特殊时钟 (0：屏蔽，1：通过)。 1
#define SCLK_MMC0_48	(0x80 | 27)		// 为MMC0 选通特殊时钟 (0：屏蔽，1：通过)。 1
#define SCLK_MMC2		(0x80 | 26)		// 为MMC2 选通特殊时钟 (0：屏蔽，1：通过)。 1
#define SCLK_MMC1		(0x80 | 25)		// 为MMC1 选通特殊时钟 (0：屏蔽，1：通过)。 1
#define SCLK_MMC0		(0x80 | 24)		// 为MMC0 选通特殊时钟 (0：屏蔽，1：通过)。 1
#define SCLK_SPI1_48	(0x80 | 23)		// 为SPI 选通特殊时钟 (0：屏蔽，1：通过)。 1
#define SCLK_SPI0_48	(0x80 | 22)		// 为SPI 选通特殊时钟 (0：屏蔽，1：通过)。 1
#define SCLK_SPI1		(0x80 | 21)		// 为SPI 选通特殊时钟 (0：屏蔽，1：通过)。 1
#define SCLK_SPI0		(0x80 | 20)		// 为SPI 选通特殊时钟 (0：屏蔽，1：通过)。 1
#define SCLK_DAC27		(0x80 | 19)		// 为DAC 选通特殊时钟 (0：屏蔽，1：通过)。 1
#define SCLK_TV27		(0x80 | 18)		// 为TV 译码器选通特殊时钟 (0：屏蔽，1：通过)。 1
#define SCLK_SCALER27	(0x80 | 17)		// 为scaler27 选通特殊时钟 (0：屏蔽，1：通过)。 1
#define SCLK_SCALER		(0x80 | 16)		// 为定标器选通特殊时钟 (0：屏蔽，1：通过)。 1
#define SCLK_LCD27		(0x80 | 15)		// 为LCD 控制器选通特殊时钟 (0：屏蔽，1：通过)。 1
#define SCLK_LCD		(0x80 | 14)		// 为LCD 控制器选通特殊时钟 (0：屏蔽，1：通过)。 1
#define SCLK_POST0_27	(0x80 | 12)		// 为POST0 选通特殊时钟 (0：屏蔽，1：通过)。 1
#define SCLK_POST0		(0x80 | 10)		// 为POST0 选通特殊时钟 (0：屏蔽，1：通过)。 1
#define SCLK_AUDIO1		(0x80 | 9)		// 为PCM1，IIS1 和AC97 1 选通特殊时钟 (0：屏蔽，1：通过)。1
#define SCLK_AUDIO0		(0x80 | 8)		// 为PCM0，IIS0 和AC97 0 选通特殊时钟 (0：屏蔽，1：通过)。1
#define SCLK_SECUR		(0x80 | 7)		// 为安全模块选通特殊时钟 (0：屏蔽，1：通过)。 1
#define SCLK_IRDA		(0x80 | 6)		// 为IRDA 选通特殊时钟 (0：屏蔽，1：通过)。 1
#define SCLK_UART		(0x80 | 5)		// 为UART0～3 选通特殊时钟 (0：屏蔽，1：通过)。 1
#define SCLK_OneNAND	(0x80 | 4)		// 为OneNAND 选通特殊时钟 (0：屏蔽，1：通过)。 1
#define SCLK_MFC		(0x80 | 3)		// 为MFC 选通特殊时钟 (0：屏蔽，1：通过)。 1
#define SCLK_CAM		(0x80 | 2)		// 为相机接口选通特殊时钟 (0：屏蔽，1：通过)。 1
#define SCLK_JPEG		(0x80 | 1)		// 为JPEG 选通特殊时钟 (0：屏蔽，1：通过)。 1

//中断源编号定义
/*		S3C6410X 支持64 位中断源，不支持ARM1176HZF-S 镜像中断运行.

		 		中断源 				中断号	描述 									组*/
#define 		INT_ADC 			63		//ADC EOC 中断 							VIC1
#define 		INT_PENDNUP 		62		//ADC 笔向下/向上中断 中断 				VIC1
#define 		INT_SEC 			61		//安全中断 								VIC1
#define 		INT_RTC_ALARM 		60		//RTC 警告中断 							VIC1
#define 		INT_IrDA 			59		//IrDA 中断 							VIC1
#define 		INT_OTG 			58		//USB OTG 中断 							VIC1
#define 		INT_HSMMC1 			57		//HSMMC1 中断 							VIC1
#define 		INT_HSMMC0 			56		//HSMMC0 中断 							VIC1
#define 		INT_HOSTIF 			55		//主机接口中断 							VIC1
#define 		INT_MSM 			54		//MSM 调制解调器 I/F 中断 				VIC1
#define 		INT_EINT4 			53		//外部中断组1~组9 						VIC1
#define 		INT_HSIrx 			52		//HS Rx 中断 							VIC1
#define 		INT_HSItx 			51		//HS Tx 中断 							VIC1
#define 		INT_I2C0 			50		//I2C 0 中断 							VIC1
#define 		INT_SPI_INT_HSMMC2 	49		//SPI 中断或HSMMC2 中断 				VIC1
#define 		INT_SPI0 			48		//SPI0 中断 							VIC1
#define 		INT_UHOST 			47		//USB 主机中断 							VIC1
#define 		INT_CFC 			46		//CFCON 中断 							VIC1
#define	 		INT_NFC 			45		//NFCON 中断 							VIC1
#define 		INT_ONENAND1 		44		//板块1 的ONENANE 中断 					VIC1
#define 		INT_ONENAND0 		43		//板块0 的ONENAND 中断 					VIC1
#define 		INT_DMA1 			42		//DMA1 中断 							VIC1
#define 		INT_DMA0 			41		//DMA0 中断 							VIC1
#define 		INT_UART3 			40		//UART3 中断 							VIC1
#define 		INT_UART2 			39		//UART2 中断 							VIC1
#define 		INT_UART1 			38		//UART1 中断 							VIC1
#define 		INT_UART0 			37		//UART0 中断 							VIC1
#define 		INT_AC97 			36		//AC 中断 								VIC1
#define 		INT_PCM1 			35		//PCM1 中断 							VIC1
#define 		INT_PCM0 			34		//PCM0 中断 							VIC1
#define 		INT_EINT3 			33		//外部中断20~27 						VIC1
#define 		INT_EINT2 			32		//外部中断12~19 						VIC1
#define 		INT_LCD_2 			31		//LCD 中断.系统I/F 完成 				VIC0
#define 		INT_LCD_1 			30		//LCD 中断.VSYNC 中断 					VIC0
#define 		INT_LCD_0 			29		//LCD 中断.FIFO 不足 					VIC0
#define 		INT_TIMER4 			28		//定时器4 中断. 						VIC0
#define 		INT_TIMER3 			27		//定时器3 中断. 						VIC0
#define 		INT_WDT 			26		//看门狗定时器中断. 					VIC0
#define 		INT_TIMER2 			25		//定时器2 中断. 						VIC0
#define 		INT_TIMER1 			24		//定时器1 中断. 						VIC0
#define 		INT_TIMER0 			23		//定时器0 中断. 						VIC0
#define 		INT_KEYPAD 			22		//键盘中断. 							VIC0
#define 		INT_ARM_DMAS 		21		//ARM DMAS 中断. 						VIC0
#define 		INT_ARM_DMA 		20		//ARM DMA 中断. 						VIC0
#define 		INT_ARM_DMAERR 		19		//ARM DMA 错误中断. 					VIC0
#define 		INT_SDMA1 			18		//安全 DMA1 中断. 						VIC0
#define 		INT_SDMA0 			17		//安全 DMA0 中断. 						VIC0
#define 		INT_MFC 			16		//MFC 中断. 							VIC0
#define 		INT_JPEG 			15		//JPEG 中断. 							VIC0
#define 		INT_BATF 			14		//电池故障中断. 						VIC0
#define 		INT_SCALER 			13		//TV 转换器中断. 						VIC0
#define 		INT_TVENC 			12		//TV 编码器中断. 						VIC0
#define 		INT_2D 				11		//2D 中断. 								VIC0
#define 		INT_ROTATOR 		10		//旋转器中断. 							VIC0
#define 		INT_POSTO 			9		//后处理器中断. 						VIC0
#define 		INT_3D 				8		//3D 图像控制器中断. 					VIC0
//#define 		Reserved 			7		//保留 									VIC0
#define 		INT_I2S				6		//I2S0/I2S1/INT_I2SV40/I2SV40中断		VIC0				
#define 		INT_I2C1 			5		//I2C1 中断			 					VIC0
#define 		INT_CAMIF_P 		4		//照相机接口中断 						VIC0
#define 		INT_CAMIF_C 		3		//照相机接口中断 						VIC0
#define 		INT_RTC_TIC 		2		//RTC TIC 中断 							VIC0
#define 		INT_EINT1 			1		//外部中断4~11 							VIC0
#define 		INT_EINT0 			0		//外部中断0~3 							VIC0	




/*************************************************************************************************/
/*			对应位声明，方便位操作		*/
#define BIT0	(0x0001 << 0)
#define BIT1	(0x0001 << 1)
#define BIT2	(0x0001 << 2)
#define BIT3	(0x0001 << 3)
#define BIT4	(0x0001 << 4)
#define BIT5	(0x0001 << 5)
#define BIT6	(0x0001 << 6)
#define BIT7	(0x0001 << 7)
#define BIT8	(0x0001 << 8)
#define BIT9	(0x0001 << 9)
#define BIT10	(0x0001 << 10)
#define BIT11	(0x0001 << 11)
#define BIT12	(0x0001 << 12)
#define BIT13	(0x0001 << 13)
#define BIT14	(0x0001 << 14)
#define BIT15	(0x0001 << 15)
#define BIT16	(0x00000001 << 16)
#define BIT17	(0x00000001 << 17)
#define BIT18	(0x00000001 << 18)
#define BIT19	(0x00000001 << 19)
#define BIT20	(0x00000001 << 20)
#define BIT21	(0x00000001 << 21)
#define BIT22	(0x00000001 << 22)
#define BIT23	(0x00000001 << 23)
#define BIT24	(0x00000001 << 24)
#define BIT25	(0x00000001 << 25)
#define BIT26	(0x00000001 << 26)
#define BIT27	(0x00000001 << 27)
#define BIT28	(0x00000001 << 28)
#define BIT29	(0x00000001 << 29)
#define BIT30	(0x00000001 << 30)
#define BIT31	(0x00000001 << 31)

//PLL选择
#define	APLL 	0	//ARM内核时钟PLL
#define MPLL 	1	//主时钟PLL
#define EPLL 	2	//外设时钟PLL

//主时钟输入
#define SYSTEM_MAIN_CLK_IN	12000000		//12MHZ

/*************************************************************************************************************************
*函数        :	__inline void VICInterruptEnd(void)
*功能        :  在中断快要结束时清除中断
*参数        :  无
*返回        :  无
*依赖        :	底层宏定义
*作者        :  陈鹏
*时间        :	20120305
*最后修改时间: 	20120305
*说明        :  写入任何数据清除中断,只有在中断服务程序中才可读,结束时才写
				两个要一起清除,否则可能导致无法再次进入中断,无意间发现的
*************************************************************************************************************************/
// __inline void VICInterruptEnd(void)
// {
// 	VIC0->ADDRESS = 0xffffffff;	//写入任何值都可以清除当前中断
// 	VIC1->ADDRESS = 0xffffffff;	//写入任何值都可以清除当前中断
// }


//通道选择
#define ch0		0
#define ch1		1
#define ch2		2

void Set_IntEnable(vu8 INT_N,FunctionalState EnInt);
u8 Get_IntEnable(vu8 INT_N);


// //使能printf输出
// //0:关闭printf输出;1:使能printf到串口;2:使能printf到液晶;3:同时使能printf到串口和液晶
// #define PRINTF_EN_		0
// //printf输出定义
// #if (PRINTF_EN_ == 1)	//使能到串口
// #define uart_printf(format,...)	(printf(format, ##__VA_ARGS__))	//串口打印
// #define DEBUG(format,...) 		(printf("<DebugFile: "__FILE__", Line: %d> "format, __LINE__, ##__VA_ARGS__))	//DEBUG输出
// #endif


// //printf输出定义
// #if (PRINTF_EN_ == 2)	//使能到液晶
// #define lcd_printf(format,...)	(printf(format, ##__VA_ARGS__))	//LCD打印
// #define DEBUG(format,...) 		(printf("<DebugFile: "__FILE__", Line: %d> "format, __LINE__, ##__VA_ARGS__))	//DEBUG输出
// #endif


// //printf输出定义
// #if (PRINTF_EN_ == 3)	//同时使能到液晶和串口
// #define uart_printf(format,...)	PrintfSet=0;printf(format, ##__VA_ARGS__)	//串口打印
// #define lcd_printf(format,...)	PrintfSet=1;printf(format, ##__VA_ARGS__)	//LCD打印
// #define DEBUG(format,...)		PrintfSet=0;printf("<DebugFile: "__FILE__", Line: %d> "format, __LINE__, ##__VA_ARGS__)	//DEBUG输出
// #endif

#endif

