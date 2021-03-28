#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "stmflash.h"
#include "iap.h" 
#include "stdio.h"
#include "string.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//IAP 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/7/21
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

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

uint32_t Addr_now = APP_ADDR;	
uint32_t Pagedata_write[1024]={0};
int TakeDataNum=0;


/***********************************************************
*函数名： UpdateFun
*功能：   发送程序更新指令
*参数：   无
*返回值： 无
***********************************************************/
void UpdateFun(void)     
{
	u8 Command[6];
	
	Command[0]=0x68;
	
	Command[1]=0x00;
	Command[2]=0x14;
	Command[3]=0x01;
	Command[4]=0x00;
	
	Command[5]=0x7D;
	
  Usart_SendData(Command,6);
}
/***********************************************************
*函数名： OnceEndFun
*功能：   发送一次接收程序完成指令
*参数：   无
*返回值： 无
***********************************************************/
void OnceEndFun(void)      
{
	u8 Command[6];
	
	Command[0]=0x68;
	
	Command[1]=0x01;
	Command[2]=0x14;
	Command[3]=0x01;
	Command[4]=0x00;
	
	Command[5]=0x7E;
	
  Usart_SendData(Command,6);
}
/***********************************************************
*函数名： FileEndFun
*功能：   发送全部接收完成指令
*参数：   无
*返回值： 无
***********************************************************/
void FileEndFun(void)      
{
	u8 Command[6];
	
	Command[0]=0x68;
	
	Command[1]=0x02;
	Command[2]=0x14;
	Command[3]=0x01;
	Command[4]=0x00;
	
	Command[5]=0x7F;
	
	Usart_SendData(Command,6);
}
/***********************************************************
*函数名： Connect
*功能：   确认连接指令，同时关闭进入APP程序的指令
*参数：   无
*返回值： 无
***********************************************************/
void Connect()            
{
	u8 Command[6];
	
	Command[0]=0x68;
	
	Command[1]=0x03;
	Command[2]=0x14;
	Command[3]=0x01;
	Command[4]=0x00;
	
	Command[5]=0x80;
	
	Usart_SendData(Command,6);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);
  TIM_Cmd(TIM3,DISABLE);
}
/***********************************************************
*函数名： EnterAPP
*功能：   进入APP程序指令，同时进入之前，要关闭所有中断
*参数：   无
*返回值： 无
***********************************************************/
void EnterAPP()
{
	u8 Command[6];
	
	Command[0]=0x68;
	
	Command[1]=0x04;
	Command[2]=0x14;
	Command[3]=0x01;
	Command[4]=0x00;
	
	Command[5]=0x81;
	
	Usart_SendData(Command,6);
	
	__disable_irq();
	iap_load_app(APP_ADDR);
}
/***********************************************************
*函数名： ReturnBootloader
*功能：   
*参数：   无
*返回值： 无
***********************************************************/
void ReturnBootloader()
{
	u8 Command[6];
	
	Command[0]=0x68;
	
	Command[1]=0x05;
	Command[2]=0x14;
	Command[3]=0x01;
	Command[4]=0x00;
	
	Command[5]=0x82;
	
	Usart_SendData(Command,6);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);
  TIM_Cmd(TIM3,DISABLE);
	
  __disable_irq();
	iap_load_app(BootLoaderADDR);
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
	Command[4]=0x01;
	
	Command[5]=0x83;
	
	Usart_SendData(Command,6);
}

/*******************************************************************************
* 函数名	: revbit  
* 描述	    : 32位数据进行位到序   如0111-->1110 
* 输入参数  : uData:源数据
*           
* 返回参数  : 倒叙后的数据
*******************************************************************************/
static uint32_t revbit(u32 uData)  
{  
		 uint32_t uRevData = 0,uIndex = 0;  
		 uRevData |= ((uData >> uIndex) & 0x01);  
		 for(uIndex = 1;uIndex < 32;uIndex++)  
		 {     
			uRevData <<= 1; 
			uRevData |= ((uData >> uIndex) & 0x01);  
		 }  
		 return uRevData;  
}

/*******************************************************************************
* 函数名	  : CRC32_ForWords  
* 描述	    : 通过STM32内部自带CRC32计算模块求CRC
* 输入参数  : pData指向数据源
*						: uLen为数据长度 
*           : BigLittle数据格式大下端,1大端，0小端
* 返回参数  : 输出计算的CRC32数值
*******************************************************************************/
static uint32_t CRC32_ForWords(uint32_t *pData,uint32_t uLen,uint32_t BigLittle)  
{  
    u32 i = 0,uData = 0;  
    if((RCC->AHB1ENR & (1<<6)) == 0)  
    {  
        RCC->AHB1ENR |= 1<<6;  
    }   
    /* Reset CRC generator */  
    CRC->CR = CRC_CR_RESET;  
    for (i = 0;i < uLen;i++)  
    {  
			if(BigLittle)      
        uData = __REV(*(pData + i));  
			else  
        uData = *(pData + i);  
      
        CRC->DR = revbit(uData);  
    }  
    return revbit(CRC->DR)^0xFFFFFFFF;  
}
/*******************************************************************************
* 函数名	  : TakeValidData_1K  
* 描述	    : 对要下载的程序进行校验，每四次写入一次FLASH
* 输入参数  : 当前接收到的程序的次数
* 返回参数  : 输出接收到的字节数，如接收错误，直接返回0XFFFF
*******************************************************************************/
uint16_t TakeValidData_1K(int num)
{
	uint32_t CRCValue=0,CRCCalvalue=0;
	uint16_t temp = 0,i;
	uint16_t data_len = 0;
	uint16_t PageNum=0;
	uint8_t data_write[1024] = { 0 };
	if(USART_RX_BUF[0]!=0x68)
	{
		return 0xFFFF;
	}
	CRCValue=USART_RX_BUF[1029];  
	data_len = (uint16_t)(USART_RX_BUF[4]<<8|USART_RX_BUF[3]);
	for(i=5;i<data_len+5;i++)
	  CRCCalvalue+=USART_RX_BUF[i];
  CRCCalvalue=CRCCalvalue%256;
	
	if(CRCValue==CRCCalvalue)
	{
    switch(USART_RX_BUF[1])
		{
			case 0x01:
			{
				for(temp=5;temp<data_len+5;temp++)
				{
					data_write[temp-5]=USART_RX_BUF[temp];
				}
				num=num%4;
				for(i=0;i<data_len;i+=4,PageNum+=1)
				{
						 Pagedata_write[num*256+PageNum]=data_write[i+3]<<24;
						 Pagedata_write[num*256+PageNum]|=data_write[i+2]<<16;
						 Pagedata_write[num*256+PageNum]|=data_write[i+1]<<8;
						 Pagedata_write[num*256+PageNum]|=data_write[i];
				}
			}
			break;
			case 0x02:
			{
				for(temp=5;temp<data_len+5;temp++)
				{
					data_write[temp-5]=USART_RX_BUF[temp];
				}
				num=num%4;
				for(i=0;i<data_len;i+=4,PageNum+=1)
				{
						 Pagedata_write[num*256+PageNum]=data_write[i+3]<<24;
						 Pagedata_write[num*256+PageNum]|=data_write[i+2]<<16;
						 Pagedata_write[num*256+PageNum]|=data_write[i+1]<<8;
						 Pagedata_write[num*256+PageNum]|=data_write[i];
				}
			}
			break;
		}
  }
	else
		data_len=0xFFFF;
	return data_len;
}
/*******************************************************************************
* 函数名	  : Iap_Write  
* 描述	    : 对接收的指令进行处理，可以处理多条指令
* 输入参数  : 无
* 返回参数  : 无
*******************************************************************************/
void Iap_Write()
{
	uint16_t DataIndex=0,IsCheckSumSucceed;
	 while(RecFlag)
	{ 
		 DataIndex=FindFrameHead(USART_RX_BUF,DataIndex);
		 if(DataIndex==200)
		 {
			  RecFlag=0;
			  //memset(USART_RX_BUF,0x00,sizeof(USART_RX_BUF));
		 }
		 else if(DataIndex<100)
		 {
			 if(ValidDataLength<1024)
			 {
				 IsCheckSumSucceed=IsCheckSumRight(USART_RX_BUF,DataIndex);
				 if(IsCheckSumSucceed)
				 {
					 DataIndex=RunCommand(USART_RX_BUF,DataIndex);
				 }
			 }
			 else
			 {
				 DataIndex=RunCommand(USART_RX_BUF,DataIndex);
			 }
			
		 }
	 }
}
/*******************************************************************************
* 函数名	  : FindFrameHead  
* 描述	    : 寻找当前帧指令包头0x68
* 输入参数  : u8 *RecData,u16 DataIndex，接收指令数组和当前帧指令包头索引
* 返回参数  : 包头在数组的索引位置
*******************************************************************************/	
u16 FindFrameHead(u8 *RecData,u16 DataIndex)
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
/*******************************************************************************
* 函数名	  : IsCheckSumRight  
* 描述	    : 判断当前帧指令校验和是否正确
* 输入参数  : u8 *RecData,u16 DataIndex，接收指令数组和当前帧指令包头索引
* 返回参数  : 校验和是否正确
*******************************************************************************/	
u16 IsCheckSumRight(u8 *RecData,u16 DataIndex)
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
/*******************************************************************************
* 函数名	  : RunCommand  
* 描述	    : 对当前帧指令进入处理
* 输入参数  : u8 *RecData,u16 DataIndex，接收指令数组和当前帧指令包头索引
* 返回参数  : 下一帧数组指令包头位置
*******************************************************************************/	
u16 RunCommand(u8 *RecData,u16 DataIndex)
{
	switch(RecData[DataIndex+1])
	{
		case 0x00:
		{
			UpdateFun();
			DataIndex+=(RecData[DataIndex+3]+5);
		}
		break;
		case 0x01:
		{
			WriteFlash();
			OnceEndFun();
			DataIndex+=((RecData[DataIndex+4]<<8) + (RecData[DataIndex+3])+5);
		}
		break;
		case 0x02:
		{
			WriteFlash();
			FileEndFun();
			DataIndex+=((RecData[DataIndex+4]<<8) + (RecData[DataIndex+3])+5);
		}
		break;
		case 0x03:
		{
			Connect();
			DataIndex+=(RecData[DataIndex+3]+5);
		}
		break;
		case 0x04:
		{
			EnterAPP();
			DataIndex+=(RecData[DataIndex+3]+5);
		}
		break;
		case 0x05:
		{
			ReturnBootloader();
			DataIndex+=(RecData[DataIndex+3]+5);
		}
		break;
		case 0x06:
		{
			ReadVersion(0x11,0x22,0x33);
			DataIndex+=(RecData[DataIndex+3]+5);
		}
		break;
		case 0x09:
		{
			ReadProgramerState();
			DataIndex+=(RecData[DataIndex+3]+5);
		}
		break;
		default:
			DataIndex+=(RecData[DataIndex+3]+5);
		break;
	}
	return DataIndex;
}
/*******************************************************************************
* 函数名	  : WriteFlash  
* 描述	    : 将接收的程序写入到对应的FLASH当中
* 输入参数  : 无
* 返回参数  : 无
*******************************************************************************/	
void WriteFlash()
{
	  u16 TempValidDataLength=0,FlashNum=0;
	
		TempValidDataLength=TakeValidData_1K(TakeDataNum);
	  if(TempValidDataLength==0xFFFF)
		{
			 return;
		}
		else
		{
			TakeDataNum++;
			if(TempValidDataLength<1024)
			{
					if(TakeDataNum==1)
					{
						FlashNum=TempValidDataLength/4+1;
					}
					else if(TakeDataNum==2)
					{
						FlashNum+=256;
						FlashNum+=TempValidDataLength/4+1;
					}
					else if(TakeDataNum==3)
					{
						FlashNum+=512;
						FlashNum+=TempValidDataLength/4+1;
					}
					else if(TakeDataNum==4)
					{
						FlashNum+=768;
						FlashNum+=TempValidDataLength/4+1;
					}
						 STMFLASH_Write(Addr_now, (uint32_t*)Pagedata_write,FlashNum);
						 FlashNum=0;
			 }
			else
			{
				if(TakeDataNum==4)
				{
						STMFLASH_Write(Addr_now, (uint32_t*)Pagedata_write,1024);
						Addr_now+=1024*4;
						TakeDataNum=0;
				}
			}
		}
}






