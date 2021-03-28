#ifndef __BSP_RTC_H
#define __BSP_RTC_H

#include "user_datastructure.h"
#include "stm32f4xx_rtc.h"

//异步分频因子
#define ASYNCHPREDIV     0x7F
#define ASYNCHPREDIV_IN  0x95
//同步分频因子
#define SYNCHPREDIV      0xFF
#define SYNCHPREDIV_IN   0xC7

//使用内部时钟源
//#define     RTC_CLOCK_SOURCE_LSI
//使用外部时钟源
#define   RTC_CLOCK_SOURCE_LSE

// 时间宏定义
#define RTC_H12_AMorPM			 RTC_H12_AM      //表示24小时制
// 时间格式宏定义
#define RTC_Format_BINorBCD  RTC_Format_BIN  //表示以下时间参数均为10进制数




// 备份寄存器宏定义
#define RTC_BKP_DRX          RTC_BKP_DR0
// 写入到备份寄存器的数据宏定义
#define RTC_BKP_DATA         0X8888



//对外开放的函数
uint8_t Record_RTC_Init(void);
void RTC_CheckRTCSet(void);
void RTC_TimeAndDate_Set(RTC_Time_T *TimeTOSet);
void RTC_TimeAndDate_Get(RTC_Time_T *TimeOFGet);

#endif
