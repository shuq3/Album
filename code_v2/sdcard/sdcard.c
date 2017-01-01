/*******************************************************************************
//功能:SDCARD 驱动,SDIO 模式
//作者:陈鹏
//创建时间:2012-05-16 10:32
//修改时间:2012-05-16 10:32
//修订说明:
//声明:源程序借鉴了意法STM32F103X库函数
********************************************************************************/
#include "sdcard.h"
#include "sdio.h"
// #include "system.h"

#define NULL 0



u8 SDMMC0_MOV_Flag = 0;		//sdmmc0卡移除信号有

/*
1.3 寄存器
名称 宽度 描述
CID 128 卡标识号
RCA 16 相对卡地址（Relative card address）:本地系统中卡的地
址，动态变化，在主机初始化的时候确定
*SPI 模式中没有
CSD 128 卡描述数据:卡操作条件相关的信息数据
SCR 64 SD 配置寄存器:SD 卡特定信息数据
OCR 32 操作条件寄存器*/

//容量 = BLOCKNR * BLOCK_LEN = (C_SIZE+1)*2^(C_SIZE_MULT+2)*2^(READ_BL_LEN)


#define SD_BlockSize 			512		//SD卡块大小
#define SDIO_CMD0TIMEOUT		1000000	//超时计数器大小
#define SDIO_READTIMEOUT		1000000	//读等待超时
#define SDIO_WRITETIMEOUT		2000000	//写等待超时

// SD卡指令表  	   
#define CMD0    0       //卡复位
#define CMD1    1
#define CMD9    9       //命令9 ，读CSD数据
#define CMD10   10      //命令10，读CID数据
#define CMD12   12      //命令12，停止数据传输
#define CMD16   16      //命令16，设置SectorSize 应返回0x00
#define CMD17   17      //命令17，读sector
#define CMD18   18      //命令18，读Multi sector
#define ACMD23  23      //命令23，设置多sector写入前预先擦除N个block
#define CMD24   24      //命令24，写sector
#define CMD25   25      //命令25，写Multi sector
#define ACMD41  41      //命令41，应返回0x00
#define CMD55   55      //命令55，应返回0x01
#define CMD58   58      //命令58，读OCR信息
#define CMD59   59      //命令59，使能/禁止CRC，应返回0x0

/* Mask for errors Card Status R1 (OCR Register) */
#define SD_OCR_ADDR_OUT_OF_RANGE        ((u32)0x80000000)
#define SD_OCR_ADDR_MISALIGNED          ((u32)0x40000000)
#define SD_OCR_BLOCK_LEN_ERR            ((u32)0x20000000)
#define SD_OCR_ERASE_SEQ_ERR            ((u32)0x10000000)
#define SD_OCR_BAD_ERASE_PARAM          ((u32)0x08000000)
#define SD_OCR_WRITE_PROT_VIOLATION     ((u32)0x04000000)
#define SD_OCR_LOCK_UNLOCK_FAILED       ((u32)0x01000000)
#define SD_OCR_COM_CRC_FAILED           ((u32)0x00800000)
#define SD_OCR_ILLEGAL_CMD              ((u32)0x00400000)
#define SD_OCR_CARD_ECC_FAILED          ((u32)0x00200000)
#define SD_OCR_CC_ERROR                 ((u32)0x00100000)
#define SD_OCR_GENERAL_UNKNOWN_ERROR    ((u32)0x00080000)
#define SD_OCR_STREAM_READ_UNDERRUN     ((u32)0x00040000)
#define SD_OCR_STREAM_WRITE_OVERRUN     ((u32)0x00020000)
#define SD_OCR_CID_CSD_OVERWRIETE       ((u32)0x00010000)
#define SD_OCR_WP_ERASE_SKIP            ((u32)0x00008000)
#define SD_OCR_CARD_ECC_DISABLED        ((u32)0x00004000)
#define SD_OCR_ERASE_RESET              ((u32)0x00002000)
#define SD_OCR_AKE_SEQ_ERROR            ((u32)0x00000008)
#define SD_OCR_ERRORBITS                ((u32)0xFDFFE008)

/* Masks for R6 Response */
#define SD_R6_GENERAL_UNKNOWN_ERROR     ((u32)0x00002000)
#define SD_R6_ILLEGAL_CMD               ((u32)0x00004000)
#define SD_R6_COM_CRC_FAILED            ((u32)0x00008000)

#define SD_VOLTAGE_WINDOW_SD            ((u32)0x80100000)
#define SD_HIGH_CAPACITY                ((u32)0x40000000)
#define SD_STD_CAPACITY                 ((u32)0x00000000)
#define SD_CHECK_PATTERN                ((u32)0x000001AA)

#define SD_MAX_VOLT_TRIAL               ((u32)0x0000FFFF)
#define SD_ALLZERO                      ((u32)0x00000000)

#define SD_WIDE_BUS_SUPPORT             ((u32)0x00040000)
#define SD_SINGLE_BUS_SUPPORT           ((u32)0x00010000)
#define SD_CARD_LOCKED                  ((u32)0x02000000)
#define SD_CARD_PROGRAMMING             ((u32)0x00000007)
#define SD_CARD_RECEIVING               ((u32)0x00000006)
#define SD_DATATIMEOUT                  ((u32)0x000FFFFF)
#define SD_0TO7BITS                     ((u32)0x000000FF)
#define SD_8TO15BITS                    ((u32)0x0000FF00)
#define SD_16TO23BITS                   ((u32)0x00FF0000)
#define SD_24TO31BITS                   ((u32)0xFF000000)
#define SD_MAX_DATA_LENGTH              ((u32)0x01FFFFFF)

#define SD_HALFFIFO                     ((u32)0x00000008)
#define SD_HALFFIFOBYTES                ((u32)0x00000020)

/* Command Class Supported */
#define SD_CCCC_LOCK_UNLOCK             ((u32)0x00000080)
#define SD_CCCC_WRITE_PROT              ((u32)0x00000040)
#define SD_CCCC_ERASE                   ((u32)0x00000020)

/* Following commands are SD Card Specific commands.
   SDIO_APP_CMD should be sent before sending these commands. */
#define SDIO_SEND_IF_COND               ((u32)0x00000008)

//#define SDIO_MULTIMEDIA_CARD               ((u32)0x0)
#define SDIO_SECURE_DIGITAL_CARD           ((u32)0x1)
//#define SDIO_SECURE_DIGITAL_IO_CARD        ((u32)0x2)
//#define SDIO_HIGH_SPEED_MULTIMEDIA_CARD    ((u32)0x3)
//#define SDIO_SECURE_DIGITAL_IO_COMBO_CARD  ((u32)0x4)
//#define SDIO_HIGH_CAPACITY_SD_CARD         ((u32)0x5)
//#define SDIO_HIGH_CAPACITY_MMC_CARD        ((u32)0x6)

#define SDIO_INIT_CLK_DIV                  ((u8)0xB2)
#define SDIO_TRANSFER_CLK_DIV              ((u8)0x01)  //时钟分频

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static u32 CardType =  SDIO_SECURE_DIGITAL_CARD;
static u32 CSD_Tab[4], CID_Tab[4], RCA = 0;
//static u32 DeviceMode = SD_POLLING_MODE;
//static u32 TotalNumberOfBytes = 0, StopCondition = 0;
u32 *SrcBuffer, *DestBuffer;
volatile SD_Error TransferError = SD_OK;
vu32 TransferEnd = 0;
vu32 NumberOfBytes = 0;
//SD卡中断服务程序,用来检测卡的插入与移除的
//static void __irq Isr_SDMMC_Card(void);


/* Private function prototypes -----------------------------------------------*/
static SD_Error CmdError(void);
static SD_Error CmdResp1Error(void);
static SD_Error CmdResp7Error(void);
static SD_Error CmdResp3Error(void);
static SD_Error CmdResp2Error(void);
static SD_Error CmdResp6Error(u16 *prca);
static SD_Error SDEnWideBus(u8 EN);
static SD_Error IsCardProgramming(u8 *pstatus);
static SD_Error FindSCR(u16 rca, u32 *pscr);

// #define DebugPrintf printf

/*************************************************************************************************************************
* 函数	:	SD_Error SD_Init(void)
* 功能	:	SD卡初始化
* 参数	:	无
* 返回	:	SD_OK:成功,其它见SD Card Error code.
* 依赖	:	底层寄存器操作函数
* 作者	:	陈鹏
* 时间	:	20120516
* 最后修改时间 : 20120524
* 说明	: 	初始化SD卡
*************************************************************************************************************************/

// use
SD_Error SD_Init(void)
{
	SD_Error errorstatus = SD_OK;
	
	SDIO_SoftwareReset(SDIO_RSTALL);	//软件复位所有寄存器
	SDIO_DeInit();	//初始化SDIO硬件
	//SDIO_FlagITConfig(SDIO_FLAG_CARDREMOVAL | SDIO_FLAG_CARDINSERTION,ENABLE);//使能卡插入拔出中断
	//Set_IsrAddr(INT_HSMMC0,(vu32)Isr_SDMMC_Card);	//设置中断矢量入口
	//Set_IntEnable(INT_HSMMC0,ENABLE);	//开启SDMMC0中断
	errorstatus = SD_SetIdleSta();	//SD卡上电
	if (errorstatus != SD_OK)		//卡上电发送错误
	{
		// DebugPrintf("SD power up error:(%d)!\n",errorstatus);	//调试,打印错误
		return(errorstatus);	//返回错误
	}

	errorstatus = SD_InitializeCards();
	if (errorstatus != SD_OK)
	{
		// DebugPrintf("SD initialize error(%d)!\n",errorstatus);	//调试,打印错误
		return(errorstatus);
	}
	
	return(errorstatus);
}



/*************************************************************************************************************************
* 函数	:	SD_Error SD_SetIdleSta(void)
* 功能	:	SD卡上电进入空闲模式,并识别卡
* 参数	:	无
* 返回	:	SD_OK:成功,其它见SD Card Error code.
* 依赖	:	底层寄存器操作函数
* 作者	:	陈鹏
* 时间	:	20120516
* 最后修改时间 : 20120516
* 说明	:   SD卡上电进入空闲模式
*************************************************************************************************************************/
// use
SD_Error SD_SetIdleSta(void)
{
  SD_Error errorstatus = SD_OK;
  volatile u32 response = 0;
  u32 count = 0;
  bool validvoltage = FALSE;
  u32 SDType = SD_STD_CAPACITY;
  u16 i;

	SDIO_CLKFrequencySelect(SDCLK_1_64);		//设置时钟400KHZ
	SDIO_SetTimeOutClock(TIME_OUT_UNIT_KHZ,1);	//设置超时时钟频率最低
	//发送至少74个时钟,等待SD卡上电成功并同步
	for(response = 0;response < 1000;response ++);
  
  	
  	//循环发生发送CMD0,无响应,无返回,让SD卡进入空闲模式
  	for(i = 0;i < 50;i ++)
  	{
	  	SDIO_SendCommand(CMD0,0,0);		
		errorstatus = CmdError();		//判断命令是否执行成功,此命令只要初始化了SDIO就会执行成功
		if(errorstatus == SD_OK)
			break;
  	}
  	if(errorstatus != SD_OK)
  	{
  		// DebugPrintf("error!(%d)\n",errorstatus);	//调试,打印信息	
  		return(errorstatus);
  	}
	
  	//发送CMD8:SEND_IF_COND;短响应,命令参数:SD_CHECK_PATTERN;返回响应R7
  	//识别卡版本
	SDIO_SendCommand(SDIO_SEND_IF_COND,SD_CHECK_PATTERN,SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
	errorstatus = CmdResp7Error();
	if (errorstatus == SD_OK)		//返回成功;说明卡为SD Card 2.0 V2.0
	{
		CardType = SDIO_STD_CAPACITY_SD_CARD_V2_0; /* SD Card 2.0 */
		SDType = SD_HIGH_CAPACITY;	
		// DebugPrintf("SDIO_STD_CAPACITY_SD_CARD_V2_0!\n");	//调试,打印错误信息
	}
	else	//V1.0 V1.1
	{
		// DebugPrintf("SD Card V1.1!\n");	//调试,打印信息
		CardType = SDIO_STD_CAPACITY_SD_CARD_V1_1; 		//V1.0 V1.1
		SDIO_SendCommand(CMD55,0,SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
		CmdResp1Error();
	}
	do
	{
		//发送CMD55 SDIO_APP_CMD;命令参数:0;返回响应R1,设置RCA为0,短响应
      	SDIO_SendCommand(SDIO_APP_CMD,0,SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
		errorstatus = CmdResp1Error();
		if (errorstatus != SD_OK)
		{
			// DebugPrintf("CMD55 error(%d)!\n",errorstatus);	//调试,打印错误信息
			return(errorstatus);
		}
		//发送ACM41命令;命令参数:SD_APP_OP_COND(0x80100000);短响应.响应为R3,返回操作条件寄存器RCA
		SDIO_SendCommand(SDIO_SD_APP_OP_COND,SD_VOLTAGE_WINDOW_SD | SDType,SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
		errorstatus = CmdResp3Error();
		if (errorstatus != SD_OK)
		{
			// DebugPrintf("ACM41 error(%d)!\n",errorstatus);	//调试,打印错误信息
			return(errorstatus);
		}
		response = SDIO_GetResponse(SDIO_RESP1);	//获取响应,RESE1
		validvoltage = (bool) (((response >> 31) == 1) ? 1 : 0);
		count++;
	}
	while((!validvoltage) && (count < SD_MAX_VOLT_TRIAL));//循环初始化,直到返回成功或者超时

    if (count >= SD_MAX_VOLT_TRIAL)	//重试次数超出
    {
		errorstatus = SD_INVALID_VOLTRANGE;
		return(errorstatus);
    }

    if (response &= SD_HIGH_CAPACITY)
    {
		CardType = SDIO_HIGH_CAPACITY_SD_CARD;
    }

	return(errorstatus);
}

/*************************************************************************************************************************
* 函数	:	SD_Error SD_PowerOFF(void)
* 功能	:	SD卡掉电
* 参数	:	无
* 返回	:	SD_OK:成功,其它见SD Card Error code.
* 依赖	:	底层寄存器操作函数
* 作者	:	陈鹏
* 时间	:	20120516
* 最后修改时间 : 20120516
* 说明	: 	SD卡掉电
*************************************************************************************************************************/
SD_Error SD_PowerOFF(void)
{
	SD_Error errorstatus = SD_OK;
	
	SDIO_BusPowerOFF();	//关闭总线电源
	
	return(errorstatus);
}


/*************************************************************************************************************************
* 函数	:	SD_Error SD_PowerON(void)
* 功能	:	SD卡电源开启
* 参数	:	无
* 返回	:	SD_OK:成功,其它见SD Card Error code.
* 依赖	:	底层寄存器操作函数
* 作者	:	陈鹏
* 时间	:	20120516
* 最后修改时间 : 20120524
* 说明	: 	SD卡电源开启
*************************************************************************************************************************/
SD_Error SD_PowerON(void)
{
	SD_Error errorstatus = SD_OK;
	
	SDIO_BusPowerON();	//打开总线电源
	
	return(errorstatus);
}



/*************************************************************************************************************************
* 函数	:	SD_Error SD_InitializeCards(void)
* 功能	:	将所有的卡进行初始化配置
* 参数	:	无
* 返回	:	SD_OK:成功,其它见SD Card Error code.
* 依赖	:	底层寄存器操作函数
* 作者	:	陈鹏
* 时间	:	20120516
* 最后修改时间 : 20120516
* 说明	: 	将所有的卡进行初始化配置
*************************************************************************************************************************/
SD_Error SD_InitializeCards(void)
{
	SD_Error errorstatus = SD_OK;
	u16 rca = 0x01;

	//发送CMD2 SDIO_ALL_SEND_CID命令,命令参数:0;长回复,R2
	//发送CMD用来获取CID信息的
	SDIO_SendCommand(SDIO_ALL_SEND_CID,0,SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Long);
	errorstatus = CmdResp2Error();				//获取响应R2
	if (errorstatus != SD_OK)
	{
		// DebugPrintf("error!(%d)\n",errorstatus);					//调试,打印错误信息
		return(errorstatus);
	}
	//到每个卡以获取每个卡的唯一标识CID
	CID_Tab[0] = SDIO_GetResponse(SDIO_RESP1);		
	CID_Tab[1] = SDIO_GetResponse(SDIO_RESP2);
	CID_Tab[2] = SDIO_GetResponse(SDIO_RESP3);
	CID_Tab[3] = SDIO_GetResponse(SDIO_RESP4);
	
	//发送CMD3,SET_REL_ADDR命令,参数0,响应,短响应,R6
	//用来获取卡地址
	//主机发送CMD3（SEND_RELATIVE_ADDR）要求卡发布一个新的相对卡地址RCA，地址比CID短，在以后的数据传输模式中用来寻址卡。一旦获得RCA后，卡状态变成就绪状态（Stand-by state）
	SDIO_SendCommand(SDIO_SET_REL_ADDR,0,SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
    errorstatus = CmdResp6Error(&rca);	//获取卡地址
    if (errorstatus != SD_OK)
    {
		// DebugPrintf("error!(%d)!\n",errorstatus);					//调试,打印错误信息
		return(errorstatus);
    }
		
	RCA = rca;//存储卡地址
	
	//发送CMD9 SDIO_SEND_CSD命令,参数:rca地址;长响应,R2;
	//给卡发送一个新的RCA,主要是用来设置卡地址的
    SDIO_SendCommand(SDIO_SEND_CSD,(u32)rca << 16,SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Long);
    errorstatus = CmdResp2Error();
    if (errorstatus != SD_OK)
    {
		// DebugPrintf("error!(%d)!\n",errorstatus);					//调试,打印错误信息
		return(errorstatus);
    }
    
    //获取CSD
    CSD_Tab[0] = SDIO_GetResponse(SDIO_RESP1);
    CSD_Tab[1] = SDIO_GetResponse(SDIO_RESP2);
    CSD_Tab[2] = SDIO_GetResponse(SDIO_RESP3);
    CSD_Tab[3] = SDIO_GetResponse(SDIO_RESP4);
    
    //选中卡,并激活
 	errorstatus = SD_SelectDeselect((u32)RCA << 16);
 	if(errorstatus != SD_OK)
 	{
 		// DebugPrintf("SelectDeselect error!(%d)!\n",errorstatus);					//调试,打印错误信息
 		return(errorstatus);
 	}

	errorstatus = SD_OK; /* All cards get intialized */
 	
 	SDIO_CLKFrequencySelect(SDCLK_1_2);	//设置时钟
 	SDIO_SetTimeOutClock(TIME_OUT_UNIT_KHZ,63);	//设置超时时钟频率最高
	errorstatus = SD_EnableWideBusMode(ENABLE);
	if(errorstatus == SD_OK)	//配置SD卡为4线模式
	{
		// DebugPrintf("SD SDIO 4BIT OK\n");
	}
	else
	{
		errorstatus = SD_EnableWideBusMode(DISABLE);
		// DebugPrintf("SD SDIO 4BIT ERROR (%d)\n",errorstatus);
	}
	
	SDIO_SetTransferBlockSize(SD_BlockSize);//设置传输块大小为512字节
	errorstatus = SD_SetBlockSize(SD_BlockSize);//配置SD卡块大小	
	if (errorstatus != SD_OK)
	{
		// DebugPrintf("SD SetBlockSize error(%d)!\n",errorstatus);
		return(errorstatus);
	}
	
	return(errorstatus);
}


/*************************************************************************************************************************
* 函数	:	SD_Error SD_SelectDeselect(u32 addr)
* 功能	:	选中一个卡,并处于传输状态
* 参数	:	addr:卡地址
* 返回	:	SD_OK:成功,其它见SD Card Error code.
* 依赖	:	底层寄存器操作函数
* 作者	:	陈鹏
* 时间	:	20120516
* 最后修改时间 : 20120516
* 说明	: 	选择一个卡并将它置于传输状态（Transfer state)
*************************************************************************************************************************/
SD_Error SD_SelectDeselect(u32 addr)
{
	SD_Error errorstatus = SD_OK;

	//CMD7用来选择一个卡并将它置于传输状态（Transfer state），在任何时间只能有一个卡处于传输状态
	SDIO_SendCommand(SDIO_SEL_DESEL_CARD,addr,SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
	errorstatus = CmdResp1Error();

	return(errorstatus);
}

/*************************************************************************************************************************
* 函数	:	SD_Error SD_GetCardInfo(SD_CardInfo *cardinfo)
* 功能	:	获取卡的细节信息
* 参数	:	cardinfo:卡信息结构指针,指向信息存放缓冲区地址
* 返回	:	SD_OK:成功,其它见SD Card Error code.
* 依赖	:	底层寄存器操作函数
* 作者	:	陈鹏
* 时间	:	20120516
* 最后修改时间 : 20120516
* 说明	: 	获取卡的信息,通过CSD信息得到
*************************************************************************************************************************/
SD_Error SD_GetCardInfo(SD_CardInfo *cardinfo)
{
  SD_Error errorstatus = SD_OK;
  u8 tmp = 0;

  cardinfo->CardType = (u8)CardType;
  cardinfo->RCA = (u16)RCA;

  /* Byte 0 */
  tmp = (u8)((CSD_Tab[0] & 0xFF000000) >> 24);
  cardinfo->SD_csd.CSDStruct = (tmp & 0xC0) >> 6;
  cardinfo->SD_csd.SysSpecVersion = (tmp & 0x3C) >> 2;
  cardinfo->SD_csd.Reserved1 = tmp & 0x03;

  /* Byte 1 */
  tmp = (u8)((CSD_Tab[0] & 0x00FF0000) >> 16);
  cardinfo->SD_csd.TAAC = tmp;

  /* Byte 2 */
  tmp = (u8)((CSD_Tab[0] & 0x0000FF00) >> 8);
  cardinfo->SD_csd.NSAC = tmp;

  /* Byte 3 */
  tmp = (u8)(CSD_Tab[0] & 0x000000FF);
  cardinfo->SD_csd.MaxBusClkFrec = tmp;

  /* Byte 4 */
  tmp = (u8)((CSD_Tab[1] & 0xFF000000) >> 24);
  cardinfo->SD_csd.CardComdClasses = tmp << 4;

  /* Byte 5 */
  tmp = (u8)((CSD_Tab[1] & 0x00FF0000) >> 16);
  cardinfo->SD_csd.CardComdClasses |= (tmp & 0xF0) >> 4;
  cardinfo->SD_csd.RdBlockLen = tmp & 0x0F;

  /* Byte 6 */
  tmp = (u8)((CSD_Tab[1] & 0x0000FF00) >> 8);
  cardinfo->SD_csd.PartBlockRead = (tmp & 0x80) >> 7;
  cardinfo->SD_csd.WrBlockMisalign = (tmp & 0x40) >> 6;
  cardinfo->SD_csd.RdBlockMisalign = (tmp & 0x20) >> 5;
  cardinfo->SD_csd.DSRImpl = (tmp & 0x10) >> 4;
  cardinfo->SD_csd.Reserved2 = 0; /* Reserved */
  cardinfo->SD_csd.DeviceSize = (tmp & 0x03) << 10;

  /* Byte 7 */
  tmp = (u8)(CSD_Tab[1] & 0x000000FF);
  cardinfo->SD_csd.DeviceSize |= (tmp) << 2;

  /* Byte 8 */
  tmp = (u8)((CSD_Tab[2] & 0xFF000000) >> 24);

  cardinfo->SD_csd.DeviceSize |= (tmp & 0xC0) >> 6;
  cardinfo->SD_csd.MaxRdCurrentVDDMin = (tmp & 0x38) >> 3;
  cardinfo->SD_csd.MaxRdCurrentVDDMax = (tmp & 0x07);

  /* Byte 9 */
  tmp = (u8)((CSD_Tab[2] & 0x00FF0000) >> 16);
  cardinfo->SD_csd.MaxWrCurrentVDDMin = (tmp & 0xE0) >> 5;
  cardinfo->SD_csd.MaxWrCurrentVDDMax = (tmp & 0x1C) >> 2;
  cardinfo->SD_csd.DeviceSizeMul = (tmp & 0x03) << 1;

  /* Byte 10 */
  tmp = (u8)((CSD_Tab[2] & 0x0000FF00) >> 8);
  cardinfo->SD_csd.DeviceSizeMul |= (tmp & 0x80) >> 7;
  cardinfo->SD_csd.EraseGrSize = (tmp & 0x40) >> 6;
  cardinfo->SD_csd.EraseGrMul = (tmp & 0x3F) << 1;

  /* Byte 11 */
  tmp = (u8)(CSD_Tab[2] & 0x000000FF);
  cardinfo->SD_csd.EraseGrMul |= (tmp & 0x80) >> 7;
  cardinfo->SD_csd.WrProtectGrSize = (tmp & 0x7F);

  /* Byte 12 */
  tmp = (u8)((CSD_Tab[3] & 0xFF000000) >> 24);
  cardinfo->SD_csd.WrProtectGrEnable = (tmp & 0x80) >> 7;
  cardinfo->SD_csd.ManDeflECC = (tmp & 0x60) >> 5;
  cardinfo->SD_csd.WrSpeedFact = (tmp & 0x1C) >> 2;
  cardinfo->SD_csd.MaxWrBlockLen = (tmp & 0x03) << 2;

  /* Byte 13 */
  tmp = (u8)((CSD_Tab[3] & 0x00FF0000) >> 16);
  cardinfo->SD_csd.MaxWrBlockLen |= (tmp & 0xC0) >> 6;
  cardinfo->SD_csd.WriteBlockPaPartial = (tmp & 0x20) >> 5;
  cardinfo->SD_csd.Reserved3 = 0;
  cardinfo->SD_csd.ContentProtectAppli = (tmp & 0x01);

  /* Byte 14 */
  tmp = (u8)((CSD_Tab[3] & 0x0000FF00) >> 8);
  cardinfo->SD_csd.FileFormatGrouop = (tmp & 0x80) >> 7;
  cardinfo->SD_csd.CopyFlag = (tmp & 0x40) >> 6;
  cardinfo->SD_csd.PermWrProtect = (tmp & 0x20) >> 5;
  cardinfo->SD_csd.TempWrProtect = (tmp & 0x10) >> 4;
  cardinfo->SD_csd.FileFormat = (tmp & 0x0C) >> 2;
  cardinfo->SD_csd.ECC = (tmp & 0x03);

  /* Byte 15 */
  tmp = (u8)(CSD_Tab[3] & 0x000000FF);
  cardinfo->SD_csd.CSD_CRC = (tmp & 0xFE) >> 1;
  cardinfo->SD_csd.Reserved4 = 1;


  /* Byte 0 */
  tmp = (u8)((CID_Tab[0] & 0xFF000000) >> 24);
  cardinfo->SD_cid.ManufacturerID = tmp;

  /* Byte 1 */
  tmp = (u8)((CID_Tab[0] & 0x00FF0000) >> 16);
  cardinfo->SD_cid.OEM_AppliID = tmp << 8;

  /* Byte 2 */
  tmp = (u8)((CID_Tab[0] & 0x000000FF00) >> 8);
  cardinfo->SD_cid.OEM_AppliID |= tmp;

  /* Byte 3 */
  tmp = (u8)(CID_Tab[0] & 0x000000FF);
  cardinfo->SD_cid.ProdName1 = tmp << 24;

  /* Byte 4 */
  tmp = (u8)((CID_Tab[1] & 0xFF000000) >> 24);
  cardinfo->SD_cid.ProdName1 |= tmp << 16;

  /* Byte 5 */
  tmp = (u8)((CID_Tab[1] & 0x00FF0000) >> 16);
  cardinfo->SD_cid.ProdName1 |= tmp << 8;

  /* Byte 6 */
  tmp = (u8)((CID_Tab[1] & 0x0000FF00) >> 8);
  cardinfo->SD_cid.ProdName1 |= tmp;

  /* Byte 7 */
  tmp = (u8)(CID_Tab[1] & 0x000000FF);
  cardinfo->SD_cid.ProdName2 = tmp;

  /* Byte 8 */
  tmp = (u8)((CID_Tab[2] & 0xFF000000) >> 24);
  cardinfo->SD_cid.ProdRev = tmp;

  /* Byte 9 */
  tmp = (u8)((CID_Tab[2] & 0x00FF0000) >> 16);
  cardinfo->SD_cid.ProdSN = tmp << 24;

  /* Byte 10 */
  tmp = (u8)((CID_Tab[2] & 0x0000FF00) >> 8);
  cardinfo->SD_cid.ProdSN |= tmp << 16;

  /* Byte 11 */
  tmp = (u8)(CID_Tab[2] & 0x000000FF);
  cardinfo->SD_cid.ProdSN |= tmp << 8;

  /* Byte 12 */
  tmp = (u8)((CID_Tab[3] & 0xFF000000) >> 24);
  cardinfo->SD_cid.ProdSN |= tmp;

  /* Byte 13 */
  tmp = (u8)((CID_Tab[3] & 0x00FF0000) >> 16);
  cardinfo->SD_cid.Reserved1 |= (tmp & 0xF0) >> 4;
  cardinfo->SD_cid.ManufactDate = (tmp & 0x0F) << 8;

  /* Byte 14 */
  tmp = (u8)((CID_Tab[3] & 0x0000FF00) >> 8);
  cardinfo->SD_cid.ManufactDate |= tmp;

  /* Byte 15 */
  tmp = (u8)(CID_Tab[3] & 0x000000FF);
  cardinfo->SD_cid.CID_CRC = (tmp & 0xFE) >> 1;
  cardinfo->SD_cid.Reserved2 = 1;

  return(errorstatus);
}


/*************************************************************************************************************************
* 函数	:	SD_Error SD_EnableWideBusMode(u8 EN)
* 功能	:	使能4bit DAT线模式,如果失败将保持原来模式
* 参数	:	Enable:4bit模式;Disable:1bit模式
* 返回	:	SD_OK:成功,其它见SD Card Error code.
* 依赖	:	底层寄存器操作函数
* 作者	:	陈鹏
* 时间	:	20120516
* 最后修改时间 : 20120524
* 说明	: 	设置卡和控制器为4bit总线模式
*************************************************************************************************************************/
SD_Error SD_EnableWideBusMode(u8 EN)
{
	SD_Error errorstatus = SD_OK;
	u8 IntFlag;

	//SD卡不支持设置
	if (CardType == SDIO_MULTIMEDIA_CARD)
	{
		errorstatus = SD_UNSUPPORTED_FEATURE;
		return(errorstatus);
	}
	else if ((CardType == SDIO_SECURE_DIGITAL_CARD) || (CardType == SDIO_HIGH_CAPACITY_SD_CARD))	//SD卡
	{
		if (EN)	//4BIT模式
		{
    		errorstatus = SDEnWideBus(ENABLE);	//设置SD卡为4bit总线模式
    		if (errorstatus == SD_OK)	//设置SD卡成功
			{
				IntFlag = Get_IntEnable(INT_HSMMC0);		
				if(IntFlag)	//如果开启了卡全局中断
				{
					Set_IntEnable(INT_HSMMC0,DISABLE);	//先关闭中断,防止误触发
				}
				SDIO_SetDataDataWidth_4b();	//设置SDIO为4bit模式
    			SDIO_ClearFlag(SDIO_FLAG_ALL);	//清除所有标志
				if(IntFlag)	//如果开启了卡全局中断
				{
					Set_IntEnable(INT_HSMMC0,ENABLE);	//开启
				}
			}	
  		}
  		else
  		{
  			errorstatus = SDEnWideBus(DISABLE);	//设置SD卡为1bit总线模式
    		if (errorstatus == SD_OK)	//设置SD卡成功
			{
				IntFlag = Get_IntEnable(INT_HSMMC0);		
				if(IntFlag)	//如果开启了卡全局中断
				{
					Set_IntEnable(INT_HSMMC0,DISABLE);	//先关闭中断,防止误触发
				}
				SDIO_SetDataDataWidth_1b();	//设置SDIO为1bit模式
    			SDIO_ClearFlag(SDIO_FLAG_ALL);	//清除所有标志
				if(IntFlag)	//如果开启了卡全局中断
				{
					Set_IntEnable(INT_HSMMC0,ENABLE);	//开启
				}
			}
  		}
  	}	
	return(errorstatus);
}


/*************************************************************************************************************************
* 函数	:	static SD_Error SDEnWideBus(u8 EN)
* 功能	:	设置SD卡宽总线模式
* 参数	:	Enable:4bit模式;Disable:1bit模式
* 返回	:	SD_OK:成功,其它见SD Card Error code.
* 依赖	:	底层寄存器操作函数
* 作者	:	陈鹏
* 时间	:	20120516
* 最后修改时间 : 20120524
* 说明	: 	设置SD卡宽总线模式
*************************************************************************************************************************/
static SD_Error SDEnWideBus(u8 EN)
{
	SD_Error errorstatus = SD_OK;
	u32 scr[2] = {0, 0};

	if (SDIO_GetResponse(SDIO_RESP1) & SD_CARD_LOCKED)
	{
		errorstatus = SD_LOCK_UNLOCK_FAILED;
		return(errorstatus);
	}

	//获取RCA
	errorstatus = FindSCR(RCA, scr);

	if (errorstatus != SD_OK)
	{
		// DebugPrintf("Get SCR error(%d)!\n",errorstatus);
		return(errorstatus);
	}
  
	if (EN)	//使能4bit dat
	{
		//如果请求的卡支持宽数据线模式
	    if ((scr[1] & SD_WIDE_BUS_SUPPORT) != SD_ALLZERO)
	    {
			//发送CMD55,SDIO_APP_CMD,激活卡
			//发送CMD55 SDIO_APP_CMD;命令参数:RCA;返回响应R1,设置RCA为0,短响应
		  	SDIO_SendCommand(SDIO_APP_CMD,(u32)RCA << 16,SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
			errorstatus = CmdResp1Error();
			if (errorstatus != SD_OK)
			{
				// DebugPrintf("CMD55 error(%d)!\n",errorstatus);	//调试,打印错误信息
				return(errorstatus);
			}
			//发送ACMD6,SDIO_APP_SD_SET_BUSWIDTH,设置宽总线模式,参数0x2,短响应,R1
			SDIO_SendCommand(SDIO_APP_SD_SET_BUSWIDTH,0x2,SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
			errorstatus = CmdResp1Error();
			if (errorstatus != SD_OK)
			{
				// DebugPrintf("ACMD6 error(%d)!\n",errorstatus);	//调试,打印错误信息
				return(errorstatus);
			}
			return(errorstatus);
	    }
	    else	//请求的卡不支持宽总线模式
	    {
	    	errorstatus = SD_REQUEST_NOT_APPLICABLE;
	    	return(errorstatus);
		}
	}   
	else	//失能宽总线模式
  	{
    	//如果请求的卡支持1bit总线模式
		if ((scr[1] & SD_SINGLE_BUS_SUPPORT) != SD_ALLZERO)
		{
			//发送CMD55 SDIO_APP_CMD;命令参数:RCA;返回响应R1,设置RCA为0,短响应
		  	SDIO_SendCommand(SDIO_APP_CMD,(u32)RCA << 16,SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
			errorstatus = CmdResp1Error();
			if (errorstatus != SD_OK)
			{
				// DebugPrintf("CMD55 error(%d)!\n",errorstatus);	//调试,打印错误信息
				return(errorstatus);
			}
			//发送ACMD6,SDIO_APP_SD_SET_BUSWIDTH,设置宽总线模式,参数0x0,短响应,R1
			SDIO_SendCommand(SDIO_APP_SD_SET_BUSWIDTH,0x0,SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
			errorstatus = CmdResp1Error();
			if (errorstatus != SD_OK)
			{
				// DebugPrintf("ACMD6 error(%d)!\n",errorstatus);	//调试,打印错误信息
				return(errorstatus);
			}
			return(errorstatus);
		}
		else	//不支持1bit总线模式
		{
			errorstatus = SD_REQUEST_NOT_APPLICABLE;
			return(errorstatus);
		}
	}
}



/*************************************************************************************************************************
* 函数	:	SD_Error SD_SetDeviceMode(u32 Mode)
* 功能	:	配置SDIO操作模式
* 参数	:	SD_DMA_MODE: DMA模式
*           SD_INTERRUPT_MODE: 中断模式
*           SD_POLLING_MODE: 查询模式(普通模式)
* 返回	:	SD_OK:成功,其它见SD Card Error code.
* 依赖	:	底层寄存器操作函数
* 作者	:	陈鹏
* 时间	:	20120516
* 最后修改时间 : 20120516
* 说明	: 	配置SDIO操作模式
*************************************************************************************************************************/
SD_Error SD_SetDeviceMode(u32 Mode)
{
  SD_Error errorstatus = SD_OK;

 /* if ((Mode == SD_DMA_MODE) || (Mode == SD_INTERRUPT_MODE) || (Mode == SD_POLLING_MODE))
  {
    DeviceMode = Mode;
  }
  else
  {
    errorstatus = SD_INVALID_PARAMETER;	//模式设置错误
  }*/
  return(errorstatus);

}



/*************************************************************************************************************************
* 函数	:	SD_Error SD_SetBlockSize(u16 BlockSize)
* 功能	:	设置SD卡块大小
* 参数	:	块大小,512,1024,等等
* 返回	:	SD_OK:成功,其它见SD Card Error code.
* 依赖	:	底层寄存器操作函数
* 作者	:	陈鹏
* 时间	:	20120516
* 最后修改时间 : 20120516
* 说明	: 	通常取512B
*************************************************************************************************************************/
SD_Error SD_SetBlockSize(u16 BlockSize)
{
	SD_Error errorstatus = SD_OK;

	//如果卡锁定了则返回
	if (SDIO_GetResponse(SDIO_RESP1) & SD_CARD_LOCKED)
	{
		errorstatus = SD_LOCK_UNLOCK_FAILED;
		// DebugPrintf("%d\r\n",errorstatus);
		return(errorstatus);
	}

	//判断块大小是否合理
	if ((BlockSize > 0) && (BlockSize <= 2048) && ((BlockSize & (BlockSize - 1)) == 0))
	{
		//Set Block Size for Card 
		SDIO_SendCommand(SDIO_SET_BLOCKLEN,BlockSize,SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
		errorstatus = CmdResp1Error();
		if (errorstatus != SD_OK)
		{
			// DebugPrintf("%d\r\n",errorstatus);
			return(errorstatus);
		}
	}
	else	//块大小设置错误
	{
		errorstatus = SD_INVALID_PARAMETER;
		return(errorstatus);
	}
	return(errorstatus);
}



/*************************************************************************************************************************
* 函数	:	SD_Error SD_ReadBlock(u32 BlockAddr, u32 *BlockBuff)
* 功能	:	读SD卡一个块
* 参数	:	BlockAddr:块地址;
			BlockBuff:块缓冲区地址
* 返回	:	SD_OK:成功,其它见SD Card Error code.
* 依赖	:	底层寄存器操作函数
* 作者	:	陈鹏
* 时间	:	20120516
* 最后修改时间 : 20120728
* 说明	: 	读SD卡一个扇区
*************************************************************************************************************************/
SD_Error SD_ReadBlock(u32 BlockAddr, u32 *BlockBuff)
{
	SD_Error errorstatus = SD_OK;
	volatile u32 timeout = SDIO_READTIMEOUT;		//读等待超时计数器
	u16 cnt = 0;
 
	if (BlockBuff == NULL)			//没有分配接收缓冲区,返回
	{
		errorstatus = SD_INVALID_PARAMETER;
		return(errorstatus);
	}
	
	SDIO_SoftwareReset(SDIO_RSTDAT);	//软件复位DAT,主要用来清空FIFO
	SDIO_SoftwareReset(SDIO_RSTCMD);	//软件复位DAT,主要用来清除状态以及命令
	SDIO_ReadMode();					//设置主机控制器为读模式
	SDIO_SetTransferBlockSize(SD_BlockSize);	//设置单次传输的块大小为SD_BlockSize
	SDIO_SingleBlockMode();				//设置为单块传输模式
	
	//发送CMD17 READ_SINGLE_BLOCK,块读取指令,参数:块地址,短回复,R1
	//此处花了我几天的时间调试,最终发现竟然是少了一条命令:SDIO_DataSelect;当前数据选择,少了这条命令DAT线上没有数据回来
	SDIO_SendCommand(SDIO_READ_SINGLE_BLOCK,(BlockAddr),SDIO_DataSelect | SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
	errorstatus = CmdResp1Error();	//获取回复
	if (errorstatus != SD_OK)		//命令发送错误,返回
	{
		// DebugPrintf("CMD17 error (%d)!\n",errorstatus);
		return(errorstatus);
	}
	
	while(((SDIO0->PRNSTS & BIT11) == 0) && timeout && (SDIO_GetFlagStatus(SDIO_FLAG_DATATIMEOUT) == 0))	//等待读缓冲区有效,或者等待超时
	{
		timeout --;
	}

	if (SDIO_GetFlagStatus(SDIO_FLAG_DATATIMEOUT) || (timeout == 0))	//超时
    {
		SDIO_ClearFlag(SDIO_FLAG_DATATIMEOUT);	//清除标志
 		errorstatus = SD_DATA_TIMEOUT;
 		// DebugPrintf("%d\r\n",errorstatus);
		return(errorstatus);
    }
	
	//缓冲区大小SD_BlockSize字节,共SD_BlockSize/4字
	for(cnt = 0;cnt < SD_BlockSize / 4;cnt ++)
  	{
  		*(BlockBuff ++) = SDIO_ReadData();
	}
    //清除所有标志
    SDIO_ClearFlag(SDIO_FLAG_ALL);
 	volatile int delay;
 	for (delay = 0; delay < 100; delay++);
	return(errorstatus);
}



/*************************************************************************************************************************
* 函数	:	SD_Error SD_ReadMultiBlocks(u32 BlockAddr, u32 *BlockBuff, u32 NumberOfBlocks)
* 功能	:	读SD卡多个块
* 参数	:	BlockAddr:块地址;
			BlockBuff:块缓冲区地址
			NumberOfBlocks:块数量
* 返回	:	SD_OK:成功,其它见SD Card Error code.
* 依赖	:	底层寄存器操作函数
* 作者	:	陈鹏
* 时间	:	20120516
* 最后修改时间 : 20120728
* 说明	: 	读SD卡多个(大于1个)扇区
*************************************************************************************************************************/
SD_Error SD_ReadMultiBlocks(u32 BlockAddr, u32 *BlockBuff, u16 NumberOfBlocks)
{
	SD_Error errorstatus = SD_OK;
	volatile u32 timeout = SDIO_READTIMEOUT;//读等待超时计数器
	u16 BlockCnt = NumberOfBlocks;	//需要传输的块计数
	u16 cnt = 0;

	if (BlockBuff == NULL)	//没有分配接收缓冲区,返回
	{
		errorstatus = SD_INVALID_PARAMETER;
		// DebugPrintf("%d\r\n",errorstatus);
		return(errorstatus);
	}

	if (NumberOfBlocks > 1)	
	{
		if (NumberOfBlocks * SD_BlockSize > SD_MAX_DATA_LENGTH)//判定地址是否超出范围
		{
			errorstatus = SD_INVALID_PARAMETER;
			// DebugPrintf("%d\r\n",errorstatus);
			return(errorstatus);
		}
		
		SDIO_SoftwareReset(SDIO_RSTDAT);			//软件复位DAT,主要用来清空FIFO
		SDIO_SoftwareReset(SDIO_RSTCMD);			//软件复位DAT,主要用来清除状态以及命令
		SDIO_MultipleBlockMode();					//设置为多区段传输模式
		SDIO_SetTransferBlockCnt(NumberOfBlocks);	//设置传输块数量
		SDIO_SetTransferBlockSize(SD_BlockSize);	//设置单次传输的块大小为SD_BlockSize
		SDIO_ReadMode();							//设置为读数据模式
		SDIO_BlockCountEnable();					//块计数启动

	    //发送CMD18 SDIO_READ_MULT_BLOCK;多区段读命令,参数:开始地址,短返回,R1
	    SDIO_SendCommand(SDIO_READ_MULT_BLOCK,BlockAddr,SDIO_DataSelect | SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
	    errorstatus = CmdResp1Error();
	    if (errorstatus != SD_OK)
	    {
	    	// DebugPrintf("%d\r\n",errorstatus);
	    	return(errorstatus);
	    }
	    
	    do
	    {
	    	if(SDIO0->PRNSTS & BIT11)	//读缓冲区有效
	    	{
	    		SDIO_ClearFlag(SDIO_FLAG_BUFFREAD);		//清除读缓冲区有效标志
	    		//缓冲区大小SD_BlockSize字节,共SD_BlockSize/4字
	    		for(cnt = 0;cnt < SD_BlockSize / 4;cnt ++)
			  	{
			  		*(BlockBuff ++) = SDIO_ReadData();
				}
			  	BlockCnt --;	//读完了一块
			  	timeout = SDIO_READTIMEOUT;	//读完了一块,超时计数器重新开始计数
	    	}
	    	timeout --;
	    }while(BlockCnt && (timeout != 0) && (SDIO_GetFlagStatus(SDIO_FLAG_DATATIMEOUT) == 0));	//剩余块数量不为0,没超时
		
		//发送CMD12 SDIO_STOP_TRANSMISSION命令,终止读取;参数:0,短响应,R1
		SDIO_SendCommand(SDIO_STOP_TRANSMISSION,0,SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
		errorstatus = CmdResp1Error();
		if (errorstatus != SD_OK)
		{
			// DebugPrintf("%d\r\n",errorstatus);
			return(errorstatus);
		}
		
		if(timeout == 0)	//超时
		{
			errorstatus = SD_DATA_TIMEOUT;
			// DebugPrintf("%d\r\n",errorstatus);
			return(errorstatus);
		}
		
		SDIO_ClearFlag(SDIO_FLAG_ALL);	//清除所有中断标志
		
	}
	volatile int delay;
 	for (delay = 0; delay < 100; delay++);
	return(errorstatus);
}





/*************************************************************************************************************************
* 函数	:	SD_Error SD_WriteBlock(u32 BlockAddr, u32 *BlockBuff)
* 功能	:	写SD卡一个块
* 参数	:	BlockAddr:块地址;
			writebuff:写缓冲区地址
* 返回	:	SD_OK:成功,其它见SD Card Error code.
* 依赖	:	底层寄存器操作函数
* 作者	:	陈鹏
* 时间	:	20120525
* 最后修改时间 : 20120728
* 说明	: 	写SD卡一个扇区
* 			20120728:修改了写入函数,现在可以成功写入数据
*************************************************************************************************************************/
// SD_Error SD_WriteBlock(u32 BlockAddr, u32 *BlockBuff)
// {
// 	SD_Error errorstatus = SD_OK;
// 	u32 timeout = SDIO_WRITETIMEOUT;		//写等待超时计数器
// 	u32 cardstatus = 0;
// 	u8 cardstate = 0;
// 	u16 cnt = 0;

// 	if (BlockBuff == NULL)
// 	{
// 		errorstatus = SD_INVALID_PARAMETER;
		// DebugPrintf("%d\r\n",errorstatus);
// 		return(errorstatus);
// 	}

// 	SDIO_SoftwareReset(SDIO_RSTDAT);	//软件复位DAT,主要用来清空FIFO
// 	SDIO_SoftwareReset(SDIO_RSTCMD);	//软件复位DAT,主要用来清除状态以及命令
// 	SDIO_WriteMode();					//设置主机控制器为写模式
// 	SDIO_SetTransferBlockSize(SD_BlockSize);	//设置单次传输的块大小为SD_BlockSize
// 	SDIO_SingleBlockMode();				//设置为单块传输模式
	
// 	do
// 	{
// 		timeout --;
// 		//发送CMD13,SDIO_SEND_STATUS,读 Card_Status 寄存器,参数,RCA地址,短返回,R1;
// 		SDIO_SendCommand(SDIO_SEND_STATUS,(u32) RCA << 16,SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
// 		errorstatus = CmdResp1Error();
// 		if (errorstatus != SD_OK)
// 		{
			// DebugPrintf("%d\r\n",errorstatus);
// 			return(errorstatus);
// 		}
// 		cardstatus = SDIO_GetResponse(SDIO_RESP1);
// 	}
// 	while (((cardstatus & 0x00000100) == 0) && (timeout > 0));
	
// 	if (timeout == 0)
// 	{
		// DebugPrintf("%d\r\n",errorstatus);
// 		return(SD_ERROR);
// 	}
	
// 	//发送CMD24,SDIO_WRITE_SINGLE_BLOCK,写命令,参数:地址,短响应,R1
// 	SDIO_SendCommand(SDIO_WRITE_SINGLE_BLOCK,(BlockAddr << 9),SDIO_DataSelect | SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
// 	errorstatus = CmdResp1Error();
// 	if (errorstatus != SD_OK)
// 	{
		// DebugPrintf("%d\r\n",errorstatus);
// 		return(errorstatus);
// 	}
// 	timeout = SDIO_WRITETIMEOUT;		//写等待超时计数器
// 	//写缓冲区是否有效应该判断PRNSTS这个寄存器
// 	while(((SDIO0->PRNSTS & BIT10) == 0) && (SDIO_GetFlagStatus(SDIO_FLAG_BUFFWRITE) == 0) && timeout && (SDIO_GetFlagStatus(SDIO_FLAG_DATATIMEOUT) == 0))	//等待写缓冲区有效,或者等待超时
// 	{
// 		timeout --;
// 	}

// 	if (SDIO_GetFlagStatus(SDIO_FLAG_DATATIMEOUT) || (timeout == 0))	//超时
//     {
// 		SDIO_ClearFlag(SDIO_FLAG_DATATIMEOUT);	//清除标志
//  		errorstatus = SD_DATA_TIMEOUT;
 		// DebugPrintf("%d\r\n",errorstatus);
// 		return(errorstatus);
//     }
	
// 	//缓冲区大小SD_BlockSize字节,共SD_BlockSize / 4字
// 	for(cnt = 0;cnt < SD_BlockSize / 4;cnt ++)
//   	{
//   		SDIO_WriteData(BlockBuff[cnt]);
// 	}
//     //清除所有标志
//     SDIO_ClearFlag(SDIO_FLAG_ALL);

//     //读取卡状态,等待写入完成
// 	do
// 	{
// 		errorstatus = IsCardProgramming(&cardstate);
// 	}while ((errorstatus == SD_OK) && ((cardstate == SD_CARD_PROGRAMMING) || (cardstate == SD_CARD_RECEIVING)));
	
// 	return(errorstatus);
// }





/*************************************************************************************************************************
* 函数	:	SD_Error SD_WriteMultiBlocks(u32 BlockAddr, u32 *BlockBuff, u16 NumberOfBlocks)
* 功能	:	写SD卡多个块
* 参数	:	BlockAddr:块地址;
			BlockBuff:块缓冲区地址
			NumberOfBlocks:块数量
* 返回	:	SD_OK:成功,其它见SD Card Error code.
* 依赖	:	底层寄存器操作函数
* 作者	:	陈鹏
* 时间	:	20120525
* 最后修改时间 : 20120728
* 说明	: 	写SD卡多个(大于1个)扇区
* 			20120728:修改了写入函数,现在可以成功写入数据
*************************************************************************************************************************/
// SD_Error SD_WriteMultiBlocks(u32 BlockAddr, u32 *BlockBuff, u16 NumberOfBlocks)
// {
// 	SD_Error errorstatus = SD_OK;
// 	u32 timeout = SDIO_WRITETIMEOUT;		//写等待超时计数器
// 	u16 BlockCnt = NumberOfBlocks;			//传输的块数量计数
// 	u8 cardstate = 0;
// 	u32 cardstatus = 0;
// 	u16 cnt = 0;

// 	if (BlockBuff == NULL)
// 	{
// 		errorstatus = SD_INVALID_PARAMETER;
		// DebugPrintf("%d\r\n",errorstatus);
// 		return(errorstatus);
// 	}
 
// 	if (NumberOfBlocks > 1)
// 	{
// 		if (NumberOfBlocks * SD_BlockSize > SD_MAX_DATA_LENGTH)	//判断地址
// 		{
// 			errorstatus = SD_INVALID_PARAMETER;
			// DebugPrintf("%d\r\n",errorstatus);
// 			return(errorstatus);
// 		}
		
// 		SDIO_SoftwareReset(SDIO_RSTDAT);			//软件复位DAT,主要用来清空FIFO
// 		SDIO_SoftwareReset(SDIO_RSTCMD);	//软件复位DAT,主要用来清除状态以及命令
// 		SDIO_MultipleBlockMode();					//设置为多区段传输模式
// 		SDIO_SetTransferBlockCnt(NumberOfBlocks);	//设置传输块数量
// 		SDIO_SetTransferBlockSize(SD_BlockSize);	//设置单次传输的块大小为SD_BlockSize
// 		SDIO_WriteMode();							//设置为写数据模式
// 		SDIO_BlockCountEnable();					//块计数启动
		
// 		do
// 		{
// 			timeout --;
// 			//发送CMD13,SDIO_SEND_STATUS,读 Card_Status 寄存器,参数,RCA地址,短返回,R1;
// 			SDIO_SendCommand(SDIO_SEND_STATUS,(u32) RCA << 16,SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
// 			errorstatus = CmdResp1Error();
// 			if (errorstatus != SD_OK)
// 			{
				// DebugPrintf("%d\r\n",errorstatus);
// 				return(errorstatus);
// 			}
// 			cardstatus = SDIO_GetResponse(SDIO_RESP1);
// 		}
// 		while (((cardstatus & 0x00000100) == 0) && (timeout > 0));
		
// 		if (timeout == 0)
// 		{
			// DebugPrintf("%d\r\n",errorstatus);
// 			return(SD_ERROR);
// 		}
		
// 		timeout = SDIO_WRITETIMEOUT;
//     	//发送CMD25,SDIO_WRITE_MULT_BLOCK,参数:字节地址,短返回,R1
// 	    SDIO_SendCommand(SDIO_WRITE_MULT_BLOCK,(BlockAddr << 9),SDIO_DataSelect | SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
// 	    errorstatus = CmdResp1Error();
// 	    if (errorstatus != SD_OK)
// 	    {
	    	// DebugPrintf("%d\r\n",errorstatus);
// 	    	return(errorstatus);
// 	    }
	    
// 	    do
// 	    {
// 	    	if(SDIO0->PRNSTS & BIT10)	//写缓冲区有效
// 	    	{
// 	    		SDIO_ClearFlag(SDIO_FLAG_BUFFWRITE);		//清除写缓冲区有效标志
// 	    		//缓冲区大小SD_BlockSize字节,共SD_BlockSize / 4字
// 			  	for(cnt = 0;cnt < SD_BlockSize / 4;cnt ++)
// 			  	{
// 			  		SDIO_WriteData(*BlockBuff ++);
// 				}
// 			  	BlockCnt --;	//读完了一块
// 			  	timeout = SDIO_READTIMEOUT;	//读完了一块,超时计数器重新开始计数
// 	    	}
// 	    	timeout --;
// 	    }while(BlockCnt && (timeout != 0) && (SDIO_GetFlagStatus(SDIO_FLAG_DATATIMEOUT) == 0));	//剩余块数量不为0,没超时
	    
// 		//发送CMD12 SDIO_STOP_TRANSMISSION命令,终止读取;参数:0,短响应,R1
// 		SDIO_SendCommand(SDIO_STOP_TRANSMISSION,0,SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
// 		errorstatus = CmdResp1Error();
// 		if (errorstatus != SD_OK)
// 		{
			// DebugPrintf("%d\r\n",errorstatus);
// 			return(errorstatus);
// 		}
		
// 		if(timeout == 0)	//超时
// 		{
// 			errorstatus = SD_DATA_TIMEOUT;
			// DebugPrintf("%d\r\n",errorstatus);
// 			return(errorstatus);
// 		}
		
// 		SDIO_ClearFlag(SDIO_FLAG_ALL);	//清除所有中断标志
// 	}
	
// 	//读取卡状态,等待写入完成
// 	do
// 	{
// 		errorstatus = IsCardProgramming(&cardstate);
// 	}while ((errorstatus == SD_OK) && ((cardstate == SD_CARD_PROGRAMMING) || (cardstate == SD_CARD_RECEIVING)));
	
// 	return(errorstatus);
// }





/*************************************************************************************************************************
* 函数	:	static SD_Error CmdError(void)
* 功能	:	指令执行状态(无响应)
* 参数	:	无
* 返回	:	SD_OK:成功,其它见SD Card Error code.
* 依赖	:	底层寄存器操作函数
* 作者	:	陈鹏
* 时间	:	20120516
* 最后修改时间 : 20120516
* 说明	: 	返回指令执行结果(内部函数)
*************************************************************************************************************************/
static SD_Error CmdError(void)
{
  SD_Error errorstatus = SD_OK;
  u32 timeout;

  timeout = SDIO_CMD0TIMEOUT;

  while ((timeout > 0) && (SDIO_GetFlagStatus(SDIO_FLAG_COMMANDEND) == 0))
  {
    timeout--;
  }

  if (timeout == 0)
  {
    errorstatus = SD_CMD_RSP_TIMEOUT;
    return(errorstatus);
  }

  //清除所有标志
  SDIO_ClearFlag(SDIO_FLAG_ALL);

  return(errorstatus);
}


/*************************************************************************************************************************
* 函数	:	static SD_Error CmdResp7Error(void)
* 功能	:	指令执行状态(R7)
* 参数	:	无
* 返回	:	SD_OK:成功,其它见SD Card Error code.
* 依赖	:	底层寄存器操作函数
* 作者	:	陈鹏
* 时间	:	20120516
* 最后修改时间 : 20120516
* 说明	: 	返回指令执行结果(内部函数)
*************************************************************************************************************************/
static SD_Error CmdResp7Error(void)
{
  SD_Error errorstatus = SD_OK;
 // u32 status;
  u32 timeout = SDIO_CMD0TIMEOUT;

  do
  {
		timeout--;
  }
	while (!(SDIO_STATUS & (SDIO_FLAG_COMMANDCRC | SDIO_FLAG_COMMANDEND | SDIO_FLAG_COMMANDTIMEOUT)) && (timeout > 0));
	
  if ((timeout == 0) || (SDIO_STATUS & SDIO_FLAG_COMMANDTIMEOUT))
  {
    /* Card is not V2.0 complient or card does not support the set voltage range */
    errorstatus = SD_CMD_RSP_TIMEOUT;
    SDIO_ClearFlag(SDIO_FLAG_COMMANDTIMEOUT);
    return(errorstatus);
  }

  if (SDIO_STATUS & SDIO_FLAG_COMMANDEND)
  {
    /* Card is SD V2.0 compliant */
    errorstatus = SD_OK;
    SDIO_ClearFlag(SDIO_FLAG_COMMANDEND);
    return(errorstatus);
  }
  //清除所有标志
  SDIO_ClearFlag(SDIO_FLAG_ALL);
  
  return(errorstatus);

}


/*************************************************************************************************************************
* 函数	:	static SD_Error CmdResp1Error(void)
* 功能	:	指令执行状态(R1)
* 参数	:	无
* 返回	:	SD_OK:成功,其它见SD Card Error code.
* 依赖	:	底层寄存器操作函数
* 作者	:	陈鹏
* 时间	:	20120516
* 最后修改时间 : 20120516
* 说明	: 	返回指令执行结果(内部函数)
*************************************************************************************************************************/
// use
static SD_Error CmdResp1Error(void)
{
  SD_Error errorstatus = SD_OK;
  u32 response_r1;
  
	while (!(SDIO_STATUS & (SDIO_FLAG_COMMANDCRC | SDIO_FLAG_COMMANDEND | SDIO_FLAG_COMMANDTIMEOUT | SDIO_FLAG_COMMANDINDEX)));
	if (SDIO_STATUS & SDIO_FLAG_COMMANDTIMEOUT)
	{
	    errorstatus = SD_CMD_RSP_TIMEOUT;
	    SDIO_ClearFlag(SDIO_FLAG_COMMANDTIMEOUT);
	    return(errorstatus);
	}
	else if (SDIO_STATUS & SDIO_FLAG_COMMANDCRC)
 	{
	    errorstatus = SD_CMD_CRC_FAIL;
	    SDIO_ClearFlag(SDIO_FLAG_COMMANDCRC);
	    return(errorstatus);
  	}
	else if (SDIO_STATUS & SDIO_FLAG_COMMANDINDEX)
	{
	    errorstatus = SD_ILLEGAL_CMD;
	    SDIO_ClearFlag(SDIO_FLAG_COMMANDINDEX);
	    return(errorstatus);
	}

  //清除所有标志
  SDIO_ClearFlag(SDIO_FLAG_ALL);

  /* We have received response, retrieve it for analysis  */
  response_r1 = SDIO_GetResponse(SDIO_RESP1);
  if ((response_r1 & SD_OCR_ERRORBITS) == SD_ALLZERO)
  {
    return(errorstatus);
  }

  if (response_r1 & SD_OCR_ADDR_OUT_OF_RANGE)
  {
    return(SD_ADDR_OUT_OF_RANGE);
  }

  if (response_r1 & SD_OCR_ADDR_MISALIGNED)
  {
    return(SD_ADDR_MISALIGNED);
  }

  if (response_r1 & SD_OCR_BLOCK_LEN_ERR)
  {
    return(SD_BLOCK_LEN_ERR);
  }

  if (response_r1 & SD_OCR_ERASE_SEQ_ERR)
  {
    return(SD_ERASE_SEQ_ERR);
  }

  if (response_r1 & SD_OCR_BAD_ERASE_PARAM)
  {
    return(SD_BAD_ERASE_PARAM);
  }

  if (response_r1 & SD_OCR_WRITE_PROT_VIOLATION)
  {
    return(SD_WRITE_PROT_VIOLATION);
  }

  if (response_r1 & SD_OCR_LOCK_UNLOCK_FAILED)
  {
    return(SD_LOCK_UNLOCK_FAILED);
  }

  if (response_r1 & SD_OCR_COM_CRC_FAILED)
  {
    return(SD_COM_CRC_FAILED);
  }

  if (response_r1 & SD_OCR_ILLEGAL_CMD)
  {
    return(SD_ILLEGAL_CMD);
  }

  if (response_r1 & SD_OCR_CARD_ECC_FAILED)
  {
    return(SD_CARD_ECC_FAILED);
  }

  if (response_r1 & SD_OCR_CC_ERROR)
  {
    return(SD_CC_ERROR);
  }

  if (response_r1 & SD_OCR_GENERAL_UNKNOWN_ERROR)
  {
    return(SD_GENERAL_UNKNOWN_ERROR);
  }

  if (response_r1 & SD_OCR_STREAM_READ_UNDERRUN)
  {
    return(SD_STREAM_READ_UNDERRUN);
  }

  if (response_r1 & SD_OCR_STREAM_WRITE_OVERRUN)
  {
    return(SD_STREAM_WRITE_OVERRUN);
  }

  if (response_r1 & SD_OCR_CID_CSD_OVERWRIETE)
  {
    return(SD_CID_CSD_OVERWRITE);
  }

  if (response_r1 & SD_OCR_WP_ERASE_SKIP)
  {
    return(SD_WP_ERASE_SKIP);
  }

  if (response_r1 & SD_OCR_CARD_ECC_DISABLED)
  {
    return(SD_CARD_ECC_DISABLED);
  }

  if (response_r1 & SD_OCR_ERASE_RESET)
  {
    return(SD_ERASE_RESET);
  }

  if (response_r1 & SD_OCR_AKE_SEQ_ERROR)
  {
    return(SD_AKE_SEQ_ERROR);
  }
  return(errorstatus);
}


/*************************************************************************************************************************
* 函数	:	static SD_Error CmdResp3Error(void)
* 功能	:	指令执行状态(R3,OCR)
* 参数	:	无
* 返回	:	SD_OK:成功,其它见SD Card Error code.
* 依赖	:	底层寄存器操作函数
* 作者	:	陈鹏
* 时间	:	20120516
* 最后修改时间 : 20120516
* 说明	: 	返回指令执行结果(内部函数)
*************************************************************************************************************************/
static SD_Error CmdResp3Error(void)
{
  SD_Error errorstatus = SD_OK;
//  u32 status;

	while (!(SDIO_STATUS & (SDIO_FLAG_COMMANDCRC | SDIO_FLAG_COMMANDEND | SDIO_FLAG_COMMANDTIMEOUT)));
  if (SDIO_STATUS & SDIO_FLAG_COMMANDTIMEOUT)
  {
    errorstatus = SD_CMD_RSP_TIMEOUT;
    SDIO_ClearFlag(SDIO_FLAG_COMMANDTIMEOUT);
    return(errorstatus);
  }
  //清除所有标志
  SDIO_ClearFlag(SDIO_FLAG_ALL);
  
  return(errorstatus);
}



/*************************************************************************************************************************
* 函数	:	static SD_Error CmdResp2Error(void)
* 功能	:	指令执行状态(R2,CID or CSD)
* 参数	:	无
* 返回	:	SD_OK:成功,其它见SD Card Error code.
* 依赖	:	底层寄存器操作函数
* 作者	:	陈鹏
* 时间	:	20120516
* 最后修改时间 : 20120516
* 说明	: 	返回指令执行结果(内部函数)
*************************************************************************************************************************/
static SD_Error CmdResp2Error(void)
{
  SD_Error errorstatus = SD_OK;
//  u32 status;

	while (!(SDIO_STATUS & (SDIO_FLAG_COMMANDCRC | SDIO_FLAG_COMMANDEND | SDIO_FLAG_COMMANDTIMEOUT)));

  if (SDIO_STATUS & SDIO_FLAG_COMMANDTIMEOUT)
  {
    errorstatus = SD_CMD_RSP_TIMEOUT;
    SDIO_ClearFlag(SDIO_FLAG_COMMANDTIMEOUT);
    return(errorstatus);
  }
  else if (SDIO_STATUS & SDIO_FLAG_COMMANDCRC)
  {
    errorstatus = SD_CMD_CRC_FAIL;
    SDIO_ClearFlag(SDIO_FLAG_COMMANDCRC);
    return(errorstatus);
  }

  //清除所有标志
  SDIO_ClearFlag(SDIO_FLAG_ALL);

  return(errorstatus);
}


/*************************************************************************************************************************
* 函数	:	static SD_Error CmdResp6Error(u16 *prca)
* 功能	:	指令执行状态(R6,RCA)
* 参数	:	prca:RCA缓冲区指针
* 返回	:	SD_OK:成功,其它见SD Card Error code.
* 依赖	:	底层寄存器操作函数
* 作者	:	陈鹏
* 时间	:	20120516
* 最后修改时间 : 20120516
* 说明	: 	返回指令执行结果(内部函数)
*************************************************************************************************************************/
static SD_Error CmdResp6Error(u16 *prca)
{
  SD_Error errorstatus = SD_OK;
 // u32 status;
  u32 response_r1;
  
  	while (!(SDIO_STATUS & (SDIO_FLAG_COMMANDCRC | SDIO_FLAG_COMMANDEND | SDIO_FLAG_COMMANDTIMEOUT | SDIO_FLAG_COMMANDINDEX)));
  if (SDIO_STATUS & SDIO_FLAG_COMMANDTIMEOUT)
  {
    errorstatus = SD_CMD_RSP_TIMEOUT;
    SDIO_ClearFlag(SDIO_FLAG_COMMANDTIMEOUT);
    return(errorstatus);
  }
  else if (SDIO_STATUS & SDIO_FLAG_COMMANDCRC)
  {
    errorstatus = SD_CMD_CRC_FAIL;
    SDIO_ClearFlag(SDIO_FLAG_COMMANDCRC);
    return(errorstatus);
  }
  else if (SDIO_STATUS & SDIO_FLAG_COMMANDINDEX)
  {
    errorstatus = SD_ILLEGAL_CMD;
    SDIO_ClearFlag(SDIO_FLAG_COMMANDINDEX);
    return(errorstatus);
  }
  
  //清除所有标志
  SDIO_ClearFlag(SDIO_FLAG_ALL);

  /* We have received response, retrieve it.  */
  response_r1 = SDIO_GetResponse(SDIO_RESP1);

  if (SD_ALLZERO == (response_r1 & (SD_R6_GENERAL_UNKNOWN_ERROR | SD_R6_ILLEGAL_CMD | SD_R6_COM_CRC_FAILED)))
  {
    *prca = (u16) (response_r1 >> 16);
    return(errorstatus);
  }

  if (response_r1 & SD_R6_GENERAL_UNKNOWN_ERROR)
  {
    return(SD_GENERAL_UNKNOWN_ERROR);
  }

  if (response_r1 & SD_R6_ILLEGAL_CMD)
  {
    return(SD_ILLEGAL_CMD);
  }

  if (response_r1 & SD_R6_COM_CRC_FAILED)
  {
    return(SD_COM_CRC_FAILED);
  }

  return(errorstatus);
}


/*************************************************************************************************************************
* 函数	:	static SD_Error FindSCR(u16 rca, u32 *pscr)
* 功能	:	获取SCR信息
* 参数	:	rca:卡地址,prca:RCA缓冲区指针
* 返回	:	SD_OK:成功,其它见SD Card Error code.
* 依赖	:	底层寄存器操作函数
* 作者	:	陈鹏
* 时间	:	20120516
* 最后修改时间 : 20120516
* 说明	: 	返回指令执行结果(内部函数)
*************************************************************************************************************************/
static SD_Error FindSCR(u16 rca, u32 *pscr)
{
	SD_Error errorstatus = SD_OK;
	u32 tempscr[2] = {0, 0};
	u32 timeout = SDIO_READTIMEOUT;//读等待超时计数器

	//设置块大小为8字节
	//发送SDIO_SET_BLOCKLEN,参数8,短响应
	SDIO_SendCommand(SDIO_SET_BLOCKLEN,8,SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
	errorstatus = CmdResp1Error();

	if (errorstatus != SD_OK)
	{
		// DebugPrintf("SDIO_SET_BLOCKLEN error(%d)!\n",errorstatus);	//调试,打印错误信息
		goto _error;
	}
	
	//发送CMD55 SDIO_APP_CMD;命令参数:RCA;返回响应R1,设置RCA为0,短响应
  	SDIO_SendCommand(SDIO_APP_CMD,(u32)rca << 16,SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
	errorstatus = CmdResp1Error();
	if (errorstatus != SD_OK)
	{
		// DebugPrintf("CMD55 error(%d)!\n",errorstatus);	//调试,打印错误信息
		goto _error;
	}
	
	SDIO_SoftwareReset(SDIO_RSTDAT);	//软件复位DAT,主要用来清空FIFO
	SDIO_ReadMode();					//设置主机控制器为读模式
	SDIO_SetTransferBlockSize(8);		//设置单次传输的块大小为8b
	SDIO_SingleBlockMode();				//设置为单块传输模式

	//发送ACMD51 SD_APP_SEND_SCR,参数0,短响应,R1
	SDIO_SendCommand(SDIO_SD_APP_SEND_SCR,0,SDIO_DataSelect | SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
	errorstatus = CmdResp1Error();
	if (errorstatus != SD_OK)
	{
		// DebugPrintf("ACMD51 error(%d)!\n",errorstatus);	//调试,打印错误信息
		goto _error;
	}
	
	while((SDIO_GetFlagStatus(SDIO_FLAG_BUFFREAD) == 0) && timeout && (SDIO_GetFlagStatus(SDIO_FLAG_DATATIMEOUT) == 0))	//等待读缓冲区有效,或者等待超时
	{
		timeout --;
	}
	
	if (SDIO_GetFlagStatus(SDIO_FLAG_DATATIMEOUT) || (timeout == 0))	//超时
    {
		SDIO_ClearFlag(SDIO_FLAG_DATATIMEOUT);	//清除标志
 		errorstatus = SD_DATA_TIMEOUT;
		goto _error;
    }
	
	//缓冲区大小8字节,共2字
	tempscr[0] = SDIO_ReadData();
	tempscr[1] = SDIO_ReadData();


	//清除所有标志
	SDIO_ClearFlag(SDIO_FLAG_ALL);

	*(pscr + 1) = ((tempscr[0] & SD_0TO7BITS) << 24) | ((tempscr[0] & SD_8TO15BITS) << 8) | ((tempscr[0] & SD_16TO23BITS) >> 8) | ((tempscr[0] & SD_24TO31BITS) >> 24);

	*(pscr) = ((tempscr[1] & SD_0TO7BITS) << 24) | ((tempscr[1] & SD_8TO15BITS) << 8) | ((tempscr[1] & SD_16TO23BITS) >> 8) | ((tempscr[1] & SD_24TO31BITS) >> 24);

_error:
	//恢复块大小为 SD_BlockSize
	SDIO_SetTransferBlockSize(SD_BlockSize);//设置传输块大小为512字节
	errorstatus = SD_SetBlockSize(SD_BlockSize);//配置SD卡块大小	
	if (errorstatus != SD_OK)
	{
		// DebugPrintf("SD SetBlockSize error(%d)!\n",errorstatus);
		return(errorstatus);
	}
	
	return(errorstatus);
}





// /*******************************************************************************
// * Function Name  : IsCardProgramming
// * Description    : Checks if the SD card is in programming state.
// * Input          : pstatus: pointer to the variable that will contain the SD
// *                  card state.
// * Output         : None
// * Return         : SD_Error: SD Card Error code.
// *******************************************************************************/
// static SD_Error IsCardProgramming(u8 *pstatus)
// {
//   SD_Error errorstatus = SD_OK;
//   vu32 respR1 = 0, status = 0;


//   //发送SDIO_SEND_STATUS,返回状态
//   SDIO_SendCommand(SDIO_SEND_STATUS,(u32) RCA << 16,SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
	
// 	while (!(SDIO_STATUS & (SDIO_FLAG_COMMANDCRC | SDIO_FLAG_COMMANDEND | SDIO_FLAG_COMMANDTIMEOUT | SDIO_FLAG_COMMANDINDEX)));
//   if (SDIO_STATUS & SDIO_FLAG_COMMANDTIMEOUT)
//   {
//     errorstatus = SD_CMD_RSP_TIMEOUT;
//     SDIO_ClearFlag(SDIO_FLAG_COMMANDTIMEOUT);
//     return(errorstatus);
//   }
//   else if (SDIO_STATUS & SDIO_FLAG_COMMANDCRC)
//   {
//     errorstatus = SD_CMD_CRC_FAIL;
//     SDIO_ClearFlag(SDIO_FLAG_COMMANDCRC);
//     return(errorstatus);
//   }
//   else if (SDIO_STATUS & SDIO_FLAG_COMMANDINDEX)
//   {
//     errorstatus = SD_ILLEGAL_CMD;
//     SDIO_ClearFlag(SDIO_FLAG_COMMANDINDEX);
//     return(errorstatus);
//   }

// 	//清除所有标志
//   SDIO_ClearFlag(SDIO_FLAG_ALL);


//   /* We have received response, retrieve it for analysis  */
//   respR1 = SDIO_GetResponse(SDIO_RESP1);

//   /* Find out card status */
//   *pstatus = (u8) ((respR1 >> 9) & 0x0000000F);

//   if ((respR1 & SD_OCR_ERRORBITS) == SD_ALLZERO)
//   {
//     return(errorstatus);
//   }

//   if (respR1 & SD_OCR_ADDR_OUT_OF_RANGE)
//   {
//     return(SD_ADDR_OUT_OF_RANGE);
//   }

//   if (respR1 & SD_OCR_ADDR_MISALIGNED)
//   {
//     return(SD_ADDR_MISALIGNED);
//   }

//   if (respR1 & SD_OCR_BLOCK_LEN_ERR)
//   {
//     return(SD_BLOCK_LEN_ERR);
//   }

//   if (respR1 & SD_OCR_ERASE_SEQ_ERR)
//   {
//     return(SD_ERASE_SEQ_ERR);
//   }

//   if (respR1 & SD_OCR_BAD_ERASE_PARAM)
//   {
//     return(SD_BAD_ERASE_PARAM);
//   }

//   if (respR1 & SD_OCR_WRITE_PROT_VIOLATION)
//   {
//     return(SD_WRITE_PROT_VIOLATION);
//   }

//   if (respR1 & SD_OCR_LOCK_UNLOCK_FAILED)
//   {
//     return(SD_LOCK_UNLOCK_FAILED);
//   }

//   if (respR1 & SD_OCR_COM_CRC_FAILED)
//   {
//     return(SD_COM_CRC_FAILED);
//   }

//   if (respR1 & SD_OCR_ILLEGAL_CMD)
//   {
//     return(SD_ILLEGAL_CMD);
//   }

//   if (respR1 & SD_OCR_CARD_ECC_FAILED)
//   {
//     return(SD_CARD_ECC_FAILED);
//   }

//   if (respR1 & SD_OCR_CC_ERROR)
//   {
//     return(SD_CC_ERROR);
//   }

//   if (respR1 & SD_OCR_GENERAL_UNKNOWN_ERROR)
//   {
//     return(SD_GENERAL_UNKNOWN_ERROR);
//   }

//   if (respR1 & SD_OCR_STREAM_READ_UNDERRUN)
//   {
//     return(SD_STREAM_READ_UNDERRUN);
//   }

//   if (respR1 & SD_OCR_STREAM_WRITE_OVERRUN)
//   {
//     return(SD_STREAM_WRITE_OVERRUN);
//   }

//   if (respR1 & SD_OCR_CID_CSD_OVERWRIETE)
//   {
//     return(SD_CID_CSD_OVERWRITE);
//   }

//   if (respR1 & SD_OCR_WP_ERASE_SKIP)
//   {
//     return(SD_WP_ERASE_SKIP);
//   }

//   if (respR1 & SD_OCR_CARD_ECC_DISABLED)
//   {
//     return(SD_CARD_ECC_DISABLED);
//   }

//   if (respR1 & SD_OCR_ERASE_RESET)
//   {
//     return(SD_ERASE_RESET);
//   }

//   if (respR1 & SD_OCR_AKE_SEQ_ERROR)
//   {
//     return(SD_AKE_SEQ_ERROR);
//   }

//   return(errorstatus);
// }



////SD卡中断服务程序,用来检测卡的插入与移除的
//static void __irq Isr_SDMMC_Card(void)
//{
//	if(SDIO_GetFlagStatus(SDIO_FLAG_CARDREMOVAL))	//卡移除了
//	{
	//	DebugPrintf("SDMMC Card MOVAL!\n");			//调试,打印错误信息
//		SDIO_ClearFlag(SDIO_FLAG_CARDREMOVAL);		//清除卡移除中断
//		SDMMC0_MOV_Flag = 1;	//卡移除了
//	}
//	if(SDIO_GetFlagStatus(SDIO_FLAG_CARDINSERTION))	//卡插入了
//	{
	//	DebugPrintf("SDMMC Card IN!\n");			//调试,打印错误信息
//		SDIO_ClearFlag(SDIO_FLAG_CARDINSERTION);	//清除卡插入中断
//	}
//	VICInterruptEnd();	//中断结束
//}

