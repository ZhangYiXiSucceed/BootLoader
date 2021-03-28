#ifndef __BSP_RTC_H
#define __BSP_RTC_H

#include "user_datastructure.h"
#include "stm32f4xx_rtc.h"

//�첽��Ƶ����
#define ASYNCHPREDIV     0x7F
#define ASYNCHPREDIV_IN  0x95
//ͬ����Ƶ����
#define SYNCHPREDIV      0xFF
#define SYNCHPREDIV_IN   0xC7

//ʹ���ڲ�ʱ��Դ
//#define     RTC_CLOCK_SOURCE_LSI
//ʹ���ⲿʱ��Դ
#define   RTC_CLOCK_SOURCE_LSE

// ʱ��궨��
#define RTC_H12_AMorPM			 RTC_H12_AM      //��ʾ24Сʱ��
// ʱ���ʽ�궨��
#define RTC_Format_BINorBCD  RTC_Format_BIN  //��ʾ����ʱ�������Ϊ10������




// ���ݼĴ����궨��
#define RTC_BKP_DRX          RTC_BKP_DR0
// д�뵽���ݼĴ��������ݺ궨��
#define RTC_BKP_DATA         0X8888



//���⿪�ŵĺ���
uint8_t Record_RTC_Init(void);
void RTC_CheckRTCSet(void);
void RTC_TimeAndDate_Set(RTC_Time_T *TimeTOSet);
void RTC_TimeAndDate_Get(RTC_Time_T *TimeOFGet);

#endif
