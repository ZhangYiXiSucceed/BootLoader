#include "bsp_rtc.h"

/*
*********************************************************************************************************
*
*	ģ������ : RTCģ��(Ƭ��RTC,LSE)
*	�ļ����� : bsp_rtc.c
*	��    �� : V1.0
*	˵    �� : ��Ҫʵ���ж�RTC�Ƿ����ù�������RTC�����á���ȡRTCʱ�书��
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2019-04-26  WLM      �ݸ�
*
*********************************************************************************************************
*/

/*RTC��ʼ��ʧ�ܻ�δ�����趨ʱ���ص�Ĭ��ʱ��*/
RTC_Time_T RTC_Time_DefaultValue;
/*RTC��ʼ���ɹ���ǣ�1Ϊ���趨����0Ϊδ�趨*/
uint8_t Flag_RTC_OK=0;

/*
*********************************************************************************************************
*	�� �� ��: RTC_CLK_Config
*	����˵��: RTCʱ�ӳ�ʼ��������Ϊ�ⲿ��������������ʱ���ʽ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RTC_CLK_Config(void)
{  
	RTC_InitTypeDef RTC_InitStructure;

	
	/*ʹ�� PWR ʱ��*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	
  /* PWR_CR:DBF��1��ʹ��RTC��RTC���ݼĴ����ͱ���SRAM�ķ��� */
  PWR_BackupAccessCmd(ENABLE);
	
	#if defined (RTC_CLOCK_SOURCE_LSI) 
  /* ʹ��LSI��ΪRTCʱ��Դ������� 
	 * Ĭ��ѡ��LSE��ΪRTC��ʱ��Դ
	 */
  /* ʹ��LSI */ 
  RCC_LSICmd(ENABLE);
  /* �ȴ�LSI�ȶ� */  
  while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {
		
  }
  /* ѡ��LSI��ΪRTC��ʱ��Դ */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

#elif defined (RTC_CLOCK_SOURCE_LSE)
	
	
  /* ʹ��LSE�ⲿʱ��Դ,32.768KHz*/ 
  RCC_LSEConfig(RCC_LSE_ON);
	
   /* �ȴ�LSE�ȶ� */   
  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET){}
	
  /* ѡ��LSE��ΪRTC��ʱ��Դ */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);    
#endif

  /* ʹ��RTCʱ�� */
  RCC_RTCCLKCmd(ENABLE);

  /* �ȴ� RTC APB �Ĵ���ͬ�� */
  RTC_WaitForSynchro();
   
/*��ʼ��ͬ��/�첽Ԥ��Ƶ����ֵ,����������ʱ��ck_spare = LSE/[(255+1)*(127+1)] = 1HZ*/	
		
	#if defined (RTC_CLOCK_SOURCE_LSI)
	/* �����첽Ԥ��Ƶ����ֵ */
	RTC_InitStructure.RTC_AsynchPrediv = ASYNCHPREDIV_IN;
	/* ����ͬ��Ԥ��Ƶ����ֵ */
	RTC_InitStructure.RTC_SynchPrediv = SYNCHPREDIV_IN;	
	#elif defined (RTC_CLOCK_SOURCE_LSE)
	/* �����첽Ԥ��Ƶ����ֵ */
	RTC_InitStructure.RTC_AsynchPrediv = ASYNCHPREDIV;
	/* ����ͬ��Ԥ��Ƶ����ֵ */
	RTC_InitStructure.RTC_SynchPrediv = SYNCHPREDIV;	
	#endif

	/* ����Сʱ��Ϊ24Сʱ�� */
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24; 
	
	/* ��RTC_InitStructure�����ݳ�ʼ��RTC�Ĵ��� */	
	RTC_Init(&RTC_InitStructure);		
}


/*
*********************************************************************************************************
*	�� �� ��: RTC_CheckRTCReset
*	����˵��: RTCʱ������������⣬���ж�Ϊû�����ù��������0��ʱ�����óɳ�ʼֵ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RTC_CheckRTCSet(void)
{
  if (RTC_ReadBackupRegister(RTC_BKP_DRX) != RTC_BKP_DATA)
  {
    /* ���ñ��λΪ0���ȴ���λ��ʱ��У׼ */
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
    
    /* ʹ�� PWR ʱ�� */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    /* PWR_CR:DBF��1��ʹ��RTC��RTC���ݼĴ����ͱ���SRAM�ķ��� */
    PWR_BackupAccessCmd(ENABLE);
    /* �ȴ� RTC APB �Ĵ���ͬ�� */
    RTC_WaitForSynchro();   
		
	/* ���ñ��λΪ1*/
		Flag_RTC_OK=1;
  } 
}

/*
*********************************************************************************************************
*	�� �� ��: Record_RTC_Init
*	����˵��: �����������õ�RTCʱ�ӳ�ʼ����������ʼ��ʱ�����ã��ж��Ƿ����ù�����λflag
*	��    ��: ��
*	�� �� ֵ: RTC�Ƿ����õı�ǣ�0Ϊû���ù���1Ϊ���ù�
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
*	�� �� ��: RTC_TimeAndDate_Set
*	����˵��: RTCʱ�����ã���ʱ�Ժ궨�崫�ݲ����������޸Ĵ�����ʵ����λ���޸�
*	��    ��: RTC_Time_Tָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RTC_TimeAndDate_Set(RTC_Time_T *TimeTOSet)
{
	/*RTCʱ��ṹ��*/
	RTC_TimeTypeDef RTC_TimeStructure;
	/*RTC���ڽṹ��*/
	RTC_DateTypeDef RTC_DateStructure;
	
	// ��ʼ��ʱ��
	/*RTCʱ��Сʱ��ʽ��12Сʱ�ƻ�24Сʱ��*/
	RTC_TimeStructure.RTC_H12 = RTC_H12_AMorPM;/*�ú���h�ļ��ж���ΪRTC_H12_AM����24Сʱ��*/
	RTC_TimeStructure.RTC_Hours = TimeTOSet->RecordTime_Hour;        
	RTC_TimeStructure.RTC_Minutes = TimeTOSet->RecordTime_Min;      
	RTC_TimeStructure.RTC_Seconds = TimeTOSet->RecordTime_Second;
	
	RTC_SetTime(RTC_Format_BINorBCD, &RTC_TimeStructure);
	/*�򱸷ݼĴ���0д������ݣ������Ѿ����ù���ʼʱ��*/
	RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
	
  // ��ʼ������	
	RTC_DateStructure.RTC_WeekDay = 0;   //��ʹ�����ڹ���    
	RTC_DateStructure.RTC_Date = TimeTOSet->RecordTime_Date;         
	RTC_DateStructure.RTC_Month = TimeTOSet->RecordTime_Month;         
	RTC_DateStructure.RTC_Year = TimeTOSet->RecordTime_Year;        
	RTC_SetDate(RTC_Format_BINorBCD, &RTC_DateStructure);
	RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
	
	//Flag_RTC_OK=1;//�ж��Ƿ�������Ȼ�Ժ���check����Ϊ׼
}

/*
*********************************************************************************************************
*	�� �� ��: RTC_TimeAndDate_Get
*	����˵��: RTCʱ���ȡ��������,���ù��ͷ���RTCʱ�䣬δ���õľͷ���Ĭ��ֵ
*	��    ��: RTC_Time_Tָ��
*	�� �� ֵ: ��
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
