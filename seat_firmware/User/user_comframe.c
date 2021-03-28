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

uint8_t receiveFrameBuf[16][8];//���Դ洢16������֡������
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
#define SCG_EVT_BIT_0  (1 << 0) //֪ͨ�����߼�������¿�������
/*
*********************************************************************************************************
*	�� �� ��: ComFrame_GetCheckSum
*	����˵��: ����У���
*	��    ��: data    ��Ҫ����У��͵������׵�ַ
						datalen �ֽ���
*	�� �� ֵ: sum     У���
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
*	�� �� ��: ComFrame_FrameCheck
*	����˵��: У������֡�Ƿ�Ϸ�
*	��    ��: frameHeader �����׵�ַ���õ�ַ�ɴ���������  					
*	�� �� ֵ: 1 �Ϸ� 0 �Ƿ�
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
*	�� �� ��: ComFrame_FrameCopyToBuf
*	����˵��: �����ڽ��յ��������ں��еĺϷ�����֡ת�Ƶ�֡����ȴ�����
*	��    ��: ��  					
*	�� �� ֵ: �Ϸ�����֡����
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
		//�ǺϷ�����֡
		if(frameOK)
		{ 
			frameLength=*(g_RxBuf+byteIndex+3)+5;
			memcpy(&receiveFrameBuf[frameIndex][0],(g_RxBuf+byteIndex),frameLength);
			byteIndex+=frameLength;
			frameIndex++;	
			
			//if(frameIndex>15){frameIndex=0;}//ѭ������
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
*	�� �� ��: ComFrame_Action_ConnectionConfirm
*	����˵��: ͨ���ظ�����֡��ʹ��λ��ȷ�����ӵ���λ��
*	��    ��: ��  					
*	�� �� ֵ: ��
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
*	�� �� ��: ComFrame_Action_SystemReboot
*	����˵��: ʹ��NVICָ��������Ƭ�����Իص�BootLoader����
*	��    ��: ��  					
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void ComFrame_Action_SystemReboot(void)
{
	  __set_FAULTMASK(1);//�ر����ж�,��ֹ�������̱�����жϴ��
    NVIC_SystemReset();//����Ƭ������
}

/*
*********************************************************************************************************
*	�� �� ��: ComFrame_Action_AppConfirm
*	����˵��: ͨ���ظ�����֡��ʹ��λ��ȷ����λ��������Ӧ�ó���APP��ģʽ
*	��    ��: ��  					
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void ComFrame_Action_AppConfirm(void)
{
	transmitFrame[0]=0x68;
	transmitFrame[1]=0x09;
	transmitFrame[2]=0x00;
	transmitFrame[3]=0x01;//�����򳤶�1
	transmitFrame[4]=0x01;//0x01��ʾ��λ����appģʽ
	transmitFrame[5]=ComFrame_GetCheckSum(transmitFrame,5);
	transmitFrameLen=6;
	
	UartSendBuf(transmitFrame, transmitFrameLen);
}

/*
*********************************************************************************************************
*	�� �� ��: ComFrame_Action_GotoTestMode
*	����˵��: �������ģʽ�����ñ�־λ��ʹ�ð����߼����뵽δ�궨״̬���ظ���λ��
*	��    ��: ��  					
*	�� �� ֵ: ��
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
*	�� �� ��: ComFrame_Action_OutTestMode
*	����˵��: �˳�����ģʽ�����ñ�־λ���ظ���λ��
*	��    ��: ��  					
*	�� �� ֵ: ��
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
*	�� �� ��: ComFrame_Action_ReportAppVersion
*	����˵��: ����1�ֽ�app�汾�ţ�����10��ʾV1.0 54��ʾV5.4
*	��    ��: ��  					
*	�� �� ֵ: ��
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
*	�� �� ��: ComFrame_Read_GetMotorSpeedofSet
*	����˵��: ���ض�Ӧ������趨�ٶ�ֵ
*	��    ��: ����֡ͷ					
*	�� �� ֵ: ��
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
	transmitFrame[2]=motorNum;//000000xx xx�ǵ�����룬������ֵ�պ���motorNumһ��
	transmitFrame[3]=0x01;
	transmitFrame[4]=Axis_Cbrt_Data[motorNum].Axis_Motor_Speed;
	transmitFrame[5]=ComFrame_GetCheckSum(transmitFrame,5);
	transmitFrameLen=6;
	
	UartSendBuf(transmitFrame, transmitFrameLen);
}

/*
*********************************************************************************************************
*	�� �� ��: ComFrame_Read_GetMotorReverseFlag
*	����˵��: ���ض�Ӧ����ķ��ӱ�־����������������Ϊ1,��û�з��ӷ���0
*	��    ��: ����֡ͷ 					
*	�� �� ֵ: ��
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
*	�� �� ��: ComFrame_Read_GetAxisClosePosition
*	����˵��: ���ض�Ӧ�������λ�õ�λ��ֵ����λ��ֵ��2���ֽڱ�ʾһ��16λ���ݣ����ֽ���ǰ���ֽ��ں�
*	��    ��: ����֡ͷ 					
*	�� �� ֵ: ��
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
	transmitFrame[3]=0x02;//2�ֽ������򣬸��ֽ���ǰ�����ֽ��ں�
	transmitFrame[4]=BYTE_H(Axis_Cbrt_Data[motorNum].Axis_Close_Position);
	transmitFrame[5]=BYTE_L(Axis_Cbrt_Data[motorNum].Axis_Close_Position);
	transmitFrame[6]=ComFrame_GetCheckSum(transmitFrame,6);
	transmitFrameLen=7;
	
	UartSendBuf(transmitFrame, transmitFrameLen);
}
/*
*********************************************************************************************************
*	�� �� ��: ComFrame_Read_GetAxisOpenPosition
*	����˵��: ���ض�Ӧ��Ĵ�λ�õ�λ��ֵ����λ��ֵ��2���ֽڱ�ʾһ��16λ���ݣ����ֽ���ǰ���ֽ��ں�
*	��    ��: ����֡ͷ					
*	�� �� ֵ: ��
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
	transmitFrame[3]=0x02;//2�ֽ������򣬸��ֽ���ǰ�����ֽ��ں�
	transmitFrame[4]=BYTE_H(Axis_Cbrt_Data[motorNum].Axis_Open_Position);
	transmitFrame[5]=BYTE_L(Axis_Cbrt_Data[motorNum].Axis_Open_Position);
	transmitFrame[6]=ComFrame_GetCheckSum(transmitFrame,6);
	transmitFrameLen=7;
	
	UartSendBuf(transmitFrame, transmitFrameLen);
}

/*
*********************************************************************************************************
*	�� �� ��: ComFrame_Read_GetPotenReverseFlag
*	����˵��: ���ض�Ӧ���λ���ķ��ӱ�־����������������Ϊ1,��û�з��ӷ���0
*	��    ��: ����֡ͷ					
*	�� �� ֵ: ��
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
*	�� �� ��: ComFrame_Read_GetLinkPosition
*	����˵��: ���ر�����ʼ������ĵ�λ��ֵ
*	��    ��: ����֡ͷ					
*	�� �� ֵ: ��
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
	
	/*5������������Ԫ��ͨ��offset�±����*/
  transmitFrame[4]=BYTE_H(Axis_LinkPoint_Position[offset]);
	transmitFrame[5]=BYTE_L(Axis_LinkPoint_Position[offset]);
	transmitFrame[6]=ComFrame_GetCheckSum(transmitFrame,6);
	transmitFrameLen=7;
	
	UartSendBuf(transmitFrame, transmitFrameLen);
}

/*
*********************************************************************************************************
*	�� �� ��: ComFrame_Read_GetAxisHalfPosition
*	����˵��: ���ض�Ӧ��İ뿪λ�õ�λ��ֵ����λ��ֵ��2���ֽڱ�ʾһ��16λ���ݣ����ֽ���ǰ���ֽ��ں�
*	��    ��: ����֡ͷ 					
*	�� �� ֵ: ��
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
	transmitFrame[3]=0x02;//2�ֽ������򣬸��ֽ���ǰ�����ֽ��ں�
	transmitFrame[4]=BYTE_H(Axis_Cbrt_Data[motorNum].Axis_Half_Position);
	transmitFrame[5]=BYTE_L(Axis_Cbrt_Data[motorNum].Axis_Half_Position);
	transmitFrame[6]=ComFrame_GetCheckSum(transmitFrame,6);
	transmitFrameLen=7;
	
	UartSendBuf(transmitFrame, transmitFrameLen);
}

/*
*********************************************************************************************************
*	�� �� ��: ComFrame_Read_GetMotorPositionNow
*	����˵��: ���ص����ǰλ�ã���λ��ֵ��2���ֽڱ�ʾһ��16λ���ݣ����ֽ���ǰ���ֽ��ں�
*	��    ��: ����֡ͷ  					
*	�� �� ֵ: ��
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
	transmitFrame[3]=0x02;//2�ֽ������򣬸��ֽ���ǰ�����ֽ��ں�
	transmitFrame[4]=BYTE_H(Seat_Position_Now[motorNum]);
	transmitFrame[5]=BYTE_L(Seat_Position_Now[motorNum]);
	transmitFrame[6]=ComFrame_GetCheckSum(transmitFrame,6);
	transmitFrameLen=7;
	
	UartSendBuf(transmitFrame, transmitFrameLen);
}

/*
*********************************************************************************************************
*	�� �� ��: ComFrame_Read_GetMotorCurrentNow
*	����˵��: ���ص����ǰ����ADCֵ��16λ�����ֽ���ǰ�����ֽ��ں�
*	��    ��: ����֡ͷ  					
*	�� �� ֵ: ��
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
	transmitFrame[3]=0x02;//2�ֽ������򣬸��ֽ���ǰ�����ֽ��ں�
	transmitFrame[4]=BYTE_H(Motor_Current_Now[motorNum]);
	transmitFrame[5]=BYTE_L(Motor_Current_Now[motorNum]);
	transmitFrame[6]=ComFrame_GetCheckSum(transmitFrame,6);
	transmitFrameLen=7;
	
	UartSendBuf(transmitFrame, transmitFrameLen);
}


/*
*********************************************************************************************************
*	�� �� ��: ComFrame_Write_SetMotorSpeed
*	����˵��: ����ָ���������趨�ٶ�ֵ��Ram����ֵ�����޸���Ч��������д��flash������һ������֡
*	��    ��: ����֡ͷ 					
*	�� �� ֵ: ��
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
*	�� �� ��: ComFrame_Write_SetMotorReverseFlag
*	����˵��: ����ָ�����������ǣ�Ram����ֵ�����޸���Ч��������д��flash������һ������֡
*	��    ��: ����֡ͷ  					
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void ComFrame_Write_SetMotorReverseFlag(uint8_t* frameHeader)
{
	uint8_t motorNum;
	int8_t  motorReverseFlag;
	
	motorNum=(*(frameHeader+2))&0x03;//&00000011
	if(motorNum>3){return;}
  //motorReverseFlag=*(frameHeader+4)?-1:1;//֡����=1��ʾ���򣬷����ڽṹ������-1ָʾ
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
*	�� �� ��: ComFrame_Write_SetAxisClosePosition
*	����˵��: ����ָ����������ֵ��Ram����ֵ�����޸���Ч��������д��flash������һ������֡
            ���ñ�����ʱ����ز���֮��Ĵ�С��ϵ����λ��ȷ������λ��ֻ�����ݴ���
*	��    ��: ����֡ͷ  					
*	�� �� ֵ: ��
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
*	�� �� ��: ComFrame_Write_SetAxisOpenPosition
*	����˵��: ����ָ����򿪼���ֵ��Ram����ֵ�����޸���Ч��������д��flash������һ������֡
            ���ñ�����ʱ����ز���֮��Ĵ�С��ϵ����λ��ȷ������λ��ֻ�����ݴ���
*	��    ��: ����֡ͷ  					
*	�� �� ֵ: ��
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
*	�� �� ��: ComFrame_Write_SetPotenReverseFlag
*	����˵��: ����ָ�����λ�������ǣ�Ram����ֵ�����޸���Ч��������д��flash������һ������֡
*	��    ��: ����֡ͷ  					
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void ComFrame_Write_SetPotenReverseFlag(uint8_t* frameHeader)
{
	uint8_t motorNum;
	int8_t  potenReverseFlag;
	
	motorNum=(*(frameHeader+2))&0x03;//&00000011
	if(motorNum>3){return;}
  //potenReverseFlag=*(frameHeader+4)?-1:1;//֡����=1��ʾ���򣬷����ڽṹ������-1ָʾ	
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
*	�� �� ��: ComFrame_Write_GetLinkPosition
*	����˵��: д��5������������
*	��    ��: ����֡ͷ 					
*	�� �� ֵ: ��
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
*	�� �� ��: ComFrame_Write_SetAxisHalfPosition
*	����˵��: д��ָ����İ뿪λ
*	��    ��: ����֡ͷ 					
*	�� �� ֵ: ��
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
*	�� �� ��: ComFrame_Write_SaveCbrtDataToFlash
*	����˵��: ��д�����ݱ��浽flash�����¼���һ��,������λ��д��ɹ����
*	��    ��: �� 					
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void ComFrame_Write_SaveCbrtDataToFlash(void)
{ 
	/*RAM�е��²���д��Flash*/
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
*	�� �� ��: ComFrame_KeyAction
*	����˵��: �������ⰴ������֡����Ҫ����������Ϣ�����﷢�����ⰴ���������뷶Χ0x30-0x3C
*	��    ��: ֡ͷָ�� 					
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void ComFrame_KeyCMDAction(uint8_t* frameHeader)
{
	uint8_t CmdCode;
	
	if(frameHeader==NULL){return;}
	
	CmdCode=*(frameHeader+1);
	
	switch(CmdCode)
	{
		/*������ --�����򿪰�������*/
	  case 0x30:Key_PutIntoFIFO(1);
		          break;
	  /*�������� --�������𰴼�����*/
	  case 0x31:Key_PutIntoFIFO(4);
		          break;
		/*����ֹͣ --�����򿪰�������*/
	  case 0x32:Key_PutIntoFIFO(2);
		          break;
		/*�ȿ��� --�ȿ��򿪰�������*/
	  case 0x33:Key_PutIntoFIFO(7);
		          break;
		/*�ȿ����� --�ȿ����𰴼�����*/
	  case 0x34:Key_PutIntoFIFO(10);
		          break;
		/*�ȿ�ֹͣ --�ȿ��򿪰�ť����*/
	  case 0x35:Key_PutIntoFIFO(8);
		          break;
		/*��̤�� --��̤�򿪰�������*/
	  case 0x36:Key_PutIntoFIFO(13);
		          break;
		/*��̤���� --��̤���𰴼�����*/
	  case 0x37:Key_PutIntoFIFO(16);
		          break;
		/*��ֹ̤ͣ --��̤�򿪰�������*/
	  case 0x38:Key_PutIntoFIFO(14);
		          break;
		/*һ��ȫ�� --һ��ȫ����������*/
	  case 0x39:Key_PutIntoFIFO(19);
		          break;
		/*һ��ȫ�� --һ��ȫ�հ�������*/
	  case 0x3A:Key_PutIntoFIFO(25);
		          break;
		/*һ������ --һ�����ɰ�������*/
	  case 0x3B:Key_PutIntoFIFO(22);
		          break;
		/*ȫ��ȫ�� --���޶�Ӧ��ֵ*/
	  case 0x3C://�������ʱû��
		          break;
		default:  break;
	}	
}

/*
*********************************************************************************************************
*	�� �� ��: ComFrame_ConnectCMDAction
*	����˵��: �������ӣ���ת���汾��������Ϊ0x00-0x0E������
*	��    ��: ֡ͷָ�� 					
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void ComFrame_ConnectCMDAction(uint8_t* frameHeader)
{
	uint8_t CmdCode;
	
	if(frameHeader==NULL){return;}
	
	CmdCode=*(frameHeader+1);
	
	switch(CmdCode)
	{ /*����ȷ������֡*/
		case 0x03:ComFrame_Action_ConnectionConfirm();
		          break;
		/*��λMCU�Խ���BootLoader*/	
		case 0x04:ComFrame_Action_SystemReboot();
		          break;
		/*���Ͱ汾������֡*/		
		case 0x06:ComFrame_Action_ReportAppVersion();
			        break;
		/*�������ģʽ�����ͽ�������֡*/	
		case 0x07:ComFrame_Action_GotoTestMode();
			        break;
		/*�˳�����ģʽ�������˳�����֡*/	
		case 0x08:ComFrame_Action_OutTestMode();
			        break;
		/*��������֡����Ŀǰ��APP��*/
		case 0x09:ComFrame_Action_AppConfirm();
		          break;
		
		default: break;
	 }	
}
/*
*********************************************************************************************************
*	�� �� ��: ComFrame_ReadCMDAction
*	����˵��: �����ȡ������������뷶ΧΪ0x10-0x1E
*	��    ��: ֡ͷָ�� 					
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void ComFrame_ReadCMDAction(uint8_t* frameHeader)
{
	uint8_t CmdCode;
	
	if(frameHeader==NULL){return;}
	
	CmdCode=*(frameHeader+1);
	
	switch(CmdCode)
	{
		/*��ȡָ����ĵ���趨�ٶ�*/
		case 0x10:ComFrame_Read_GetMotorSpeedofSet(frameHeader);
		          break;
		/*��ȡָ����ĵ����װ�����־*/
		case 0x11:ComFrame_Read_GetMotorReverseFlag(frameHeader);
		          break;
		/*��ȡָ�����������λ�õ�λ��ֵ*/
		case 0x12:ComFrame_Read_GetAxisClosePosition(frameHeader);
		          break;
		/*��ȡָ����Ĵ򿪼���λ�õ�λ��ֵ*/
		case 0x13:ComFrame_Read_GetAxisOpenPosition(frameHeader);
		          break;
		/*��ȡָ����ĵ�λ����װ�����־*/
		case 0x14:ComFrame_Read_GetPotenReverseFlag(frameHeader);
		          break;
		/*��ȡ5��������λ�õ�λ��ֵ*/
		case 0x15:

		          ComFrame_Read_GetLinkPosition(frameHeader);
  
			break;
		/*��ȡ3���뿪λ�õ�λ��ֵ*/
		case 0x1A:
		          ComFrame_Read_GetAxisHalfPosition(frameHeader);
		          break;
		/*��ȡָ����ĵ�ǰλ�õ�λ��ֵ*/
		case 0x1D:ComFrame_Read_GetMotorPositionNow(frameHeader);
		          break;
		/*��ȡָ����ĵ����ǰ����ADCֵ*/
    case 0x1E:ComFrame_Read_GetMotorCurrentNow(frameHeader);
		          break;

		default:  break;	
	}
}

/*
*********************************************************************************************************
*	�� �� ��: ComFrame_WriteCMDAction
*	����˵��: ����д��������������뷶ΧΪ0x20-0x2D
*	��    ��: ֡ͷָ�� 					
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void ComFrame_WriteCMDAction(uint8_t* frameHeader)
{
	uint8_t CmdCode;
	
	if(frameHeader==NULL){return;}
	
	CmdCode=*(frameHeader+1);
	
	switch(CmdCode)
	{
		/*д��ָ����ĵ�������ٶ�*/
		case 0x20: ComFrame_Write_SetMotorSpeed(frameHeader);
		           break;
		/*д��ָ����ĵ����װ������*/
		case 0x21: ComFrame_Write_SetMotorReverseFlag(frameHeader);
		           break;
		/*д��ָ�����������λ�õ�λ��ֵ*/
		case 0x22: ComFrame_Write_SetAxisClosePosition(frameHeader);
		           break;
		/*д��ָ����Ĵ򿪼���λ�õ�λ��ֵ*/
		case 0x23: ComFrame_Write_SetAxisOpenPosition(frameHeader);
		           break;
		/*д��ָ����ĵ�λ����װ������*/
		case 0x24: ComFrame_Write_SetPotenReverseFlag(frameHeader);
		           break;
		/*д��5��������λ�õ�λ��ֵ*/
		case 0x25: 
			         ComFrame_Write_SetLinkPosition(frameHeader);
		           break;
		/*д��3���뿪λ�õ�λ��ֵ*/
		case 0x2A: 
			         ComFrame_Write_SetAxisHalfPosition(frameHeader);
		           break;
		/*����д������ݵ��ڲ�Flash����*/
		case 0x2D: ComFrame_Write_SaveCbrtDataToFlash();
		           /*֪ͨ�����߼��������¼����ж�����*/
		           os_evt_set(SCG_EVT_BIT_0, HandleTaskSeatControlLogic);
		           break;
	  default  : break;	
	}
}

/*
*********************************************************************************************************
*	�� �� ��: UartDataProcess
*	����˵��: ���������ݣ����������������ݽ������������Ե���
*	��    ��: ��
*	�� �� ֵ: ��
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
		{ /*�������ӵ�����*/
			{
				case 0x00:case 0x01:case 0x02:case 0x03:case 0x07:case 0x08:ComFrame_ConnectCMDAction(&receiveFrameBuf[comFrameIndex][0]);
			}
				break;
			/*�����ȡ����*/       
      {
				case 0x1D:case 0x11:case 0x14:ComFrame_ReadCMDAction(&receiveFrameBuf[comFrameIndex][0]);
			}			  
				break;
			/*����д������*/
			case 0x20:ComFrame_WriteCMDAction(&receiveFrameBuf[comFrameIndex][0]);
			                   break;
			/*����������*/
			case 0x30:ComFrame_KeyCMDAction(&receiveFrameBuf[comFrameIndex][0]);
			                   break;
		}
	}
	/*���н��յ��ĺϷ�����֡���Ѵ������*/

	receiveFrameCounter=0;
}
