#ifndef __IAP_H__
#define __IAP_H__
#include "sys.h"  
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
#define   BootLoaderADDR                    0x08000000
#define   APPSaveData                       0x08010000
#define 	APP_ADDR													0x08010000
#define 	APP2_ADDR													0x08080000
#define   BootSaveData                      0x080E0000



extern u16 MotorBackAdValue;
extern u16 MotorLegAdValue;
extern u16 MotorFootAdValue;


extern u16 MotorCurrent[3];

typedef  void (*iapfun)(void);				//����һ���������͵Ĳ���. 
void iap_load_app(u32 appxaddr);			//��ת��APP����ִ��
void Iap_Write(void);
u8 CalFrameCheckCode(u8 *TempBuf,u8 Length);
void IsConnnect(void);
void ReturnBootLoader(void);
void CalibrateMode(uint8_t CommandCode);
void SendMotorAdValue(uint8_t CommandCode);
void SendMotorSpeed(uint8_t CommandCode);
void IsMotorReverse(uint8_t CommandCode);
void IsMotorPositionReverse(uint8_t CommandCode);
u8 IsCheckSumRight(u8 *RecData,u8 DataIndex);
u8 FindFrameHead(u8 *RecData,u8 DataIndex);
u8 RunCommand(u8 *RecData,u8 DataIndex);
#endif







































