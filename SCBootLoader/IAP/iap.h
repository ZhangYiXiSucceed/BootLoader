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
#define   BootSaveData                      0x080E0000

extern uint8_t RecCommandStringList[][10];
extern uint8_t SendCommandStringList[][10];
typedef  void (*iapfun)(void);				//定义一个函数类型的参数.   

void iap_load_app(u32 appxaddr);			//跳转到APP程序执行
static uint32_t CRC32_ForWords(uint32_t *pData,uint32_t uLen,uint32_t BigLittle);
static uint32_t revbit(u32 uData);
void Iap_Write(void);
void UpdateFun(void);
void OnceEndFun(void);
void FileEndFun(void);
void Connect(void);
void EnterAPP(void);
void ReturnBootloader(void);
void ReadVersion(u8 VBoot,u8 VFirmware,u8 VHardware);
void ReadProgramerState(void);
u16 IsCheckSumRight(u8 *RecData,u16 DataIndex);
u16 FindFrameHead(u8 *RecData,u16 DataIndex);
void WriteFlash(void);
u16 RunCommand(u8 *RecData,u16 DataIndex);
#endif







































