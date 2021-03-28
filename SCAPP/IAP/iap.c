#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "stmflash.h"
#include "iap.h" 
#include "stdio.h"
#include "string.h"

iapfun jump2app; 
//跳转到应用程序段
//appxaddr:用户代码起始地址.
void iap_load_app(u32 appxaddr)
{
	if(((*(vu32*)appxaddr)&0x2FFE0000)==0x20000000)	//检查栈顶地址是否合法.
	{ 
		jump2app=(iapfun)*(vu32*)(appxaddr+4);		//用户代码区第二个字为程序开始地址(复位地址)		
		MSR_MSP(*(vu32*)appxaddr);					//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
		jump2app();									        //跳转到APP.
	}
	else
	{
		printf("error!");
	}
}		 

uint32_t Addr_now = APP_ADDR;	        //写入到FLASH的地址
uint8_t JumpAddr=0;

u16 MotorBackAdValue=0;         //测试数据，背靠电机ADC值
u16 MotorLegAdValue=0;          //测试数据，腿靠电机ADC值
u16 MotorFootAdValue=0;         //测试数据，脚踏电机ADC值

u8 MotorSpeed[3]={50,70,90};    //测试数据，三个电机的速度值（占空比%）

u16 MotorClosePosition[3]={123,456,789};           //测试数据，三个电机的收起极限位置（占空比%）
u16 MotorOpenPosition[3]={234,345,456};            //测试数据，三个电机的打开极限位置（占空比%）
u16 MotorLinkPosition[8]={11,22,33,44,55,66,77,88};//测试数据，三个电机的联动点值（占空比%）

u16 MotorCurrent[3]={1520,3524,4130};   //测试数据，三个电机的运行电流值（占空比%）

/***********************************************************
*函数名： CalFrameCheckCode
*功能：   计算当前帧指令校验和
*参数：   u8 *TempBuf,u8 Length，帧数组和帧长度
*返回值： 校验和
***********************************************************/
u8 CalFrameCheckCode(u8 *TempBuf,u8 Length)
{
	u8 i=0,CheckSum=0;
	for(i=0;i<Length;i++)
	   CheckSum+=TempBuf[i];
	return CheckSum;
}
/***********************************************************
*函数名： IsConnnect
*功能：   发送下位机连接响应指令
*参数：   无
*返回值： 无
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
*函数名： ReturnBootLoader()
*功能：   发送下位机返回BootLoader指令
*参数：   无
*返回值： 无
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
*函数名： ReadProgramerState
*功能：   读取程序运行状态
*参数：   无
*返回值： 无
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
*函数名： CalibrateMode(uint8_t CommandCode)
*功能：   发送下位机返回标定模式指令，
*参数：   CommandCode  为打开或者关闭指令
*返回值： 无
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
*函数名： ReadProgramerState
*功能：   清空标定数据
*参数：   无
*返回值： 无
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
*函数名： SendMotorAdValue(uint8_t CommandCode)
*功能：   发送下位机ADC值
*参数：   CommandCode，电机指令码
*返回值： 无
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
*函数名： SendMotorAdValue(uint8_t CommandCode)
*功能：   发送下位机电机速度值
*参数：   CommandCode，电机指令码
*返回值： 无
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
*函数名： SendMotorAdValue(uint8_t CommandCode)
*功能：   发送下位机收起极限位置值
*参数：   CommandCode，电机指令码
*返回值： 无
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
*函数名： SendMotorAdValue(uint8_t CommandCode)
*功能：   发送下位机电机运行电流值
*参数：   CommandCode，电机指令码
*返回值： 无
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
*函数名： SendMotorAdValue(uint8_t CommandCode)
*功能：   发送下位机电机打开极限位置值
*参数：   CommandCode，电机指令码
*返回值： 无
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
*函数名： SendMotorAdValue(uint8_t CommandCode)
*功能：   发送下位机电机是否反向指令
*参数：   CommandCode，电机指令码
*返回值： 无
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
*函数名： IsMotorPositionReverse(uint8_t CommandCode)
*功能：   发送下位机电位器是否反向
*参数：   CommandCode，电机指令码
*返回值： 无
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
*函数名： SendWriteParaCommand(uint8_t CommandCode)
*功能：   发送下位机写入参数成功指令
*参数：   u8 CommandCode,功能码  u8 ControlCode，控制码
*返回值： 无
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
*函数名： ReadVersion
*功能：   读取boot/固件/硬件的版本
*参数：   VBoot：boot版本，VFirmware：固件版本，VHardware：硬件版本
          3.2 即0x32，前四位大号，后四位小号
*返回值： 无
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
*函数名： RunCommand(u8 *RecData,u8 DataIndex)
*功能：   处理接收帧函数
*参数：   u8 *RecData,u8 DataIndex，接收指令数组，当前帧包头索引位置
*返回值： 下一帧包头索引位置
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
*函数名： FindFrameHead(u8 *RecData,u8 DataIndex)
*功能：   寻找当前帧包头索引位置
*参数：   u8 *RecData,u8 DataIndex，接收指令数组，当前帧包头索引位置
*返回值： 当前包头索引位置
***********************************************************/
u8 FindFrameHead(u8 *RecData,u8 DataIndex)
{
	  while(RecData[DataIndex]!=0x68)
    {
        if(RecData[DataIndex]==0x00)       //判断是否接收完成，即判断下面数组是否为0，如若为0，则接收完成。(默认数组初始化为0）
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
*函数名： IsCheckSumRight(u8 *RecData,u8 DataIndex)
*功能：   计算当前帧校验和是否正确
*参数：   u8 *RecData,u8 DataIndex，接收指令数组，当前帧包头索引位置
*返回值： 当前帧校验和
***********************************************************/
u8 IsCheckSumRight(u8 *RecData,u8 DataIndex)
{
	  uint8_t DataLength=RecData[DataIndex+3]+4;//计算帧数据长度，不包含校验位
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
*函数名： Iap_Write()
*功能：   接收数组处理程序
*参数：   无
*返回值： 无
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

