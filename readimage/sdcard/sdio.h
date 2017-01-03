#ifndef _SDIO_H_
#define _SDIO_H_

#include "system.h"



#define FIFO_A0	0	//FIFO中断地址0
#define FIFO_A1	1	//FIFO中断地址1
#define FIFO_A2	2	//FIFO中断地址2
#define FIFO_A3	3	//FIFO中断地址3


//SDIO总线宽度设置
#define SDIO_BusWide_1b                     ((u8)0)
#define SDIO_BusWide_4b                     ((u8)1)
#define SDIO_BusWide_8b                     ((u8)2)

//SDIO 命令杂项设置
//SDIO响应类型
#define SDIO_Response_No                    ((u8)0)	//无应答
#define SDIO_Response_Long                  ((u8)1)	//长应答	136
#define SDIO_Response_Short                 ((u8)2)	//短应答	48
#define SDIO_Response_ShortBusy             ((u8)3)	//短应答+检测繁忙情况
//其它设置
#define SDIO_CMDIndexEn                     ((u8)1 << 4)//SDIO指令索引使能
#define SDIO_CMDCrcEn                       ((u8)1 << 3)//SDIO指令CRC校验使能
#define SDIO_DataSelect						((u8)1 << 5)//SDIO当前数据选择
//SDIO指令类型CmdTyp
#define SDIO_Type_Default					((u8)0 << 6)//一般为0
#define SDIO_Type_Pend                     	((u8)1 << 6)//写总线暂停
#define SDIO_Type_FS						((u8)2 << 6)//功能选择
#define SDIO_Type_IT					  	((u8)3 << 6)//中断模式



//SDIO响应寄存器选择
#define SDIO_RESP1                          ((u8)0)
#define SDIO_RESP2                          ((u8)1)
#define SDIO_RESP3                          ((u8)2)
#define SDIO_RESP4                          ((u8)3)





/* SDIO Data Block Size ------------------------------------------------------*/
#define SDIO_DataBlockSize_1b               ((u16)1)
#define SDIO_DataBlockSize_2b               ((u16)2)
#define SDIO_DataBlockSize_4b               ((u16)3)
#define SDIO_DataBlockSize_8b               ((u16)4)
#define SDIO_DataBlockSize_16b              ((u16)16)
#define SDIO_DataBlockSize_32b              ((u16)32)
#define SDIO_DataBlockSize_64b              ((u16)64)
#define SDIO_DataBlockSize_128b             ((u16)128)
#define SDIO_DataBlockSize_256b             ((u16)256)
#define SDIO_DataBlockSize_512b             ((u16)512)
#define SDIO_DataBlockSize_1024b            ((u16)1024)
#define SDIO_DataBlockSize_2048b            ((u16)2048)
#define SDIO_DataBlockSize_4096b            ((u16)4096)
#define SDIO_DataBlockSize_8192b            ((u16)8192)
#define SDIO_DataBlockSize_16384b           ((u16)16384)


/* SDIO Flags ----------------------------------------------------------------*/
//SDIO中断状态
#define SDIO_STATUS							(SDIO0->INTSTS)
#define SDIO_FLAG_FIFOADDERR3				((u32)1 << 14)		//FIFO SD 地址指示器中断3 状态 (RW1C)
#define SDIO_FLAG_FIFOADDERR2				((u32)1 << 13)		//FIFO SD 地址指示器中断2 状态 (RW1C)
#define SDIO_FLAG_FIFOADDERR1				((u32)1 << 12)		//FIFO SD 地址指示器中断1 状态 (RW1C)
#define SDIO_FLAG_FIFOADDERR0				((u32)1 << 11)		//FIFO SD 地址指示器中断0 状态 (RW1C)
#define SDIO_FLAG_READWAIT					((u32)1 << 10)		//读等待中断状态 (RW1C)。
#define SDIO_FLAG_CCS						((u32)1 << 9)		//CCS 中断状态 (RW1C)。
#define SDIO_FLAG_CARD						((u32)1 << 8)		//卡 中断。
#define SDIO_FLAG_CARDREMOVAL				((u32)1 << 7)		//卡移除
#define SDIO_FLAG_CARDINSERTION				((u32)1 << 6)		//卡插入
#define SDIO_FLAG_BUFFREAD					((u32)1 << 5)		//读缓冲区就绪
#define SDIO_FLAG_BUFFWRITE					((u32)1 << 4)		//写缓冲区就绪
#define SDIO_FLAG_DMA						((u32)1 << 3)		//DMA
#define SDIO_FLAG_BLOCKGAP					((u32)1 << 2)		//块间隔事件。
#define SDIO_FLAG_TRANSFER					((u32)1 << 1)		//传输完成。超时错误优先
#define SDIO_FLAG_COMMANDEND				((u32)1 << 0)		//指令完成。超时错误优先
#define SDIO_FLAG_ADMA						((u32)1 << 25)		//DMA错误
#define SDIO_FLAG_AUTOCMD12					((u32)1 << 24)		//自动CMD12错误
#define SDIO_FLAG_DATAENDBIT				((u32)1 << 22)		//数据最后位错误
#define SDIO_FLAG_DATACRC					((u32)1 << 21)		//数据CRC错误
#define SDIO_FLAG_DATATIMEOUT				((u32)1 << 20)		//数据超时错误
#define SDIO_FLAG_COMMANDINDEX				((u32)1 << 19)		//命令索引错误
#define SDIO_FLAG_COMMANDENDBIT				((u32)1 << 18)		//指令最后位错误
#define SDIO_FLAG_COMMANDCRC				((u32)1 << 17)		//指令CRC错误
#define SDIO_FLAG_COMMANDTIMEOUT			((u32)1 << 16)		//指令超时错误
#define SDIO_FLAG_ALL						((u32)0xffffffff)	//所有标志







void SDIO_DeInit(void);
void SDIO_ClockCmd(u8 EN);
void SDIO_SetPowerState(u32 SDIO_PowerState);
void SDIO_FlagITConfig(u32 SDIO_FLAG, u8 EN);
void SDIO_FlagConfig(u32 SDIO_FLAG, u8 EN);
void SDIO_SendCommand(vu8 CmdIdx,vu32 CmdArg,vu8 CmdMisc);
u32 SDIO_GetResponse(u8 SDIO_RESP);
u32 SDIO_ReadData(void);
// void SDIO_WriteData(u32 Data);
u8 SDIO_GetFlagStatus(u32 SDIO_FLAG);
void SDIO_ClearFlag(u32 SDIO_FLAG);
void SDIO_SetFIFOInterruptAddress(u8 FIFOxAdd,u8 WordAdd);
void SDIO_SetTimeOutClock(u8 Unit,u8 Frequency);
void SDIO_CLKFrequencySelect(u8 SDCLK);
void SDIO_SoftwareReset(u32 SDIO_RST);//软件复位


//设置需要传输的块数量
#define SDIO_SetTransferBlockCnt(x)					(SDIO0->BLKCNT = x)
//获取剩余传输的块的数量
#define SDIO_GetTransferBlockCnt()					(SDIO0->BLKCNT)
//设置单次传输的块的大小
#define SDIO_SetTransferBlockSize(x)				(SDIO0->BLKSIZE = (x & 0xfff))
//设置为单块传输模式
#define SDIO_SingleBlockMode()						(SDIO0->TRNMOD &= ~(BIT5))
//设置为多区段传输模式
#define SDIO_MultipleBlockMode()					(SDIO0->TRNMOD |= BIT5)
//自动CMD12命令启动
#define SDIO_AutoCMD12Enable()						(SDIO0->TRNMOD |= BIT2)
//自动CMD12命令禁止
#define SDIO_AuotCMD12Disable()						(SDIO0->TRNMOD &= ~(BIT2))
//设置SDIO为写数据模式
#define SDIO_WriteMode()							(SDIO0->TRNMOD &= ~(BIT4))
//设置SDIO为读数据模式
#define SDIO_ReadMode()								(SDIO0->TRNMOD |= BIT4)
//块计数器启动
#define SDIO_BlockCountEnable()						(SDIO0->TRNMOD |= BIT1)
//禁止块计数器启动
#define SDIO_BlockCountDisable()					(SDIO0->TRNMOD &= ~(BIT1))
//DMA启动
#define SDIO_DMAEnable()							(SDIO0->TRNMOD |= BIT0)
//DMA禁止	
#define SDIO_DMADisable()							(SDIO0->TRNMOD &= ~(BIT0))
//数据线忙
#define SDIO_DATLineActive()						(SDIO0->PRNSTS & BIT2)
//指令禁止
#define SDIO_CommandInhibit()						(SDIO0->PRNSTS & BIT0)
//高速时钟模式使能
#define SDIO_HighSpeedEnable()						(SDIO0->CAPAREG |= BIT21)
//取消高速时钟模式
#define SDIO_HighSpeedDisable()						(SDIO0->CAPAREG &= ~BIT21)
//数据线宽度设置为4bit
#define SDIO_SetDataDataWidth_4b()					(SDIO0->HOSTCTL |= BIT1)
//数据线宽设置为1bit
#define SDIO_SetDataDataWidth_1b()					(SDIO0->HOSTCTL &= ~BIT1)
//总线电压选择3.3V
#define SDIO_SetBusVoltage_3_3V()					(SDIO0->PWRCON &= ~(0x7 << 1); SDIO0->PWRCON |= (7 << 1))
//总线电源开启
#define SDIO_BusPowerON()							(SDIO0->PWRCON |= BIT0)
//总线电压关闭
#define SDIO_BusPowerOFF()							(SDIO0->PWRCON &= ~BIT0)
//唤醒发生在SD卡插入
#define SDIO_WakeupOnSDInsertion()					(SDIO0->WAKCON |= BIT1
//使能SD时钟
#define SDIO_SDClkEnable()							(SDIO0->CLKCON |= BIT2)
//SD时钟停止	
#define SDIO_SDClkDisable()							(SDIO0->CLKCON &= ~BIT2)				

//超时时钟单位设置
#define TIME_OUT_UNIT_KHZ	0	//超时时钟单位为KHZ
#define TIME_OUT_UNIT_MHZ	1	//超时时钟单位为MHZ


//时钟分频系数
#define SDCLK_1_256 	((u8)0x80)	//基础时钟256分频
#define SDCLK_1_128 	((u8)0x40)	//基础时钟128分频
#define SDCLK_1_64 	((u8)0x20)	//基础时钟64分频
#define SDCLK_1_32	((u8)0x10)	//基础时钟32分频
#define SDCLK_1_16 	((u8)0x08)	//基础时钟16分频
#define SDCLK_1_8 	((u8)0x04)	//基础时钟8分频
#define SDCLK_1_4 	((u8)0x02)	//基础时钟4分频
#define SDCLK_1_2 	((u8)0x01)	//基础时钟2分频
#define SDCLK_1_1		((u8)0x00)	//基础时钟1分频

//软件复位选择
#define SDIO_RSTDAT		BIT2	//复位DAT线
#define SDIO_RSTCMD		BIT1	//复位CMD线
#define SDIO_RSTALL		BIT0	//复位所有


#endif
