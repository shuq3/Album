/*************************************************************************************************************
 * 文件名:	sdio.c
 * 功能:		S3C6410 SDIO底层驱动函数
 * 作者:		cp1300@139.com
 * 创建时间:	2012年5月16日
 * 最后修改时间:2012年9月17日
 * 详细:		sdio相关操作函数
*************************************************************************************************************/
#include "sdio.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//配置
//块大小寄存器(0通道)BLKSIZE0
#define BLKSIZE0_BLKSIZE			0x0200		//块大小为512字节			
//传输模式寄存器(0通道)TRNMOD0
#define TRNMOD0_CCSCON				0			//正常模式
#define TRNMOD0_MUL1SIN0			(0 << 5)	//默认为单区段操作模式
#define TRNMOD0_RD1WT0				(1 << 4)	//默认为读操作模式
#define TRNMOD0_ENACMD12			(0 << 2)	//自动CMD12禁止,多区段操作完成后主机自动发出停止命令
#define TRNMOD0_ENBLKCNT			(0 << 1)	//块计数器无效
#define TRNMOD0_ENDMA				(0 << 0)	//DMA无效
//主机控制寄存器(0通道)HOSTCTL0
#define HOSTCTL0_WIDE8				(0 << 5)	//这个为宽度被位1指定 (数据传输宽度)
#define HOSTCTL0_DMASEL				(0 << 3)	//SDMA
#define HOSTCTL0_OUTEDGEINV			(0 << 2)	//时钟上升沿沿数据有效		//这个寄存器有歧义,不知道到底是干啥用的
#define HOSTCTL0_WIDE4				(0 << 4)	//数据传输宽度。1BIT模式
//电源控制寄存器(0通道)PWRCON0
#define PWRCON0_SELPWRLVL			(7 << 1)	//3.3V电源模式
#define PWRCON0_PWRON				(1 << 0)	//电源打开
//容限寄存器(0 通道)CAPAREG0
#define CAPAREG0_V18				(1 << 26)	//电压支持1.8V
#define CAPAREG0_V30				(0 << 25)	//电压不支持3v
#define CAPAREG0_V33				(1 << 24)	//电压支持3.3V
#define CAPAREG0_SUSRES				(1 << 23)	//支持暂停/恢复操作
#define CAPAREG0_DMA				(1 << 22)	//支持DMA
#define CAPAREG0_HSPD				(1 << 21)	//支持高速模式
#define CAPAREG0_ADMA2				(0 << 19)	//不支持DMA2
#define CAPAREG0_MAXBLKLEN			(0 << 16)	//最大块大小为512B
#define CAPAREG0_BASECLK			(25 << 8)	//SD基础始终25MHz
#define CAPAREG0_TOUTUNIT			(0 << 7)	//超时时钟单位KHZ
#define CAPAREG0_TOUTCLK			(10 << 0)	//超时时钟频率为10KHZ
//最大电流容限寄存器(0 通道)MAXCURR0
#define MAXCURR0_MAXCURR18			(10 << 16)	//对于1.8V,最大电流为40MA
#define MAXCURR0_MAXCURR30			(10 << 8)	//对于3.0V,最大电流为40MA
#define MAXCURR0_MAXCURR33			(10 << 0)	//对于3.3V,最大电流为40MA
//控制寄存器2 CONTROL2_0
#define CONTROL2_0_ENSTAASYNCCLR	(0 << 31)	//该位可以使正常和错误中断的异步明确启用状态位
#define CONTROL2_0_ENCMDCNFMSK		(0 << 30)	//不屏蔽指令冲突
#define CONTROL2_0_CDINVRXD3		(0 << 29)	//卡检测信号倒置对于RX_DAT[3]。禁止
#define CONTROL2_0_SELCARDOUT		(0 << 28)	//卡移除条件是“无卡插入” 状态。
#define CONTROL2_0_FLTCLKSEL		(8 << 24)	//滤波器时钟 (iFLTCLK) 选择。
#define CONTROL2_0_ENFBCLKTX		(0 << 15)	//反馈时钟禁止,对于发送数据时钟
#define CONTROL2_0_ENFBCLKRX		(0 << 14)	//反馈时钟禁止,对于接收数据时钟
#define CONTROL2_0_SDCDSEL			(0 << 13)	//nSDCD 用于SD 卡检测信号
#define CONTROL2_0_SDSIGPC			(0 << 12)	//同步控制输出有效信号
#define CONTROL2_0_ENBUSYCHKTXSTART (0 << 11)	//发送数据启动状态前忙碌状态检测。
#define CONTROL2_0_DFCNT			(0 << 9)	//反跳滤波器计数16 iSDCLK
#define CONTROL2_0_ENCLKOUTHOLD		(1 << 8)	//SDCLK 操作有效。
#define CONTROL2_0_RWAITMODE		(0 << 7)	//主机控制器释放读等待状态（自动）
#define CONTROL2_0_DISBUFRD			(0 << 6)	//正常模式，用0x20 寄存器使用者可以读缓冲区（FIFO）数据
//HCLK = 128MHZ		EPLL = 24MHZ
#define CONTROL2_0_SELBASECLK		(2 << 4)	//基础时钟源选择。00 或01 = HCLK，10 = EPLL 输出时钟（来自系统）11 = 外部时钟源（XTI 或XEXTCLk）
#define CONTROL2_0_PWRSYNC			(0 << 3)	//不同步，控制输入有效信号（指令，数据）
#define CONTROL2_0_ENCLKOUTMSKCON	(0 << 1)	//当卡插入被清除时，SDCLK 输出时钟屏蔽。当处于无卡状态时，设置该区域为高位来停止SDCLK。
#define CONTROL2_0_HWINITFIN		(1 << 0)	//SD 主机控制器硬件初始化完成。
//时钟控制寄存器(0 通道)CLKCON0
#define CLKCON0_SELFREQ				(0x80 << 8)	//SDCLK频率最低
#define CLKCON0_ENSDCLK				(1 << 2)	//SD 时钟启动。
#define CLKCON0_ENINTCLK			(1 << 0)	//中断时钟启动。
//超时控制寄存器(0 通道)TIMEOUTCON0



/*******************************************************************************
* Function Name  : SDIO_DeInit
* Description    : Deinitializes the SDIO peripheral registers to their default
*                  reset values.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
// use
void SDIO_DeInit(void)
{
	//初始化硬件IO	
	rGPGCON = 0x2222222;	//初始化IO为SDIO模式
	rGPGPUD = 0;			//禁止上下拉
		
	//时钟控制寄存器配置
	SDIO0->CLKCON = CLKCON0_SELFREQ + CLKCON0_ENSDCLK + CLKCON0_ENINTCLK;
	//主机控制寄存器配置
	SDIO0->HOSTCTL = HOSTCTL0_WIDE8 + HOSTCTL0_DMASEL + HOSTCTL0_OUTEDGEINV + HOSTCTL0_WIDE4;
	//容限寄存器配置
	SDIO0->CAPAREG = CAPAREG0_V18 + CAPAREG0_V30 + CAPAREG0_V33 + CAPAREG0_SUSRES + CAPAREG0_DMA + CAPAREG0_HSPD + CAPAREG0_ADMA2 +\
					CAPAREG0_MAXBLKLEN + CAPAREG0_BASECLK + CAPAREG0_TOUTUNIT + CAPAREG0_TOUTCLK;
	//控制寄存器2配置				
	SDIO0->CONTROL2 = CONTROL2_0_ENSTAASYNCCLR + CONTROL2_0_ENCMDCNFMSK + CONTROL2_0_CDINVRXD3 + CONTROL2_0_SELCARDOUT + CONTROL2_0_FLTCLKSEL + \
					CONTROL2_0_ENFBCLKTX + CONTROL2_0_ENFBCLKRX + CONTROL2_0_SDCDSEL + CONTROL2_0_SDSIGPC + CONTROL2_0_ENBUSYCHKTXSTART + \
	 				CONTROL2_0_DFCNT + CONTROL2_0_ENCLKOUTHOLD + CONTROL2_0_RWAITMODE + CONTROL2_0_DISBUFRD + CONTROL2_0_SELBASECLK + \
					CONTROL2_0_PWRSYNC + CONTROL2_0_ENCLKOUTMSKCON + CONTROL2_0_HWINITFIN;	
	//传输模式寄存器配置
	SDIO0->TRNMOD = TRNMOD0_CCSCON + TRNMOD0_MUL1SIN0 + TRNMOD0_RD1WT0 + TRNMOD0_ENACMD12 + TRNMOD0_ENBLKCNT + TRNMOD0_ENDMA;	
	//超时控制寄存器(0 通道)
	SDIO0->TIMEOUTCON	= 0x0e;		//超时设置最大			
	//电源控制寄存器配置
	SDIO0->PWRCON = PWRCON0_SELPWRLVL + PWRCON0_PWRON;
	//块间隔寄存器
	SDIO0->BLKGAP = 0;
	//开启卡插入+卡移除+传输完成+指令完成中断状态+数据超时错误+命令索引错误+指令最后位错误+指令超时错误+指令CRC + 缓冲区读就绪 + 写缓冲区就绪
	SDIO_FlagConfig(SDIO_FLAG_CARDREMOVAL | SDIO_FLAG_CARDINSERTION | SDIO_FLAG_TRANSFER | SDIO_FLAG_COMMANDEND | SDIO_FLAG_DATACRC |
					SDIO_FLAG_DATATIMEOUT | SDIO_FLAG_COMMANDINDEX | SDIO_FLAG_COMMANDCRC | SDIO_FLAG_COMMANDTIMEOUT | SDIO_FLAG_BUFFREAD | SDIO_FLAG_BUFFWRITE,ENABLE);
	//使能卡插入拔出中断
	SDIO_FlagITConfig(SDIO_FLAG_CARDREMOVAL | SDIO_FLAG_CARDINSERTION,ENABLE);	
	SDIO_ClearFlag(SDIO_FLAG_ALL);	//清除所有中断标志			
}


// use
void SDIO_FlagITConfig(u32 SDIO_FLAG, u8 EN)
{
	if(EN)	//中断使能
		SDIO0->INTSEGEN |= SDIO_FLAG;
	else
		SDIO0->INTSEGEN &= ~SDIO_FLAG;
		
}

// use
void SDIO_FlagConfig(u32 SDIO_FLAG, u8 EN)
{
	if(EN)	//状态使能
		SDIO0->INTSTSEN |= SDIO_FLAG;
	else
		SDIO0->INTSTSEN &= ~SDIO_FLAG;
		
}

/*************************************************************************************************************************
*函数        :	void SD_SendCommand(vu8 CmdIdx,vu32 CmdArg,vu8 CmdTyp,vu8 Misc)
*功能        :	向SD卡发送一个命令
*参数        :	CmdIdx:指令索引;CmdArg:命令参数;CmdMisc:其它杂项设置,详见说明
*返回        :	无
*依赖        : 	底层宏定义
*作者        :	陈鹏
*时间        :	20120507
*最后修改时间:	20120507
*说明        :	写SD命令寄存器;
				指令索引:这些位设置为SD存储卡物理层规范中指令格式的第40到45位和SDIO卡规范中指定的指令数(CMD0-63, ACMD0-63)。
				
				杂项:	SDIO_Type_Default	一般为0
						SDIO_Type_Pend      写总线暂停
						SDIO_Type_FS		功能选择
						SDIO_Type_IT		中断模式
						
						SDIO_CMDIndexEn     SDIO指令索引使能
						SDIO_CMDCrcEn       SDIO指令CRC校验使能
						
						SDIO_Response_No           无应答
						SDIO_Response_Short        短应答	48
						SDIO_Response_Long        长应答	136
						SDIO_Response_ShortBusy    短应答+检测繁忙情况
*************************************************************************************************************************/
// use
void SDIO_SendCommand(vu8 CmdIdx,vu32 CmdArg,vu8 CmdMisc)
{	
	volatile u16 i;	
	u16 temreg = 0;
	
	while(SDIO0->PRNSTS & BIT0);//等待命令线空闲
	while(SDIO0->PRNSTS & BIT1);//等待命令线空闲
	SDIO_ClearFlag(SDIO_FLAG_ALL);	//清除所有状态寄存
	
	temreg = CmdIdx;
	temreg <<= 8;
	temreg |= CmdMisc;
	SDIO0->ARGUMENT = CmdArg;	//先写入命令参数
	for(i = 0;i < 400;i ++);	//不加延时会出现各种错误
	SDIO0->CMDREG = temreg;	//再写入命令索引以及类型等参数
}





/*************************************************************************************************************************
*函数        :	u32 SD_GetResponse(u8 SDIO_RESP)
*功能        :	获取SD卡的应答
*参数        :	Rep:应答数据存放的位置
				SDIO_RESP1: Response Register 1
*               SDIO_RESP2: Response Register 2
*               SDIO_RESP3: Response Register 3
*               SDIO_RESP4: Response Register 4  
*返回        :	返回应答
*依赖        : 	底层宏定义
*作者        :	陈鹏
*时间        :	20120507
*最后修改时间:	20120507
*说明        :	存放应答数据的缓冲区为128BIT;4个32bit;
				指令应答。下表27-4为每一个应答描述了从SD总线到寄存器的指令映射。在这个表中，在表中 R[]指出在SD总线上传输的应答数据的范围，
				REP[]指出应答寄存器中位的范围。128位应答位的顺序： {RSPREG3, RSPREG2, RSPREG1, RSPREG0}
*************************************************************************************************************************/
// use
u32 SDIO_GetResponse(u8 SDIO_RESP)
{
	return (SDIO0->RSPREG[SDIO_RESP]);
}


u32 SDIO_ReadData(void)
{ 
  	return SDIO0->BDATA;		//从缓冲区读数据
}


//void SDIO_WriteData(u32 Data)
//{ 
//	SDIO0->BDATA = Data;		//写数据到缓冲区
//}

// use
u8 SDIO_GetFlagStatus(u32 SDIO_FLAG)	//获取状态中断标志
{ 
	return ((SDIO_STATUS & SDIO_FLAG) ? 1 : 0);
}

//use
void SDIO_ClearFlag(u32 SDIO_FLAG)	//清除状态中断标志
{ 
	SDIO_STATUS |= SDIO_FLAG;
}




/*************************************************************************************************************************
* 函数	:	void SDIO_SetFIFOInterruptAddress(u8 FIFOxAdd,u8 WordAdd)
* 功能	:	设置FIFO中断触发位置
* 参数	:	FIFOxAdd:FIFO地址选择
			WordAdd:触发位置选择,单位为字,共512B,也就是1-128字
* 返回	:	SD_OK:成功,其它见SD Card Error code.
* 依赖	:	底层寄存器操作函数
* 作者	:	陈鹏
* 时间	:	20120516
* 最后修改时间 : 20120516
* 说明	: 	无
*************************************************************************************************************************/
void SDIO_SetFIFOInterruptAddress(u8 FIFOxAdd,u8 WordAdd)
{
	if(WordAdd > 128)
		WordAdd = 128;
	switch (FIFOxAdd)
	{
		case FIFO_A0:
		{
			SDIO0->CONTROL3 &= ~(0x1f);
			SDIO0->CONTROL3 |= WordAdd;
		}break;
		case FIFO_A1:
		{
			SDIO0->CONTROL3 &= ~(0x1f << 8);
			SDIO0->CONTROL3 |= WordAdd << 8;
		}break;
		case FIFO_A2:
		{
			SDIO0->CONTROL3 &= ~(0x1f << 16);
			SDIO0->CONTROL3 |= WordAdd << 16;
		}break;
		case FIFO_A3:
		{
			SDIO0->CONTROL3 &= ~(0x1f << 24);
			SDIO0->CONTROL3 |= WordAdd << 24;
		}break;
		default : break;
	}
}


/*************************************************************************************************************************
* 函数	:	void SDIO_CLKFrequencySelect(u8 SDCLK)	
* 功能	:	设置SDIO时钟分频系数
* 参数	:	SDCLK:设置SDIO时钟分频系数
* 返回	:	SD_OK:成功,其它见SD Card Error code.
* 依赖	:	底层寄存器操作函数
* 作者	:	陈鹏
* 时间	:	20120516
* 最后修改时间 : 20120516
* 说明	: 	高速基础时钟为50MHz
			低速基础时钟为25MHz
*************************************************************************************************************************/
void SDIO_CLKFrequencySelect(u8 SDCLK)	
{
	SDIO_SDClkDisable();			//时钟停止
	SDIO0->CAPAREG &= ~(0x3f << 8);	//清除设置
	SDIO0->CAPAREG |= (50 << 8);	//这一位设置其实没作用
	SDIO0->CLKCON &= ~(0xff << 8);	//清除
	SDIO0->CLKCON |= (SDCLK << 8);	//设置基础时钟分频系数
	SDIO_SDClkEnable();				//时钟使能
	while(!(SDIO0->CLKCON & BIT0));	//等待时钟稳定
}



/*************************************************************************************************************************
* 函数	:	void SDIO_SoftwareReset(u32 SDIO_RST)
* 功能	:	设置SDIO软件复位
* 参数	:	SDIO_RST:
					SDIO_RSTDAT	//复位DAT线
					SDIO_RSTCMD	//复位CMD线
					SDIO_RSTALL	//复位所有
* 返回	:	SD_OK:成功,其它见SD Card Error code.
* 依赖	:	底层寄存器操作函数
* 作者	:	陈鹏
* 时间	:	20120516
* 最后修改时间 : 20120516
* 说明	: 	无
*************************************************************************************************************************/

// use
void SDIO_SoftwareReset(u32 SDIO_RST)
{
	SDIO0->SWRST |= SDIO_RST;			//复位
	while(SDIO0->SWRST & SDIO_RST);		//等待复位完成
}



/*************************************************************************************************************************
*函数        :	void SDIO_SetTimeOutClock(u8 Unit,u8 Frequency)
*功能        :	超时时钟设置
*参数        :	Unit:超时时钟单位选择
							TIME_OUT_UNIT_KHZ(0):超时时钟单位为KHZ
							TIME_OUT_UNIT_MHZ(1):超时时钟单位为MHZ
				Frequency:时钟频率:1~63
*返回        :	返回应答
*依赖        : 	底层宏定义
*作者        :	陈鹏
*时间        :	20120521
*最后修改时间:	20120521
*说明        :	配置指令超时时间
*************************************************************************************************************************/
void SDIO_SetTimeOutClock(u8 Unit,u8 Frequency)
{
	SDIO0->CAPAREG &= ~(0xff);	//清除原先设置
	if(Unit)					//配置超时时钟单位:MHZ
		SDIO0->CAPAREG |= (1 << 7);
	if(Frequency > 63)			//最大只能设置为63
		Frequency = 63;
	SDIO0->CAPAREG |= Frequency;
}


