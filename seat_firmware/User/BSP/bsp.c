#include "bsp.h"
#include "user_controllogic.h"

/*
*********************************************************************************************************
*
*	ģ������ : BSPģ��(For SeatControlBoard_V1)
*	�ļ����� : bsp.c
*	��    �� : V1.0
*	˵    �� : ����Ӳ���ײ�������������ļ���ÿ��c�ļ�����ͨ�� #include "bsp.h" ���������е����裨Ƭ�ڼ����ϣ���ͷ�ļ���
             ʵ�ֵ������ǵĶ���ӿں������ܡ����ļ��ĺ�����Ҫ����RTX����ǰ���Ӳ�������ݵĳ�ʼ��
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2019-04-23  WLM      �ݸ�
*
*********************************************************************************************************
*/

/*main.c�е�ȫ�ֱ���*/
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

/*���ļ��ڵı���*/
uint8_t comstrlen;
uint8_t comInfo[80];

/*���ļ��ڵĸ�������ǰ������*/

void GetAllMotorStatus(void);
void bsp_SoftDelay(uint32_t delayValue);
/*********************************************************************************************************
*	�� �� ��: bsp_Init
*	����˵��: ��ʼ�����е�Ӳ���豸���ú�������CPU�Ĵ���������ļĴ�������ʼ��һЩȫ�ֱ�����ֻ��Ҫ����һ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_Init(void)
{ 
	uint8_t InitFlag_I2C,InitFlag_RTC;

	/*
		����ST�̼���������ļ��Ѿ�ִ����CPUϵͳʱ�ӵĳ�ʼ�������Բ����ٴ��ظ�����ϵͳʱ�ӡ�

		ϵͳʱ������Ϊ180MHz�������Ҫ���ģ������޸� system_stm32f4xx.c �ļ�
	*/
	//RCC_DeInit();
	//NVIC_DeInit();
	/* ���ȼ���������Ϊ4��0bit�����ȼ���4bit��ռ���ȼ���*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//��������֮�������ļ��ڲ�������ķ��飬ֻ����Ծ����жϽ��еȼ��趨
	
	//NVIC_SetVectorTable(NVIC_VectTab_FLASH,NVIC_VECTOR_TABLE_OFFSET);
	
	/*RS485���ڳ�ʼ��*/
  RS485_UART_Init();
	/*sprintf�������д�볤�ȣ������ر�֤�ַ���<cominfo���ȣ�������������*/
	comstrlen=sprintf((char*)comInfo,"USART1 Init OK\n");//�������ֿ����������Ϣ˵������������
	UartSendBuf(comInfo, comstrlen);
	
	
	/*LEDָʾ�ƿ�������PB4��PB5��ʼ��*/
	LED_GPIO_Config();
	
	/*ģ��I2C��ʼ��*/
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
	
	
	/*SPI���߳�ʼ��*/
	Motor_SPI_Init();
  comstrlen=sprintf((char*)comInfo,"SPI Inited\n");//ָֻʾ��ʼ����ɣ����ܳɹ����
  UartSendBuf(comInfo, comstrlen);
	
	/*Ƭ��ADC��ʼ��*/
	Motor_ADC_Init();
	comstrlen=sprintf((char*)comInfo,"ADC Inited\n");
	UartSendBuf(comInfo, comstrlen);
	
	/*Ƭ��RTC��ʼ��*/
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
	
	/*������̳�ʼ��*/
	Key_Init();
	comstrlen=sprintf((char*)comInfo,"Keyboard Inited\n");
	UartSendBuf(comInfo, comstrlen);
	
	/*�������оƬ��ʼ��*/
	MotorInit();
	comstrlen=sprintf((char*)comInfo,"Motor Driver Chips Inited\n");
	UartSendBuf(comInfo, comstrlen);
	
  comstrlen=sprintf((char*)comInfo,"-------------Board Initialization Over------------\n");
	UartSendBuf(comInfo, comstrlen);
	
	/*ͨ�����ڷ�������������Ϣ*/
	//DmaSendDataProc();
}

/*********************************************************************************************************
*	�� �� ��: data_Init
*	����˵��: RTX����ǰ��ʼ����Ҫ�����ݣ�ֻ����һ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void data_Init(void)
{
	/*FRAM�����м�¼��ȡ��ȷ*/
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
	
	/*Flash�б궨���ݶ�ȡ��ȷ*/
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
	
	/*�ɼ���λ��ֵ������ֵ����ֵ�����ģ�*/
  Motor_GetPositionCurrentData(Seat_Position_Now,Motor_Current_Now);
	comstrlen=sprintf((char*)comInfo,"Position and Current Initial Value Get OK \n");
	UartSendBuf(comInfo, comstrlen);
	
	/*���ڼ����������ڵ���λ����*/
	memcpy(Seat_Position_Last,Seat_Position_Now,4*sizeof(uint16_t));
		
	/*��������������ʼ��*/
	GetAllMotorStatus();
	comstrlen=sprintf((char*)comInfo,"Motor Driver Chips Status Reg Value is 0x%02X,0x%02X,0x%02X,0x%02X\n",Motor_Status_Flag[0],
		                            Motor_Status_Flag[1],Motor_Status_Flag[2],Motor_Status_Flag[3]);
	UartSendBuf(comInfo, comstrlen);
	
	comstrlen=sprintf((char*)comInfo,"-------------Data Initialization Over-------------\n");
	UartSendBuf(comInfo, comstrlen);
	
	DmaSendDataProc();	
	
}


/*********************************************************************************************************
*	�� �� ��: GetAllMotorStatus
*	����˵��: ��ȡ4��DRV8873�Ĵ���Ĵ�����ֵ
*	��    �Σ���
*	�� �� ֵ: ��
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
*	�� �� ��: bsp_WatchdogInit
*	����˵��: ��ʼ��Ƭ�ڿ��Ź���ʱ��ʹ��LSI
*	��    �Σ�WDGTime,���Ź��ĸ�λʱ�䣬��λms
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_WatchdogInit(uint32_t WDGTime)
{
		
	/* ���ϵͳ�Ƿ�Ӷ������Ź���λ�лָ� */
	if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
	{		
		/* �����λ��־ */
		RCC_ClearFlag();
	}
	
	/* ʹ��LSI */
	RCC_LSICmd(ENABLE);
	
	/* �ȴ�ֱ��LSI���� */
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
	{}
	
	/* д��0x5555��ʾ�������IWDG_PR ��IWDG_RLR�Ĵ��� */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	/*  LSI/32 ��Ƶ*/
	IWDG_SetPrescaler(IWDG_Prescaler_32);
	
	IWDG_SetReload(WDGTime);
	
	/* ����IWDG���� */
	IWDG_ReloadCounter();

	/* ʹ�� IWDG (LSI oscillator ��Ӳ��ʹ��) */
	IWDG_Enable();		
}

/*********************************************************************************************************
*	�� �� ��: bsp_SoftDelay
*	����˵��: ����ʱ
*	��    �Σ�delayvalue
*	�� �� ֵ: ��
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
*	�� �� ��: bsp_SetDefaultControlValue
*	����˵��: ����û�б궨�����ȡFlash��¼ʧ�ܵĽ��л���������ʼ��
*	��    �Σ���
*	�� �� ֵ: ��
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

