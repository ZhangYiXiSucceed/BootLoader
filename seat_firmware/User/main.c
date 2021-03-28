#include "stm32f4xx.h"
#include "bsp.h"
#include "RTL.h"
#include "user_datastructure.h"
#include "user_rtx.h"
//-------------------------------------ȫ�ֱ���--------------------------------------
/*���ε�ǰλ��*/
 uint16_t Seat_Position_Now[4];
 uint16_t Seat_Position_Last[4];
 /*4���˶��ٶȣ��Ե�λ����ֵ��Ϊ���٣�*/
 int16_t Axis_MoveVelocity[4];
/*�����ǰ����*/
 uint16_t Motor_Current_Now[4];
/*DRV8873����״̬�Ĵ���ֵ*/
uint8_t  Motor_Status_Flag[4];
/*nFault�����жϱ�־*/
uint8_t  Motor_Status_IT[4];
/*5��������ADCֵ*/
uint16_t Axis_LinkPoint_Position[5];
/*4��궨����*/
Axis_Cbrt_Data_T Axis_Cbrt_Data[4];
/*��¼���ݣ�����RAM��Flash֮���ṩ��ʱ�洢*/
Cbrt_Data_Storage_T SeatCbrtData;
/*����˶����ƽṹ������*/
Motor_Ctrl_Struct_T Motor_Ctrl_Info[4];
/*FRAM�еļ�¼��*/
uint32_t RecordNum;
/*FRAM�м�¼�洢���Զ���Block��*/
uint8_t RecordStorageBlockNum;
/*FRAM�����ɹ���־*/
uint8_t RecordSeekOK;
/*�궨���ݳɹ���ȡ��־*/
uint8_t CbrtDataOK;
/*����״̬*/
Seat_Status_T SeatStatus;
/*д��NVM�ļ�¼*/
uint8_t SeatInfoRecordMemory[128];
/*RTC��ʱ��*/
RTC_Time_T TimeOfRtc;
/*���Ƴ���汾*/
uint8_t AppVer=10;
//------------------------------------RTXϵͳ��ر���---------------------------------
/*�������������ջ*/
static uint64_t TaskStk_SystemSetup[512/8];

//------------------------------------������------------------------------------------
int main(void)
{ 
	//RCC_DeInit();
	
	
	
	//__enable_irq();
	
	/*Ӳ����ʼ��*/	
	bsp_Init();
	
	/*RTXϵͳ����ǰ�����ݳ�ʼ��*/
	data_Init();
	
	NVIC_SetVectorTable(NVIC_VectTab_FLASH,NVIC_VECTOR_TABLE_OFFSET);
	
	/*��ʼ��RTXϵͳ�������������������������ȼ�Ϊ9,����ǰ��ߣ�*/
  os_sys_init_user(Task_System_Setup,9,&TaskStk_SystemSetup,sizeof(TaskStk_SystemSetup));
	
  /*��ѭ��*/
	while(1){};
}

