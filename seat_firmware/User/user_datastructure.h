#ifndef __USER_DATASTRUCTURE_H
#define __USER_DATASTRUCTURE_H

#include "stdint.h"
#include "RTL.h"

/*座椅当前运行状态，枚举变量*/
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

/*座椅中每个轴标定数据结构体*/
typedef __packed struct
{
  uint16_t Axis_Open_Position;
	uint16_t Axis_Close_Position;
	uint16_t Axis_Half_Position;
	int8_t  Axis_Motor_Reverse;//电机反接指示，-1为装反，1为正常
	int8_t  Axis_Poten_Reverse;//电位器反接指示，-1为装反，1为正常
	uint8_t  Axis_Motor_Speed;
}Axis_Cbrt_Data_T;

/*用于参与逻辑判断、计算的实际标定数据,每轴3个数据*/
typedef __packed struct
{
  int16_t Open_Position;
	int16_t Close_Position;
	int16_t Half_Position;	
}Axis_Logic_Data_T;

/*用于参与逻辑判断、计算的实际标定数据,4轴+5联动点*/
typedef __packed struct
{
  Axis_Logic_Data_T Axis_Logic_Data[4];
	
	int16_t LinkPoint_B1;
	int16_t LinkPoint_B2;
	int16_t LinkPoint_L1;
	int16_t LinkPoint_L2;
	int16_t LinkPoint_L3;	
}Seat_Logic_Data_T;

/*座椅中每个轴控制结构体，用于控制电机*/
typedef __packed struct
{
  uint8_t  MotorNum;             /*0-3*/
	uint8_t  MotorEnable;          /*1=enable,0=disable*/
	int8_t   MotorDirection;       /*1=forward,-1=reverse*/
	uint8_t  MotorSpeedNow;        /*0-SpeedSet*/
	uint8_t  MotorSpeedSet;        /*0-100*/
	uint16_t MotorPositionNow;     /*当前位置*/
	uint16_t MotorPositionSet;     /*目标位置*/
	uint8_t  MotorPreviousStatus;  /*1=Run,0=Stop*/
	uint8_t  MotorBrakeStatus;     /*1=Braked,0=Release*/
}Motor_Ctrl_Struct_T;

/*座椅标定数据结构体，存储于内部Flash*/
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

/*RTC时间记录结构体*/
typedef __packed struct
{
	uint8_t  RecordTime_Year;
	uint8_t  RecordTime_Month;
	uint8_t  RecordTime_Date;
	uint8_t  RecordTime_Hour;
	uint8_t  RecordTime_Min;
	uint8_t  RecordTime_Second;
}RTC_Time_T;

/*座椅电机运行记录结构体，先存储于RAM，满8条后存到FRAM*/
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

