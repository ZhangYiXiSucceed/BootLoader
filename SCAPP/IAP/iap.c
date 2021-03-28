#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "stmflash.h"
#include "iap.h" 
#include "stdio.h"
#include "string.h"

iapfun jump2app; 
//��ת��Ӧ�ó����
//appxaddr:�û�������ʼ��ַ.
void iap_load_app(u32 appxaddr)
{
	if(((*(vu32*)appxaddr)&0x2FFE0000)==0x20000000)	//���ջ����ַ�Ƿ�Ϸ�.
	{ 
		jump2app=(iapfun)*(vu32*)(appxaddr+4);		//�û��������ڶ�����Ϊ����ʼ��ַ(��λ��ַ)		
		MSR_MSP(*(vu32*)appxaddr);					//��ʼ��APP��ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ)
		jump2app();									        //��ת��APP.
	}
	else
	{
		printf("error!");
	}
}		 

uint32_t Addr_now = APP_ADDR;	        //д�뵽FLASH�ĵ�ַ
uint8_t JumpAddr=0;

u16 MotorBackAdValue=0;         //�������ݣ��������ADCֵ
u16 MotorLegAdValue=0;          //�������ݣ��ȿ����ADCֵ
u16 MotorFootAdValue=0;         //�������ݣ���̤���ADCֵ

u8 MotorSpeed[3]={50,70,90};    //�������ݣ�����������ٶ�ֵ��ռ�ձ�%��

u16 MotorClosePosition[3]={123,456,789};           //�������ݣ����������������λ�ã�ռ�ձ�%��
u16 MotorOpenPosition[3]={234,345,456};            //�������ݣ���������Ĵ򿪼���λ�ã�ռ�ձ�%��
u16 MotorLinkPosition[8]={11,22,33,44,55,66,77,88};//�������ݣ����������������ֵ��ռ�ձ�%��

u16 MotorCurrent[3]={1520,3524,4130};   //�������ݣ�������������е���ֵ��ռ�ձ�%��

/***********************************************************
*�������� CalFrameCheckCode
*���ܣ�   ���㵱ǰָ֡��У���
*������   u8 *TempBuf,u8 Length��֡�����֡����
*����ֵ�� У���
***********************************************************/
u8 CalFrameCheckCode(u8 *TempBuf,u8 Length)
{
	u8 i=0,CheckSum=0;
	for(i=0;i<Length;i++)
	   CheckSum+=TempBuf[i];
	return CheckSum;
}
/***********************************************************
*�������� IsConnnect
*���ܣ�   ������λ��������Ӧָ��
*������   ��
*����ֵ�� ��
***********************************************************/
void IsConnnect()
{
	u8 Command[6];
	
	Command[0]=0x68;
	Command[1]=0x03;
	Command[2]=0x04;
	Command[3]=0x01;
	Command[4]=0x00;
	Command[5]=0x70;
	
  Usart_SendData(Command,6);
}
/***********************************************************
*�������� ReturnBootLoader()
*���ܣ�   ������λ������BootLoaderָ��
*������   ��
*����ֵ�� ��
***********************************************************/
void ReturnBootLoader()
{
	u8 Command[6];
	
	Command[0]=0x68;
	
	Command[1]=0x05;
	Command[2]=0x14;
	Command[3]=0x01;
	Command[4]=0x00;
	
	Command[5]=0x82;
	
	Usart_SendData(Command,6);
	
  __disable_irq();
	iap_load_app(BootLoaderADDR);
}
/***********************************************************
*�������� ReadProgramerState
*���ܣ�   ��ȡ��������״̬
*������   ��
*����ֵ�� ��
***********************************************************/
void ReadProgramerState()
{
	u8 Command[6];
	
	Command[0]=0x68;
	
	Command[1]=0x09;
	Command[2]=0x10;
	Command[3]=0x01;
	Command[4]=0x00;
	
	Command[5]=0x82;
	
	Usart_SendData(Command,6);
}
/***********************************************************
*�������� CalibrateMode(uint8_t CommandCode)
*���ܣ�   ������λ�����ر궨ģʽָ�
*������   CommandCode  Ϊ�򿪻��߹ر�ָ��
*����ֵ�� ��
***********************************************************/
void CalibrateMode(uint8_t CommandCode)
{
	u8 Command[6];
	
	if(CommandCode==0x07)
	{
			Command[0]=0x68;
			Command[1]=0x07;
			Command[2]=0x04;
			Command[3]=0x01;
			Command[4]=0x00;
			Command[5]=0x74;
	}
	else
	{
			Command[0]=0x68;
			Command[1]=0x08;
			Command[2]=0x04;
			Command[3]=0x01;
			Command[4]=0x00;
			Command[5]=0x75;
	}
	Usart_SendData(Command,6);
}
/***********************************************************
*�������� ReadProgramerState
*���ܣ�   ��ձ궨����
*������   ��
*����ֵ�� ��
***********************************************************/
void ClearCalibrationData()
{
	u8 Command[6];
	Command[0]=0x68;
	Command[1]=0x0A;
	Command[2]=0x04;
	Command[3]=0x01;
	Command[4]=0x00;
	Command[5]=0x77;
	Usart_SendData(Command,6);
	
	MotorBackAdValue=0;
	MotorLegAdValue=0;
	MotorFootAdValue=0;
    
	MotorCurrent[0]=0;
	MotorCurrent[1]=0;
	MotorCurrent[2]=0;
	
}
/***********************************************************
*�������� SendMotorAdValue(uint8_t CommandCode)
*���ܣ�   ������λ��ADCֵ
*������   CommandCode�����ָ����
*����ֵ�� ��
***********************************************************/
void SendMotorAdValue(uint8_t CommandCode)
{
	u8 Command[7],CheckCode=0;

	Command[0]=0x68;
	Command[1]=0x1D;
	Command[3]=0x02;
	
	if(CommandCode==0x08)
 {
	Command[2]=0x00; 
	Command[4]=MotorBackAdValue/256;
	Command[5]=MotorBackAdValue%256;
 }
 else if(CommandCode==0x09)
 {
	Command[2]=0x01; 
	Command[4]=MotorLegAdValue/256;
	Command[5]=MotorLegAdValue%256;
 }
 else if(CommandCode==0x0A)
 {
	Command[2]=0x02; 
	Command[4]=MotorFootAdValue/256;
	Command[5]=MotorFootAdValue%256;
 }
	for(u8 i=0;i<6;i++)
	   CheckCode+=Command[i];
	Command[6]=CheckCode;
 Usart_SendData(Command,7);
 
}
/***********************************************************
*�������� SendMotorAdValue(uint8_t CommandCode)
*���ܣ�   ������λ������ٶ�ֵ
*������   CommandCode�����ָ����
*����ֵ�� ��
***********************************************************/
void SendMotorSpeed(uint8_t CommandCode)
{
	
	u8 Command[6],CheckCode=0;
	
	Command[0]=0x68;
	Command[1]=0x10;
	Command[3]=0x01;
	
	if(CommandCode==0x08)
 {
	Command[2]=0x00; 
	Command[4]=MotorSpeed[0];
 }
 else if(CommandCode==0x09)
 {
	Command[2]=0x01; 
	Command[4]=MotorSpeed[1];
 }
 else if(CommandCode==0x0A)
 {
	Command[2]=0x02; 
	Command[4]=MotorSpeed[2];
 }
	for(u8 i=0;i<5;i++)
	   CheckCode+=Command[i];
	Command[5]=CheckCode;
 
  Usart_SendData(Command,6);
}
/***********************************************************
*�������� SendMotorAdValue(uint8_t CommandCode)
*���ܣ�   ������λ��������λ��ֵ
*������   CommandCode�����ָ����
*����ֵ�� ��
***********************************************************/
void SendMotorClosePosition(uint8_t CommandCode)
{
	
	u8 Command[7],CheckCode=0;
	Command[0]=0x68;
	Command[1]=0x12;
	Command[3]=0x02;
	
	if(CommandCode==0x08)
 {
	Command[2]=0x00; 
	Command[4]=MotorClosePosition[0]/256;
	Command[5]=MotorClosePosition[0]%256;
 }
 else if(CommandCode==0x09)
 {
	Command[2]=0x01; 
	Command[4]=MotorClosePosition[1]/256;
	Command[5]=MotorClosePosition[1]%256;
 }
 else if(CommandCode==0x0A)
 {
	Command[2]=0x02; 
	Command[4]=MotorClosePosition[2]/256;
	Command[5]=MotorClosePosition[2]%256;
 }
	for(u8 i=0;i<6;i++)
	   CheckCode+=Command[i];
	Command[6]=CheckCode;
 
  Usart_SendData(Command,7);
}
/***********************************************************
*�������� SendMotorAdValue(uint8_t CommandCode)
*���ܣ�   ������λ��������е���ֵ
*������   CommandCode�����ָ����
*����ֵ�� ��
***********************************************************/
void SendMotorCurrent(uint8_t CommandCode)
{
	
	u8 Command[7],CheckCode=0;
	Command[0]=0x68;
	Command[1]=0x1E;
	Command[3]=0x02;
	
	if(CommandCode==0x08)
 {
	Command[2]=0x00; 
	Command[4]=MotorCurrent[0]/256;
	Command[5]=MotorCurrent[0]%256;
 }
 else if(CommandCode==0x09)
 {
	Command[2]=0x01; 
	Command[4]=MotorCurrent[1]/256;
	Command[5]=MotorCurrent[1]%256;
 }
 else if(CommandCode==0x0A)
 {
	Command[2]=0x02; 
	Command[4]=MotorCurrent[2]/256;
	Command[5]=MotorCurrent[2]%256;
 }
	for(u8 i=0;i<6;i++)
	   CheckCode+=Command[i];
	Command[6]=CheckCode;
 
  Usart_SendData(Command,7);
}
/***********************************************************
*�������� SendMotorAdValue(uint8_t CommandCode)
*���ܣ�   ������λ������򿪼���λ��ֵ
*������   CommandCode�����ָ����
*����ֵ�� ��
***********************************************************/
void SendMotorOpenPosition(uint8_t CommandCode)
{
	
	u8 Command[7],CheckCode=0;
	Command[0]=0x68;
	Command[1]=0x13;
	Command[3]=0x02;
	
	if(CommandCode==0x08)
 {
	Command[2]=0x00; 
	Command[4]=MotorOpenPosition[0]/256;
	Command[5]=MotorOpenPosition[0]%256;
 }
 else if(CommandCode==0x09)
 {
	Command[2]=0x01; 
	Command[4]=MotorOpenPosition[1]/256;
	Command[5]=MotorOpenPosition[1]%256;
 }
 else if(CommandCode==0x0A)
 {
	Command[2]=0x02; 
	Command[4]=MotorOpenPosition[2]/256;
	Command[5]=MotorOpenPosition[2]%256;
 }
	for(u8 i=0;i<6;i++)
	   CheckCode+=Command[i];
	Command[6]=CheckCode;
 
  Usart_SendData(Command,7);
}
/***********************************************************
*�������� SendMotorAdValue(uint8_t CommandCode)
*���ܣ�   ������λ������Ƿ���ָ��
*������   CommandCode�����ָ����
*����ֵ�� ��
***********************************************************/
void IsMotorReverse(uint8_t CommandCode)
{
	u8 Command[6],CheckCode=0;
	
	Command[0]=0x68;
	
	Command[1]=0x11;
	Command[3]=0x01;
	
	if(CommandCode==0x08)
 {
	Command[2]=0x00; 
	Command[4]=0x01;
 }
 else if(CommandCode==0x09)
 {
	Command[2]=0x01; 
	Command[4]=0x01;
 }
 else if(CommandCode==0x0A)
 {
	Command[2]=0x02; 
	Command[4]=0x01;
 }
	for(u8 i=0;i<5;i++)
	   CheckCode+=Command[i];
	Command[5]=CheckCode;
  Usart_SendData(Command,6);
}
/***********************************************************
*�������� IsMotorPositionReverse(uint8_t CommandCode)
*���ܣ�   ������λ����λ���Ƿ���
*������   CommandCode�����ָ����
*����ֵ�� ��
***********************************************************/
void IsMotorPositionReverse(uint8_t CommandCode)
{
	u8 Command[6],CheckCode=0;
	
	Command[0]=0x68;
	
	Command[1]=0x14;
	Command[3]=0x01;
	
	if(CommandCode==0x08)
 {
	Command[2]=0x00; 
	Command[4]=0x01;
 }
 else if(CommandCode==0x09)
 {
	Command[2]=0x01; 
	Command[4]=0x01;
 }
 else if(CommandCode==0x0A)
 {
	Command[2]=0x02; 
	Command[4]=0x01;
 }
	for(u8 i=0;i<5;i++)
	   CheckCode+=Command[i];
	Command[5]=CheckCode;
  Usart_SendData(Command,6);
}
/***********************************************************
*�������� SendWriteParaCommand(uint8_t CommandCode)
*���ܣ�   ������λ��д������ɹ�ָ��
*������   u8 CommandCode,������  u8 ControlCode��������
*����ֵ�� ��
***********************************************************/
void SendWriteParaCommand(u8 CommandCode,u8 ControlCode)
{
	u8 Command[7],CheckCode=0;;
	
	Command[0]=0x68;
  if(CommandCode==0x20)
	{
		Command[1]=0x20;
		if(ControlCode==0x0C)
		{
			Command[2]=0x04;
		}
		if(ControlCode==0x0D)
		{
			Command[2]=0x05;
		}
		if(ControlCode==0x0E)
		{
			Command[2]=0x06;
		}
		Command[3]=0x01;
		Command[4]=0x00;
		
		for(u8 i=0;i<6;i++)
	   CheckCode+=Command[i];
	   Command[5]=CheckCode;
		
		Usart_SendData(Command,6);
	}
	else if(CommandCode==0x22)
	{
		Command[1]=0x22;
		if(ControlCode==0x0C)
		{
			Command[2]=0x04;
		}
		if(ControlCode==0x0D)
		{
			Command[2]=0x05;
		}
		if(ControlCode==0x0E)
		{
			Command[2]=0x06;
		}
		Command[3]=0x01;
		Command[4]=0x00;
		
		for(u8 i=0;i<6;i++)
	   CheckCode+=Command[i];
	   Command[5]=CheckCode;
		
		Usart_SendData(Command,6);
	}
	else if(CommandCode==0x23)
	{
		Command[1]=0x23;
		if(ControlCode==0x0C)
		{
			Command[2]=0x04;
		}
		if(ControlCode==0x0D)
		{
			Command[2]=0x05;
		}
		if(ControlCode==0x0E)
		{
			Command[2]=0x06;
		}
		Command[3]=0x01;
		Command[4]=0x00;
		
		for(u8 i=0;i<6;i++)
	   CheckCode+=Command[i];
	   Command[5]=CheckCode;
		
		Usart_SendData(Command,6);
	}
	else if(CommandCode>0x24&&CommandCode<0x2D)
	{
		
	}
}
/***********************************************************
*�������� ReadVersion
*���ܣ�   ��ȡboot/�̼�/Ӳ���İ汾
*������   VBoot��boot�汾��VFirmware���̼��汾��VHardware��Ӳ���汾
          3.2 ��0x32��ǰ��λ��ţ�����λС��
*����ֵ�� ��
***********************************************************/
void ReadVersion(u8 VBoot,u8 VFirmware,u8 VHardware)
{
	u8 Command[8];
	u8 CheckCode=0;
	
	Command[0]=0x68;
	
	Command[1]=0x06;
	Command[2]=0x10;
	Command[3]=0x03;
	
	Command[4]=VBoot;
	Command[5]=VFirmware;
	Command[6]=VHardware;
	
	Command[7]=0x7F;
	
	for(u8 i=0;i<7;i++)
	{
		CheckCode+=Command[i];
	}
	Command[7]=CheckCode;
	Usart_SendData(Command,8);
}
void MotorBackOpenButton()
{
	u8 Command[6];
	
	Command[0]=0x68;
	
	Command[1]=0x30;
	Command[2]=0x04;
	Command[3]=0x01;
	Command[4]=0x00;
	
	Command[5]=0x9D;
	
	Usart_SendData(Command,6);
}
void MotorBackCloseButton()
{
	u8 Command[6];
	
	Command[0]=0x68;
	
	Command[1]=0x31;
	Command[2]=0x04;
	Command[3]=0x01;
	Command[4]=0x00;
	
	Command[5]=0x9E;
	
	Usart_SendData(Command,6);
}
void MotorBackstopButton()
{
	u8 Command[6];
	
	Command[0]=0x68;
	
	Command[1]=0x32;
	Command[2]=0x04;
	Command[3]=0x01;
	Command[4]=0x00;
	
	Command[5]=0x9F;
	
	Usart_SendData(Command,6);
}
void SaveParaToFlash()
{
	u8 Command[6];
	
	Command[0]=0x68;
	
	Command[1]=0x2D;
	Command[2]=0x04;
	Command[3]=0x01;
	Command[4]=0x00;
	
	Command[5]=0x9A;
	
	Usart_SendData(Command,6);
}
/***********************************************************
*�������� RunCommand(u8 *RecData,u8 DataIndex)
*���ܣ�   �������֡����
*������   u8 *RecData,u8 DataIndex������ָ�����飬��ǰ֡��ͷ����λ��
*����ֵ�� ��һ֡��ͷ����λ��
***********************************************************/
u8 RunCommand(u8 *RecData,u8 DataIndex)
{
	switch(RecData[DataIndex+1])
	{
		case 0x01:
	{
		DataIndex+=(RecData[DataIndex+3]+5);
	}
				break;
	  case 0x02:
	{
		DataIndex+=(RecData[DataIndex+3]+5);
	}
				break;
	  case 0x03:
	{
		IsConnnect();
		DataIndex+=(RecData[DataIndex+3]+5);
	}
				break;
	  case 0x04:
	{
		DataIndex+=(RecData[DataIndex+3]+5);
	}
				break;
	  case 0x05:
	{
		ReturnBootLoader();
		DataIndex+=(RecData[DataIndex+3]+5);
	}
				break;
	case 0x06:
	{
		ReadVersion(0x11,0x22,0x33);
		DataIndex+=(RecData[DataIndex+3]+5);
	}
				break;
	case 0x07:
	{
		CalibrateMode(0x07);
		TIM_Cmd(TIM3,ENABLE);
		DataIndex+=(RecData[DataIndex+3]+5);
	}
				break;
	case 0x08:
	{
		CalibrateMode(0x08);
		TIM_Cmd(TIM3,DISABLE);
		DataIndex+=(RecData[DataIndex+3]+5);
	}
				break;
	case 0x09:
	{
		ReadProgramerState();
		DataIndex+=(RecData[DataIndex+3]+5);
	}
				break;
	case 0x0A:
	{
		ClearCalibrationData();
		DataIndex+=(RecData[DataIndex+3]+5);
	}
				break;
	case 0x10:
	{
		SendMotorSpeed(USART_RX_BUF[DataIndex+2]);
		DataIndex+=(RecData[DataIndex+3]+5);
	}
				break;
	case 0x11:
	{
		IsMotorReverse(USART_RX_BUF[DataIndex+2]);
		DataIndex+=(RecData[DataIndex+3]+5);
	}
				break;
	case 0x12:
	{
		SendMotorClosePosition(USART_RX_BUF[DataIndex+2]);
		DataIndex+=(RecData[DataIndex+3]+5);
	}
				break;
	case 0x13:
	{
		SendMotorOpenPosition(USART_RX_BUF[DataIndex+2]);
		DataIndex+=(RecData[DataIndex+3]+5);
	}
				break;
	case 0x14:
	{
		IsMotorPositionReverse(USART_RX_BUF[DataIndex+2]);
		DataIndex+=(RecData[DataIndex+3]+5);
	}
				break;
	case 0x1D:
	{
		SendMotorAdValue(USART_RX_BUF[DataIndex+2]);
		DataIndex+=(RecData[DataIndex+3]+5);
	}
				break;
	case 0x1E:
	{
		TIM_Cmd(TIM3,ENABLE);
		SendMotorCurrent(USART_RX_BUF[DataIndex+2]);
		DataIndex+=(RecData[DataIndex+3]+5);
	}
				break;
	case 0x20:
	{
		MotorSpeed[RecData[DataIndex+2]-0x0C]=RecData[DataIndex+4];
		SendWriteParaCommand(0x20,RecData[DataIndex+2]);
		DataIndex+=(RecData[DataIndex+3]+5);
	}
				break;
	case 0x22:
	{
		MotorClosePosition[RecData[DataIndex+2]-0x0C]=(RecData[DataIndex+4]<<8)+(RecData[DataIndex+5]);
		SendWriteParaCommand(0x22,RecData[DataIndex+2]);
		DataIndex+=(RecData[DataIndex+3]+5);
	}
				break;
	case 0x23:
	{
		MotorOpenPosition[RecData[DataIndex+2]-0x0C]=(RecData[DataIndex+4]<<8)+(RecData[DataIndex+5]);
		SendWriteParaCommand(0x23,RecData[DataIndex+2]);
		DataIndex+=(RecData[DataIndex+3]+5);
	}
				break;
	case 0x25:
	{
		
	}
				break;
	case 0x26:
	{
		
	}
				break;
	case 0x27:
	{
		
	}
				break;
	case 0x28:
	{
		
	}
				break;
	case 0x29:
	{
		
	}
				break;
	case 0x2A:
	{
		
	}
				break;
	case 0x2B:
	{
		
	}
				break;
	case 0x2C:
	{
		
	}
				break;
	case 0x2D:
	{
		 SaveParaToFlash();
		 DataIndex+=(RecData[DataIndex+3]+5);
	}
				break;
	case 0x30:
	{
		MotorBackOpenButton();
		DataIndex+=(RecData[DataIndex+3]+5);
	}
				break;
	case 0x31:
	{
		MotorBackCloseButton();
		DataIndex+=(RecData[DataIndex+3]+5);
	}
				break;
	case 0x32:
	{
		 MotorBackstopButton();
		DataIndex+=(RecData[DataIndex+3]+5);
	}
				break;
	default:
		DataIndex+=(RecData[DataIndex+3]+5);
		break;
 }
	return DataIndex;
}
/***********************************************************
*�������� FindFrameHead(u8 *RecData,u8 DataIndex)
*���ܣ�   Ѱ�ҵ�ǰ֡��ͷ����λ��
*������   u8 *RecData,u8 DataIndex������ָ�����飬��ǰ֡��ͷ����λ��
*����ֵ�� ��ǰ��ͷ����λ��
***********************************************************/
u8 FindFrameHead(u8 *RecData,u8 DataIndex)
{
	  while(RecData[DataIndex]!=0x68)
    {
        if(RecData[DataIndex]==0x00)       //�ж��Ƿ������ɣ����ж����������Ƿ�Ϊ0������Ϊ0���������ɡ�(Ĭ�������ʼ��Ϊ0��
        {
            DataIndex=200;
            break;
        }
        else
        {
            DataIndex++;
            if(DataIndex>=100)
            {
                break;
            }
        }
     }
    return DataIndex;
}
/***********************************************************
*�������� IsCheckSumRight(u8 *RecData,u8 DataIndex)
*���ܣ�   ���㵱ǰ֡У����Ƿ���ȷ
*������   u8 *RecData,u8 DataIndex������ָ�����飬��ǰ֡��ͷ����λ��
*����ֵ�� ��ǰ֡У���
***********************************************************/
u8 IsCheckSumRight(u8 *RecData,u8 DataIndex)
{
	  uint8_t DataLength=RecData[DataIndex+3]+4;//����֡���ݳ��ȣ�������У��λ
    uint8_t Sum=0;
    uint8_t CheckNum=0;
    for(u8 i=DataIndex;i<DataLength+DataIndex;i++)
    {
        Sum+=RecData[i];
    }
    CheckNum=RecData[DataLength+DataIndex];
    if(Sum==CheckNum)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
/***********************************************************
*�������� Iap_Write()
*���ܣ�   �������鴦�����
*������   ��
*����ֵ�� ��
***********************************************************/
void Iap_Write()
{
	 	uint16_t DataIndex=0,IsCheckSumSucceed;
	 while(RecFlag)
	{ 
		 DataIndex=FindFrameHead(USART_RX_BUF,DataIndex);
		 if(DataIndex==200)
		 {
			  RecFlag=0;
			  memset(USART_RX_BUF,0x00,sizeof(USART_RX_BUF));
		 }
		 else if(DataIndex<100)
		 {
				 IsCheckSumSucceed=IsCheckSumRight(USART_RX_BUF,DataIndex);
				 if(IsCheckSumSucceed)
				 {
					 DataIndex=RunCommand(USART_RX_BUF,DataIndex);
				 }
				 else
			   {
						 break;
				 } 
		 }
		 else 
		 {
			 break;
		 }
	 }
}

