#include "user_rtx.h"


//-----------------------------------����ջ--------------------------------------------------
/*����ջ*/
static uint64_t TaskStk_UartRcv[256/8];
static uint64_t TaskStk_UartTsmt[256/8];
static uint64_t TaskStk_KeyScan[256/8];
static uint64_t TaskStk_ADCValueGain[512/8];
static uint64_t TaskStk_SeatControlLogic[512/8];
static uint64_t TaskStk_SeatInfoRecord[512/8];
static uint64_t TaskStk_MotorStatusCheck[256/8];
static uint64_t TaskStk_Watchdog[256/8];
//-----------------------------------OS������ź������ڴ�ص�--------------------------------
OS_TID HandleTaskUartRcv=NULL;
OS_TID HandleTaskUartTsmt=NULL;
OS_TID HandleTaskADCGetValue=NULL;
OS_TID HandleTaskKeyScan=NULL;
OS_TID HandleTaskSeatControlLogic=NULL;
OS_TID HandleTaskSeatInfoRecord=NULL;
OS_TID HandleTaskMotorStatusCheck=NULL;
OS_TID HandleTaskTaskSystemWatchdog=NULL;
OS_SEM UartTrans;
//-----------------------------------��������------------------------------------------------
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
//-----------------------------------RTX����-------------------------------------------------
/*
*********************************************************************************************************
*	�� �� ��: ��������
*	����˵��: �����������񣬳�ʼ��һЩ������ɾ������
*********************************************************************************************************
*/
__task void Task_System_Setup(void)
{
	while(1)
	{ /*�ȴ���һ�׶εĳ�ʼ����Ϣ������*/
		os_dly_wait(200);
		
		/*�����е������оƬ��Ϊ�״̬*/
		Motor_ChangeToStatus_Active(Motor_Channel_123);
		
		/*��������Ϣ���䡢memory��ʼ��*/
	  RTX_Key_Init();
		
		/*ͬ��485�շ�ͨ�����ź�����ʼ��Ϊ1*/
		os_sem_init(UartTrans,1);
			
		/*�������Ź�����ʱ1s*/
	 // bsp_WatchdogInit(1000);
		
		/*������������,�������ȼ����ø��ڵ�ǰ����*/
//	  HandleTaskSeatInfoRecord     =os_tsk_create_user(Task_Seat_InfoRecord,1,&TaskStk_SeatInfoRecord,sizeof(TaskStk_SeatInfoRecord));
		HandleTaskUartRcv            =os_tsk_create_user(Task_Uart_Receive,2,&TaskStk_UartRcv,sizeof(TaskStk_UartRcv));
		HandleTaskUartTsmt           =os_tsk_create_user(Task_Uart_Transmit,3,&TaskStk_UartTsmt,sizeof(TaskStk_UartTsmt));
		HandleTaskKeyScan            =os_tsk_create_user(Task_Key_Scan,4,&TaskStk_KeyScan,sizeof(TaskStk_KeyScan));
	  HandleTaskADCGetValue        =os_tsk_create_user(Task_ADC_GetValue,5,&TaskStk_ADCValueGain,sizeof(TaskStk_ADCValueGain));
		HandleTaskSeatControlLogic   =os_tsk_create_user(Task_Seat_ControlLogic,6,&TaskStk_SeatControlLogic,sizeof(TaskStk_SeatControlLogic));
//    HandleTaskMotorStatusCheck   =os_tsk_create_user(Task_Motor_StatusCheck,7,TaskStk_MotorStatusCheck,sizeof(TaskStk_MotorStatusCheck));
  //  HandleTaskTaskSystemWatchdog =os_tsk_create_user(Task_System_Watchdog,8,&TaskStk_Watchdog,sizeof(TaskStk_Watchdog));

		/*ɾ������*/
		os_tsk_delete_self();		
	}  
}

/*
*********************************************************************************************************
*	�� �� ��: RS485�շ�����
*	����˵��: �����жϺ������õ��¼���ǽ��ж�Ӧ����
*********************************************************************************************************
*/
__task void Task_Uart_Receive(void)
{
	OS_RESULT ret;
	uint16_t uart_event_wait_interval=10;//��λ��ms
	
	while(1)
	{
		ret=os_evt_wait_and(event_uart_received,uart_event_wait_interval);
	  		
	  switch(ret)
		  {
				 case OS_R_TMO :  break;
				
				 case OS_R_EVT:   UartDataProcess();//���������ڴ˴���
					                DmaReceiveReopen();				                
                          break;				 
		  }
			
		/*���ÿ��Ź�������¼���־λ*/
	//	os_evt_set(WTD_EVT_BIT_0, HandleTaskTaskSystemWatchdog);
		/*����20ms*/
		os_dly_wait(20);		
	}
}

/*
*********************************************************************************************************
*	�� �� ��: RS485��������
*	����˵��: �����жϺ������õ��¼���ǽ��ж�Ӧ����
*********************************************************************************************************
*/
__task void Task_Uart_Transmit(void)
{
	OS_RESULT ret1;
	uint16_t uart_event_wait_interval=10;//��λ��ms
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
		/*���ÿ��Ź�������¼���־λ*/
	//	os_evt_set(WTD_EVT_BIT_1, HandleTaskTaskSystemWatchdog);
		os_dly_wait(20);		
	}
}

/*
*********************************************************************************************************
*	�� �� ��: ADC����
*	����˵��: �Ը�ͨ����������λ��ֵ����ADC���ò�����ƽ��ֵ
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
		
		/*����λ���ٶ�*/
		for(index=0;index<4;index++)
		{
			Axis_MoveVelocity[index]=Seat_Position_Now[index]-Seat_Position_Last[index];
		}
		
		/*��λ�����ݸ���Last*/
		memcpy(Seat_Position_Last,Seat_Position_Now,4*sizeof(uint16_t));
		
		/*���ÿ��Ź�������¼���־λ*/
		os_evt_set(WTD_EVT_BIT_2, HandleTaskTaskSystemWatchdog);
		os_dly_wait(20);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: �����ɼ�����
*	����˵��: �ɼ�������İ���������Ϣ
*********************************************************************************************************
*/
__task void Task_Key_Scan(void)
{
	while(1)
	{ /*ȫ������ɨ��*/
	  Key_ScanAll();
		
		/*���ÿ��Ź�������¼���־λ*/
	//	os_evt_set(WTD_EVT_BIT_3, HandleTaskTaskSystemWatchdog);
		os_dly_wait(20);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: �����߼���������
*	����˵��: ����Ϣ�����ð���������Ϣ�����ݱ궨��������Ӧ�ĵ������
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
			/* ���յ����������к��������ĸ���֪ͨ */
			case OS_R_EVT:SeatLogicDataInit();/*���¼����ж��߼��������*/
	                	break;	
			case OS_R_TMO:break;	/* ��ʱ */			            			
			default: 	break; /* ����ֵ������ */          	
		}	
	
		KeyValue=Key_GetFromFIFO();//����Ϣ�����ȡ����
		
	  /*�����������������ٰ���������������ȡʧ�ܣ���û��ɨ�赽������û�д��룬��ȡʧ��*/
		if(KeyValue<0)
     {
       KeyValue=0;
		 }
		 
		if(KeyValue>0)
     { /*���м�ֵ����ӡ*/
			 //testlen=sprintf((char*)teststr,"Key Value is %d \n",KeyValue);
	     UartSendBuf(teststr, testlen);
		 }
			  			 
		/*�궨������*/
	  if(CbrtDataOK)
		{
		   ControlLogic_CbrtedSeat(KeyValue);
	  }
		else/*δ�궨������*/
		{
			 ControlLogic_NonCbrtedSeat(KeyValue);
		}
		
		/*���ÿ��Ź�������¼���־λ*/
		//os_evt_set(WTD_EVT_BIT_4, HandleTaskTaskSystemWatchdog);
		os_dly_wait(20);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: ������Ϣ��¼
*	����˵��: ��¼���ι���ʱ�������Ϣ
*********************************************************************************************************
*/
__task void Task_Seat_InfoRecord(void)
{
	static uint8_t dataNum=0;
	
	while(1)
	{
		 RTC_TimeAndDate_Get(&TimeOfRtc);
		/*Rtcʱ��*/
		SeatInfoRecordMemory[8+dataNum*31]=TimeOfRtc.RecordTime_Year;
		SeatInfoRecordMemory[8+dataNum*31+1]=TimeOfRtc.RecordTime_Month;
		SeatInfoRecordMemory[8+dataNum*31+2]=TimeOfRtc.RecordTime_Date;
		SeatInfoRecordMemory[8+dataNum*31+3]=TimeOfRtc.RecordTime_Hour;
		SeatInfoRecordMemory[8+dataNum*31+4]=TimeOfRtc.RecordTime_Min;
		SeatInfoRecordMemory[8+dataNum*31+5]=TimeOfRtc.RecordTime_Second;
		/*4�����stage��int�ͣ����з��������ٶ�ֵ��������uint�洢����ȡʱע������������*/
		memcpy((SeatInfoRecordMemory+8+dataNum*31+6),MotorStage,4);
		/*4�����λ�ã�8���ֽ�*/
		memcpy((SeatInfoRecordMemory+8+dataNum*31+10),Seat_Position_Now,8);
		/*4·����*/
		memcpy((SeatInfoRecordMemory+8+dataNum*31+18),Motor_Current_Now,8);
		
		dataNum++;
		/*�Ѽ�¼4������Ҫд��NVM*/
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
		
		/*���ÿ��Ź�������¼���־λ*/
		os_evt_set(WTD_EVT_BIT_5, HandleTaskTaskSystemWatchdog);
		os_dly_wait(20);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: ���״̬��Ϣ��ѯ��������
*	����˵��: ͨ��SPI������ѯ�������оƬ��״̬���д�������ͬʱʹ��LED��ָʾ����״̬
            ��Ƴ����̵��� �������оƬ�д���
            ������̵Ƴ��� ����
            ������̵���   �궨���ݼ��ز���ȷ

*********************************************************************************************************
*/
__task void Task_Motor_StatusCheck(void)
{
  uint8_t MotorNum;
	
  while(1)
	{
		/*SPIͨ��ʱ��ֹ�����л�*/
		tsk_lock();
		/*ͨ��SPI��ȡоƬ�����־*/
		for(MotorNum=0;MotorNum<4;MotorNum++)
		{
			Motor_Status_Flag[MotorNum]=Motor_GetChipStatus(MotorNum);
		}
			
		tsk_unlock();
		/*����д��󣬽��д���*/
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
		//--------�����Ǽ��������------------------
		
		if(!CbrtDataOK)
		{
			LED_RUN_OFF();//�궨���ݳ���������ʹ�ó���ȱʡ�������ݣ��̵���
		}
		else
		{
			LED_RUN_ON();
		}
		//--------�����Ǽ��궨���ݼ�¼--------------
		
		/*���ÿ��Ź�������¼���־λ*/
	  os_evt_set(WTD_EVT_BIT_6, HandleTaskTaskSystemWatchdog);
		os_dly_wait(20);	
	}
}

/*
*********************************************************************************************************
*	�� �� ��: ���Ź�����
*	����˵��: ��������ͨ���¼���־֪ͨ�������յ����б�־��ι��
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
			/* ���յ��������񶼷������¼���־ */
			case OS_R_EVT:
				IWDG_ReloadCounter();	//ι��
				break;	
			/* ��ʱ */
			case OS_R_TMO:
				break;		
			/* ����ֵ������ */
			default:                     
				break;
		}	
	}
}
