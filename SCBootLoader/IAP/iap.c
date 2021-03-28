#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "stmflash.h"
#include "iap.h" 
#include "stdio.h"
#include "string.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//IAP ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/7/21
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

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

uint32_t Addr_now = APP_ADDR;	
uint32_t Pagedata_write[1024]={0};
int TakeDataNum=0;


/***********************************************************
*�������� UpdateFun
*���ܣ�   ���ͳ������ָ��
*������   ��
*����ֵ�� ��
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
*�������� OnceEndFun
*���ܣ�   ����һ�ν��ճ������ָ��
*������   ��
*����ֵ�� ��
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
*�������� FileEndFun
*���ܣ�   ����ȫ���������ָ��
*������   ��
*����ֵ�� ��
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
*�������� Connect
*���ܣ�   ȷ������ָ�ͬʱ�رս���APP�����ָ��
*������   ��
*����ֵ�� ��
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
*�������� EnterAPP
*���ܣ�   ����APP����ָ�ͬʱ����֮ǰ��Ҫ�ر������ж�
*������   ��
*����ֵ�� ��
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
*�������� ReturnBootloader
*���ܣ�   
*������   ��
*����ֵ�� ��
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
	Command[4]=0x01;
	
	Command[5]=0x83;
	
	Usart_SendData(Command,6);
}

/*******************************************************************************
* ������	: revbit  
* ����	    : 32λ���ݽ���λ����   ��0111-->1110 
* �������  : uData:Դ����
*           
* ���ز���  : ����������
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
* ������	  : CRC32_ForWords  
* ����	    : ͨ��STM32�ڲ��Դ�CRC32����ģ����CRC
* �������  : pDataָ������Դ
*						: uLenΪ���ݳ��� 
*           : BigLittle���ݸ�ʽ���¶�,1��ˣ�0С��
* ���ز���  : ��������CRC32��ֵ
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
* ������	  : TakeValidData_1K  
* ����	    : ��Ҫ���صĳ������У�飬ÿ�Ĵ�д��һ��FLASH
* �������  : ��ǰ���յ��ĳ���Ĵ���
* ���ز���  : ������յ����ֽ���������մ���ֱ�ӷ���0XFFFF
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
* ������	  : Iap_Write  
* ����	    : �Խ��յ�ָ����д������Դ������ָ��
* �������  : ��
* ���ز���  : ��
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
* ������	  : FindFrameHead  
* ����	    : Ѱ�ҵ�ǰָ֡���ͷ0x68
* �������  : u8 *RecData,u16 DataIndex������ָ������͵�ǰָ֡���ͷ����
* ���ز���  : ��ͷ�����������λ��
*******************************************************************************/	
u16 FindFrameHead(u8 *RecData,u16 DataIndex)
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
/*******************************************************************************
* ������	  : IsCheckSumRight  
* ����	    : �жϵ�ǰָ֡��У����Ƿ���ȷ
* �������  : u8 *RecData,u16 DataIndex������ָ������͵�ǰָ֡���ͷ����
* ���ز���  : У����Ƿ���ȷ
*******************************************************************************/	
u16 IsCheckSumRight(u8 *RecData,u16 DataIndex)
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
/*******************************************************************************
* ������	  : RunCommand  
* ����	    : �Ե�ǰָ֡����봦��
* �������  : u8 *RecData,u16 DataIndex������ָ������͵�ǰָ֡���ͷ����
* ���ز���  : ��һ֡����ָ���ͷλ��
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
* ������	  : WriteFlash  
* ����	    : �����յĳ���д�뵽��Ӧ��FLASH����
* �������  : ��
* ���ز���  : ��
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






