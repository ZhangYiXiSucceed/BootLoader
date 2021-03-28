#ifndef __IAP_H__
#define __IAP_H__
#include "sys.h"  
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
#define   BootLoaderADDR                    0x08000000
#define   APPSaveData                       0x08010000
#define 	APP_ADDR													0x08010000
#define 	APP2_ADDR													0x08080000
#define   BootSaveData                      0x080E0000



extern u16 MotorBackAdValue;
extern u16 MotorLegAdValue;
extern u16 MotorFootAdValue;


extern u16 MotorCurrent[3];

typedef  void (*iapfun)(void);				//定义一个函数类型的参数. 
void iap_load_app(u32 appxaddr);			//跳转到APP程序执行
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







































