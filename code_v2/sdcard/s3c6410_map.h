/*************************************************************************************************************
 * 文件名:	s3c6410_map.h
 * 功能:		S3C6410相关寄存器宏定义
 * 作者:		陈鹏
 * 创建时间:	2012年3月4日11:22
 * 最后修改时间:2012年3月4日
 * 详细:		相关寄存器宏定义
*************************************************************************************************************/


#ifndef _S3C6410_MAP_H_
#define _S3C6410_MAP_H_

#include "sys_types.h"
 
//使能编译选项
//UART
#define _UART0
#define _UART1
#define _UART2
#define _UART3
//VIC
#define _VIC0
#define _VIC1
//MODEM
#define _MODEM
//NAND FLASH
#define _NAND
//RTC
#define _RTC
//ADC
#define _ADC
//SDIO
#define _SDIO0
#define _SDIO1
#define _SDIO2
//AC97
#define _AC97
//2D
#define _G2D
//SPI
#define _SPI0
#define _SPI1


//寄存器地址映射
//映射成为变量
#define	S3C6410_SFR32(add)	(*((volatile unsigned long int*)(add)))	//直接映射
#define	S3C6410_SFR16(add)	(*((volatile unsigned short*)(add)))	//直接映射
#define	S3C6410_SFR8(add)	(*((volatile unsigned char *)(add)))	//直接映射
//映射成为指针
#define S3C6410_SFR32_P(add)	((volatile unsigned long int*)(add))
#define S3C6410_SFR16_P(add)	((volatile unsigned short*)(add))
#define S3C6410_SFR8_P(add)		((volatile unsigned char*)(add))




//**********************************************************************************//
//外设寄存器结构定义

//VICx中断控制器
typedef struct
{	
/*      寄存器				补偿区	类型	描述	复位值*/
vu32	IRQSTATUS; 		//	0x000 	读 		IRQ 状态寄存器 0x00000000
vu32	FIQSTATUS; 		//	0x004 	读 		FIQ 状态寄存器 0x00000000
vu32	IRAWINTR; 		//	0x008 	读 		原始中断状态寄存器 0x00000000
vu32	INTSELECT; 		//	0x00C 	读写 	中断选择寄存器 0x00000000
vu32	INTENABLE; 		//	0x010 	读写 	中断使能寄存器 0x00000000
vu32	INTENCLEAR; 	//	0x014 	写 		中断使能清除寄存器 -
vu32	SOFTINT; 		//	0x018 	读写 	软件中断寄存器 0x00000000
vu32	SOFTINTCLEAR; 	//	0x01C 	写 		软件中断清除寄存器 -
vu32	PROTECTION; 	//	0x020 	读写 	保护使能寄存器 0x0
vu32	SWPRIORITYMASK; //	0x024 	读写 	软件优先屏蔽寄存器 0x0FFFF
vu32	PRIORITYDAISY;	//	0x028 	读写 	菊花链的矢量优先寄存器 0xF 
uc32	Reserved0[53];	//保留
vu32	VECTADDR[32];	//			读写	矢量地址寄存器   0x00000000
uc32	Reserved[32];	//保留
vu32	VECTRPRIORITY[32];//			读写	矢量优先级寄存器	0xf
uc32	Reserved2[800];	//保留
vu32	ADDRESS;		//			读写	当前矢量地址寄存器	0x00000000
}VICx_TypeDef;


//NAND FLASH控制器
typedef struct
{
	vu32	CONF;		//配置寄存器
	vu32	CONT;		//控制寄存器
	vu8		CMMD;		//命令寄存器
	u8		Reserved0[3];
	vu8		ADDR;		//地址寄存器
	u8		Reserved1[3];
	vu32	DATA;		//数据寄存器
	vu32	MECCD0;		//第一个和第二个主ECC 数据寄存器
	vu32	MECCD1;		//第三个和第四个主ECC 数据寄存器
	vu32	SECCD;		//备用ECC 读寄存器
	vu32	SBLK;		//可编程开始块地址寄存器
	vu32	EBLK;		//可编程结束块地址寄存器
	vu32	STAT;		//NAND 状态寄存器
	vuc32	ECCERR0;	//ECC 错误状态0 寄存器
	vuc32	ECCERR1;	//ECC 错误状态1 寄存器
	vuc32	MECC0;		//生成ECC 状态0 寄存器
	vuc32	MECC1;		//生成ECC 状态1 寄存器
	vuc32	SECC;		//生成备用区ECC 状态寄存器
	vuc32	MLCBITPT;	//4 位ECC 错误位模式寄存器
}NAND_TypeDef;


//RTC 实时时钟寄存器
typedef struct
{
	//寄存器 地址 读/写 描述 复位值
	vu32	INTP; 			//   0x7E005030 读/写 中断等候寄存器 0x0
	uc32	Reserved0[3];
	vu32	CON; 			//	 0x7E005040 读/写 实时时钟控制寄存器 0x0
	vu32	TICNT; 			//	 0x7E005044 读/写 标记时间计数寄存器 0x0
	uc32	Reserved1[2];
	vu32	ALM; 			//	 0x7E005050 读/写 实时时钟报警控制寄存器 0x0
	vu32	ALMSEC; 		//	 0x7E005054 读/写 报警秒数据寄存器 0x0
	vu32	ALMMIN; 		//	 0x7E005058 读/写 报警分钟数据寄存器 0x00
	vu32	ALMHOUR; 		//	 0x7E00505C 读/写 报警小时数据寄存器 0x0
	vu32	ALMDATE; 		//	 0x7E005060 读/写 报警天数据寄存器 0x01
	vu32	ALMMON; 		//	 0x7E005064 读/写 报警月数据寄存器 0x01
	vu32	ALMYEAR; 		//	 0x7E005068 读/写 报警年数据寄存器 0x0
	uc32	Reserved2;
	vu32	BCDSEC; 		//	 0x7E005070 读/写 BCD 秒寄存器 未定义
	vu32	BCDMIN; 		//	 0x7E005074 读/写 BCD 分钟寄存器 未定义
	vu32	BCDHOUR; 		//	 0x7E005078 读/写 BCD 小时寄存器 未定义
	vu32	BCDDATE; 		//	 0x7E00507C 读/写 BCD 日期寄存器 未定义
	vu32	BCDDAY; 		//	 0x7E005080 读/写 BCD 天寄存器 未定义
	vu32	BCDMON; 		//	 0x7E005084 读/写 BCD 月寄存器 未定义
	vu32	BCDYEAR; 		//	 0x7E005088 读/写 BCD 年寄存器。 未定义
	uc32	Reserved3;
	vu32	CURTICCNT; 		//	 0x7E005090 读 当前标记时间计数寄存器。 0x0
}RTC_TypeDef;


//ADC 寄存器
typedef struct
{
	vu32	CON;			//ADC控制寄存器 
	vu32	TSC;			//触摸屏控制寄存器
	vu32	DLY;			//ADC开始延迟寄存器
	vu32	DAT0;			//ADC数据寄存器0
	vu32	DAT1;			//ADC数据寄存器1
	vu32	UPDN;			//触摸屏UP-DOWN寄存器
	vu32	CLRINT;			//ADC中断清除寄存器
	u32		Reserved;
	vu32	CLRINTPNDNUP;	//触摸屏笔中断寄存器
}ADC_TypeDef;


//SDIO 寄存器
typedef struct
{
	vu32 	SDMASYSAD;		//0x7C200000	SDMA系统配置寄存器
	vu16 	BLKSIZE;		//0x7C200004	块大小配置寄存器
	vu16	BLKCNT;			//0x7C200006	块计数寄存器
	vu32	ARGUMENT;		//0x7C200008	命令索引寄存器
	vu16	TRNMOD;			//0x7C20000C	传输模式寄存器
	vu16	CMDREG;			//0x7C20000E	命令寄存器
	vu32	RSPREG[4];		//0x7C200010-0x7C20001C	应答寄存器
	vu32	BDATA;			//0x7C200020	缓冲区数据端口寄存器
	vu32	PRNSTS;			//0x7C200024	当前状态寄存器
	vu8		HOSTCTL;		//0x7C200028	主机控制寄存器
	vu8		PWRCON;			//0x7C200029	电源控制寄存器
	vu8		BLKGAP;			//0x7C20002A	块间隔控制寄存器
	vu8		WAKCON;			//0x7C20002B	唤醒控制寄存器
	vu16	CLKCON;			//0x7C20002C	时钟控制寄存器
	vu8		TIMEOUTCON;		//0x7C20002E	超时控制寄存器
	vu8		SWRST;			//0x7C20002F	软件复位寄存器
	
//	vu16	NORINTSTS;		//0x7C200030	正常中断状态寄存器
//	vu16	ERRINTSTS;		//0x7C200032	错误中断状态寄存器
	//将错误状态和正常状态寄存器合并,方便操作,高位为错误状态寄存器,低位为正常状态急促请你
	vu32	INTSTS;			//中断状态寄存器
	vu32	INTSTSEN;		//中断状态信号使能
	vu32	INTSEGEN;		//中断触发使能		
	
//	vu16	NORINTSTSEN;	//0x7C200034	正常中断状态使能寄存器
//	vu16	ERRINTSTSEN;	//0x7C200036	错误中断状态使能寄存器
//	vu16	NORINTSIGEN;	//0x7C200038	正常中断状态中断使能寄存器
//	vu16	ERRINTSIGEN;	//0x7C20003A	错误中断状态中断使能寄存器
	vu16	ACMD12ERRSTS;	//0x7C20003C	自动CMD12错误状态寄存器
	u16		Reserved0;
	vu32	CAPAREG;		//0x7C200040	电压容限寄存器
	u32		Reserved1;
	vu32	MAXCURR;		//0x7C200048	最大电流容限寄存器
	u32		Reserved2;
	vu16	FEAER;			//0x7C200050	FORCE EVENT REGISTER FOR AUTO CMD12 ERROR STATUS
	vu16	FEERR;			//0x7C200052	FORCE EVENT REGISTER FOR ERROR INTERRUPT STATUS
	vu32	ADMAERR;		//0x7C200054	ADMA错误状态寄存器
	vu32	ADMASYSADDR;	//0x7C200058	ADMA地址寄存器	
	u32		Reserved3[9];
	vu32	CONTROL2;		//0x7C200080	主机控制寄存器2
	vu32	CONTROL3;		//0x7C200084	主机控制寄存器3
	u32		Reserved4;
	vu32	CONTROL4;		//0x7C20008C	主机控制寄存器4
	u16		Reserved5[55];
	vu16	HCVER;			//0x7C2000FE	主机控制器版本寄存器
}SDIO_TypeDef;



//AC97控制器 寄存器
typedef struct
{
	vu32	GLBCTRL;	 	 // 0x7F001000 读/写 	AC97 通用控制寄存器。 					0x00000000
	vu32	GLBSTAT;	 	 // 0x7F001004 读 		AC97 全球状态寄存器。 					0x00000001
	vu32	CODEC_CMD;	 	 // 0x7F001008 读/写 	AC97 编解码器命令寄存器。 				0x00000000
	vu32	CODEC_STAT;	 	 // 0x7F00100C 读		AC97 编解码器状态寄存器。 				0x00000000
	vu32	PCMADDR;	 	 // 0x7F001010 读 		AC97 的PCM 输出/输入 通道FIFO 地址		0x00000000
	vu32	MICADDR;	 	 // 0x7F001014 读 		AC97 的MIC 输入通道FIFO 地址寄存器。	0x00000000
	vu32	PCMDATA;	 	 // 0x7F001018 读/写 	AC97 的PCM 输出/输入 通道FIFO 数据		0x00000000
	vu32	MICDATA;	 	 // 0x7F00101C 读/写 	AC97 的MIC 输入通道FIFO 数据寄存器。 	0x00000000
}AC97_TypeDef;


//SPI
typedef struct
{
	vu32	CHCFG;			//配置寄存器
	vu32	CLKCFG;			//时钟配置寄存器
	vu32	MODECFG;		//FIFO控制寄存器
	vu32	SLAVE;			//从属器选择寄存器
	vu32	INTEN;			//中断启动寄存器
	vu32	STATUS;			//状态寄存器
	vu32	TXDATA;			//发送数据寄存器
	vu32	RXDATA;			//接收数据寄存器
	vu32	CNT;			//计数，主控器收到多少数据
	vu32	CLR;			//状态清除
	vu32	SWAPCFG	;		//交换配置寄存器
	vu32	FBCLK;			//反馈时钟选择寄存器	
}SPI_TypeDef;



//2D 图形寄存器定义
typedef struct
{
//			寄存器 				地址映射 		读/写 	描述 						复位值
//通用寄存器
	vu32	CONTROL;		// 0x76100000	写		控制
	vu32	INTEN;			// 0x76100004 	读/写 	中断有效寄存器。 			0x0000_0000
	vu32	FIFO_INTC;		// 0x76100008 	读/写 	中断控制寄存器。 			0x0000_0018
	vu32	INTC_PEND;		// 0x7610000C 	读/写 	中断控制等待寄存器。 		0x0000_0000
	vu32	FIFO_STAT;		// 0x76100010 	读 		指令FIFO状态寄存器。		0x0000_0600
	vu32	FB_BASE;		// 0x76100020 	读/写 	帧缓冲器基址寄存器。 		0x0000_0000
	u32		Reserved1[10];
//指令寄存器
	vu32	CMD0;			// 0x76100100 	写 		用于线/点绘图的指令寄存器。 -
	vu32	CMD1;			// 0x76100104	写 		用于BitBLT的指令寄存器。 -
	vu32	CMD2;			// 0x76100108 	写 		用于主机到屏Bitblt传送开始点的指令寄存器。
	vu32	CMD3;			// 0x7610010C 	写	 	用于主机到屏Bitblt传送恢复的指令寄存器。
	vu32	CMD4;			// 0x76100110 	写 		用于彩色扩展的指令寄存器（主-机到屏，字型开始点）。
	vu32	CMD5;			// 0x76100114 	写 		用于彩色扩展的指令寄存器（主机到屏，字型恢复）。
	u32		Reserved2;
	vu32	CMD7;			// 0x7610011C 	写 		用于彩色扩展的指令寄存器（存储器到屏）。
//指令资源寄存器
	vu32	COLOR_MODE;		// 0x76100200 	读/写 	色彩模式寄存器。 			0x0000_0008
	vu32	HORI;			// 0x76100204 	读/写 	水平分辨率寄存器。 			0x0000_0000
	vu32	SC_WIN;			// 0x76100210 	读/写 	屏幕裁剪窗口寄存器。 		0x0000_0000
	vu32	SC_WIN_X;		// 0x76100214 	写 		屏幕裁剪窗口的最大X寄存器。 	0x0000_0000
	vu32	SC_WIN_Y;		// 0x76100218 	写 		屏幕裁剪窗口的最大Y寄存器。 	0x0000_0000
	vu32	CW_LT;			// 0x76100220 	读/写 	裁剪窗口左上方坐标。 		0x0000_0000
	vu32	CW_LT_X;		// 0x76100224 	写 		裁剪窗口左X坐标。 			0x0000_0000-
	vu32	CW_LT_Y;		// 0x76100228 	写 		裁剪窗口上Y坐标。 			0x0000_0000-
	vu32	CW_RB;			// 0x76100230 	读/写 	裁剪窗口右下方坐标。 		0x0000_0000
	vu32	CW_RB_X;		// 0x76100234 	写 		裁剪窗口右X坐标。 			0x0000_0000-
	vu32	CW_RB_Y;		// 0x76100238 	写 		裁剪窗口下Y坐标。 			0x0000_0000-
	vu32	COORD0;			// 0x76100300 	读/写 	坐标0寄存器。 				0x0000_0000
	vu32	COORD0_X;		// 0x76100304 	写 		坐标0的X坐标。 				0x0000_0000-
	vu32	COORD0_Y;		// 0x76100308 	写 		坐标0的Y坐标。 				0x0000_0000-
	vu32	COORD1;			// 0x76100310 	读/写 	坐标1寄存器。 				0x0000_0000
	vu32	COORD1_X;		// 0x76100314 	写 		坐标1的X坐标。 				0x0000_0000-
	vu32	COORD1_Y;		// 0x76100318 	写 		坐标1的Y坐标。 				0x0000_0000-
	vu32	COORD2;			// 0x76100320 	读/写 	坐标2寄存器。 				0x0000_0000
	vu32	COORD2_X;		// 0x76100324 	写 		坐标2的X坐标。 				0x0000_0000-
	vu32	COORD2_Y;		// 0x76100328 	写 		坐标2的Y坐标。 				0x0000_0000-
	vu32	COORD3;			// 0x76100330 	读/写 	坐标3寄存器。 				0x0000_0000
	vu32	COORD3_X;		// 0x76100334 	写 		坐标3的X坐标。 				0x0000_0000-
	vu32	COORD3_Y;		// 0x76100338 	写 		坐标3的Y坐标。 				0x0000_0000-
	vu32	ROT_OC;			// 0x76100340 	读/写 	旋转原点坐标。 				0x0000_0000
	vu32	ROT_OC_X;		// 0x76100344 	写 		旋转原点坐标的X坐标。 		0x0000_0000-
	vu32	ROT_OC_Y;		// 0x76100348 	写 		旋转原点坐标的Y坐标。 		0x0000_0000-
	vu32	ROTATE;			// 0x7610034C 	读/写 	旋转模式寄存器。 			0x0000_0001
	vu32	ENDIA_READSIZE;	// 0x76100350 	读/写 	大端或小端选择，读取的顺序。 	0x0000_0001
	vu32	X_INCR;			// 0x76100400 	读/写 	X增量寄存器。 				0x0000_0000
	vu32	Y_INCR;			// 0x76100404 	读/写 	Y增量寄存器。 				0x0000_0000
	vu32	ROP;			// 0x76100410 	读/写 	光栅操作寄存器。 			0x0000_0000
	vu32	ALPHA;			// 0x76100420 	读/写 	α值，衰退偏移量。 			0x0000_0000
	vu32	FG_COLOR;		// 0x76100500 	读/写 	前景色/α寄存器。 			0x0000_0000
	vu32	BG_COLOR;		// 0x76100504 	读/写 	背景色寄存器。 				0x0000_0000
	vu32	BS_COLOR;		// 0x76100508 	读/写 	蓝屏颜色寄存器。 			0x0000_0000
	vu32	PATTERN;		// 0x76100600～0x7610067C 读/写 图案寄存器。		 -
	vu32	PATOFF;			// 0x76100700 	读/写 	图案偏移量XY寄存器。 		0x0000_0000
	vu32	PATOFF_X;		// 0x76100704 	写 		图案偏移量X寄存器。 			0x0000_0000
	vu32	PATOFF_Y;		// 0x76100708 	写 		图案偏移量Y寄存器。 			0x0000_0000
}G2D_TypeDef;


 
//**********************************************************************************//
//外设基址宏定义


//中断控制器VIC基址 
#define VIC0_BASE	0x71200000
#define VIC1_BASE	0x71300000

//NAND控制器基地址
#define NAND_BASE	0x70200000


//RTC 实时时钟寄存器
#define RTC_BASE	0x7E005030

//ADC 寄存器
#define ADC_BASE	0x7E00B000

//SDMMC	寄存器
#define SDIO0_BASE 0x7C200000
#define SDIO1_BASE 0x7C300000
#define SDIO2_BASE 0x7C400000

//AC97 控制器
#define AC97_BASE	0x7F001000

//G2D	//2D图形加速
#define G2D_BASE	0x76100000

//SPI
#define SPI0_BASE	0x7f00b000
#define SPI1_BASE	0x7f00c000


 
//**********************************************************************************//
//外设结构体指针定义

//VIC
#ifdef _VIC0	/*VIC0*/
	#define VIC0        ((VICx_TypeDef*)VIC0_BASE)
#endif
#ifdef _VIC1	/*VIC1*/
	#define VIC1        ((VICx_TypeDef*)VIC1_BASE)
#endif

//MODEM
#ifdef _MODEM	/*MODEM*/
	#define MODEM		((MODEM_TypeDef*)MODEM_BASE)
#endif

//NAND
#ifdef _NAND	/*NAND*/
	#define NAND		((NAND_TypeDef*)NAND_BASE)
#endif


//RTC
#ifdef _RTC	/*RTC*/
	#define RTC		((RTC_TypeDef*)RTC_BASE)
#endif


//ADC
#ifdef _ADC	/*ADC*/
	#define ADC		((ADC_TypeDef*)ADC_BASE)
#endif


//SDMMC
#ifdef _SDIO0	/*SDIO0*/
	#define SDIO0	((SDIO_TypeDef*)SDIO0_BASE)
#endif

#ifdef _SDIO1	/*SDIO1*/
	#define SDIO1	((SDIO_TypeDef*)SDIO1_BASE)
#endif

#ifdef _SDIO2	/*SDIO2*/
	#define SDIO2	((SDIO_TypeDef*)SDIO2_BASE)
#endif


//AC97
#ifdef _AC97	/*AC97*/
	#define AC97	((AC97_TypeDef*)AC97_BASE)
#endif

//2D
#ifdef _G2D		/*G2D*/
	#define G2D		((G2D_TypeDef*)G2D_BASE)
#endif	


//SPI0
#ifdef _SPI0		/*SPI0*/
	#define SPI0	((SPI_TypeDef*)SPI0_BASE)
#endif	

//SPI1
#ifdef _SPI1		/*SPI1*/
	#define SPI1	((SPI_TypeDef*)SPI1_BASE)
#endif	











//**********************************************************************************//
//直接地址映射

//UART的寄存器
//			寄存器 			地址 						  读/写 	说明 											初始值
//UART0	
#ifdef	_UART0
	#define	rULCON0 		S3C6410_SFR32(0x7F005000) 		//读/写 	UART 通道0 行控制寄存器 						0x00
	#define	rUCON0 			S3C6410_SFR32(0x7F005004)		//读/写 	UART 通道0 控制寄存器 							0x00
	#define	rUFCON0 		S3C6410_SFR32(0x7F005008)		//读/写 	UART 通道0 FIFO 控制寄存器 						0x0
	#define	rUMCON0 		S3C6410_SFR32(0x7F00500C)		//读/写 	UART 通道0 调制解调器（Modem）控制寄存器 		0x0
	#define	rUTRSTAT0 		S3C6410_SFR32(0x7F005010)		//读 		UART 通道0 发送/接收状态寄存器 					0x6
	#define	rUERSTAT0 		S3C6410_SFR32(0x7F005014)		//读 		UART 通道0 接收错误状态寄存器 					0x0
	#define	rUFSTAT0 		S3C6410_SFR32(0x7F005018)		//读 		UART 通道0 FIFO 状态寄存器 						0x00
	#define	rUMSTAT0 		S3C6410_SFR32(0x7F00501C)		//读 		UART 通道0 调制解调器（Modem）状态寄存器 		0x0
	#define	rUTXH0 			S3C6410_SFR32(0x7F005020)		//写 		UART 通道0 发送缓冲寄存器 						-
	#define	rURXH0 			S3C6410_SFR32(0x7F005024)		//读 		UART 通道0 接收缓冲寄存器 						0x00
	#define	rUBRDIV0 		S3C6410_SFR32(0x7F005028)		//读/写 	UART 通道0 波特率分频寄存器 					0x0000
	#define	rUDIVSLOT0 		S3C6410_SFR32(0x7F00502C)		//读/写 	UART 通道0 分频插槽寄存器 						0x0000
	#define	rUINTP0 		S3C6410_SFR32(0x7F005030)		//读/写 	UART 通道0 中断处理寄存器 						0x0
	#define	rUINTSP0 		S3C6410_SFR32(0x7F005034)		//读/写 	UART 通道0 中断源处理寄存器 					0x0
	#define	rUINTM0 		S3C6410_SFR32(0x7F005038)		//读/写 	UART 通道0 中断屏蔽寄存器						0x0
#endif
//UART1	
#ifdef	_UART1
	#define	rULCON1 		S3C6410_SFR32(0x7F005400)		//读/写 	UART 通道1 行控制寄存器 						0x00
	#define	rUCON1 			S3C6410_SFR32(0x7F005404)		//读/写 	UART 通道1 控制寄存器读 						0x00
	#define	rUFCON1 		S3C6410_SFR32(0x7F005408)		//读/写 	UART 通道1 FIFO 控制寄存器 						0x0
	#define	rUMCON1 		S3C6410_SFR32(0x7F00540C)		//读/写 	UART 通道1 调制解调器（Modem）控制寄存器 		0x0
	#define	rUTRSTAT1		S3C6410_SFR32(0x7F005410)		//读 		UART 通道1 发送/接收状态寄存器 					0x6
	#define	rUERSTAT1 		S3C6410_SFR32(0x7F005414)		//读 		UART 通道1 接收错误状态寄存器 					0x0
	#define	rUFSTAT1 		S3C6410_SFR32(0x7F005418)		//读 		UART 通道1 FIFO 状态寄存器 						0x00
	#define	rUMSTAT1 		S3C6410_SFR32(0x7F00541C)		//读 		UART 通道1 调制解调器（Modem）状态寄存器 		0x0
	#define	rUTXH1 			S3C6410_SFR32(0x7F005420)		//写 		UART 通道1 发送缓冲寄存器	 					-
	#define	rURXH1 			S3C6410_SFR32(0x7F005424)		//读 		UART 通道1 接收缓冲寄存器。 					0x00
	#define	rUBRDIV1 		S3C6410_SFR32(0x7F005428)		//读/写 	UART 通道1 波特率分频寄存器。 					0x0000
	#define	rUDIVSLOT1 		S3C6410_SFR32(0x7F00542C)		//读/写 	UART 通道1 分频插槽寄存器。 					0x0000
	#define	rUINTP1 		S3C6410_SFR32(0x7F005430)		//读/写 	UART 通道1 中断处理寄存器。 					0x0
	#define	rUINTSP1		S3C6410_SFR32(0x7F005434)		//读/写 	UART 通道1 中断源处理寄存器。 					0x0
	#define	rUINTM1 		S3C6410_SFR32(0x7F005438)		//读/写 	UART 通道1 中断屏蔽寄存器。 					0x0
#endif
//UART2	
#ifdef	_UART2
	#define	rULCON2 		S3C6410_SFR32(0x7F005800)		//读/写 	UART 通道2 行控制寄存器。 						0x00
	#define	rUCON2 			S3C6410_SFR32(0x7F005804)		//读/写 	UART 通道2 控制寄存器。 						0x00
	#define	rUFCON2 		S3C6410_SFR32(0x7F005808)		//读/写 	UART 通道2 FIFO 控制寄存器。 					0x0
	#define	rUTRSTAT2 		S3C6410_SFR32(0x7F005810)		//读 		UART 通道2 发送/接收状态寄存器。 				0x6
	#define	rUERSTAT2 		S3C6410_SFR32(0x7F005814)		//读 		UART 通道2 接收错误状态寄存器。 				0x0
	#define	rUFSTAT2 		S3C6410_SFR32(0x7F005818)		//读 		UART 通道2 FIFO 状态寄存器。 					0x00
	#define	rUTXH2 			S3C6410_SFR32(0x7F005820)		//写 		UART 通道2 发送缓冲寄存器。 					-
	#define	rURXH2 			S3C6410_SFR32(0x7F005824)		//读 		UART 通道2 接收缓冲寄存器。 					0x00
	#define	rUBRDIV2 		S3C6410_SFR32(0x7F005828)		//读/写 	UART 通道2 波特率分频寄存器。 					0x0000
	#define	rUDIVSLOT2 		S3C6410_SFR32(0x7F00582C)		//读/写 	UART 通道2 分频插槽寄存器。 					0x0000
	#define	rINTP2 			S3C6410_SFR32(0x7F005830)		//读/写 	UART 通道2 中断处理寄存器。 					0x0
	#define	rUINTM2 		S3C6410_SFR32(0x7F005838)		//读/写 	UART 通道2 中断屏蔽寄存器。 					0x0
#endif
//UART3
#ifdef	_UART3
	#define	rULCON3 		S3C6410_SFR32(0x7F005C00)		//读/写 	UART 通道3 行控制寄存器。 						0x00
	#define	rUCON3 			S3C6410_SFR32(0x7F005C04)		//读/写	UART 通道3 控制寄存器。 							0x00
	#define	rUFCON3 		S3C6410_SFR32(0x7F005C08)		//读/写 	UART 通道3 FIFO 控制寄存器。 					0x0
	#define	rUTRSTAT3 		S3C6410_SFR32(0x7F005C10)		//读 		UART 通道3 发送/接收状态寄存器。 				0x6
	#define	rUERSTAT3 		S3C6410_SFR32(0x7F005C14)		//读 		UART 通道3 接收错误状态寄存器。 				0x0
	#define	rUFSTAT3 		S3C6410_SFR32(0x7F005C18)		//读 		UART 通道3 FIFO 状态寄存器。 					0x00
	#define	rUTXH3 			S3C6410_SFR32(0x7F005C20)		//写 		UART 通道3 发送缓冲寄存器。 					-
	#define	rURXH3 			S3C6410_SFR32(0x7F005C24)		//读 		UART 通道3 接收缓冲寄存器。 					0x00
	#define	rUBRDIV3 		S3C6410_SFR32(0x7F005C28)		//读/写 	UART 通道3 波特率分频寄存器。 					0x0000
	#define	rUDIVSLOT3 		S3C6410_SFR32(0x7F005C2C)		//读/写 	UART 通道3 分频插槽寄存器。 					0x0000
	#define	rINTP3 			S3C6410_SFR32(0x7F005C30)		//读/写 	UART 通道3 中断处理寄存器。 					0x0
	#define	rUINTSP3 		S3C6410_SFR32(0x7F005C34)		//读/写 	UART 通道3 中断源处理寄存器。 					0x0
	#define	rUINTM3 		S3C6410_SFR32(0x7F005C38)		//读/写 	UART 通道3 中断屏蔽寄存器。 					0x0
#endif


//系统控制寄存器
//			以下突出系统控制器内的34 个寄存器。
//		寄存器 			地址 						   读/写 	描述 						复位值
#define	rAPLL_LOCK		S3C6410_SFR32(0x7E00F000)		// 读/写 	控制PLL 锁定期APLL。 		0x0000_FFFF
#define	rMPLL_LOCK 		S3C6410_SFR32(0x7E00F004)		// 读/写 	控制PLL 锁定期MPLL。	 	0x0000_FFFF
#define	rEPLL_LOCK 		S3C6410_SFR32(0x7E00F008)		// 读/写 	控制PLL 锁定期EPLL。 		0x0000_FFFF
#define	rAPLL_CON 		S3C6410_SFR32(0x7E00F00C)		// 读/写 	控制PLL 输出频率 APLL。 	0x0190_0302
#define	rMPLL_CON 		S3C6410_SFR32(0x7E00F010)		// 读/写 	控制PLL 输出频率 MPLL。 	0x0214_0603
#define	rEPLL_CON0 		S3C6410_SFR32(0x7E00F014)		// 读/写 	控制PLL 输出频率 EPLL。 	0x0020_0102
#define	rEPLL_CON1 		S3C6410_SFR32(0x7E00F018)		// 读/写 	控制PLL 输出频率 EPLL。 	0x0000_9111
#define	rCLK_SRC 		S3C6410_SFR32(0x7E00F01C)		// 读/写 	选择时钟源。 				0x0000_0000
#define	rCLK_DIV0 		S3C6410_SFR32(0x7E00F020)		// 读/写 	设置时钟分频器的比例。 		0x0105_1000
#define	rCLK_DIV1 		S3C6410_SFR32(0x7E00F024)		// 读/写 	设置时钟分频器的比例。 		0x0000_0000
#define	rCLK_DIV2 		S3C6410_SFR32(0x7E00F028)		// 读/写 	设置时钟分频器的比例。 		0x0000_0000
#define	rCLK_OUT 		S3C6410_SFR32(0x7E00F02C)		// 读/写 	选择时钟输出。 				0x0000_0000
#define	rHCLK_GATE 		S3C6410_SFR32(0x7E00F030)		// 读/写 	控制HCLK 时钟选通。 		0xFFFF_FFFF
#define	rPCLK_GATE 		S3C6410_SFR32(0x7E00F034)		// 读/写 	控制PCLK 时钟选通。 		0xFFFF_FFFF
#define	rSCLK_GATE 		S3C6410_SFR32(0x7E00F038)		// 读/写 	控制SCLK 时钟选通。 		0xFFFF_FFFF
#define	rAHB_CON0 		S3C6410_SFR32(0x7E00F100)		// 读/写 	配置AHB I/P/X/F 总线。 		0x0400_0000
#define	rAHB_CON1 		S3C6410_SFR32(0x7E00F104)		// 读/写 	配置AHB M1/M0/T1/T0 总线。	0x0000_0000
#define	rAHB_CON2 		S3C6410_SFR32(0x7E00F108)		// 读/写 	配置AHB R/S1/S0 总线。 		0x0000_0000
#define	rSDMA_SEL 		S3C6410_SFR32(0x7E00F110)		// 读/写 	选择安全DMA 输入。 			0x0000_0000
#define	rSW_RST 		S3C6410_SFR32(0x7E00F114)		// 读/写 	产生软件复位。 				0x0000_0000
#define	rSYS_ID 		S3C6410_SFR32(0x7E00F118)		// 读 		系统ID 版本和审查通过。 	0x0000_0000
#define	rMEM_SYS_CFG 	S3C6410_SFR32(0x7E00F120)		// 读/写 	配置存储器子系统。 			0x0000_0080
#define	rQOS_OVERRIDE0 	S3C6410_SFR32(0x7E00F124)		// 读/写 	取代DMC0 QOS。 				0x0000_0000
#define	rQOS_OVERRIDE1 	S3C6410_SFR32(0x7E00F128)		// 读/写 	取代DMC1 QOS。 				0x0000_0000
#define	rMEM_CFG_STAT 	S3C6410_SFR32(0x7E00F12C)		// 读 存	储器子系统建立状态。 		0x0000_0000
#define	rPWR_CFG 		S3C6410_SFR32(0x7E00F804)		// 读/写 	配置电源管理。 				0x0000_0001
#define	rEINT_MASK 		S3C6410_SFR32(0x7E00F808)		// 读/写 	配置EINT（外部中断）屏蔽。 	0x0000_0000
#define	rNORMAL_CFG 	S3C6410_SFR32(0x7E00F810)		// 读/写 	在正常模式下，配置电源管理。0xFFFF_FF00
#define	rSTOP_CFG 		S3C6410_SFR32(0x7E00F814)		// 读/写 	在停止模式下，配置电源管理。0x2012_0100
#define	rSLEEP_CFG 		S3C6410_SFR32(0x7E00F818)		// 读/写 	在睡眠模式下，配置电源管理。0x0000_0000
#define	rOSC_FREQ 		S3C6410_SFR32(0x7E00F820)		// 读/写 	振荡器频率刻度计数器。 		0x0000_000F
#define	rPWR_STABLE 	S3C6410_SFR32(0x7E00F828)		// 读/写 	电源稳定计数器。 			0x0000_0001
#define	rMTC_STABLE 	S3C6410_SFR32(0x7E00F830)		// 读/写 	MTC 稳定计数器。 			0xFFFF_FFFF
#define	rOTHERS 		S3C6410_SFR32(0x7E00F900)		// 读/写 	其他控制寄存器。 			0x0000_801E
#define	rRST_STAT 		S3C6410_SFR32(0x7E00F904)		// 读 		复位状态寄存器。 			0x0000_0001
#define	rWAKEUP_STAT 	S3C6410_SFR32(0x7E00F908)		// 读/写 	唤醒状态寄存器。 			0x0000_0000
#define	rBLK_PWR_STAT 	S3C6410_SFR32(0x7E00F90C)		// 读 块	电源状态寄存器。 			0x0000_007F
#define	rINFO_RM0 		S3C6410_SFR32(0x7E00FA00)		// 读/写 	信息寄存器0。 				0x0000_0000
#define	rINFO_RM1 		S3C6410_SFR32(0x7E00FA04)		// 读/写 	信息寄存器1。				0x0000_0000
#define	rINFO_RM2 		S3C6410_SFR32(0x7E00FA08)		// 读/写 	信息寄存器2。 				0x0000_0000
#define	rINFO_RM3 		S3C6410_SFR32(0x7E00FA0C)		// 读/写 	信息寄存器3。 				0x0000_0000


//GPIO控制寄存器
//		寄存器 			地址 								读/写 	描述 						复位值
#define	rGPACON 		S3C6410_SFR32(0x7F008000)		// 读/写 	端口A 配置寄存器 			0x0
#define	rGPADAT 		S3C6410_SFR32(0x7F008004)		// 读/写 	端口A 数据寄存器 			未定义
#define	rGPAPUD 		S3C6410_SFR32(0x7F008008)		// 读/写 	端口A 上拉/下拉寄存器 		0x0000555
#define	rGPACONSLP 		S3C6410_SFR32(0x7F00800C)		// 读/写 	端口A 睡眠模式配置寄存器 	0x0
#define	rGPACONSLP 		S3C6410_SFR32(0x7F00800C)		// 读/写 	端口A 睡眠模式配置寄存器 	0x0
#define	rGPAPUDSLP 		S3C6410_SFR32(0x7F008010)		// 读/写 	端口A 睡眠模式拉/下拉寄存器 0x0
#define	rGPBCON 		S3C6410_SFR32(0x7F008020)		// 读/写 	端口B 配置寄存器 			0x40000
#define	rGPBDAT 		S3C6410_SFR32(0x7F008024)		// 读/写 	端口B 数据寄存器 			未定义
#define	rGPBPUD 		S3C6410_SFR32(0x7F008028)		// 读/写 	端口B 上拉/下拉寄存器 		0x00001555
#define	rGPBCONSLP 		S3C6410_SFR32(0x7F00802C)		// 读/写 	端口B 睡眠模式配置寄存器 	0x0
#define	rGPBPUDSLP 		S3C6410_SFR32(0x7F008030)		// 读/写 	端口B 睡眠模式拉/下拉寄存器 0x0
#define	rGPCCON 		S3C6410_SFR32(0x7F008040)		// 读/写 	端口C 配置寄存器 			0x0
#define	rGPCDAT 		S3C6410_SFR32(0x7F008044)		// 读/写 	端口C 数据寄存器 			未定义
#define	rGPCPUD 		S3C6410_SFR32(0x7F008048)		// 读/写 	端口C 上拉/下拉寄存器 		0x00005555
#define	rGPCCONSLP 		S3C6410_SFR32(0x7F00804C)		// 读/写 	端口C 睡眠模式配置寄存器 	0x0
#define	rGPCPUDSLP 		S3C6410_SFR32(0x7F008050)		// 读/写 	端口C 睡眠模式拉/下拉寄存器 0x0
#define	rGPDCON 		S3C6410_SFR32(0x7F008060)		// 读/写 	端口D 配置寄存器 			0x0
#define	rGPDDAT 		S3C6410_SFR32(0x7F008064)		// 读/写 	端口D 数据寄存器 			未定义
#define	rGPDPUD 		S3C6410_SFR32(0x7F008068)		// 读/写 	端口D 上拉/下拉寄存器 		0x00000155
#define	rGPDCONSLP 		S3C6410_SFR32(0x7F00806C)		// 读/写 	端口D 睡眠模式配置寄存器 	0x0
#define	rGPDPUDSLP 		S3C6410_SFR32(0x7F008070)		// 读/写 	端口D 睡眠模式拉/下拉寄存器 0x0
#define	rGPECON 		S3C6410_SFR32(0x7F008080)		// 读/写 	端口E 配置寄存器 			0x0
#define	rGPEDAT 		S3C6410_SFR32(0x7F008084)		// 读/写 	端口E 数据寄存器 			未定义
#define	rGPEPUD 		S3C6410_SFR32(0x7F008088)		// 读/写 	端口E 上拉/下拉寄存器 		0x00000155
#define	rGPECONSLP 		S3C6410_SFR32(0x7F00808C)		// 读/写 	端口E 睡眠模式配置寄存器 	0x0
#define	rGPEPUDSLP 		S3C6410_SFR32(0x7F008090)		// 读/写 	端口E 睡眠模式拉/下拉寄存器 0x0
#define	rGPFCON 		S3C6410_SFR32(0x7F0080A0)		// 读/写 	端口F 配置寄存器 			0x0
#define	rGPFDAT 		S3C6410_SFR32(0x7F0080A4)		// 读/写 	端口F 数据寄存器 			未定义
#define	rGPFPUD 		S3C6410_SFR32(0x7F0080A8)		// 读/写 	端口F 上拉/下拉寄存器 		0x55555555
#define	rGPFCONSLP 		S3C6410_SFR32(0x7F0080AC)		// 读/写 	端口F 睡眠模式配置寄存器 	0x0
#define	rGPFPUDSLP 		S3C6410_SFR32(0x7F0080B0)		// 读/写 	端口F 睡眠模式拉/下拉寄存器 0x0
#define	rGPGCON 		S3C6410_SFR32(0x7F0080C0)		// 读/写 	端口G 配置寄存器 			0x0
#define	rGPGDAT 		S3C6410_SFR32(0x7F0080C4)		// 读/写 	端口G 数据寄存器 			未定义
#define	rGPGPUD 		S3C6410_SFR32(0x7F0080C8)		// 读/写 	端口G 上拉/下拉寄存器 		0x00001555
#define	rGPGCONSLP 		S3C6410_SFR32(0x7F0080CC)		// 读/写 	端口G 睡眠模式配置寄存器 	0x0
#define	rGPGPUDSLP 		S3C6410_SFR32(0x7F0080D0)		// 读/写 	端口G 睡眠模式拉/下拉寄存器 0x0
#define	rGPHCON 		S3C6410_SFR32(0x7F0080E0)		// 读/写 	端口H 配置寄存器 			0x0
#define	rGPHDAT 		S3C6410_SFR32(0x7F0080E4)		// 读/写 	端口H 数据寄存器 			未定义
#define	rGPHPUD 		S3C6410_SFR32(0x7F0080E8)		// 读/写 	端口H 上拉/下拉寄存器 		0x00055555
#define	rGPHCONSLP 		S3C6410_SFR32(0x7F0080EC)		// 读/写 	端口H 睡眠模式配置寄存器 	0x0
#define	rGPHPUDSLP 		S3C6410_SFR32(0x7F0080F0)		// 读/写 	端口H 睡眠模式拉/下拉寄存器 0x0
#define	rGPICON 		S3C6410_SFR32(0x7F008100)		// 读/写 	端口I 配置寄存器 			0x0
#define	rGPIDAT 		S3C6410_SFR32(0x7F008104)		// 读/写 	端口I 数据寄存器 			未定义
#define	rGPIPUD 		S3C6410_SFR32(0x7F008108)		// 读/写 	端口I 上拉/下拉寄存器 		0x55555555
#define	rGPICONSLP 		S3C6410_SFR32(0x7F00810C)		// 读/写 	端口I 睡眠配置寄存器 		0x0
#define	rGPIPUDSLP 		S3C6410_SFR32(0x7F008110)		// 读/写 	端口I 睡眠拉/下拉寄存器 	0x0
#define	rGPJCON 		S3C6410_SFR32(0x7F008120)		// 读/写 	端口J 配置寄存器 			0x0
#define	rGPJDAT 		S3C6410_SFR32(0x7F008124)		// 读/写 	端口J 数据寄存器 			未定义
#define	rGPJPUD 		S3C6410_SFR32(0x7F008128)		// 读/写 	端口J 上拉/下拉寄存器 		0x00555555
#define	rGPJCONSLP 		S3C6410_SFR32(0x7F00812C)		// 读/写 	端口J 睡眠模式配置寄存器 	0x0
#define	rGPJPUDSLP 		S3C6410_SFR32(0x7F008130)		// 读/写	端口J 睡眠模式拉/下拉寄存器 0x0
#define	rGPKCON0 		S3C6410_SFR32(0x7F008800)		// 读/写 	端口K 配置寄存器0 			0x22222222
#define	rGPKCON1 		S3C6410_SFR32(0x7F008804)		// 读/写 	端口K 配置寄存器1 			0x22222222
#define	rGPKDAT 		S3C6410_SFR32(0x7F008808)		// 读/写 	端口K 数据寄存器 			未定义
#define	rGPKPUD 		S3C6410_SFR32(0x7F00880C)		// 读/写	端口K 上拉/下拉寄存器 		0x55555555
#define	rGPLCON0 		S3C6410_SFR32(0x7F008810)		// 读/写 	端口L 配置寄存器0 			0x22222222
#define	rGPLCON1 		S3C6410_SFR32(0x7F008814)		// 读/写 	端口L 配置寄存器1 			0x22222222
#define	rGPLDAT 		S3C6410_SFR32(0x7F008818)		// 读/写 	端口L 数据寄存器 			未定义
#define	rGPLPUD 		S3C6410_SFR32(0x7F00881C)		// 读/写 	端口L 上拉/下拉寄存器 		0x55555555
#define	rGPMCON 		S3C6410_SFR32(0x7F008820)		// 读/写 	端口M 配置寄存器 			0x00222222
#define	rGPMDAT 		S3C6410_SFR32(0x7F008824)		// 读/写 	端口M 数据寄存器 			未定义
#define	rGPMPUD 		S3C6410_SFR32(0x7F008828)		// 读/写 	端口M 上拉/下拉寄存器 		0x000002AA
#define	rGPNCON 		S3C6410_SFR32(0x7F008830)		// 读/写 	端口N 配置寄存器 			0x0
#define	rGPNDAT 		S3C6410_SFR32(0x7F008834)		// 读/写 	端口N 数据寄存器 			未定义
#define	rGPNPUD 		S3C6410_SFR32(0x7F008838)		// 读/写 	端口N 上拉/下拉寄存器 		0x55555555
#define	rGPOCON 		S3C6410_SFR32(0x7F008140)		// 读/写 	端口O 配置寄存器 			0xAAAAAAAA
#define	rGPODAT 		S3C6410_SFR32(0x7F008144)		// 读/写 	端口O 数据寄存器 			未定义
#define	rGPOPUD 		S3C6410_SFR32(0x7F008148)		// 读/写 	端口O 上拉/下拉寄存器 		0x0
#define	rGPOCONSLP 		S3C6410_SFR32(0x7F00814C)		// 读/写 	端口O 睡眠模式配置寄存器 	0x0
#define	rGPOPUDSLP 		S3C6410_SFR32(0x7F008150)		// 读/写 	端口O 睡眠模式拉/下拉寄存器 0x0
#define	rGPPCON 		S3C6410_SFR32(0x7F008160)		// 读/写 	端口P 配置寄存器 			0x2AAAAAAA
#define	rGPPDAT 		S3C6410_SFR32(0x7F008164)		// 读/写 	端口P 数据寄存器 			未定义
#define	rGPPPUD 		S3C6410_SFR32(0x7F008168)		// 读/写 	端口P 上拉/下拉寄存器 		0x1011AAA0
#define	rGPPCONSLP 		S3C6410_SFR32(0x7F00816C)		// 读/写 	端口P 睡眠模式配置寄存器 	0x0
#define	rGPPPUDSLP 		S3C6410_SFR32(0x7F008170)		// 读/写 	端口P 睡眠模式拉/下拉寄存器 0x0
#define	rGPQCON 		S3C6410_SFR32(0x7F008180)		// 读/写 	端口Q 配置寄存器 			0x0002AAAA
#define	rGPQDAT 		S3C6410_SFR32(0x7F008184)		// 读/写 	端口Q 数据寄存器 			未定义
#define	rGPQPUD 		S3C6410_SFR32(0x7F008188)		// 读/写 	端口Q 上拉/下拉寄存器 		0x00001555
#define	rGPQCONSLP 		S3C6410_SFR32(0x7F00818C)		// 读/写 	端口Q 睡眠模式配置寄存器 	0x0
#define	rGPQPUDSLP 		S3C6410_SFR32(0x7F008180)		// 读/写 	端口Q 睡眠模式拉/下拉寄存器 0x0
#define	rSPCON 			S3C6410_SFR32(0x7F0081A0)		// 读/写 	特殊端口配置寄存器 			0Xbfc11500
#define	rMEM0IOCONSLP0 	S3C6410_SFR32(0x7F0081C0)		// 读/写 	存储器端口0 睡眠 模式配置0 	0x0
#define	rMEM0IOCONSLP1 	S3C6410_SFR32(0x7F0081C4)		// 读/写 	存储器端口0 睡眠 模式配置1 	0x0
#define	rMEM0IOCONSLP 	S3C6410_SFR32(0x7F0081C8)		// 读/写 	存储器端口0 睡眠模式配置 	0x0
#define	rMEM0IODRVCON 	S3C6410_SFR32(0x7F0081D0)		// 读/写 	存储器端口0 驱动控制寄存器 	0x10555551
#define	rMEM1IODRVCON 	S3C6410_SFR32(0x7F0081D4)		// 读/写 	存储器端口0 驱动控制寄存器 	0x0


//外部中断控制寄存器
/*下面的表格是外部中断控制寄存器的清单。0 组有专用的管脚，0 组的中断可以比其它组的中断控制
更多的内容。S3C6410 显示出0 组的多个寄存器以及每对中断信号所占的位。其他组同样有多个寄存器，
每个寄存器可以控制2 个组或多个组。0 组数字滤波器计数脉冲源是FIN，其它组的数字滤波器计数脉冲
源是PCLK.
		寄存器 			地址 						   读/写 	描述 								复位值*/
#define	rEINT0CON0		S3C6410_SFR32(0x7F008900)		// 读/写 	外部中断0（0 组）配置寄存器0 		0x0
#define	rEINT0CON1 		S3C6410_SFR32(0x7F008904)		// 读/写 	外部中断0（0 组）配置寄存器1 		0x0
#define	rEINT0FLTCON0 	S3C6410_SFR32(0x7F008910)		// 读/写 	外部中断0（0 组）过滤控制寄存器0 	0x0
#define	rEINT0FLTCON1 	S3C6410_SFR32(0x7F008914)		// 读/写 	外部中断0（0 组）过滤控制寄存器1 	0x0
#define	rEINT0FLTCON2 	S3C6410_SFR32(0x7F008918)		// 读/写 	外部中断0（0 组）过滤控制寄存器2 	0x0
#define	rEINT0FLTCON3 	S3C6410_SFR32(0x7F00891C)		// 读/写 	外部中断0（0 组）过滤控制寄存器3 	0x0
#define	rEINT0MASK 		S3C6410_SFR32(0x7F008920)		// 读/写 	外部中断0（0 组）屏蔽寄存器 		0x0FFFFFFF
#define	rEINT0PEND 		S3C6410_SFR32(0x7F008924)		// 读/写 	外部中断0（0 组）悬挂寄存器 		0x0
#define	rEINT12CON 		S3C6410_SFR32(0x7F008200)		// 读/写 	外部中断1、2（1、2 组）配置寄存器 	0x0
#define	rEINT34CON 		S3C6410_SFR32(0x7F008204)		// 读/写 	外部中断3、4（3、4 组）配置寄存器 	0x0
#define	rEINT56CON 		S3C6410_SFR32(0x7F008208)		// 读/写 	外部中断5、6（5、6 组）配置寄存器 	0x0
#define	rEINT78CON 		S3C6410_SFR32(0x7F00820C)		// 读/写 	外部中断7、8（7、8 组）配置寄存器 	0x0
#define	rEINT9CON 		S3C6410_SFR32(0x7F008210)		// 读/写 	外部中断9（9 组）配置寄存器 0x0
#define	rEINT12FLTCON 	S3C6410_SFR32(0x7F008220)		// 读/写 	外部中断1、2（1、2 组）过滤控制寄存器0x0
#define	rEINT34FLTCON 	S3C6410_SFR32(0x7F008224)		// 读/写 	外部中断3、4（3、4 组）过滤控制寄存器0x0
#define	rEINT56FLTCON 	S3C6410_SFR32(0x7F008228)		// 读/写 	外部中断5、6（5、6 组）过滤控制寄存器0x0
#define	rEINT78FLTCON 	S3C6410_SFR32(0x7F00822C)		// 读/写 	外部中断7、8（7、8 组）过滤控制寄存器0x0
#define	rEINT9FLTCON 	S3C6410_SFR32(0x7F008230)		// 读/写 	外部中断9（9 组）过滤控制寄存器0 	0x0
#define	rEINT12MASK 	S3C6410_SFR32(0x7F008240)		// 读/写 	外部中断1、2（1、2 组）屏蔽寄存器 	0x00FF7FFF
#define	rEINT34MASK 	S3C6410_SFR32(0x7F008244)		// 读/写 	外部中断3、4（3、4 组）屏蔽寄存器 	0x3FFF03FF
#define	rEINT56MASK 	S3C6410_SFR32(0x7F008248)		// 读/写 	外部中断5、6（5、6 组）屏蔽寄存器 	0x03FF007F
#define	rEINT78MASK 	S3C6410_SFR32(0x7F00824C)		// 读/写 	外部中断7、8（7、8 组）屏蔽寄存器 	0x7FFFFFFF
#define	rEINT9MASK 		S3C6410_SFR32(0x7F008250)		// 读/写 	外部中断9（9 组）屏蔽存器 			0x000001FF
#define	rEINT12PEND 	S3C6410_SFR32(0x7F008260)		// 读/写 	外部中断1、2（1、2 组）悬挂寄存器 	0x0
#define	rEINT34PEND 	S3C6410_SFR32(0x7F008264)		// 读/写 	外部中断3、4（3、4 组）悬挂寄存器 	0x0
#define	rEINT56PEND 	S3C6410_SFR32(0x7F008268)		// 读/写 	外部中断5、6（5、6 组）悬挂寄存器 	0x0
#define	rEINT78PEND 	S3C6410_SFR32(0x7F00826C)		// 读/写 	外部中断7、8（7、8 组）悬挂寄存器 	0x0
#define	rEINT9PEND 		S3C6410_SFR32(0x7F008270)		// 读/写 	外部中断9（9 组）悬挂寄存器 		0x0
#define	rPRIORITY 		S3C6410_SFR32(0x7F008280)		// 读/写 	优先控制寄存器 						0x3FF
#define	rSERVICE 		S3C6410_SFR32(0x7F008284)		// 读 		当前服务寄存器 						0x0
#define	rSERVICEPEND 	S3C6410_SFR32(0x7F008288)		// 读/写 	当前服务悬挂寄存器 					0x0


//定时器相关寄存器(0~4)
//只有定时器0,1有外部PWM输出模式,定时器2,3,4无外部引脚,属于内部定时器
//		寄存器 		偏移量 							读/写 		描述 													复位值
#define	rTCFG0  	S3C6410_SFR32(0x7F006000)		//  读/写 		定时器配置寄存器0 时配置两个8 位预定标器和死区长度。	0x0000_0101
#define	rTCFG1  	S3C6410_SFR32(0x7F006004)		//  读/写 		定时器配置寄存器1 时，5-MUX 和DMA 模式选择寄存器。		0x0000_0000
#define	rTCON  		S3C6410_SFR32(0x7F006008)		//  读/写 		定时器控制寄存器。 										0x0000_0000
#define	rTCNTB0  	S3C6410_SFR32(0x7F00600C)		//  读/写 		定时器0 计数缓冲器。 									0x0000_0000
#define	rTCMPB0  	S3C6410_SFR32(0x7F006010)		//  读/写 		定时器0 比较缓冲寄存器。 								0x0000_0000
#define	rTCNTO0  	S3C6410_SFR32(0x7F006014)		//  读 			定时器0 计数观察寄存器。 								0x0000_0000
#define	rTCNTB1  	S3C6410_SFR32(0x7F006018)		//  读/写 		定时器1 计数缓冲器。 									0x0000_0000
#define	rTCMPB1  	S3C6410_SFR32(0x7F00601c)		//  读/写 		定时器1 比较缓冲寄存器。 								0x0000_0000
#define	rTCNTO1  	S3C6410_SFR32(0x7F006020)		//  读 			定时器1 计数观察寄存器。 								0x0000_0000
#define	rTCNTB2  	S3C6410_SFR32(0x7F006024)		//  读/写 		定时器2 计数缓冲器。 									0x0000_0000
#define	rTCMPB2  	S3C6410_SFR32(0x7F006028)		//  读/写 		定时器2 比较缓冲寄存器。 								0x0000_0000
#define	rTCNTO2  	S3C6410_SFR32(0x7F00602c)		//  读 			定时器2 计数观察寄存器。 								0x0000_0000
#define	rTCNTB3  	S3C6410_SFR32(0x7F006030)		//  读/写 		定时器3 计数缓冲器。 									0x0000_0000
#define	rTCMPB3  	S3C6410_SFR32(0x7F006034)		//  读/写 		定时器3 比较缓冲寄存器。 								0x0000_0000
#define	rTCNTO3  	S3C6410_SFR32(0x7F006038)		//  读 			定时器3 计数观察寄存器。 								0x0000_0000
#define	rTCNTB4  	S3C6410_SFR32(0x7F00603c)		//  读/写 		定时器4 计数缓冲器。 									0x0000_0000
#define	rTCNTO4  	S3C6410_SFR32(0x7F006040)		//  读 			定时器4 计数观察寄存器。 								0x0000_0000
#define	rTINT_CSTAT S3C6410_SFR32(0x7F006044)		//  读/写 		定时器中断控制和状态寄存器。 							0x0000_0000




	 
//显示控制器(TFT_LCD控制器) 
//SFR 内存映射
//		寄存器 			地址 							 读/写 			描述 									复位值
#define	rVIDCON0   		S3C6410_SFR32(0x77100000)		//   读/写 		视频控制0 寄存器器 						0x0000_0000
#define	rVIDCON1   		S3C6410_SFR32(0x77100004)		//   读/写 		视频控制1 寄存器 						0x0000_0000
#define	rVIDCON2   		S3C6410_SFR32(0x77100008)		//   读/写 		视频控制2 寄存器 						0x0000_0000
#define	rVIDTCON0   	S3C6410_SFR32(0x77100010)		//   读/写 		视频时序控制0 寄存器 					0x0000_0000
#define	rVIDTCON1   	S3C6410_SFR32(0x77100014)		//   读/写 		视频时序控制1 寄存器 					0x0000_0000
#define	rVIDTCON2   	S3C6410_SFR32(0x77100018)		//   读/写 		视频时序控制2 寄存器 					0x0000_0000
#define	rWINCON0   		S3C6410_SFR32(0x77100020)		//   读/写 		窗口控制0 寄存器 						0x0000_0000
#define	rWINCON1   		S3C6410_SFR32(0x77100024)		//   读/写 		窗口控制1 寄存器 						0x0000_0000
#define	rWINCON2   		S3C6410_SFR32(0x77100028)		//   读/写 		窗口控制2 寄存器 						0x0000_0000
#define	rWINCON3   		S3C6410_SFR32(0x7710002C)		//   读/写 		窗口控制3 寄存器 						0x0000_0000
#define	rWINCON4   		S3C6410_SFR32(0x77100030)		//   读/写 		窗口控制4 寄存器 						0x0000_0000
#define	rVIDOSD0A   	S3C6410_SFR32(0x77100040)		//   读/写 		视频窗口0 的位置控制寄存器 				0x0000_0000
#define	rVIDOSD0B   	S3C6410_SFR32(0x77100044)		//   读/写 		视频窗口0 的位置控制寄存器 				0x0000_0000
#define	rVIDOSD0C   	S3C6410_SFR32(0x77100048)		//   读/写 		视频窗口0 的尺寸控制寄存器 				0x0000_0000
#define	rVIDOSD1A   	S3C6410_SFR32(0x77100050)		//   读/写 		视频窗口1 的位置控制寄存器 				0x0000_0000
#define	rVIDOSD1B   	S3C6410_SFR32(0x77100054)		//   读/写 		视频窗口1 的位置控制寄存器 				0x0000_0000
#define	rVIDOSD1C   	S3C6410_SFR32(0x77100058)		//   读/写 		视频窗口1 的alpha 控制寄存器 			0x0000_0000
#define	rVIDOSD1D   	S3C6410_SFR32(0x7710005C)		//   读/写 		视频窗口1 的尺寸控制寄存器 				0x0000_0000
#define	rVIDOSD2A   	S3C6410_SFR32(0x77100060)		//   读/写 		视频窗口2 的位置控制寄存器 				0x0000_0000
#define	rVIDOSD2B   	S3C6410_SFR32(0x77100064)		//   读/写 		视频窗口2 的位置控制寄存器 				0x0000_0000
#define	rVIDOSD2C   	S3C6410_SFR32(0x77100068)		//   读/写 		视频窗口2 的alpha 控制寄存器			0x0000_0000
#define	rVIDOSD2D   	S3C6410_SFR32(0x7710006C)		//   读/写 		视频窗口2 的尺寸控制寄存器 				0x0000_0000
#define	rVIDOSD3A   	S3C6410_SFR32(0x77100070)		//   读/写 		视频窗口3 的位置控制寄存器 				0x0000_0000
#define	rVIDOSD3B   	S3C6410_SFR32(0x77100074)		//   读/写 		视频窗口3 的位置控制寄存器 				0x0000_0000
#define	rVIDOSD3C   	S3C6410_SFR32(0x77100078)		//   读/写 		视频窗口3 的alpha 控制寄存器 			0x0000_0000
#define	rVIDOSD4A   	S3C6410_SFR32(0x77100080)		//   读/写 		视频窗口4 的位置控制寄存器 				0x0000_0000
#define	rVIDOSD4B   	S3C6410_SFR32(0x77100084)		//   读/写 		视频窗口4 的位置控制寄存器 				0x0000_0000
#define	rVIDOSD4C   	S3C6410_SFR32(0x77100088)		//   读/写 		视频窗口4 的alpha 控制寄存器 			0x0000_0000
#define	rVIDW00ADD0B0  	S3C6410_SFR32(0x771000A0)		//   读/写 		窗口0 的缓冲区开始地址寄存器，缓冲区0	0x0000_0000
#define	rVIDW00ADD0B1  	S3C6410_SFR32(0x771000A4)		//   读/写 		窗口0 的缓冲区开始地址寄存器，缓冲区1	0x0000_0000
#define	rVIDW01ADD0B0  	S3C6410_SFR32(0x771000A8)		//   读/写 		窗口1 的缓冲区开始地址寄存器，缓冲区0	0x0000_0000
#define	rVIDW01ADD0B1  	S3C6410_SFR32(0x771000AC)		//   读/写 		窗口1 的缓冲区开始地址寄存器，缓冲区1	0x0000_0000
#define	rVIDW02ADD0   	S3C6410_SFR32(0x771000B0)		//   读/写 		窗口2 的缓冲区开始地址寄存器 			0x0000_0000
#define	rVIDW03ADD0   	S3C6410_SFR32(0x771000B8)		//   读/写 		窗口3 的缓冲区开始地址寄存器 			0x0000_0000
#define	rVIDW04ADD0   	S3C6410_SFR32(0x771000C0)		//   读/写 		窗口4 的缓冲区开始地址寄存器 			0x0000_0000
#define	rVIDW00ADD1B0  	S3C6410_SFR32(0x771000D0)		//   读/写 		窗口0 的缓冲区开始地址寄存器，缓冲区0	0x0000_0000
#define	rVIDW00ADD1B1  	S3C6410_SFR32(0x771000D4)		//   读/写 		窗口0 的缓冲区开始地址寄存器，缓冲区1	0x0000_0000
#define	rVIDW01ADD1B0  	S3C6410_SFR32(0x771000D8)		//   读/写 		窗口1 的缓冲区开始地址寄存器,缓冲区0	0x0000_0000
#define	rVIDW01ADD1B1  	S3C6410_SFR32(0x771000DC)		//   读/写 		窗口1 的缓冲区开始地址寄存器，缓冲区1	0x0000_0000
#define	rVIDW02ADD1   	S3C6410_SFR32(0x771000E0)		//   读/写 		窗口2 的缓冲区开始地址寄存器 			0x0000_0000
#define	rVIDW03ADD1   	S3C6410_SFR32(0x771000E8)		//   读/写 		窗口3 的缓冲区开始地址寄存器 			0x0000_0000
#define	rVIDW04ADD1   	S3C6410_SFR32(0x771000F0)		//   读/写 		窗口4 的缓冲区开始地址寄存器 			0x0000_0000
#define	rVIDW00ADD2   	S3C6410_SFR32(0x77100100)		//   读/写 		窗口0 的缓冲区开始地址寄存器 			0x0000_0000
#define	rVIDW01ADD2   	S3C6410_SFR32(0x77100104)		//   读/写 		窗口1 的缓冲区开始地址寄存器 			0x0000_0000
#define	rVIDW02ADD2   	S3C6410_SFR32(0x77100108)		//   读/写 		窗口2 的缓冲区开始地址寄存器 			0x0000_0000
#define	rVIDW03ADD2   	S3C6410_SFR32(0x7710010C)		//   读/写 		窗口3 的缓冲区开始地址寄存器 			0x0000_0000
#define	rVIDW04ADD2   	S3C6410_SFR32(0x77100110)		//   读/写 		窗口4 的缓冲区开始地址寄存器 			0x0000_0000
#define	rVIDINTCON0   	S3C6410_SFR32(0x77100130)		//   读/写 		指明视屏中断控制寄存器 					0x03F0_0000
#define	rVIDINTCON1   	S3C6410_SFR32(0x77100134)		//   读/写 		视频中断悬挂控制寄存器 					0x0000_0000
#define	rW1KEYCON0   	S3C6410_SFR32(0x77100140)		//   读/写 		Color key 控制寄存器 					0x0000_0000
#define	rW1KEYCON1   	S3C6410_SFR32(0x77100144)		//   读/写 		Color key 值寄存器 						0x0000_0000
#define	rW2KEYCON0   	S3C6410_SFR32(0x77100148)		//   读/写 		Color key 控制寄存器 					0x0000_0000
#define	rW2KEYCON1   	S3C6410_SFR32(0x7710014C)		//   读/写 		Color key 值寄存器 						0x0000_0000
#define	rW3KEYCON0   	S3C6410_SFR32(0x77100150)		//   读/写 		Color key 控制寄存器 					0x0000_0000
#define	rW3KEYCON1   	S3C6410_SFR32(0x77100154)		//   读/写 		Color key 值寄存器 						0x0000_0000
#define	rW4KEYCON0   	S3C6410_SFR32(0x77100158)		//   读/写 		Color key 控制寄存器 					0x0000_0000
#define	rW4KEYCON1   	S3C6410_SFR32(0x7710015C)		//   读/写 		Color key 值寄存器 						0x0000_0000
#define	rDITHMODE   	S3C6410_SFR32(0x77100170)		//   读/写 		抖动模式寄存器 							0x0000_0000
#define	rWIN0MAP   		S3C6410_SFR32(0x77100180)		//   读/写 		窗口颜色控制 							0x0000_0000
#define	rWIN1MAP   		S3C6410_SFR32(0x77100184)		//   读/写 		窗口颜色控制 							0x0000_0000
#define	rWIN2MAP   		S3C6410_SFR32(0x77100188)		//   读/写 		窗口颜色控制 							0x0000_0000
#define	rWIN3MAP   		S3C6410_SFR32(0x7710018C)		//   读/写 		窗口颜色控制 							0x0000_0000
#define	rWIN4MAP   		S3C6410_SFR32(0x77100190)		//   读/写 		窗口颜色控制 							0x0000_0000
#define	rWPALCON   		S3C6410_SFR32(0x771001A0)		//   读/写 		窗口调色板控制寄存器 					0x0000_0000
#define	rTRIGCON   		S3C6410_SFR32(0x771001A4)		//   读/写 		I80/RGB 触发控制寄存器 					0x0000_0000
#define	rITUIFCON0   	S3C6410_SFR32(0x771001A8)		//   读/写 		ITU(BT.601)接口控制 					0x0000_0000
#define	rI80IFCONA0   	S3C6410_SFR32(0x771001B0)		//   读/写 		主LDI 的I80 接口控制0 					0x0000_0000
#define	rI80IFCONA1   	S3C6410_SFR32(0x771001B4)		//   读/写 		子LDI 的I80 接口控制0 					0x0000_0000
#define	rI80IFCONB0   	S3C6410_SFR32(0x771001B8)		//   读/写 		主LDI 的I80 接口控制1 					0x0000_0000
#define	rI80IFCONB1   	S3C6410_SFR32(0x771001BC)		//   读/写 		子LDI 的I80 接口控制1 					0x0000_0000
#define	rLDI_CMDCON0   	S3C6410_SFR32(0x771001D0)		//   读/写 		I80 接口LDI 命令控制0 					0x0000_0000
#define	rLDI_CMDCON1   	S3C6410_SFR32(0x771001D4)		//   读/写 		I80 接口LDI 命令控制1 					0x0000_0000
#define	rSIFCCON0   	S3C6410_SFR32(0x771001E0)		//   读/写 		LCD I80 系统接口手册命令控制 			0x0000_0000
#define	rSIFCCON1   	S3C6410_SFR32(0x771001E4)		//   读/写 		LCD I80 系统接口手册命令数据写控制		0x0000_0000
#define	rSIFCCON2   	S3C6410_SFR32(0x771001E8)		//   读/写 		LCD I80 系统接口手册命令数据读控制2		未定义
#define	rLDI_CMD0   	S3C6410_SFR32(0x77100280)		//   读/写 		I80 接口LDI 命令0 						0x0000_0000
#define	rLDI_CMD1   	S3C6410_SFR32(0x77100284)		//   读/写 		I80 接口LDI 命令1 						0x0000_0000
#define	rLDI_CMD2   	S3C6410_SFR32(0x77100288)		//   读/写 		I80 接口LDI 命令2 						0x0000_0000
#define	rLDI_CMD3   	S3C6410_SFR32(0x7710028C)		//   读/写 		I80 接口LDI 命令3 						0x0000_0000
#define	rLDI_CMD4   	S3C6410_SFR32(0x77100290)		//   读/写 		I80 接口LDI 命令4 						0x0000_0000
#define	rLDI_CMD5   	S3C6410_SFR32(0x77100294)		//   读/写 		I80 接口LDI 命令5 						0x0000_0000
#define	rLDI_CMD6   	S3C6410_SFR32(0x77100298)		//   读/写 		I80 接口LDI 命令6 						0x0000_0000
#define	rLDI_CMD7   	S3C6410_SFR32(0x7710029C)		//   读/写 		I80 接口LDI 命令7 						0x0000_0000
#define	rLDI_CMD8   	S3C6410_SFR32(0x771002A0)		//   读/写 		I80 接口LDI 命令8 						0x0000_0000
#define	rLDI_CMD9   	S3C6410_SFR32(0x771002A4)		//   读/写 		I80 接口LDI 命令9 						0x0000_0000
#define	rLDI_CMD10   	S3C6410_SFR32(0x771002A8)		//   读/写 		I80 接口LDI 命令10 						0x0000_0000
#define	rLDI_CMD11   	S3C6410_SFR32(0x771002AC)		//   读/写 		I80 接口LDI 命令11 						0x0000_0000
#define	rW2PDATA01   	S3C6410_SFR32(0x77100300)		//   读/写 		索引0，1 的窗口2 调色板数据 			0x0000_0000
#define	rW2PDATA23   	S3C6410_SFR32(0x77100304)		//   读/写 		索引2，3 的窗口2 调色板数据 			0x0000_0000
#define	rW2PDATA45   	S3C6410_SFR32(0x77100308)		//   读/写 		索引4，5 的窗口2 调色板数据 			0x0000_0000
#define	rW2PDATA67   	S3C6410_SFR32(0x7710030C)		//   读/写 		索引6，7 的窗口2 调色板数据 			0x0000_0000
#define	rW2PDATA89   	S3C6410_SFR32(0x77100310)		//   读/写 		索引8，9 的窗口2 调色板数据 			0x0000_0000
#define	rW2PDATAAB   	S3C6410_SFR32(0x77100314)		//   读/写 		索引A，B 的窗口2 调色板数据 			0x0000_0000
#define	rW2PDATACD   	S3C6410_SFR32(0x77100318)		//   读/写 		索引C，D 的窗口2 调色板数据 			0x0000_0000
#define	rW2PDATAEF   	S3C6410_SFR32(0x7710031C)		//   读/写 		索引E，F 的窗口2 调色板数据 			0x0000_0000
#define	rW3PDATA01   	S3C6410_SFR32(0x77100320)		//   读/写 		索引0，1 的窗口3 调色板数据 			0x0000_0000
#define	rW3PDATA23   	S3C6410_SFR32(0x77100324)		//   读/写 		索引2，3 的窗口3 调色板数据 			0x0000_0000
#define	rW3PDATA45   	S3C6410_SFR32(0x77100328)		//   读/写 		索引4，5 的窗口3 调色板数据 			0x0000_0000
#define	rW3PDATA67   	S3C6410_SFR32(0x7710032C)		//   读/写 		索引6，7 的窗口3 调色板数据 			0x0000_0000
#define	rW3PDATA89   	S3C6410_SFR32(0x77100330)		//   读/写 		索引8，9 的窗口3 调色板数据 			0x0000_0000
#define	rW3PDATAAB   	S3C6410_SFR32(0x77100334)		//   读/写 		索引A，B 的窗口3 调色板数据 			0x0000_0000
#define	rW3PDATACD   	S3C6410_SFR32(0x77100338)		//   读/写 		索引C，D 的窗口3 调色板数据 			0x0000_0000
#define	rW3PDATAEF   	S3C6410_SFR32(0x7710033C)		//   读/写 		索引E，F 的窗口3 调色板数据 			0x0000_0000
#define	rW4PDATA01   	S3C6410_SFR32(0x77100340)		//   读/写 		索引0，1 的窗口4 调色板数据 			0x0000_0000
#define	rW4PDATA23   	S3C6410_SFR32(0x77100344)		//   读/写 		索引2，3 的窗口4 调色板数据 			0x0000_0000 



//SDI特殊寄存器
//系统地址寄存器,该寄存器包含用于DMA 传输的物理存储地址。
//		寄存器 			地址 							   读/写 		描述 						复位值
#define	rSYSAD0 		S3C6410_SFR32(0x7C200000)			// 读/写 	系统地址寄存器(0通道) 		0x0
#define	rSYSAD1 		S3C6410_SFR32(0x7C300000)			// 读/写 	系统地址寄存器(1通道) 		0x0
#define	rSYSAD2 		S3C6410_SFR32(0x7C400000)			// 读/写 	系统地址寄存器(2通道) 		0x0
//块大小寄存器,该寄存器用来设置一个数据块的字节数。
#define	rBLKSIZE0  		S3C6410_SFR16(0x7C200004)			// 读/写 	主机DMA缓冲区边界和传输块大小寄存器(0通道) 0x0
#define	rBLKSIZE1  		S3C6410_SFR16(0x7C300004)			// 读/写 	主机DMA缓冲区边界和传输块大小寄存器(1通道) 0x0
#define	rBLKSIZE2  		S3C6410_SFR16(0x7C400004)			// 读/写 	主机DMA缓冲区边界和传输块大小寄存器(2通道) 0x0
//块计数寄存器,该寄存器用来设置数据块的数量。
#define	rBLKCNT0   		S3C6410_SFR16(0x7C200006)			// 读/写 	为当前传输的块计数( 0通道) 0x0
#define	rBLKCNT1   		S3C6410_SFR16(0x7C300006)			// 读/写 	为当前传输的块计数( 1通道) 0x0
#define	rBLKCNT2   		S3C6410_SFR16(0x7C400006)			// 读/写 	为当前传输的块计数( 2通道) 0x0
//变元寄存器,该寄存器包含SD指令变元。
#define	rARGUMENT0  	S3C6410_SFR32(0x7C200008)			// 读/写 	指令变元寄存器(0通道) 0x0
#define	rARGUMENT1  	S3C6410_SFR32(0x7C300008)			// 读/写 	指令变元寄存器(1通道) 0x0
#define	rARGUMENT2  	S3C6410_SFR32(0x7C400008)			// 读/写 	指令变元寄存器(2通道) 0x0
/*传输模式寄存器
该寄存器用于控制数据传输的操作。在发出用于传输数据的指令前，或者是在发出一个恢复指令前，
主机控制驱动器将设置这个寄存器。当数据传输暂停和使用恢复指令恢复它之前，主机驱动器将保存这个
寄存器的值。为了防止数据丢失，在数据传输期间，主机控制器将为寄存器执行写保护。当指令禁止(DAT)
在当前状态寄存器中设置为1时，写入这个寄存器将被忽略*/
#define	rTRNMOD0    	S3C6410_SFR16(0x7C20000C)			// 读/写 	传输模式设置寄存器(0通道) 0x0
#define	rTRNMOD1    	S3C6410_SFR16(0x7C30000C)			// 读/写 	传输模式设置寄存器(1通道) 0x0
#define	rTRNMOD2    	S3C6410_SFR16(0x7C40000C)			// 读/写 	传输模式设置寄存器(2通道) 0x0
/*命令寄存器
这个寄存器包含SD指令变元。在写入这个寄存器前，主机驱动器将在当前状态寄存器检查指令禁止
(DAT)位和指令禁止(CMD)位。写入这个寄存器的高位字节引发SD指令产生。主机驱动器将负责写入这个寄
存器，因为当指令禁止(CMD)被设置，主机控制器不提供写保护。*/
#define	rCMDREG0     	S3C6410_SFR16(0x7C20000E)			// 读/写 	命令寄存器(通道0)。 0x0
#define	rCMDREG1    	S3C6410_SFR16(0x7C30000E)			// 读/写 	命令寄存器(通道1)。 0x0
#define	rCMDREG2    	S3C6410_SFR16(0x7C40000E)			// 读/写 	命令寄存器(通道2)。 0x0
//应答寄存器,该寄存器用来存储来自SD卡的应答。
#define	rRSPREG0_0    	S3C6410_SFR32(0x7C200010)			// ROC 		应答寄存器0(0通道)。 0x0
#define	rRSPREG1_0    	S3C6410_SFR32(0x7C200014)			// ROC 		应答寄存器1(0通道)。 0x0
#define	rRSPREG2_0    	S3C6410_SFR32(0x7C200018)			// ROC 		应答寄存器2(0通道)。 0x0
#define	rRSPREG3_0    	S3C6410_SFR32(0x7C20001C)			// ROC 		应答寄存器3(0通道)。 0x0
#define	rRSPREG0_1    	S3C6410_SFR32(0x7C300010)			// ROC	 	应答寄存器0(1通道)。 0x0
#define	rRSPREG1_1    	S3C6410_SFR32(0x7C300014)			// ROC 		应答寄存器1(1通道)。 0x0
#define	rRSPREG2_1    	S3C6410_SFR32(0x7C300018)			// ROC 		应答寄存器2(1通道)。 0x0
#define	rRSPREG3_1    	S3C6410_SFR32(0x7C30001C)			// ROC 		应答寄存器3(1通道)。 0x0
#define	rRSPREG0_2    	S3C6410_SFR32(0x7C400010)			// ROC 		应答寄存器0(2通道)。 0x0
#define	rRSPREG1_2    	S3C6410_SFR32(0x7C400014)			// ROC 		应答寄存器1(2通道)。 0x0
#define	rRSPREG2_2    	S3C6410_SFR32(0x7C400018)			// ROC 		应答寄存器2(2通道)。 0x0
#define	rRSPREG3_2    	S3C6410_SFR32(0x7C40001C)			// ROC		应答寄存器3(2通道)。 0x0
//缓冲区数据端口寄存器,32位数据端口寄存器来访问内部缓冲区。
#define	rBDATA0   		S3C6410_SFR32(0x7C200020)			//  读/写 	缓冲区数据 寄存器(0通道)。 0x0
#define	rBDATA1   		S3C6410_SFR32(0x7C300020)			//  读/写 	缓冲区数据 寄存器(1通道)。 0x0
#define	rBDATA2   		S3C6410_SFR32(0x7C400020)			//  读/写 	缓冲区数据 寄存器(2通道)。 0x0
//当前状态寄存器,该寄存器包含SD指令变元。
#define	rPRNSTS0   		S3C6410_SFR32(0x7C200024)			//  RO/ROC 	当前状态寄存器(通道0)。 0x000A0000
#define	rPRNSTS1   		S3C6410_SFR32(0x7C300024)			//  RO/ROC 	当前状态寄存器(通道1)。 0x000A0000
#define	rPRNSTS2   		S3C6410_SFR32(0x7C400024)			//  RO/ROC 	当前状态寄存器(通道2)。 0x000A0000
//主机控制寄存器,该寄存器包含SD指令变元。
#define	rHOSTCTL0   	S3C6410_SFR8(0x7C200028)			//  读/写 	主机控制寄存器(0通道)。 0x0
#define	rHOSTCTL1   	S3C6410_SFR8(0x7C300028)			//  读/写 	主机控制寄存器(1通道)。 0x0
#define	rHOSTCTL2   	S3C6410_SFR8(0x7C400028)			//  读/写 	主机控制寄存器(2通道)。 0x0
//电源控制寄存器,该寄存器包含SD指令变元。
#define	rPWRCON0   		S3C6410_SFR8(0x7C200029)			//  读/写 	电源控制寄存器(0通道)。 0x0
#define	rPWRCON1   		S3C6410_SFR8(0x7C300029)			//  读/写 	电源控制寄存器(1通道)。 0x0
#define	rPWRCON2   		S3C6410_SFR8(0x7C400029)			//  读/写 	电源控制寄存器(2通道)。 0x0
//块间隔控制寄存器,该寄存器包含SD指令变元。
#define	rBLKGAP0   		S3C6410_SFR8(0x7C20002A)			//  读/写 	块间隔控制寄存器(0 通道)。 0x0
#define	rBLKGAP1   		S3C6410_SFR8(0x7C30002A)			//  读/写 	块间隔控制寄存器(1 通道)。 0x0
#define	rBLKGAP2   		S3C6410_SFR8(0x7C40002A)			//  读/写 	块间隔控制寄存器(2 通道)。 0x0
/*唤醒控制寄存器
这个寄存器是强制性的，但是唤醒功能依靠主机控制器系统硬件和软件。主机驱动器维持总线上的电
压，通过在电源控制器中设置SD 总线电源为1，唤醒时间由卡中断得到。*/
#define	rWAKCON0   		S3C6410_SFR8(0x7C20002B)			//  读/写 	唤醒控制寄存器(0 通道)。 0x0
#define	rWAKCON1   		S3C6410_SFR8(0x7C30002B)			//  读/写 	唤醒控制寄存器(1 通道)。 0x0
#define	rWAKCON2   		S3C6410_SFR8(0x7C40002B)			//  读/写 	唤醒控制寄存器(2 通道)。 0x0
//时钟控制寄存器,在主机控制器初始化时，主机驱动器依据容限寄存器设置SDCLK 频率选择。
#define	rCLKCON0   		S3C6410_SFR16(0x7C20002C)			//  读/写 	时钟控制寄存器(0 通道)。 0x0
#define	rCLKCON1   		S3C6410_SFR16(0x7C30002C)			//  读/写 	时钟控制寄存器(1 通道)。 0x0
#define	rCLKCON2   		S3C6410_SFR16(0x7C40002C)			//  读/写 	时钟控制寄存器(2 通道)。 0x0
//超时控制寄存器,主机控制器初始化时，主机驱动器能根据容限寄存器设置数据超时计数器值。
#define	rTIMEOUTCON0   	S3C6410_SFR8(0x7C20002E)			//  读/写 	超时控制寄存器(0 通道)。 0x0
#define	rTIMEOUTCON1   	S3C6410_SFR8(0x7C30002E)			//  读/写 	超时控制寄存器(1 通道)。 0x0
#define	rTIMEOUTCON2   	S3C6410_SFR8(0x7C40002E)			//  读/写 	超时控制寄存器(2 通道)。 0x0
/*软件复位寄存器
写1 到这个寄存器的每一位，则产生复位脉冲。复位完成后，主机控制器清除每一位。由于完成软件
复位要花一些时间，SD 主机驱动器将确认这些位是0。*/
#define	rSWRST0   		S3C6410_SFR8(0x7C20002F)			//  读/写 	软件复位寄存器(0 通道)。 0x0
#define	rSWRST1   		S3C6410_SFR8(0x7C30002F)			//  读/写 	软件复位寄存器(1 通道)。 0x0
#define	rSWRST2   		S3C6410_SFR8(0x7C40002F)			//  读/写 	软件复位寄存器(2 通道)。 0x0
/*正常中断状态寄存器
正常中断状态启动影响这个寄存器的读，但是正常中断信号启动就没有这个影响。当正常中断信号启
动激活，并且至少一个状态位设置为1，一个中断产生。除了卡中断和错误中断位外，对于所有的位写1
到一个位来清除它；写0 到这些位保持不变。通过写入一个信号寄存器，超过一个状态能被清除。当卡停
止中断有效时，卡中断被清除，也就是，卡驱动器为中断提供服务的条件。*/
#define	rNORINTSTS0   	S3C6410_SFR16(0x7C200030)			//  ROC/RW1C 正常中断状态寄存器(0 通道)。 0x0
#define	rNORINTSTS1   	S3C6410_SFR16(0x7C300030)			//  ROC/RW1C 正常中断状态寄存器(1 通道)。 0x0
#define	rNORINTSTS2   	S3C6410_SFR16(0x7C400030)			//  ROC/RW1C 正常中断状态寄存器(2 通道)。 0x0
/*错误中断状态寄存器
通过错误中断状态启动寄存器能启动这个寄存器中的信号定义，但是不能通过错误中断信号启动寄存
器。当错误中断信号启动激活，并且至少一个状态设置为1，中断产生。写1 清除这位，写0 保持这位不
变。在一个寄存器写，超过一个状态能被清除。*/
#define	rERRINTSTS0   	S3C6410_SFR16(0x7C200032)			//  ROC/RW1C 错误中断状态寄存器(0 通道)。 0x0
#define	rERRINTSTS1   	S3C6410_SFR16(0x7C300032)			//  ROC/RW1C 错误中断状态寄存器(1 通道)。 0x0
#define	rERRINTSTS2   	S3C6410_SFR16(0x7C400032)			//  ROC/RW1C 错误中断状态寄存器(2 通道)。 0x0
//正常中断状态启动寄存器,设置1 来使中断状态启动。
#define	rNORINTSTSEN0   S3C6410_SFR16(0x7C200034)			//  读/写 	正常中断状态启动寄存器(0 通道)。 0x0
#define	rNORINTSTSEN1   S3C6410_SFR16(0x7C300034)			//  读/写 	正常中断状态启动寄存器(1 通道)。 0x0
#define	rNORINTSTSEN2   S3C6410_SFR16(0x7C400034)			//  读/写 	正常中断状态启动寄存器(2 通道)。 0x0
//错误中断状态启动寄存器,设置1 来激活错误中断状态。
#define	rERRINTSTSEN0   S3C6410_SFR16(0x7C200036)			//  读/写 	错误中断状态启动寄存器(0 通道)。 0x0
#define	rERRINTSTSEN1   S3C6410_SFR16(0x7C300036)			//  读/写 	错误中断状态启动寄存器(1 通道)。 0x0
#define	rERRINTSTSEN2   S3C6410_SFR16(0x7C400036)			//  读/写 	错误中断状态启动寄存器(2 通道)。 0x0
//正常中断信号启动寄存器,该寄存器用于选择主机系统哪一个中断状态作为中断。这些中断状态将共享相同的1 位中断线。
#define	rNORINTSIGEN0   S3C6410_SFR16(0x7C200038)			//  读/写 	正常中断信号启动寄存器(0 通道)。 0x0
#define	rNORINTSIGEN1   S3C6410_SFR16(0x7C300038)			//  读/写 	正常中断信号启动寄存器(1 通道)。 0x0
#define	rNORINTSIGEN2   S3C6410_SFR16(0x7C400038)			//  读/写 	正常中断信号启动寄存器(2 通道)。 0x0
//错误中断信号启动寄存器,该寄存器用于选择主机系统哪一个中断状态作为中断。所有的状态位共享相同的1 位中断线。
#define	rERRINTSIGEN0   S3C6410_SFR16(0x7C20003A)			//  读/写 	错误中断信号启动寄存器(0 通道)。 0x0
#define	rERRINTSIGEN1   S3C6410_SFR16(0x7C30003A)			//  读/写 	错误中断信号启动寄存器(1 通道)。 0x0
#define	rERRINTSIGEN2   S3C6410_SFR16(0x7C40003A)			//  读/写 	错误中断信号启动寄存器(2 通道)。 0x0
//自动CMD12 错误状态寄存器
//当自动CMD12 错误状态被设置，主机驱动器检查这个寄存器来鉴别自动CMD12 显示的是什么类型错误。
//这个寄存器只有当自动CMD12 错误被设置时有效。
#define	rACMD12ERRSTS0  S3C6410_SFR16(0x7C20003C)			//  ROC 	自动CMD12 错误状态寄存器(0 通道)。 0x0
#define	rACMD12ERRSTS1  S3C6410_SFR16(0x7C30003C)			//  ROC 	自动CMD12 错误状态寄存器(1 通道)。 0x0
#define	rACMD12ERRSTS2  S3C6410_SFR16(0x7C40003C)			//  ROC 	自动CMD12 错误状态寄存器(2 通道)。 0x0
//容限寄存器
//对于主机控制器执行，该寄存器为主机驱动器提供了详细信息。上电初始化期间，主机控制器执行这些值作为固定的或者从闪存载入。
#define	rCAPAREG0   	S3C6410_SFR32(0x7C200040)			//  读/写 	容限寄存器(0 通道)。 0x05E00080
#define	rCAPAREG1   	S3C6410_SFR32(0x7C300040)			//  读/写 	容限寄存器(1 通道)。 0x05E00080
#define	rCAPAREG2   	S3C6410_SFR32(0x7C400040)			//  读/写 	容限寄存器(2 通道)。 0x05E00080
/*最大电流容限寄存器
这个寄存器指示最大的电流容限。如果电压支持在容限寄存器中被设置，那么这个最大电流容限值是
很有意义的。如果主机系统通过另一种方法提供这个信息，所有的最大电流容限寄存器将是0。*/
#define	rMAXCURR0   	S3C6410_SFR32(0x7C200048)			//  HWInit 	最大电流容限寄存器(0 通道)。 0x0
#define	rMAXCURR1   	S3C6410_SFR32(0x7C300048)			//  HWInit 	最大电流容限寄存器(1 通道)。 0x0
#define	rMAXCURR2   	S3C6410_SFR32(0x7C400048)			//  HWInit 	最大电流容限寄存器(2 通道)。 0x0
//控制寄存器2,该寄存器包含SD 指令变元。
#define	rCONTROL2_0   	S3C6410_SFR32(0x7C200080)			//  读/写	控制寄存器2 (0 通道)。 0x0
#define	rCONTROL2_1   	S3C6410_SFR32(0x7C300080)			//  读/写 	控制寄存器2 (1 通道)。 0x0
#define	rCONTROL2_2   	S3C6410_SFR32(0x7C400080)			//  读/写 	控制寄存器2 (2 通道)。 0x0
//控制寄存器3寄存器
#define	rCONTROL3_0   	S3C6410_SFR32(0x7C200084)			//  读/写 	FIFO 中断控制(控制寄存器3) (0 通道)。 0x7F5F3F1F
#define	rCONTROL3_1   	S3C6410_SFR32(0x7C300084)			//  读/写 	FIFO 中断控制(控制寄存器3) (1 通道)。 0x7F5F3F1F
#define	rCONTROL3_2   	S3C6410_SFR32(0x7C400084)			//  读/写 	FIFO 中断控制(控制寄存器3) (2 通道)。 0x7F5F3F1F
//主机控制器版本寄存器,该寄存器包含SD 指令变元。
#define	rHCVER0   		S3C6410_SFR16(0x7C2000FE)			//  HWInit 	主机控制版本寄存器（0 通道）。 0x1300
#define	rHCVER1   		S3C6410_SFR16(0x7C3000FE)			//  HWInit 	主机控制版本寄存器（1 通道）。 0x1300
#define	rHCVER2   		S3C6410_SFR16(0x7C4000FE)			//  HWInit 	主机控制版本寄存器（2 通道）。 0x1300


//JPEG
#define JPEG_BASE		0x78800000
//相关寄存器定义
#define	JPGMOD       	S3C6410_SFR32(JPEG_BASE+0x000)		//处理模式寄存器
#define	JPGSTS       	S3C6410_SFR32(JPEG_BASE+0x004)		//操作状态寄存器
#define	JPGQHNO      	S3C6410_SFR32(JPEG_BASE+0x008)		//量化表数目寄存器和哈夫曼表数目寄存器
#define	JPGDRI       	S3C6410_SFR32(JPEG_BASE+0x00C)		//微控制器嵌入RST标记
#define	JPGY         	S3C6410_SFR32(JPEG_BASE+0x010)		//垂直分辨率
#define	JPGX        	S3C6410_SFR32(JPEG_BASE+0x014)		//水平分辨率
#define	JPGCNT      	S3C6410_SFR32(JPEG_BASE+0x018)		//压缩数据的字节数
#define	JPGIRQEN   		S3C6410_SFR32(JPEG_BASE+0x01C)		//中断设置寄存器
#define	JPGIRQ        	S3C6410_SFR32(JPEG_BASE+0x020)		//中断状态寄存器 
#define	JQTBL0        	S3C6410_SFR32_P(JPEG_BASE+0x400)	//8位量化表0	64
#define	JQTBL1        	S3C6410_SFR32_P(JPEG_BASE+0x500)	//8位量化表1	64
#define	JQTBL2        	S3C6410_SFR32_P(JPEG_BASE+0x600)	//8位量化表2	64
#define	JQTBL3        	S3C6410_SFR32_P(JPEG_BASE+0x700)	//8位量化表3	64
#define	JHDCTBL0      	S3C6410_SFR32_P(JPEG_BASE+0x800)	//每个代码长度的数量	16
#define	JHDCTBLG0    	S3C6410_SFR32_P(JPEG_BASE+0x840)	//用于发生率的群阶数	12
#define	JHACTBL0      	S3C6410_SFR32_P(JPEG_BASE+0x880)	//每个代码长度的数量	16
#define	JHACTBLG0    	S3C6410_SFR32_P(JPEG_BASE+0x8c0)	//用于发生率/组数量的群阶数	162
#define	JHDCTBL1      	S3C6410_SFR32_P(JPEG_BASE+0xc00)	//每个代码长度的数量	16
#define	JHDCTBLG1    	S3C6410_SFR32_P(JPEG_BASE+0xc40)	//用于发生率的群阶数	12
#define	JHACTBL1      	S3C6410_SFR32_P(JPEG_BASE+0xc80)	//每个代码长度的数量	16
#define	JHACTBLG1    	S3C6410_SFR32_P(JPEG_BASE+0xcc0)	//用于发生率/组数量的群阶数	162
#define	JIMGADDR0     	S3C6410_SFR32(JPEG_BASE+0x1000)		//源或目的图像地址1
#define	JIMGADDR1     	S3C6410_SFR32(JPEG_BASE+0x1004)		//源或目的图像地址2
#define	JHUFADDR0     	S3C6410_SFR32(JPEG_BASE+0x1008)		//源或目的JPEG 文件地址1
#define	JHUFADDR1     	S3C6410_SFR32(JPEG_BASE+0x100c)		//源或目的JPEG 文件地址2
#define	JSTART        	S3C6410_SFR32(JPEG_BASE+0x1010)		//开始JPEG过程
#define	JRSTART      	S3C6410_SFR32(JPEG_BASE+0x1014)		//重新开始JPEG过程
#define	RESET_CON 		S3C6410_SFR32(JPEG_BASE+0x1018)		//软件复位
#define	JPG_CON     	S3C6410_SFR32(JPEG_BASE+0x101c)		//JPEG控制寄存器
#define	JCOEF1       	S3C6410_SFR32(JPEG_BASE+0x1020)		//系数值RGB ? YcbC 的转换器
#define	JCOEF2       	S3C6410_SFR32(JPEG_BASE+0x1024)		//系数值RGB ? YcbC 的转换器
#define	JCOEF3       	S3C6410_SFR32(JPEG_BASE+0x1028)		//系数值RGB ? YcbC 的转换器
#define	JMISC       	S3C6410_SFR32(JPEG_BASE+0x102c)		//杂项
#define	JFRAME_INTV   	S3C6410_SFR32(JPEG_BASE+0x1030)		//帧间的间隔时间值,基础时钟周期
//后面这些寄存器在手册上面找不到,但是官方测试程序中出现了
#define	RESUME        	S3C6410_SFR32(JPEG_BASE+0x1038)		//
#define	IMGADDR_END   	S3C6410_SFR32(JPEG_BASE+0x103c)		//	
#define	HUFADDR_END   	S3C6410_SFR32(JPEG_BASE+0x1040)		//	
#define	HUFADDR_MNT   	S3C6410_SFR32(JPEG_BASE+0x1044)		//



//图形旋转 内存映射
//寄存器 地址 读/写 描述 复位值
#define	rGR_CTRLCFG 		S3C6410_SFR32(0x77200000) 		//读/写 	旋转器控制寄存器 						0x0000_0000
#define	rGR_SRCADDRREG0 	S3C6410_SFR32(0x77200004) 		//读/写 	旋转器源图像（RGB 或Y 组成部分）地址寄存器 	0x0000_0000
#define	rGR_SRCADDRREG1 	S3C6410_SFR32(0x77200008) 		//读/写 	旋转器源图像（CB 组成部分）地址寄存器		0x0000_0000
#define	rGR_SRCADDRREG2 	S3C6410_SFR32(0x7720000C) 		//读/写 	旋转器源图像（CR 组成部分）地址寄存器		0x0000_0000
#define	rGR_SRCSIZEREG 		S3C6410_SFR32(0x77200010) 		//读/写 	旋转器源图像尺寸寄存器 					0x0000_0000
#define	rGR_DESTADDRREG0 	S3C6410_SFR32(0x77200018) 		//读/写 	旋转器目标图像（RGB 或Y 成分）地址寄存器	0x0000_0000
#define	rGR_DESTADDRREG1 	S3C6410_SFR32(0x7720001C) 		//读/写 	旋转器目标图像（CB 组成部分）地址寄存器		0x0000_0000
#define	rGR_DESTADDRREG2 	S3C6410_SFR32(0x77200020) 		//读/写 	旋转器目标图像（CR 组成部分）地址寄存器		0x0000_0000
#define	rGR_STATCFG 		S3C6410_SFR32(0x7720002C) 		//读 	旋转器状态寄存器 						0x0000_0000


 
#endif
