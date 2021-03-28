#include "bsp_rtc.h"

/*
*********************************************************************************************************
*
*	模块名称 : RTC模块(片内RTC,LSE)
*	文件名称 : bsp_rtc.c
*	版    本 : V1.0
*	说    明 : 主要实现判断RTC是否被设置过，配置RTC，设置、获取RTC时间功能
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2019-04-26  WLM      草稿
*
*********************************************************************************************************
*/

/*RTC初始化失败或未进行设定时返回的默认时间*/
RTC_Time_T RTC_Time_DefaultValue;
/*RTC初始化成功标记，1为已设定过，0为未设定*/
uint8_t Flag_RTC_OK=0;

/*
*********************************************************************************************************
*	函 数 名: RTC_CLK_Config
*	功能说明: RTC时钟初始化，配置为外部晶体驱动，设置时间格式
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void RTC_CLK_Config(void)
{  
	RTC_InitTypeDef RTC_InitStructure;

	
	/*使能 PWR 时钟*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	
  /* PWR_CR:DBF置1，使能RTC、RTC备份寄存器和备份SRAM的访问 */
  PWR_BackupAccessCmd(ENABLE);
	
	#if defined (RTC_CLOCK_SOURCE_LSI) 
  /* 使用LSI作为RTC时钟源会有误差 
	 * 默认选择LSE作为RTC的时钟源
	 */
  /* 使能LSI */ 
  RCC_LSICmd(ENABLE);
  /* 等待LSI稳定 */  
  while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {
		
  }
  /* 选择LSI做为RTC的时钟源 */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

#elif defined (RTC_CLOCK_SOURCE_LSE)
	
	
  /* 使能LSE外部时钟源,32.768KHz*/ 
  RCC_LSEConfig(RCC_LSE_ON);
	
   /* 等待LSE稳定 */   
  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET){}
	
  /* 选择LSE做为RTC的时钟源 */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);    
#endif

  /* 使能RTC时钟 */
  RCC_RTCCLKCmd(ENABLE);

  /* 等待 RTC APB 寄存器同步 */
  RTC_WaitForSynchro();
   
/*初始化同步/异步预分频器的值,驱动日历的时钟ck_spare = LSE/[(255+1)*(127+1)] = 1HZ*/	
		
	#if defined (RTC_CLOCK_SOURCE_LSI)
	/* 设置异步预分频器的值 */
	RTC_InitStructure.RTC_AsynchPrediv = ASYNCHPREDIV_IN;
	/* 设置同步预分频器的值 */
	RTC_InitStructure.RTC_SynchPrediv = SYNCHPREDIV_IN;	
	#elif defined (RTC_CLOCK_SOURCE_LSE)
	/* 设置异步预分频器的值 */
	RTC_InitStructure.RTC_AsynchPrediv = ASYNCHPREDIV;
	/* 设置同步预分频器的值 */
	RTC_InitStructure.RTC_SynchPrediv = SYNCHPREDIV;	
	#endif

	/* 设置小时制为24小时制 */
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24; 
	
	/* 用RTC_InitStructure的内容初始化RTC寄存器 */	
	RTC_Init(&RTC_InitStructure);		
}


/*
*********************************************************************************************************
*	函 数 名: RTC_CheckRTCReset
*	功能说明: RTC时间重置条件检测，若判断为没有设置过，标记置0，时间设置成初始值
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void RTC_CheckRTCSet(void)
{
  if (RTC_ReadBackupRegister(RTC_BKP_DRX) != RTC_BKP_DATA)
  {
    /* 设置标记位为0，等待上位机时间校准 */
		Flag_RTC_OK=0;
		
		RTC_Time_DefaultValue.RecordTime_Year=19;
		RTC_Time_DefaultValue.RecordTime_Month=04;
		RTC_Time_DefaultValue.RecordTime_Date=01;
		RTC_Time_DefaultValue.RecordTime_Hour=12;
		RTC_Time_DefaultValue.RecordTime_Min=00;
		RTC_Time_DefaultValue.RecordTime_Second=00;
  }
  else
  {
    
    /* 使能 PWR 时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    /* PWR_CR:DBF置1，使能RTC、RTC备份寄存器和备份SRAM的访问 */
    PWR_BackupAccessCmd(ENABLE);
    /* 等待 RTC APB 寄存器同步 */
    RTC_WaitForSynchro();   
		
	/* 设置标记位为1*/
		Flag_RTC_OK=1;
  } 
}

/*
*********************************************************************************************************
*	函 数 名: Record_RTC_Init
*	功能说明: 供主函数调用的RTC时钟初始化函数，初始化时钟配置，判断是否设置过并置位flag
*	形    参: 无
*	返 回 值: RTC是否被设置的标记，0为没设置过，1为设置过
*********************************************************************************************************
*/
uint8_t Record_RTC_Init(void)
{
  RTC_CLK_Config();

  RTC_CheckRTCSet();
	
	return Flag_RTC_OK;
}

/*
*********************************************************************************************************
*	函 数 名: RTC_TimeAndDate_Set
*	功能说明: RTC时钟设置，暂时以宏定义传递参数，后续修改代码以实现上位机修改
*	形    参: RTC_Time_T指针
*	返 回 值: 无
*********************************************************************************************************
*/
void RTC_TimeAndDate_Set(RTC_Time_T *TimeTOSet)
{
	/*RTC时间结构体*/
	RTC_TimeTypeDef RTC_TimeStructure;
	/*RTC日期结构体*/
	RTC_DateTypeDef RTC_DateStructure;
	
	// 初始化时间
	/*RTC时间小时制式，12小时制或24小时制*/
	RTC_TimeStructure.RTC_H12 = RTC_H12_AMorPM;/*该宏在h文件中定义为RTC_H12_AM，即24小时制*/
	RTC_TimeStructure.RTC_Hours = TimeTOSet->RecordTime_Hour;        
	RTC_TimeStructure.RTC_Minutes = TimeTOSet->RecordTime_Min;      
	RTC_TimeStructure.RTC_Seconds = TimeTOSet->RecordTime_Second;
	
	RTC_SetTime(RTC_Format_BINorBCD, &RTC_TimeStructure);
	/*向备份寄存器0写标记数据，表明已经设置过初始时间*/
	RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
	
  // 初始化日期	
	RTC_DateStructure.RTC_WeekDay = 0;   //不使用星期功能    
	RTC_DateStructure.RTC_Date = TimeTOSet->RecordTime_Date;         
	RTC_DateStructure.RTC_Month = TimeTOSet->RecordTime_Month;         
	RTC_DateStructure.RTC_Year = TimeTOSet->RecordTime_Year;        
	RTC_SetDate(RTC_Format_BINorBCD, &RTC_DateStructure);
	RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
	
	//Flag_RTC_OK=1;//判断是否设置仍然以后续check操作为准
}

/*
*********************************************************************************************************
*	函 数 名: RTC_TimeAndDate_Get
*	功能说明: RTC时间获取，无星期,设置过就返回RTC时间，未设置的就返回默认值
*	形    参: RTC_Time_T指针
*	返 回 值: 无
*********************************************************************************************************
*/
void RTC_TimeAndDate_Get(RTC_Time_T *TimeOFGet)
{
  RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;
	
	RTC_CheckRTCSet();

	if(Flag_RTC_OK)
	 {
	  RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	  RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
		 
		TimeOFGet->RecordTime_Year=RTC_DateStructure.RTC_Year;
		TimeOFGet->RecordTime_Month=RTC_DateStructure.RTC_Month;
		TimeOFGet->RecordTime_Date=RTC_DateStructure.RTC_Date;
    TimeOFGet->RecordTime_Hour=RTC_TimeStructure.RTC_Hours;
	  TimeOFGet->RecordTime_Min=RTC_TimeStructure.RTC_Minutes;
		TimeOFGet->RecordTime_Second=RTC_TimeStructure.RTC_Seconds;   		 
	 }
	else
	 {	 
	 *TimeOFGet=RTC_Time_DefaultValue;
	 }
}
