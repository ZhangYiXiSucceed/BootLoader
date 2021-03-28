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
#define   BootSaveData                      0x080E0000

extern uint8_t RecCommandStringList[][10];
extern uint8_t SendCommandStringList[][10];
typedef  void (*iapfun)(void);				//����һ���������͵Ĳ���.   

void iap_load_app(u32 appxaddr);			//��ת��APP����ִ��
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







































