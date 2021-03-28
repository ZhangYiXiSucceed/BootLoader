#include "user_rtx.h"


//-----------------------------------任务栈--------------------------------------------------
/*任务栈*/
static uint64_t TaskStk_UartRcv[256/8];
static uint64_t TaskStk_UartTsmt[256/8];
static uint64_t TaskStk_KeyScan[256/8];
static uint64_t TaskStk_ADCValueGain[512/8];
static uint64_t TaskStk_SeatControlLogic[512/8];
static uint64_t TaskStk_SeatInfoRecord[512/8];
static uint64_t TaskStk_MotorStatusCheck[256/8];
static uint64_t TaskStk_Watchdog[256/8];
//-----------------------------------OS句柄、信号量、内存池等--------------------------------
OS_TID HandleTaskUartRcv=NULL;
OS_TID HandleTaskUartTsmt=NULL;
OS_TID HandleTaskADCGetValue=NULL;
OS_TID HandleTaskKeyScan=NULL;
OS_TID HandleTaskSeatControlLogic=NULL;
OS_TID HandleTaskSeatInfoRecord=NULL;
OS_TID HandleTaskMotorStatusCheck=NULL;
OS_TID HandleTaskTaskSystemWatchdog=NULL;
OS_SEM UartTrans;
//-----------------------------------其他变量------------------------------------------------
extern uint8_t  UART1_ReceiveSize;
extern uint8_t  UART1_TransmitSize;
extern uint16_t Seat_Position_Now[4];
extern uint16_t Seat_Position_Last[4];
extern uint16_t Motor_Current_Now[4]; 
extern int16_t  Axis_MoveVelocity[4];

extern Seat_Status_T SeatStatus;
extern uint8_t CbrtDataOK;
extern uint32_t RecordNum;
extern uint8_t RecordStorageBlockNum;
extern uint8_t SeatInfoRecordMemory[128];
extern RTC_Time_T TimeOfRtc;
extern uint8_t Motor_Status_Flag[4];
extern int8_t MotorStage[4];

uint8_t testMode=0;
//-----------------------------------RTX任务-------------------------------------------------
/*
*********************************************************************************************************
*	任 务 名: 启动任务
*	功能说明: 启动其它任务，初始化一些参数，删除自身
*********************************************************************************************************
*/
__task void Task_System_Setup(void)
{
	while(1)
	{ /*等待上一阶段的初始化信息输出完成*/
		os_dly_wait(200);
		
		/*将所有电机驱动芯片置为活动状态*/
		Motor_ChangeToStatus_Active(Motor_Channel_123);
		
		/*按键的消息邮箱、memory初始化*/
	  RTX_Key_Init();
		
		/*同步485收发通道的信号量初始化为1*/
		os_sem_init(UartTrans,1);
			
		/*开启看门狗，定时1s*/
	 // bsp_WatchdogInit(1000);
		
		/*创建其他任务,任务优先级不得高于当前任务*/
//	  HandleTaskSeatInfoRecord     =os_tsk_create_user(Task_Seat_InfoRecord,1,&TaskStk_SeatInfoRecord,sizeof(TaskStk_SeatInfoRecord));
		HandleTaskUartRcv            =os_tsk_create_user(Task_Uart_Receive,2,&TaskStk_UartRcv,sizeof(TaskStk_UartRcv));
		HandleTaskUartTsmt           =os_tsk_create_user(Task_Uart_Transmit,3,&TaskStk_UartTsmt,sizeof(TaskStk_UartTsmt));
		HandleTaskKeyScan            =os_tsk_create_user(Task_Key_Scan,4,&TaskStk_KeyScan,sizeof(TaskStk_KeyScan));
	  HandleTaskADCGetValue        =os_tsk_create_user(Task_ADC_GetValue,5,&TaskStk_ADCValueGain,sizeof(TaskStk_ADCValueGain));
		HandleTaskSeatControlLogic   =os_tsk_create_user(Task_Seat_ControlLogic,6,&TaskStk_SeatControlLogic,sizeof(TaskStk_SeatControlLogic));
//    HandleTaskMotorStatusCheck   =os_tsk_create_user(Task_Motor_StatusCheck,7,TaskStk_MotorStatusCheck,sizeof(TaskStk_MotorStatusCheck));
  //  HandleTaskTaskSystemWatchdog =os_tsk_create_user(Task_System_Watchdog,8,&TaskStk_Watchdog,sizeof(TaskStk_Watchdog));

		/*删除自身*/
		os_tsk_delete_self();		
	}  
}

/*
*********************************************************************************************************
*	任 务 名: RS485收发任务
*	功能说明: 根据中断函数设置的事件标记进行对应处理
*********************************************************************************************************
*/
__task void Task_Uart_Receive(void)
{
	OS_RESULT ret;
	uint16_t uart_event_wait_interval=10;//单位是ms
	
	while(1)
	{
		ret=os_evt_wait_and(event_uart_received,uart_event_wait_interval);
	  		
	  switch(ret)
		  {
				 case OS_R_TMO :  break;
				
				 case OS_R_EVT:   UartDataProcess();//串口数据在此处理
					                DmaReceiveReopen();				                
                          break;				 
		  }
			
		/*设置看门狗任务的事件标志位*/
	//	os_evt_set(WTD_EVT_BIT_0, HandleTaskTaskSystemWatchdog);
		/*挂起20ms*/
		os_dly_wait(20);		
	}
}

/*
*********************************************************************************************************
*	任 务 名: RS485发送任务
*	功能说明: 根据中断函数设置的事件标记进行对应处理
*********************************************************************************************************
*/
__task void Task_Uart_Transmit(void)
{
	OS_RESULT ret1;
	uint16_t uart_event_wait_interval=10;//单位是ms
//	int8_t keyvaluetest;
//	uint8_t teststr[30];
//  uint8_t testlen;
	while(1)
	{
		//keyvaluetest=Key_GetFromFIFO();
		
//		if(keyvaluetest>0)
//		{
//			testlen=sprintf((char*)teststr,"Key Value is %d \n",keyvaluetest);
//	    UartSendBuf(teststr, testlen);
//		}
		
		if(UART1_TransmitSize)
		{
			ret1=os_sem_wait(UartTrans,uart_event_wait_interval);
		
	    if(ret1!=OS_R_TMO)
		   {
			  DmaSendDataProc();	
		   }
		}	
		/*设置看门狗任务的事件标志位*/
	//	os_evt_set(WTD_EVT_BIT_1, HandleTaskTaskSystemWatchdog);
		os_dly_wait(20);		
	}
}

/*
*********************************************************************************************************
*	任 务 名: ADC任务
*	功能说明: 对各通道电流、电位器值进行ADC采用并计算平均值
*********************************************************************************************************
*/
__task void Task_ADC_GetValue(void)
{
	uint8_t index;
	
  while(1)
	{	
		tsk_lock();
		
		Motor_GetPotenValue();
		Motor_GetCurrentValue();
		
		tsk_unlock();
		
		Motor_ADCValueAverage(Seat_Position_Now,Motor_Current_Now);
		
		/*计算位移速度*/
		for(index=0;index<4;index++)
		{
			Axis_MoveVelocity[index]=Seat_Position_Now[index]-Seat_Position_Last[index];
		}
		
		/*新位置数据覆盖Last*/
		memcpy(Seat_Position_Last,Seat_Position_Now,4*sizeof(uint16_t));
		
		/*设置看门狗任务的事件标志位*/
		os_evt_set(WTD_EVT_BIT_2, HandleTaskTaskSystemWatchdog);
		os_dly_wait(20);
	}
}

/*
*********************************************************************************************************
*	任 务 名: 按键采集任务
*	功能说明: 采集按键板的按键输入信息
*********************************************************************************************************
*/
__task void Task_Key_Scan(void)
{
	while(1)
	{ /*全部按键扫描*/
	  Key_ScanAll();
		
		/*设置看门狗任务的事件标志位*/
	//	os_evt_set(WTD_EVT_BIT_3, HandleTaskTaskSystemWatchdog);
		os_dly_wait(20);
	}
}

/*
*********************************************************************************************************
*	任 务 名: 按键逻辑处理任务
*	功能说明: 从消息邮箱获得按键输入信息，根据标定与否输出对应的电机动作
*********************************************************************************************************
*/
__task void Task_Seat_ControlLogic(void)
{
	int8_t KeyValue;
	OS_RESULT xResult;
	uint8_t usMaxWaitTime;
	

  uint8_t teststr[30];
  uint8_t testlen;
	
	while(1)
	{	
		usMaxWaitTime=1;
	  xResult = os_evt_wait_and(SCG_EVT_BIT_0 , usMaxWaitTime);	
		
		switch (xResult)
		{
			/* 接收到串口任务中函数发来的更新通知 */
			case OS_R_EVT:SeatLogicDataInit();/*重新加载判断逻辑所需参数*/
	                	break;	
			case OS_R_TMO:break;	/* 超时 */			            			
			default: 	break; /* 其他值不处理 */          	
		}	
	
		KeyValue=Key_GetFromFIFO();//从消息邮箱获取按键
		
	  /*下面包含两种情况，①按键放入正常，读取失败；②没有扫描到按键，没有存入，读取失败*/
		if(KeyValue<0)
     {
       KeyValue=0;
		 }
		 
		if(KeyValue>0)
     { /*空闲键值不打印*/
			 //testlen=sprintf((char*)teststr,"Key Value is %d \n",KeyValue);
	     UartSendBuf(teststr, testlen);
		 }
			  			 
		/*标定的座椅*/
	  if(CbrtDataOK)
		{
		   ControlLogic_CbrtedSeat(KeyValue);
	  }
		else/*未标定的座椅*/
		{
			 ControlLogic_NonCbrtedSeat(KeyValue);
		}
		
		/*设置看门狗任务的事件标志位*/
		//os_evt_set(WTD_EVT_BIT_4, HandleTaskTaskSystemWatchdog);
		os_dly_wait(20);
	}
}

/*
*********************************************************************************************************
*	任 务 名: 运行信息记录
*	功能说明: 记录座椅工作时的相关信息
*********************************************************************************************************
*/
__task void Task_Seat_InfoRecord(void)
{
	static uint8_t dataNum=0;
	
	while(1)
	{
		 RTC_TimeAndDate_Get(&TimeOfRtc);
		/*Rtc时间*/
		SeatInfoRecordMemory[8+dataNum*31]=TimeOfRtc.RecordTime_Year;
		SeatInfoRecordMemory[8+dataNum*31+1]=TimeOfRtc.RecordTime_Month;
		SeatInfoRecordMemory[8+dataNum*31+2]=TimeOfRtc.RecordTime_Date;
		SeatInfoRecordMemory[8+dataNum*31+3]=TimeOfRtc.RecordTime_Hour;
		SeatInfoRecordMemory[8+dataNum*31+4]=TimeOfRtc.RecordTime_Min;
		SeatInfoRecordMemory[8+dataNum*31+5]=TimeOfRtc.RecordTime_Second;
		/*4个电机stage，int型，既有方向又有速度值，这里以uint存储，读取时注意是有正负的*/
		memcpy((SeatInfoRecordMemory+8+dataNum*31+6),MotorStage,4);
		/*4个电机位置，8个字节*/
		memcpy((SeatInfoRecordMemory+8+dataNum*31+10),Seat_Position_Now,8);
		/*4路电流*/
		memcpy((SeatInfoRecordMemory+8+dataNum*31+18),Motor_Current_Now,8);
		
		dataNum++;
		/*已记录4条，需要写入NVM*/
		if(dataNum==4)
		{
			dataNum=0;
			
		  RecordNum++;
			SeatInfoRecordMemory[0]=(*(char *)(&RecordNum));
			SeatInfoRecordMemory[1]=(*((char *)(&RecordNum)+1));
			SeatInfoRecordMemory[2]=(*((char *)(&RecordNum)+2));
			SeatInfoRecordMemory[3]=(*((char *)(&RecordNum)+3));
			
			RecordStorageBlockNum++;
			
			if(RecordStorageBlockNum==64){RecordStorageBlockNum=0;}
		 
			WriteRecord(RecordStorageBlockNum,SeatInfoRecordMemory);			
		}
		
		/*设置看门狗任务的事件标志位*/
		os_evt_set(WTD_EVT_BIT_5, HandleTaskTaskSystemWatchdog);
		os_dly_wait(20);
	}
}

/*
*********************************************************************************************************
*	任 务 名: 电机状态信息查询及错误处理
*	功能说明: 通过SPI主动查询电机驱动芯片的状态，有错误则处理，同时使用LED灯指示工作状态
            红灯常亮绿灯灭 电机驱动芯片有错误
            红灯灭绿灯长亮 正常
            红灯灭绿灯灭   标定数据加载不正确

*********************************************************************************************************
*/
__task void Task_Motor_StatusCheck(void)
{
  uint8_t MotorNum;
	
  while(1)
	{
		/*SPI通信时禁止任务切换*/
		tsk_lock();
		/*通过SPI获取芯片错误标志*/
		for(MotorNum=0;MotorNum<4;MotorNum++)
		{
			Motor_Status_Flag[MotorNum]=Motor_GetChipStatus(MotorNum);
		}
			
		tsk_unlock();
		/*如果有错误，进行处理*/
		for(MotorNum=0;MotorNum<4;MotorNum++)
		{
			if(Motor_Status_Flag[MotorNum]!=0)
			{
				Motor_Error_Handle(MotorNum,Motor_Status_Flag[MotorNum]);
				LED_ERROR_ON();
				LED_RUN_OFF();
			}
			else
			{
				LED_ERROR_OFF();
			}
		}
		//--------以上是检测电机错误------------------
		
		if(!CbrtDataOK)
		{
			LED_RUN_OFF();//标定数据出错，控制器使用出厂缺省控制数据，绿灯灭
		}
		else
		{
			LED_RUN_ON();
		}
		//--------以上是检测标定数据记录--------------
		
		/*设置看门狗任务的事件标志位*/
	  os_evt_set(WTD_EVT_BIT_6, HandleTaskTaskSystemWatchdog);
		os_dly_wait(20);	
	}
}

/*
*********************************************************************************************************
*	任 务 名: 看门狗任务
*	功能说明: 其他任务通过事件标志通知本任务，收到所有标志后喂狗
*********************************************************************************************************
*/
__task void Task_System_Watchdog(void)
{
  OS_RESULT xResult;
	uint16_t usMaxBlockTime = 500;
	
	while(1)
	{
		xResult = os_evt_wait_and(WTD_EVT_BIT_ALL , usMaxBlockTime);
		
		switch (xResult)
		{
			/* 接收到所有任务都发来的事件标志 */
			case OS_R_EVT:
				IWDG_ReloadCounter();	//喂狗
				break;	
			/* 超时 */
			case OS_R_TMO:
				break;		
			/* 其他值不处理 */
			default:                     
				break;
		}	
	}
}
