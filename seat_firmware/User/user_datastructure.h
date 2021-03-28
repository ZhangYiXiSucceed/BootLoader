#ifndef __USER_DATASTRUCTURE_H
#define __USER_DATASTRUCTURE_H

#include "stdint.h"
#include "RTL.h"

/*���ε�ǰ����״̬��ö�ٱ���*/
typedef enum
{
  Seat_Status_Idle=0,
	Seat_Status_Back_Open,
	Seat_Status_Back_Close,
	Seat_Status_Leg_Open,
	Seat_Status_Leg_Close,
	Seat_Status_Foot_Open,
	Seat_Status_Foot_Close,
	Seat_Status_OneKeyOpen,
	Seat_Status_OneKeyHalf,
	Seat_Status_OneKeyClose,
	Seat_Status_EmergencyStop,
}Seat_Status_T;

/*������ÿ����궨���ݽṹ��*/
typedef __packed struct
{
  uint16_t Axis_Open_Position;
	uint16_t Axis_Close_Position;
	uint16_t Axis_Half_Position;
	int8_t  Axis_Motor_Reverse;//�������ָʾ��-1Ϊװ����1Ϊ����
	int8_t  Axis_Poten_Reverse;//��λ������ָʾ��-1Ϊװ����1Ϊ����
	uint8_t  Axis_Motor_Speed;
}Axis_Cbrt_Data_T;

/*���ڲ����߼��жϡ������ʵ�ʱ궨����,ÿ��3������*/
typedef __packed struct
{
  int16_t Open_Position;
	int16_t Close_Position;
	int16_t Half_Position;	
}Axis_Logic_Data_T;

/*���ڲ����߼��жϡ������ʵ�ʱ궨����,4��+5������*/
typedef __packed struct
{
  Axis_Logic_Data_T Axis_Logic_Data[4];
	
	int16_t LinkPoint_B1;
	int16_t LinkPoint_B2;
	int16_t LinkPoint_L1;
	int16_t LinkPoint_L2;
	int16_t LinkPoint_L3;	
}Seat_Logic_Data_T;

/*������ÿ������ƽṹ�壬���ڿ��Ƶ��*/
typedef __packed struct
{
  uint8_t  MotorNum;             /*0-3*/
	uint8_t  MotorEnable;          /*1=enable,0=disable*/
	int8_t   MotorDirection;       /*1=forward,-1=reverse*/
	uint8_t  MotorSpeedNow;        /*0-SpeedSet*/
	uint8_t  MotorSpeedSet;        /*0-100*/
	uint16_t MotorPositionNow;     /*��ǰλ��*/
	uint16_t MotorPositionSet;     /*Ŀ��λ��*/
	uint8_t  MotorPreviousStatus;  /*1=Run,0=Stop*/
	uint8_t  MotorBrakeStatus;     /*1=Braked,0=Release*/
}Motor_Ctrl_Struct_T;

/*���α궨���ݽṹ�壬�洢���ڲ�Flash*/
typedef __packed struct
{
	Axis_Cbrt_Data_T Axis_Cbrt_Data_Back;
	Axis_Cbrt_Data_T Axis_Cbrt_Data_Leg;
	Axis_Cbrt_Data_T Axis_Cbrt_Data_Foot;
	Axis_Cbrt_Data_T Axis_Cbrt_Data_Rsvd;
	uint16_t         Axis_LinkPoint_B1;
	uint16_t         Axis_LinkPoint_B2;
	uint16_t         Axis_LinkPoint_L1;
	uint16_t         Axis_LinkPoint_L2;
	uint16_t         Axis_LinkPoint_L3;
	uint8_t          Cbrt_Data_CheckSum;
}Cbrt_Data_Storage_T;

/*RTCʱ���¼�ṹ��*/
typedef __packed struct
{
	uint8_t  RecordTime_Year;
	uint8_t  RecordTime_Month;
	uint8_t  RecordTime_Date;
	uint8_t  RecordTime_Hour;
	uint8_t  RecordTime_Min;
	uint8_t  RecordTime_Second;
}RTC_Time_T;

/*���ε�����м�¼�ṹ�壬�ȴ洢��RAM����8����浽FRAM*/
typedef __packed struct
{
  RTC_Time_T RTC_Time;
	uint8_t  KeyValue;
	uint8_t  MotorNumber;
	uint8_t  MotorDirection;
	uint8_t  MotorSpeed;
	uint8_t  MotorStatus;
	uint16_t MotorCurrent;
	uint16_t MotorPosition;
}Motor_Motion_Record_T;


uint8_t CheckSum(uint8_t * dataBuf,uint16_t bufLength);
#endif

