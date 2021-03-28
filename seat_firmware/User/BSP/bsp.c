#include "bsp.h"
#include "user_controllogic.h"

/*
*********************************************************************************************************
*
*	模块名称 : BSP模块(For SeatControlBoard_V1)
*	文件名称 : bsp.c
*	版    本 : V1.0
*	说    明 : 这是硬件底层驱动程序的主文件。每个c文件可以通过 #include "bsp.h" 来包含所有的外设（片内及板上）的头文件，
             实现调用他们的对外接口函数功能。本文件的函数主要是在RTX启动前完成硬件及数据的初始化
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2019-04-23  WLM      草稿
*
*********************************************************************************************************
*/

/*main.c中的全局变量*/
extern uint16_t Axis_LinkPoint_Position[5];
extern Axis_Cbrt_Data_T Axis_Cbrt_Data[4];
extern Cbrt_Data_Storage_T SeatCbrtData;
extern uint32_t RecordNum;
extern uint8_t RecordStorageBlockNum;
extern uint16_t Seat_Position_Now[4];
extern uint16_t Seat_Position_Last[4];
extern uint16_t Motor_Current_Now[4];
extern uint8_t  Motor_Status_Flag[4];
extern Motor_Ctrl_Struct_T Motor_Ctrl_Info[4];
extern uint8_t RecordSeekOK;
extern uint8_t CbrtDataOK;
extern RTC_Time_T TimeOfRtc;

/*本文件内的变量*/
uint8_t comstrlen;
uint8_t comInfo[80];

/*本文件内的辅助函数前置声明*/

void GetAllMotorStatus(void);
void bsp_SoftDelay(uint32_t delayValue);
/*********************************************************************************************************
*	函 数 名: bsp_Init
*	功能说明: 初始化所有的硬件设备。该函数配置CPU寄存器和外设的寄存器并初始化一些全局变量。只需要调用一次
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_Init(void)
{ 
	uint8_t InitFlag_I2C,InitFlag_RTC;

	/*
		由于ST固件库的启动文件已经执行了CPU系统时钟的初始化，所以不必再次重复配置系统时钟。

		系统时钟配置为180MHz，如果需要更改，可以修改 system_stm32f4xx.c 文件
	*/
	//RCC_DeInit();
	//NVIC_DeInit();
	/* 优先级分组设置为4，0bit子优先级，4bit抢占优先级。*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//这里设置之后，其他文件内不允许更改分组，只允许对具体中断进行等级设定
	
	//NVIC_SetVectorTable(NVIC_VectTab_FLASH,NVIC_VECTOR_TABLE_OFFSET);
	
	/*RS485串口初始化*/
  RS485_UART_Init();
	/*sprintf不会控制写入长度，因此务必保证字符串<cominfo长度，否则会数组溢出*/
	comstrlen=sprintf((char*)comInfo,"USART1 Init OK\n");//串口助手看不到这个信息说明串口有问题
	UartSendBuf(comInfo, comstrlen);
	
	
	/*LED指示灯控制引脚PB4，PB5初始化*/
	LED_GPIO_Config();
	
	/*模拟I2C初始化*/
	InitFlag_I2C=NVM_I2C_Init();
	if(InitFlag_I2C)
	{
		comstrlen=sprintf((char*)comInfo,"I2C Init OK,NVM Chip Online\n");
	}
	else
	{
		comstrlen=sprintf((char*)comInfo,"I2C Init Failed or NVM Chip Offline\n");
	}
	UartSendBuf(comInfo, comstrlen);
	
	
	/*SPI总线初始化*/
	Motor_SPI_Init();
  comstrlen=sprintf((char*)comInfo,"SPI Inited\n");//只指示初始化完成，不管成功与否
  UartSendBuf(comInfo, comstrlen);
	
	/*片内ADC初始化*/
	Motor_ADC_Init();
	comstrlen=sprintf((char*)comInfo,"ADC Inited\n");
	UartSendBuf(comInfo, comstrlen);
	
	/*片内RTC初始化*/
	InitFlag_RTC=Record_RTC_Init();
	if(InitFlag_RTC)
	{
		RTC_TimeAndDate_Get(&TimeOfRtc);
		comstrlen=sprintf((char*)comInfo,"RTC OK,System Time is %d-%02d-%02d %02d:%02d:%02d\n",TimeOfRtc.RecordTime_Year,
	  TimeOfRtc.RecordTime_Month,TimeOfRtc.RecordTime_Date,TimeOfRtc.RecordTime_Hour,TimeOfRtc.RecordTime_Min,TimeOfRtc.RecordTime_Second);
		UartSendBuf(comInfo, comstrlen);
	}
	else
	{
		comstrlen=sprintf((char*)comInfo,"RTC Time Is Blank\n");
		UartSendBuf(comInfo, comstrlen);
	}
	
	/*矩阵键盘初始化*/
	Key_Init();
	comstrlen=sprintf((char*)comInfo,"Keyboard Inited\n");
	UartSendBuf(comInfo, comstrlen);
	
	/*电机驱动芯片初始化*/
	MotorInit();
	comstrlen=sprintf((char*)comInfo,"Motor Driver Chips Inited\n");
	UartSendBuf(comInfo, comstrlen);
	
  comstrlen=sprintf((char*)comInfo,"-------------Board Initialization Over------------\n");
	UartSendBuf(comInfo, comstrlen);
	
	/*通过串口发送上述调试信息*/
	//DmaSendDataProc();
}

/*********************************************************************************************************
*	函 数 名: data_Init
*	功能说明: RTX运行前初始化必要的数据，只运行一次
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void data_Init(void)
{
	/*FRAM中运行记录读取正确*/
	RecordSeekOK=SeekRecord(&RecordNum,&RecordStorageBlockNum);
	if(RecordSeekOK)
	{
		comstrlen=sprintf((char*)comInfo,"NVM Record Seek OK,RecordNum=0x%08X,StorageBlockNum=%d \n",RecordNum,RecordStorageBlockNum);
		UartSendBuf(comInfo, comstrlen);
	}
	else
	{
		comstrlen=sprintf((char*)comInfo,"NVM Record Seek Failed\n");
		UartSendBuf(comInfo, comstrlen);
	}
	
	/*Flash中标定数据读取正确*/
	CbrtDataOK=Flash_ReadRecord(&SeatCbrtData,Axis_LinkPoint_Position,Axis_Cbrt_Data);
  if(CbrtDataOK)
	{
		comstrlen=sprintf((char*)comInfo,"Calibration Data Read OK\n");
		SeatLogicDataInit();
	}
	else
	{
		comstrlen=sprintf((char*)comInfo,"Calibration Data Read Failed\n");
		bsp_SetDefaultControlValue();
	}
	UartSendBuf(comInfo, comstrlen);
	
	/*采集电位器值、电流值（数值不关心）*/
  Motor_GetPositionCurrentData(Seat_Position_Now,Motor_Current_Now);
	comstrlen=sprintf((char*)comInfo,"Position and Current Initial Value Get OK \n");
	UartSendBuf(comInfo, comstrlen);
	
	/*用于计算两次周期的轴位移量*/
	memcpy(Seat_Position_Last,Seat_Position_Now,4*sizeof(uint16_t));
		
	/*电机错误标记数组初始化*/
	GetAllMotorStatus();
	comstrlen=sprintf((char*)comInfo,"Motor Driver Chips Status Reg Value is 0x%02X,0x%02X,0x%02X,0x%02X\n",Motor_Status_Flag[0],
		                            Motor_Status_Flag[1],Motor_Status_Flag[2],Motor_Status_Flag[3]);
	UartSendBuf(comInfo, comstrlen);
	
	comstrlen=sprintf((char*)comInfo,"-------------Data Initialization Over-------------\n");
	UartSendBuf(comInfo, comstrlen);
	
	DmaSendDataProc();	
	
}


/*********************************************************************************************************
*	函 数 名: GetAllMotorStatus
*	功能说明: 获取4个DRV8873的错误寄存器数值
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void GetAllMotorStatus(void)
{
	uint8_t index;
	
	for(index=0;index<4;index++)
	{
		Motor_Status_Flag[index]=Motor_GetChipStatus(index);
	}
}

/*********************************************************************************************************
*	函 数 名: bsp_WatchdogInit
*	功能说明: 初始化片内看门狗，时钟使用LSI
*	形    参：WDGTime,看门狗的复位时间，单位ms
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_WatchdogInit(uint32_t WDGTime)
{
		
	/* 检测系统是否从独立看门狗复位中恢复 */
	if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
	{		
		/* 清除复位标志 */
		RCC_ClearFlag();
	}
	
	/* 使能LSI */
	RCC_LSICmd(ENABLE);
	
	/* 等待直到LSI就绪 */
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
	{}
	
	/* 写入0x5555表示允许访问IWDG_PR 和IWDG_RLR寄存器 */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	/*  LSI/32 分频*/
	IWDG_SetPrescaler(IWDG_Prescaler_32);
	
	IWDG_SetReload(WDGTime);
	
	/* 重载IWDG计数 */
	IWDG_ReloadCounter();

	/* 使能 IWDG (LSI oscillator 由硬件使能) */
	IWDG_Enable();		
}

/*********************************************************************************************************
*	函 数 名: bsp_SoftDelay
*	功能说明: 软延时
*	形    参：delayvalue
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_SoftDelay(uint32_t delayValue)
{
	while(delayValue)
	{
		delayValue--;
	}
}

/*********************************************************************************************************
*	函 数 名: bsp_SetDefaultControlValue
*	功能说明: 对于没有标定过或读取Flash记录失败的进行基本参数初始化
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_SetDefaultControlValue(void)
{
	uint8_t axisIndex;
	
	for(axisIndex=0;axisIndex<4;axisIndex++)
	{
		Axis_Cbrt_Data[axisIndex].Axis_Motor_Speed=50;
		Axis_Cbrt_Data[axisIndex].Axis_Motor_Reverse=1;
		Axis_Cbrt_Data[axisIndex].Axis_Poten_Reverse=1;
	}
	
}

