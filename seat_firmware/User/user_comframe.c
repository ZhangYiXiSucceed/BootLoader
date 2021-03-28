#include "user_comframe.h"
#include "stdint.h"
#include "bsp_rs485.h"
#include "bsp_flash.h"
#include "bsp_key.h"
#include "user_datastructure.h"
#include "user_controllogic.h"
#include "rtl.h"

#define BYTE_L(dwTemp)       (*(char *)(&dwTemp))
#define BYTE_H(dwTemp)       (*((char *)(&dwTemp) + 1))

uint8_t receiveFrameBuf[16][8];//可以存储16个数据帧的数组
uint8_t receiveFrameCounter=0;
uint8_t transmitFrame[8];
uint8_t transmitFrameLen=0;


extern uint8_t g_RxBuf[UART1_RX_BUF_SIZE];
extern uint8_t testMode;
extern uint8_t AppVer;
extern uint8_t CbrtDataOK;
extern Cbrt_Data_Storage_T SeatCbrtData;
extern Axis_Cbrt_Data_T Axis_Cbrt_Data[4];
extern uint16_t Axis_LinkPoint_Position[5];
extern uint16_t Seat_Position_Now[4];
extern uint16_t Motor_Current_Now[4]; 

extern OS_TID HandleTaskSeatControlLogic;
#define SCG_EVT_BIT_0  (1 << 0) //通知控制逻辑任务更新控制数据
/*
*********************************************************************************************************
*	函 数 名: ComFrame_GetCheckSum
*	功能说明: 计算校验和
*	形    参: data    需要计算校验和的数组首地址
						datalen 字节数
*	返 回 值: sum     校验和
*********************************************************************************************************
*/
uint8_t ComFrame_GetCheckSum(uint8_t* data,uint8_t datalen)
{
 	uint8_t sum=0;
	uint8_t index=0;
	
	for(index=0;index<datalen;index++)
	{
	   sum+=*(data+index);
		 sum=sum&0xFF;	
	}
	
	return sum;
}

/*
*********************************************************************************************************
*	函 数 名: ComFrame_FrameCheck
*	功能说明: 校验数据帧是否合法
*	形    参: frameHeader 数组首地址，该地址由串口任务传入  					
*	返 回 值: 1 合法 0 非法
*********************************************************************************************************
*/
uint8_t ComFrame_FrameCheck(uint8_t* frameHeader)
{
	uint8_t checkSum=0;
	uint8_t dataLen=0;
	
	if(!frameHeader){return 0;}
	
	if(*frameHeader!=0x68){return 0;}
	
	dataLen=*(frameHeader+3);
	checkSum=ComFrame_GetCheckSum(frameHeader,(4+dataLen));
	
	if(checkSum==*(frameHeader+4+dataLen))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
*********************************************************************************************************
*	函 数 名: ComFrame_FrameCopyToBuf
*	功能说明: 将串口接收到的数据内含有的合法数据帧转移到帧数组等待处理
*	形    参: 无  					
*	返 回 值: 合法数据帧个数
*********************************************************************************************************
*/
uint8_t ComFrame_FrameCopyToBuf(void)
{
	uint8_t frameIndex=0;
	uint8_t frameOK=0;
	uint8_t byteIndex=0;
	uint8_t frameLength=0;
	
	for(;byteIndex<=(UART1_RX_BUF_SIZE-5);)
	{
		frameOK=ComFrame_FrameCheck(g_RxBuf+byteIndex);
		//是合法数据帧
		if(frameOK)
		{ 
			frameLength=*(g_RxBuf+byteIndex+3)+5;
			memcpy(&receiveFrameBuf[frameIndex][0],(g_RxBuf+byteIndex),frameLength);
			byteIndex+=frameLength;
			frameIndex++;	
			
			//if(frameIndex>15){frameIndex=0;}//循环覆盖
		}
		else
		{
			byteIndex++;
		}
	}
	
	memset(g_RxBuf,0,UART1_RX_BUF_SIZE);
	return frameIndex;
}


/*
*********************************************************************************************************
*	函 数 名: ComFrame_Action_ConnectionConfirm
*	功能说明: 通过回复数据帧来使上位机确认连接到下位机
*	形    参: 无  					
*	返 回 值: 无
*********************************************************************************************************
*/
void ComFrame_Action_ConnectionConfirm(void)
{
	transmitFrame[0]=0x68;
	transmitFrame[1]=0x03;
	transmitFrame[2]=0x00;//0000 0000
	transmitFrame[3]=0x00;
	transmitFrame[4]=ComFrame_GetCheckSum(transmitFrame,4);
	transmitFrameLen=5;
	
	UartSendBuf(transmitFrame, transmitFrameLen);
}

/*
*********************************************************************************************************
*	函 数 名: ComFrame_Action_SystemReboot
*	功能说明: 使用NVIC指令重启单片机，以回到BootLoader程序
*	形    参: 无  					
*	返 回 值: 无
*********************************************************************************************************
*/
void ComFrame_Action_SystemReboot(void)
{
	  __set_FAULTMASK(1);//关闭总中断,防止重启过程被别的中断打断
    NVIC_SystemReset();//请求单片机重启
}

/*
*********************************************************************************************************
*	函 数 名: ComFrame_Action_AppConfirm
*	功能说明: 通过回复数据帧，使上位机确认下位机工作在应用程序（APP）模式
*	形    参: 无  					
*	返 回 值: 无
*********************************************************************************************************
*/
void ComFrame_Action_AppConfirm(void)
{
	transmitFrame[0]=0x68;
	transmitFrame[1]=0x09;
	transmitFrame[2]=0x00;
	transmitFrame[3]=0x01;//数据域长度1
	transmitFrame[4]=0x01;//0x01表示下位机在app模式
	transmitFrame[5]=ComFrame_GetCheckSum(transmitFrame,5);
	transmitFrameLen=6;
	
	UartSendBuf(transmitFrame, transmitFrameLen);
}

/*
*********************************************************************************************************
*	函 数 名: ComFrame_Action_GotoTestMode
*	功能说明: 进入调试模式，设置标志位，使得按键逻辑进入到未标定状态并回复上位机
*	形    参: 无  					
*	返 回 值: 无
*********************************************************************************************************
*/
void ComFrame_Action_GotoTestMode(void)
{
	testMode=1;
	
	transmitFrame[0]=0x68;
	transmitFrame[1]=0x07;
	transmitFrame[2]=0x00;
	transmitFrame[3]=0x00;
	transmitFrame[4]=ComFrame_GetCheckSum(transmitFrame,4);
	transmitFrameLen=5;
	
	UartSendBuf(transmitFrame, transmitFrameLen);
}

/*
*********************************************************************************************************
*	函 数 名: ComFrame_Action_OutTestMode
*	功能说明: 退出调试模式，设置标志位，回复上位机
*	形    参: 无  					
*	返 回 值: 无
*********************************************************************************************************
*/
void ComFrame_Action_OutTestMode(void)
{
	testMode=0;
	
	transmitFrame[0]=0x68;
	transmitFrame[1]=0x08;
	transmitFrame[2]=0x00;
	transmitFrame[3]=0x00;
	transmitFrame[4]=ComFrame_GetCheckSum(transmitFrame,4);
	transmitFrameLen=5;
	
	UartSendBuf(transmitFrame, transmitFrameLen);
}

/*
*********************************************************************************************************
*	函 数 名: ComFrame_Action_ReportAppVersion
*	功能说明: 返回1字节app版本号，例如10表示V1.0 54表示V5.4
*	形    参: 无  					
*	返 回 值: 无
*********************************************************************************************************
*/
void ComFrame_Action_ReportAppVersion(void)
{	
	transmitFrame[0]=0x68;
	transmitFrame[1]=0x06;
	transmitFrame[2]=0x00;
	transmitFrame[3]=0x01;
	transmitFrame[4]=AppVer;
	transmitFrame[5]=ComFrame_GetCheckSum(transmitFrame,5);
	transmitFrameLen=6;
	
	UartSendBuf(transmitFrame, transmitFrameLen);
}

/*
*********************************************************************************************************
*	函 数 名: ComFrame_Read_GetMotorSpeedofSet
*	功能说明: 返回对应电机的设定速度值
*	形    参: 数据帧头					
*	返 回 值: 无
*********************************************************************************************************
*/
void ComFrame_Read_GetMotorSpeedofSet(uint8_t* frameHeader)
{ 
	uint8_t motorNum;
	
	if(frameHeader==NULL){return;}
	motorNum=(*(frameHeader+2))&0x03;
  if(motorNum>3){return;}
	
	transmitFrame[0]=0x68;
	transmitFrame[1]=0x10;
	transmitFrame[2]=motorNum;//000000xx xx是电机编码，整个数值刚好与motorNum一致
	transmitFrame[3]=0x01;
	transmitFrame[4]=Axis_Cbrt_Data[motorNum].Axis_Motor_Speed;
	transmitFrame[5]=ComFrame_GetCheckSum(transmitFrame,5);
	transmitFrameLen=6;
	
	UartSendBuf(transmitFrame, transmitFrameLen);
}

/*
*********************************************************************************************************
*	函 数 名: ComFrame_Read_GetMotorReverseFlag
*	功能说明: 返回对应电机的反接标志，若反接则数据域为1,若没有反接返回0
*	形    参: 数据帧头 					
*	返 回 值: 无
*********************************************************************************************************
*/
void ComFrame_Read_GetMotorReverseFlag(uint8_t* frameHeader)
{
	uint8_t motorNum;
	
	if(frameHeader==NULL){return;}
	motorNum=(*(frameHeader+2))&0x03;
  if(motorNum>3){return;}
	
	transmitFrame[0]=0x68;
	transmitFrame[1]=0x11;
	transmitFrame[2]=motorNum;
	transmitFrame[3]=0x01;
	transmitFrame[4]=Axis_Cbrt_Data[motorNum].Axis_Motor_Reverse>0?0:1;
	transmitFrame[5]=ComFrame_GetCheckSum(transmitFrame,5);
	transmitFrameLen=6;
	
	UartSendBuf(transmitFrame, transmitFrameLen);
}

/*
*********************************************************************************************************
*	函 数 名: ComFrame_Read_GetAxisClosePosition
*	功能说明: 返回对应轴的收起位置电位器值，电位器值用2个字节表示一个16位数据，高字节在前低字节在后
*	形    参: 数据帧头 					
*	返 回 值: 无
*********************************************************************************************************
*/
void ComFrame_Read_GetAxisClosePosition(uint8_t* frameHeader)
{
	uint8_t motorNum;
	
	if(frameHeader==NULL){return;}
	motorNum=(*(frameHeader+2))&0x03;
  if(motorNum>3){return;}
	
	transmitFrame[0]=0x68;
	transmitFrame[1]=0x12;
	transmitFrame[2]=motorNum;
	transmitFrame[3]=0x02;//2字节数据域，高字节在前，低字节在后
	transmitFrame[4]=BYTE_H(Axis_Cbrt_Data[motorNum].Axis_Close_Position);
	transmitFrame[5]=BYTE_L(Axis_Cbrt_Data[motorNum].Axis_Close_Position);
	transmitFrame[6]=ComFrame_GetCheckSum(transmitFrame,6);
	transmitFrameLen=7;
	
	UartSendBuf(transmitFrame, transmitFrameLen);
}
/*
*********************************************************************************************************
*	函 数 名: ComFrame_Read_GetAxisOpenPosition
*	功能说明: 返回对应轴的打开位置电位器值，电位器值用2个字节表示一个16位数据，高字节在前低字节在后
*	形    参: 数据帧头					
*	返 回 值: 无
*********************************************************************************************************
*/
void ComFrame_Read_GetAxisOpenPosition(uint8_t* frameHeader)
{ 
	uint8_t motorNum;
	
	if(frameHeader==NULL){return;}
	motorNum=(*(frameHeader+2))&0x03;
  if(motorNum>3){return;}
	
	transmitFrame[0]=0x68;
	transmitFrame[1]=0x13;
	transmitFrame[2]=motorNum;
	transmitFrame[3]=0x02;//2字节数据域，高字节在前，低字节在后
	transmitFrame[4]=BYTE_H(Axis_Cbrt_Data[motorNum].Axis_Open_Position);
	transmitFrame[5]=BYTE_L(Axis_Cbrt_Data[motorNum].Axis_Open_Position);
	transmitFrame[6]=ComFrame_GetCheckSum(transmitFrame,6);
	transmitFrameLen=7;
	
	UartSendBuf(transmitFrame, transmitFrameLen);
}

/*
*********************************************************************************************************
*	函 数 名: ComFrame_Read_GetPotenReverseFlag
*	功能说明: 返回对应轴电位器的反接标志，若反接则数据域为1,若没有反接返回0
*	形    参: 数据帧头					
*	返 回 值: 无
*********************************************************************************************************
*/
void ComFrame_Read_GetPotenReverseFlag(uint8_t* frameHeader)
{
	uint8_t motorNum;
	
	if(frameHeader==NULL){return;}
	motorNum=(*(frameHeader+2))&0x03;
  if(motorNum>3){return;}
	
	transmitFrame[0]=0x68;
	transmitFrame[1]=0x14;
	transmitFrame[2]=motorNum;
	transmitFrame[3]=0x01;
	transmitFrame[4]=Axis_Cbrt_Data[motorNum].Axis_Poten_Reverse>0?0:1;
	transmitFrame[5]=ComFrame_GetCheckSum(transmitFrame,5);
	transmitFrameLen=6;
	
	UartSendBuf(transmitFrame, transmitFrameLen);
}

/*
*********************************************************************************************************
*	函 数 名: ComFrame_Read_GetLinkPosition
*	功能说明: 返回背靠开始联动点的电位器值
*	形    参: 数据帧头					
*	返 回 值: 无
*********************************************************************************************************
*/
void ComFrame_Read_GetLinkPosition(uint8_t* frameHeader)
{
	uint8_t offset;
	
	if(frameHeader==NULL){return;}
	offset=*(frameHeader+1)-0x15;
  if(offset>4){return;}
	
	transmitFrame[0]=0x68;
	transmitFrame[1]=*(frameHeader+1);
	
	if(*(frameHeader+2)==0x0C)
	{
		transmitFrame[2]=0x00;
	}
	else
	{
		transmitFrame[2]=0x01;
	}

	transmitFrame[3]=0x02;
	
	/*5个联动点数组元素通过offset下标访问*/
  transmitFrame[4]=BYTE_H(Axis_LinkPoint_Position[offset]);
	transmitFrame[5]=BYTE_L(Axis_LinkPoint_Position[offset]);
	transmitFrame[6]=ComFrame_GetCheckSum(transmitFrame,6);
	transmitFrameLen=7;
	
	UartSendBuf(transmitFrame, transmitFrameLen);
}

/*
*********************************************************************************************************
*	函 数 名: ComFrame_Read_GetAxisHalfPosition
*	功能说明: 返回对应轴的半开位置电位器值，电位器值用2个字节表示一个16位数据，高字节在前低字节在后
*	形    参: 数据帧头 					
*	返 回 值: 无
*********************************************************************************************************
*/
void ComFrame_Read_GetAxisHalfPosition(uint8_t* frameHeader)
{
	uint8_t motorNum;
	
	if(frameHeader==NULL){return;}
	motorNum=(*(frameHeader+2))&0x03;
  if(motorNum>3){return;}
	
	transmitFrame[0]=0x68;
	transmitFrame[1]=(0x1A+motorNum);
	transmitFrame[2]=motorNum;
	transmitFrame[3]=0x02;//2字节数据域，高字节在前，低字节在后
	transmitFrame[4]=BYTE_H(Axis_Cbrt_Data[motorNum].Axis_Half_Position);
	transmitFrame[5]=BYTE_L(Axis_Cbrt_Data[motorNum].Axis_Half_Position);
	transmitFrame[6]=ComFrame_GetCheckSum(transmitFrame,6);
	transmitFrameLen=7;
	
	UartSendBuf(transmitFrame, transmitFrameLen);
}

/*
*********************************************************************************************************
*	函 数 名: ComFrame_Read_GetMotorPositionNow
*	功能说明: 返回电机当前位置，电位器值用2个字节表示一个16位数据，高字节在前低字节在后
*	形    参: 数据帧头  					
*	返 回 值: 无
*********************************************************************************************************
*/
void ComFrame_Read_GetMotorPositionNow(uint8_t* frameHeader)
{
	uint8_t motorNum;
	
	if(frameHeader==NULL){return;}
	motorNum=(*(frameHeader+2))&0x03;
  if(motorNum>3){return;}
	
	transmitFrame[0]=0x68;
	transmitFrame[1]=0x1D;
	transmitFrame[2]=motorNum;
	transmitFrame[3]=0x02;//2字节数据域，高字节在前，低字节在后
	transmitFrame[4]=BYTE_H(Seat_Position_Now[motorNum]);
	transmitFrame[5]=BYTE_L(Seat_Position_Now[motorNum]);
	transmitFrame[6]=ComFrame_GetCheckSum(transmitFrame,6);
	transmitFrameLen=7;
	
	UartSendBuf(transmitFrame, transmitFrameLen);
}

/*
*********************************************************************************************************
*	函 数 名: ComFrame_Read_GetMotorCurrentNow
*	功能说明: 返回电机当前电流ADC值，16位，高字节在前，低字节在后
*	形    参: 数据帧头  					
*	返 回 值: 无
*********************************************************************************************************
*/
void ComFrame_Read_GetMotorCurrentNow(uint8_t* frameHeader)
{
	uint8_t motorNum;
	
	if(frameHeader==NULL){return;}
	motorNum=(*(frameHeader+2))&0x03;
  if(motorNum>3){return;}
	
	transmitFrame[0]=0x68;
	transmitFrame[1]=0x1E;
	transmitFrame[2]=motorNum;
	transmitFrame[3]=0x02;//2字节数据域，高字节在前，低字节在后
	transmitFrame[4]=BYTE_H(Motor_Current_Now[motorNum]);
	transmitFrame[5]=BYTE_L(Motor_Current_Now[motorNum]);
	transmitFrame[6]=ComFrame_GetCheckSum(transmitFrame,6);
	transmitFrameLen=7;
	
	UartSendBuf(transmitFrame, transmitFrameLen);
}


/*
*********************************************************************************************************
*	函 数 名: ComFrame_Write_SetMotorSpeed
*	功能说明: 设置指定轴电机的设定速度值，Ram中数值立即修改生效，但不会写入flash，返回一个数据帧
*	形    参: 数据帧头 					
*	返 回 值: 无
*********************************************************************************************************
*/
void ComFrame_Write_SetMotorSpeed(uint8_t* frameHeader)
{
	uint8_t motorNum;
	uint8_t motorSpeed;
	
	motorNum=(*(frameHeader+2))&0x03;//&00000011
	if(motorNum>3){return;}
	motorSpeed=*(frameHeader+4);
	if(motorSpeed>100){motorSpeed=100;}
  if(motorSpeed<10){motorSpeed=10;}	 
	Axis_Cbrt_Data[motorNum].Axis_Motor_Speed=motorSpeed;
	
	transmitFrame[0]=0x68;
	transmitFrame[1]=0x20;
	transmitFrame[2]=motorNum;
	transmitFrame[3]=0x00;
	transmitFrame[4]=ComFrame_GetCheckSum(transmitFrame,4);
	transmitFrameLen=5;
	
	UartSendBuf(transmitFrame, transmitFrameLen);
}

/*
*********************************************************************************************************
*	函 数 名: ComFrame_Write_SetMotorReverseFlag
*	功能说明: 设置指定轴电机反向标记，Ram中数值立即修改生效，但不会写入flash，返回一个数据帧
*	形    参: 数据帧头  					
*	返 回 值: 无
*********************************************************************************************************
*/
void ComFrame_Write_SetMotorReverseFlag(uint8_t* frameHeader)
{
	uint8_t motorNum;
	int8_t  motorReverseFlag;
	
	motorNum=(*(frameHeader+2))&0x03;//&00000011
	if(motorNum>3){return;}
  //motorReverseFlag=*(frameHeader+4)?-1:1;//帧数据=1表示反向，反向在结构体中以-1指示
	motorReverseFlag=Axis_Cbrt_Data[motorNum].Axis_Motor_Reverse;
	motorReverseFlag=motorReverseFlag>0?-1:1;
	Axis_Cbrt_Data[motorNum].Axis_Motor_Reverse=motorReverseFlag;
	
	transmitFrame[0]=0x68;
	transmitFrame[1]=0x21;
	transmitFrame[2]=motorNum;
	transmitFrame[3]=0x00;
	transmitFrame[4]=ComFrame_GetCheckSum(transmitFrame,4);
	transmitFrameLen=5;
	
	UartSendBuf(transmitFrame, transmitFrameLen);
}

/*
*********************************************************************************************************
*	函 数 名: ComFrame_Write_SetAxisClosePosition
*	功能说明: 设置指定轴收起极限值，Ram中数值立即修改生效，但不会写入flash，返回一个数据帧
            设置本参数时，相关参数之间的大小关系由上位机确保，下位机只做简单容错处理
*	形    参: 数据帧头  					
*	返 回 值: 无
*********************************************************************************************************
*/
void ComFrame_Write_SetAxisClosePosition(uint8_t* frameHeader)
{
	uint8_t motorNum;
	uint16_t axisCloseposition;
	
	motorNum=(*(frameHeader+2))&0x03;//&00000011
	if(motorNum>3){return;}
	axisCloseposition=*(frameHeader+4);
	axisCloseposition<<=8;
	axisCloseposition+=*(frameHeader+5);
	
	if((axisCloseposition==0)||(axisCloseposition>4095)){return;}
   
	Axis_Cbrt_Data[motorNum].Axis_Close_Position=axisCloseposition;
	
	transmitFrame[0]=0x68;
	transmitFrame[1]=0x22;
	transmitFrame[2]=motorNum;
	transmitFrame[3]=0x00;
	transmitFrame[4]=ComFrame_GetCheckSum(transmitFrame,4);
	transmitFrameLen=5;
	
	UartSendBuf(transmitFrame, transmitFrameLen);
}

/*
*********************************************************************************************************
*	函 数 名: ComFrame_Write_SetAxisOpenPosition
*	功能说明: 设置指定轴打开极限值，Ram中数值立即修改生效，但不会写入flash，返回一个数据帧
            设置本参数时，相关参数之间的大小关系由上位机确保，下位机只做简单容错处理
*	形    参: 数据帧头  					
*	返 回 值: 无
*********************************************************************************************************
*/
void ComFrame_Write_SetAxisOpenPosition(uint8_t* frameHeader)
{
	uint8_t motorNum;
	uint16_t axisOpenposition;
	
	motorNum=(*(frameHeader+2))&0x03;//&00000011
	if(motorNum>3){return;}
	axisOpenposition=*(frameHeader+4);
	axisOpenposition<<=8;
	axisOpenposition+=*(frameHeader+5);
	
	if((axisOpenposition==0)||(axisOpenposition>4095)){return;}
   
	Axis_Cbrt_Data[motorNum].Axis_Open_Position=axisOpenposition;
	
	transmitFrame[0]=0x68;
	transmitFrame[1]=0x23;
	transmitFrame[2]=motorNum;
	transmitFrame[3]=0x00;
	transmitFrame[4]=ComFrame_GetCheckSum(transmitFrame,4);
	transmitFrameLen=5;
	
	UartSendBuf(transmitFrame, transmitFrameLen);
}

/*
*********************************************************************************************************
*	函 数 名: ComFrame_Write_SetPotenReverseFlag
*	功能说明: 设置指定轴电位器反向标记，Ram中数值立即修改生效，但不会写入flash，返回一个数据帧
*	形    参: 数据帧头  					
*	返 回 值: 无
*********************************************************************************************************
*/
void ComFrame_Write_SetPotenReverseFlag(uint8_t* frameHeader)
{
	uint8_t motorNum;
	int8_t  potenReverseFlag;
	
	motorNum=(*(frameHeader+2))&0x03;//&00000011
	if(motorNum>3){return;}
  //potenReverseFlag=*(frameHeader+4)?-1:1;//帧数据=1表示反向，反向在结构体中以-1指示	
	potenReverseFlag=Axis_Cbrt_Data[motorNum].Axis_Poten_Reverse;
	potenReverseFlag=potenReverseFlag>0?-1:1;
	Axis_Cbrt_Data[motorNum].Axis_Poten_Reverse=potenReverseFlag;
	
	transmitFrame[0]=0x68;
	transmitFrame[1]=0x24;
	transmitFrame[2]=motorNum;
	transmitFrame[3]=0x00;
	transmitFrame[4]=ComFrame_GetCheckSum(transmitFrame,4);
	transmitFrameLen=5;
	
	UartSendBuf(transmitFrame, transmitFrameLen);
}

/*
*********************************************************************************************************
*	函 数 名: ComFrame_Write_GetLinkPosition
*	功能说明: 写入5个联动点数据
*	形    参: 数据帧头 					
*	返 回 值: 无
*********************************************************************************************************
*/
void ComFrame_Write_SetLinkPosition(uint8_t* frameHeader)
{
	uint8_t offset;
	uint16_t linkposition;
	
	offset=*(frameHeader+1)-0x25;
	if(offset>=5){return;}
	linkposition=*(frameHeader+4);
	linkposition<<=8;
	linkposition+=*(frameHeader+5);
	Axis_LinkPoint_Position[offset]=linkposition;

  transmitFrame[0]=0x68;
	transmitFrame[1]=0x25;
	transmitFrame[2]=*(frameHeader+2)-0x0C;
	transmitFrame[3]=0x00;
	transmitFrame[4]=ComFrame_GetCheckSum(transmitFrame,4);
	transmitFrameLen=5;
	
	UartSendBuf(transmitFrame, transmitFrameLen);
}

/*
*********************************************************************************************************
*	函 数 名: ComFrame_Write_SetAxisHalfPosition
*	功能说明: 写入指定轴的半开位
*	形    参: 数据帧头 					
*	返 回 值: 无
*********************************************************************************************************
*/
void ComFrame_Write_SetAxisHalfPosition(uint8_t* frameHeader)
{
	uint8_t motorNum;
	uint16_t axisHalfPosition;
	
	motorNum=*(frameHeader+1)-0x2A;
	axisHalfPosition=*(frameHeader+4);
	axisHalfPosition<<=8;
	axisHalfPosition+=*(frameHeader+5);
	
	Axis_Cbrt_Data[motorNum].Axis_Half_Position=axisHalfPosition;
	
	transmitFrame[0]=0x68;
	transmitFrame[1]=*(frameHeader+1);
	transmitFrame[2]=motorNum;
	transmitFrame[3]=0;
	transmitFrame[4]=ComFrame_GetCheckSum(transmitFrame,4);
	transmitFrameLen=5;
	
	UartSendBuf(transmitFrame, transmitFrameLen);
}

/*
*********************************************************************************************************
*	函 数 名: ComFrame_Write_SaveCbrtDataToFlash
*	功能说明: 将写入数据保存到flash并重新加载一次,返回上位机写入成功与否
*	形    参: 无 					
*	返 回 值: 无
*********************************************************************************************************
*/
void ComFrame_Write_SaveCbrtDataToFlash(void)
{ 
	/*RAM中的新参数写入Flash*/
	Flash_WriteRecord(&SeatCbrtData,Axis_LinkPoint_Position,Axis_Cbrt_Data);
	CbrtDataOK=Flash_ReadRecord(&SeatCbrtData,Axis_LinkPoint_Position,Axis_Cbrt_Data);
	
	
	transmitFrame[0]=0x68;
	transmitFrame[1]=0x2D;
	transmitFrame[2]=0x00;
	transmitFrame[3]=0x01;
	transmitFrame[4]=CbrtDataOK;
	transmitFrame[5]=ComFrame_GetCheckSum(transmitFrame,5);
	transmitFrameLen=6;
	
	UartSendBuf(transmitFrame, transmitFrameLen);	
}

/*
*********************************************************************************************************
*	函 数 名: ComFrame_KeyAction
*	功能说明: 解析虚拟按键数据帧，主要是往按键消息邮箱里发送虚拟按键，命令码范围0x30-0x3C
*	形    参: 帧头指针 					
*	返 回 值: 无
*********************************************************************************************************
*/
void ComFrame_KeyCMDAction(uint8_t* frameHeader)
{
	uint8_t CmdCode;
	
	if(frameHeader==NULL){return;}
	
	CmdCode=*(frameHeader+1);
	
	switch(CmdCode)
	{
		/*背靠打开 --背靠打开按键按下*/
	  case 0x30:Key_PutIntoFIFO(1);
		          break;
	  /*背靠收起 --背靠收起按键按下*/
	  case 0x31:Key_PutIntoFIFO(4);
		          break;
		/*背靠停止 --背靠打开按键弹起*/
	  case 0x32:Key_PutIntoFIFO(2);
		          break;
		/*腿靠打开 --腿靠打开按键按下*/
	  case 0x33:Key_PutIntoFIFO(7);
		          break;
		/*腿靠收起 --腿靠收起按键按下*/
	  case 0x34:Key_PutIntoFIFO(10);
		          break;
		/*腿靠停止 --腿靠打开按钮弹起*/
	  case 0x35:Key_PutIntoFIFO(8);
		          break;
		/*脚踏打开 --脚踏打开按键按下*/
	  case 0x36:Key_PutIntoFIFO(13);
		          break;
		/*脚踏收起 --脚踏收起按键按下*/
	  case 0x37:Key_PutIntoFIFO(16);
		          break;
		/*脚踏停止 --脚踏打开按键弹起*/
	  case 0x38:Key_PutIntoFIFO(14);
		          break;
		/*一键全开 --一键全开按键按下*/
	  case 0x39:Key_PutIntoFIFO(19);
		          break;
		/*一键全收 --一键全收按键按下*/
	  case 0x3A:Key_PutIntoFIFO(25);
		          break;
		/*一键半躺 --一键半躺按键按下*/
	  case 0x3B:Key_PutIntoFIFO(22);
		          break;
		/*全开全收 --暂无对应键值*/
	  case 0x3C://此项功能暂时没有
		          break;
		default:  break;
	}	
}

/*
*********************************************************************************************************
*	函 数 名: ComFrame_ConnectCMDAction
*	功能说明: 处理连接，跳转，版本等命令码为0x00-0x0E的命令
*	形    参: 帧头指针 					
*	返 回 值: 无
*********************************************************************************************************
*/
void ComFrame_ConnectCMDAction(uint8_t* frameHeader)
{
	uint8_t CmdCode;
	
	if(frameHeader==NULL){return;}
	
	CmdCode=*(frameHeader+1);
	
	switch(CmdCode)
	{ /*发送确认连接帧*/
		case 0x03:ComFrame_Action_ConnectionConfirm();
		          break;
		/*复位MCU以进入BootLoader*/	
		case 0x04:ComFrame_Action_SystemReboot();
		          break;
		/*发送版本号数据帧*/		
		case 0x06:ComFrame_Action_ReportAppVersion();
			        break;
		/*进入调试模式并发送进入数据帧*/	
		case 0x07:ComFrame_Action_GotoTestMode();
			        break;
		/*退出调试模式并发送退出数据帧*/	
		case 0x08:ComFrame_Action_OutTestMode();
			        break;
		/*发送数据帧表明目前在APP中*/
		case 0x09:ComFrame_Action_AppConfirm();
		          break;
		
		default: break;
	 }	
}
/*
*********************************************************************************************************
*	函 数 名: ComFrame_ReadCMDAction
*	功能说明: 处理读取数据命令，命令码范围为0x10-0x1E
*	形    参: 帧头指针 					
*	返 回 值: 无
*********************************************************************************************************
*/
void ComFrame_ReadCMDAction(uint8_t* frameHeader)
{
	uint8_t CmdCode;
	
	if(frameHeader==NULL){return;}
	
	CmdCode=*(frameHeader+1);
	
	switch(CmdCode)
	{
		/*读取指定轴的电机设定速度*/
		case 0x10:ComFrame_Read_GetMotorSpeedofSet(frameHeader);
		          break;
		/*读取指定轴的电机安装反向标志*/
		case 0x11:ComFrame_Read_GetMotorReverseFlag(frameHeader);
		          break;
		/*读取指定轴的收起极限位置电位器值*/
		case 0x12:ComFrame_Read_GetAxisClosePosition(frameHeader);
		          break;
		/*读取指定轴的打开极限位置电位器值*/
		case 0x13:ComFrame_Read_GetAxisOpenPosition(frameHeader);
		          break;
		/*读取指定轴的电位器安装反向标志*/
		case 0x14:ComFrame_Read_GetPotenReverseFlag(frameHeader);
		          break;
		/*读取5个联动点位置电位器值*/
		case 0x15:

		          ComFrame_Read_GetLinkPosition(frameHeader);
  
			break;
		/*读取3个半开位置电位器值*/
		case 0x1A:
		          ComFrame_Read_GetAxisHalfPosition(frameHeader);
		          break;
		/*读取指定轴的当前位置电位器值*/
		case 0x1D:ComFrame_Read_GetMotorPositionNow(frameHeader);
		          break;
		/*读取指定轴的电机当前电流ADC值*/
    case 0x1E:ComFrame_Read_GetMotorCurrentNow(frameHeader);
		          break;

		default:  break;	
	}
}

/*
*********************************************************************************************************
*	函 数 名: ComFrame_WriteCMDAction
*	功能说明: 处理写入数据命令，命令码范围为0x20-0x2D
*	形    参: 帧头指针 					
*	返 回 值: 无
*********************************************************************************************************
*/
void ComFrame_WriteCMDAction(uint8_t* frameHeader)
{
	uint8_t CmdCode;
	
	if(frameHeader==NULL){return;}
	
	CmdCode=*(frameHeader+1);
	
	switch(CmdCode)
	{
		/*写入指定轴的电机运行速度*/
		case 0x20: ComFrame_Write_SetMotorSpeed(frameHeader);
		           break;
		/*写入指定轴的电机安装反向标记*/
		case 0x21: ComFrame_Write_SetMotorReverseFlag(frameHeader);
		           break;
		/*写入指定轴的收起极限位置电位器值*/
		case 0x22: ComFrame_Write_SetAxisClosePosition(frameHeader);
		           break;
		/*写入指定轴的打开极限位置电位器值*/
		case 0x23: ComFrame_Write_SetAxisOpenPosition(frameHeader);
		           break;
		/*写入指定轴的电位器安装反向标记*/
		case 0x24: ComFrame_Write_SetPotenReverseFlag(frameHeader);
		           break;
		/*写入5个联动点位置电位器值*/
		case 0x25: 
			         ComFrame_Write_SetLinkPosition(frameHeader);
		           break;
		/*写入3个半开位置电位器值*/
		case 0x2A: 
			         ComFrame_Write_SetAxisHalfPosition(frameHeader);
		           break;
		/*保存写入的数据到内部Flash命令*/
		case 0x2D: ComFrame_Write_SaveCbrtDataToFlash();
		           /*通知运行逻辑任务重新加载判断数据*/
		           os_evt_set(SCG_EVT_BIT_0, HandleTaskSeatControlLogic);
		           break;
	  default  : break;	
	}
}

/*
*********************************************************************************************************
*	函 数 名: UartDataProcess
*	功能说明: 处理串口数据，本函数被串口数据接收任务周期性调用
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void UartDataProcess(void)
{ 
	uint8_t comFrameIndex;
	uint8_t comFrameCMD;
	
  receiveFrameCounter=ComFrame_FrameCopyToBuf();
	
	if(receiveFrameCounter==0){return;}
	
	for(comFrameIndex=0;comFrameIndex<receiveFrameCounter;comFrameIndex++)
	{
		comFrameCMD=*((&receiveFrameBuf[comFrameIndex][0])+1);
		
		switch(comFrameCMD)
		{ /*处理连接等命令*/
			{
				case 0x00:case 0x01:case 0x02:case 0x03:case 0x07:case 0x08:ComFrame_ConnectCMDAction(&receiveFrameBuf[comFrameIndex][0]);
			}
				break;
			/*处理读取命令*/       
      {
				case 0x1D:case 0x11:case 0x14:ComFrame_ReadCMDAction(&receiveFrameBuf[comFrameIndex][0]);
			}			  
				break;
			/*处理写入命令*/
			case 0x20:ComFrame_WriteCMDAction(&receiveFrameBuf[comFrameIndex][0]);
			                   break;
			/*处理按键命令*/
			case 0x30:ComFrame_KeyCMDAction(&receiveFrameBuf[comFrameIndex][0]);
			                   break;
		}
	}
	/*所有接收到的合法数据帧均已处理完毕*/

	receiveFrameCounter=0;
}
