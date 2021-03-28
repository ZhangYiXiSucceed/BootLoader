#include "bsp_adc.h"
#include "string.h"
#include "arm_math.h"
/*
*********************************************************************************************************
*
*	ģ������ : ADCģ��
*	�ļ����� : bsp_i2c.h
*	��    �� : V1.0
*	˵    �� : ��ģ��ʹ��ADC1���е����������λ����ֵ�Ĳɼ���ADC1�����ڶ���ɨ��ģʽ��DMA���䣬����Ϊ2��4ͨ����
             ����ͨ���򲨶��Ƚϴ󣬲ɼ�100�κ�ȡƽ������λ��ͨ��ֻ�ɼ�10�Ρ������õ���ɨ�����룬����ٶ���
						 ������������������Ȼ���л�ADCͨ��
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2019-04-24  WLM      �ݸ�
*
*********************************************************************************************************
*/

//ADC����ȫ�ֱ������壬������Ԫ��Ϊvolatile����,��λ��ֵ�ɼ�10�飬����ֵ�ɼ�100��
__IO uint16_t Motor_ADC_PotenValue[MOTOR_ADC_CHANNEL_NUM*MOTOR_ADC_SAMPLETIME_POTEN]={0};
__IO uint16_t Motor_ADC_CurrentValue[MOTOR_ADC_CHANNEL_NUM*MOTOR_ADC_SAMPLETIME_CURRENT]={0};


int16_t Motor_ADCValueArray_Poten1[MOTOR_ADC_SAMPLETIME_POTEN];
int16_t Motor_ADCValueArray_Poten2[MOTOR_ADC_SAMPLETIME_POTEN];
int16_t Motor_ADCValueArray_Poten3[MOTOR_ADC_SAMPLETIME_POTEN];
int16_t Motor_ADCValueArray_Poten4[MOTOR_ADC_SAMPLETIME_POTEN];

int16_t Motor_ADCValueArray_Current1[MOTOR_ADC_SAMPLETIME_CURRENT];
int16_t Motor_ADCValueArray_Current2[MOTOR_ADC_SAMPLETIME_CURRENT];
int16_t Motor_ADCValueArray_Current3[MOTOR_ADC_SAMPLETIME_CURRENT];
int16_t Motor_ADCValueArray_Current4[MOTOR_ADC_SAMPLETIME_CURRENT];
/*
*********************************************************************************************************
*	�� �� ��: Motor_ADC_GPIO_Config
*	����˵��: ���������λ�ô������õ�8��ADC���ŵ����ų�ʼ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void Motor_ADC_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	
	// ʹ�� GPIO ʱ��
	RCC_AHB1PeriphClockCmd( MOTOR_ADC_GPIO_CLK,ENABLE);		
	
	// ��Ҫ���õ�GPIOΪPA0-7
  GPIO_InitStructure.GPIO_Pin = MOTOR1_Position_ADC |
	                              MOTOR2_Position_ADC |
																MOTOR3_Position_ADC |
																MOTOR4_Position_ADC |
																MOTOR1_Current_ADC  |
																MOTOR2_Current_ADC  |
																MOTOR3_Current_ADC  |
																MOTOR4_Current_ADC  ;
  
	// ģʽ����Ϊģ������AIN
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
 
	// ����AINʱ������Ҳ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	
	//��ʼ��GPIO
	GPIO_Init(MOTOR_ADC_GPIO_PORT, &GPIO_InitStructure);
}

/*
*********************************************************************************************************
*	�� �� ��: Motor_ADC_Mode_Config
*	����˵��: ADCģʽ��ʼ��������ΪADC1������ 4ͨ����DMAģʽ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void Motor_ADC_Mode_Config(void)
{
	//ADC��ʼ���ṹ��
	ADC_InitTypeDef ADC_InitStructure;
	//ADCͨ�ó�ʼ���ṹ��
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
	
		// ����ADCʱ��
	RCC_APB2PeriphClockCmd(MOTOR_ADC_CLK  , ENABLE);
	// ����DMAʱ��
  RCC_AHB1PeriphClockCmd(MOTOR_ADC_DMA_CLK , ENABLE);
	
  // -------------------ADC Common �ṹ�� ���� ��ʼ��------------------------
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);
	// ����ADCģʽ
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  // ʱ��Ϊfpclk 4��Ƶ	ADC_CLK=22.5MHz
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
  // ��ֹDMAֱ�ӷ���ģʽ������ģʽ�²���Ҫ��ֻ��˫�ؼ�����ʱ����Ҫ	
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  // ����ʱ����������ģʽ�²�Ӱ�죬ֻ��˫�ؼ�����ʱ���й�ϵ
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_10Cycles;  
  ADC_CommonInit(&ADC_CommonInitStructure);
	
  // -------------------ADC Init �ṹ�� ���� ��ʼ��--------------------------
  // ADC 12Bit�ֱ���
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  // ɨ��ģʽ����ͨ���ɼ���Ҫ	
  ADC_InitStructure.ADC_ScanConvMode = ENABLE; 
  // ����ת��	
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
  //��ֹ�ⲿ���ش���
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  //�ⲿ����ͨ����������ʹ���������������ֵ��ֵ=((uint32_t)0x00000000)
	//�˴����븳ֵ�������ʼֵ0xFF�ᵼ��ADC��������ת��ģʽ
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  //�����Ҷ���	
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  //ת��ͨ�� 8��
  ADC_InitStructure.ADC_NbrOfConversion = MOTOR_ADC_CHANNEL_NUM ;                                    
  ADC_Init(MOTOR_ADC, &ADC_InitStructure);
}

/*
*********************************************************************************************************
*	�� �� ��: Motor_DMAConfig_PotenMode
*	����˵��: ����DMA��������40����λ��ADCֵ���䵽��λ������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Motor_DMAConfig_PotenMode(void)
{
	//DMA��ʼ���ṹ��
	DMA_InitTypeDef DMA_InitStructure;
		
	//��λDMA����
	DMA_DeInit(MOTOR_ADC_DMA_STREAM);	 
	// ѡ�� DMA ͨ��
  DMA_InitStructure.DMA_Channel = MOTOR_ADC_DMA_CHANNEL ; 	
	// DMA�����ַΪADC1���ݼĴ�����ַ
	DMA_InitStructure.DMA_PeripheralBaseAddr = MOTOR_ADC_DR_ADDR;	
  // DMAĿ���ַΪ�����ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)Motor_ADC_PotenValue;  
  // ���ݴ��䷽��Ϊ���赽�洢��	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	
	// ��������С,4*10 uint16
	DMA_InitStructure.DMA_BufferSize = MOTOR_ADC_CHANNEL_NUM*MOTOR_ADC_SAMPLETIME_POTEN;	
	// ����Ĵ���ֻ��һ������ַ������
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  // �洢����ַ��Ҫ����
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
  // // �������ݴ�СΪ���֣��������ֽ� 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
  //	�洢�����ݴ�СҲΪ���֣����������ݴ�С��ͬ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	
	
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
 	  // DMA ����ͨ�����ȼ�Ϊ�ߣ���ʹ��һ��DMAͨ��ʱ�����ȼ����ò�Ӱ��
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  // ��ֹDMA FIFO	��ʹ��ֱ��ģʽ
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;  
  // FIFOģʽ��ֹʱ���������ò�Ӱ��	
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  

  //��ʼ��DMA��
	DMA_Init(MOTOR_ADC_DMA_STREAM, &DMA_InitStructure);
	// ʹ��DMA��
  DMA_Cmd(MOTOR_ADC_DMA_STREAM, ENABLE);
}	
/*
*********************************************************************************************************
*	�� �� ��: Motor_DMAConfig_CurrentMode
*	����˵��: ����DMA��������400����λ��ADCֵ���䵽��������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
 void Motor_DMAConfig_CurrentMode(void)
{
	//DMA��ʼ���ṹ��
	DMA_InitTypeDef DMA_InitStructure;
		
	//��λDMA����
	DMA_DeInit(MOTOR_ADC_DMA_STREAM);	 
	// ѡ�� DMA ͨ��
  DMA_InitStructure.DMA_Channel = MOTOR_ADC_DMA_CHANNEL ; 	
	// DMA�����ַΪADC1���ݼĴ�����ַ
	DMA_InitStructure.DMA_PeripheralBaseAddr = MOTOR_ADC_DR_ADDR;	
  // DMAĿ���ַΪ�����ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)Motor_ADC_CurrentValue;  
  // ���ݴ��䷽��Ϊ���赽�洢��	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	
	// ��������С,4*100 uint16
	DMA_InitStructure.DMA_BufferSize = MOTOR_ADC_CHANNEL_NUM*MOTOR_ADC_SAMPLETIME_CURRENT;	
	// ����Ĵ���ֻ��һ������ַ������
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  // �洢����ַ��Ҫ����
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
  // // �������ݴ�СΪ���֣��������ֽ� 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
  //	�洢�����ݴ�СҲΪ���֣����������ݴ�С��ͬ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	
	
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
 	  // DMA ����ͨ�����ȼ�Ϊ�ߣ���ʹ��һ��DMAͨ��ʱ�����ȼ����ò�Ӱ��
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  // ��ֹDMA FIFO	��ʹ��ֱ��ģʽ
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;  
  // FIFOģʽ��ֹʱ���������ò�Ӱ��	
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  

  //��ʼ��DMA��
	DMA_Init(MOTOR_ADC_DMA_STREAM, &DMA_InitStructure);
	// ʹ��DMA��
  DMA_Cmd(MOTOR_ADC_DMA_STREAM, ENABLE);
}	
/*
*********************************************************************************************************
*	�� �� ��: Motor_ADC_SetPotenChannel
*	����˵��: ����ADC1��4ͨ��Ϊ4����λ��ͨ����ʹ��DMA����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Motor_ADC_SetPotenChannel(void)
{
  /*����DMAͨ������Ϊ��λ��ģʽ*/ 
	Motor_DMAConfig_PotenMode();
	
	// ���� ADC ͨ��ת��˳��Ͳ���ʱ�����ڣ�4ͨ����λ��λ�ã���λ��1->4��
	ADC_RegularChannelConfig(MOTOR_ADC, Motor_Pos_CH4, 1,  ADC_SampleTime_56Cycles);//PA3
	ADC_RegularChannelConfig(MOTOR_ADC, Motor_Pos_CH3, 2,  ADC_SampleTime_56Cycles);//PA2
	ADC_RegularChannelConfig(MOTOR_ADC, Motor_Pos_CH2, 3,  ADC_SampleTime_56Cycles);//PA1
	ADC_RegularChannelConfig(MOTOR_ADC, Motor_Pos_CH1, 4,  ADC_SampleTime_56Cycles);//PA0
	
  // ��λ��1��ֻҪ��������ת����ʹ����ADC DMA���ͻᷢ��DMA����
  ADC_DMARequestAfterLastTransferCmd(MOTOR_ADC, ENABLE);
  // ʹ��ADC DMA
  ADC_DMACmd(MOTOR_ADC, ENABLE); 
}

/*
*********************************************************************************************************
*	�� �� ��: Motor_ADC_SetCurrentChannel
*	����˵��: ADCģʽ��ʼ��������ΪADC1������ 4ͨ����DMAģʽ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Motor_ADC_SetCurrentChannel(void)
{
  /*����DMAͨ������Ϊ��λ��ģʽ*/ 
	Motor_DMAConfig_CurrentMode();
	
	// ���� ADC ͨ��ת��˳��Ͳ���ʱ�����ڣ�4ͨ�����������1->4��
  ADC_RegularChannelConfig(MOTOR_ADC, Motor_Crt_CH1, 1,  ADC_SampleTime_56Cycles);//PA4
	ADC_RegularChannelConfig(MOTOR_ADC, Motor_Crt_CH2, 2,  ADC_SampleTime_56Cycles);//PA5                       
  ADC_RegularChannelConfig(MOTOR_ADC, Motor_Crt_CH3, 3,  ADC_SampleTime_56Cycles);//PA6
  ADC_RegularChannelConfig(MOTOR_ADC, Motor_Crt_CH4, 4,  ADC_SampleTime_56Cycles);//PA7
	
  // ��λ��1��ֻҪ��������ת����ʹ����ADC DMA���ͻᷢ��DMA����
  ADC_DMARequestAfterLastTransferCmd(MOTOR_ADC, ENABLE);
  // ʹ��ADC DMA
  ADC_DMACmd(MOTOR_ADC, ENABLE); 
}



/*
*********************************************************************************************************
*	�� �� ��: Motor_ADC_Init
*	����˵��: ���������λ��ʹ��ADC����ǰ�ĳ�ʼ������������ADC��ʼ�������ų�ʼ����DMA��ʼ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Motor_ADC_Init(void)
{
		
	//ADC���ų�ʼ��
	Motor_ADC_GPIO_Config();
	
	//ADCģʽ��ʼ��
	Motor_ADC_Mode_Config();
	
	//ADC�����ʼ��
	memset((void*)&Motor_ADC_PotenValue[0],0,40*sizeof(uint16_t));
	memset((void*)&Motor_ADC_CurrentValue[0],0,400*sizeof(uint16_t));
}

/*
*********************************************************************************************************
*	�� �� ��: Motor_ADC_Start
*	����˵��: ��ʼADCת��������ʹ��ADC��Ȼ��ʹ���������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Motor_ADC_Start(void)
{
	// ʹ��ADC
  ADC_Cmd(MOTOR_ADC, ENABLE); 
	DMA_Cmd(MOTOR_ADC_DMA_STREAM, ENABLE);
	//��ʼadcת�����������
	ADC_SoftwareStartConv(MOTOR_ADC);
  
}

/*
*********************************************************************************************************
*	�� �� ��: Motor_ADC_Stop
*	����˵��: ֹͣADCת����ֱ�ӹر�ADC
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Motor_ADC_Stop(void)
{
	ADC_Cmd(MOTOR_ADC, DISABLE); 
}

/*
*********************************************************************************************************
*	�� �� ��: Motor_GetPotenValue
*	����˵��: ��ȡ��λ��ADC��ֵ��������ADCͨ��Ϊ4����λ��ͨ��������ADC��ת����ɺ�ر�ADC
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Motor_GetPotenValue(void)
{
	Motor_ADC_SetPotenChannel();
	Motor_ADC_Start();
	
	while(DMA_GetFlagStatus(MOTOR_ADC_DMA_STREAM,DMA_FLAG_TCIF0) == RESET);
	
	Motor_ADC_Stop();
}

/*
*********************************************************************************************************
*	�� �� ��: Motor_GetCurrentValue
*	����˵��: ��ȡ����ADC��ֵ��������ADCͨ��Ϊ4������ͨ��������ADC��ת����ɺ�ر�ADC
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Motor_GetCurrentValue(void)
{
	Motor_ADC_SetCurrentChannel();
	Motor_ADC_Start();
	
	while(DMA_GetFlagStatus(MOTOR_ADC_DMA_STREAM,DMA_FLAG_TCIF0) == RESET);
	
	Motor_ADC_Stop();
}

/*
*********************************************************************************************************
*	�� �� ��: Motor_ADCValueAverage
*	����˵��: �����������λ��ADC��ֵƽ��ֵ������DSPʵ��
*	��    ��: AverageValuePoten �洢4ͨ����λ��ֵ������
            AverageValueCurrent �洢4ͨ������ֵ������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Motor_ADCValueAverage(uint16_t* AverageValuePoten,uint16_t* AverageValueCurrent)
{
  uint16_t index;
	
	/*����λ������ֵת��4����λ����ƽ��ֵ������*/
	for(index=0;index<MOTOR_ADC_SAMPLETIME_POTEN;index++)
	{
	   Motor_ADCValueArray_Poten1[index]=Motor_ADC_PotenValue[MOTOR_ADC_CHANNEL_NUM*index];
     Motor_ADCValueArray_Poten2[index]=Motor_ADC_PotenValue[MOTOR_ADC_CHANNEL_NUM*index+1];
     Motor_ADCValueArray_Poten3[index]=Motor_ADC_PotenValue[MOTOR_ADC_CHANNEL_NUM*index+2];
     Motor_ADCValueArray_Poten4[index]=Motor_ADC_PotenValue[MOTOR_ADC_CHANNEL_NUM*index+3];	   
	}
	
		/*����������ֵת��4����λ����ƽ��ֵ������*/
	for(index=0;index<MOTOR_ADC_SAMPLETIME_CURRENT;index++)
	{
	   Motor_ADCValueArray_Current1[index]=Motor_ADC_CurrentValue[MOTOR_ADC_CHANNEL_NUM*index];
     Motor_ADCValueArray_Current2[index]=Motor_ADC_CurrentValue[MOTOR_ADC_CHANNEL_NUM*index+1];
     Motor_ADCValueArray_Current3[index]=Motor_ADC_CurrentValue[MOTOR_ADC_CHANNEL_NUM*index+2];
     Motor_ADCValueArray_Current4[index]=Motor_ADC_CurrentValue[MOTOR_ADC_CHANNEL_NUM*index+3];	   
	}

	 /*ʹ��DSP�����ƽ��ֵ*/
	arm_mean_q15(Motor_ADCValueArray_Poten1,MOTOR_ADC_SAMPLETIME_POTEN,(int16_t*)AverageValuePoten);
	arm_mean_q15(Motor_ADCValueArray_Poten2,MOTOR_ADC_SAMPLETIME_POTEN,(int16_t*)AverageValuePoten+1);
	arm_mean_q15(Motor_ADCValueArray_Poten3,MOTOR_ADC_SAMPLETIME_POTEN,(int16_t*)AverageValuePoten+2);
	arm_mean_q15(Motor_ADCValueArray_Poten4,MOTOR_ADC_SAMPLETIME_POTEN,(int16_t*)AverageValuePoten+3);
	
	arm_mean_q15(Motor_ADCValueArray_Current1,MOTOR_ADC_SAMPLETIME_CURRENT,(int16_t*)AverageValueCurrent);
	arm_mean_q15(Motor_ADCValueArray_Current2,MOTOR_ADC_SAMPLETIME_CURRENT,(int16_t*)AverageValueCurrent+1);
	arm_mean_q15(Motor_ADCValueArray_Current3,MOTOR_ADC_SAMPLETIME_CURRENT,(int16_t*)AverageValueCurrent+2);
	arm_mean_q15(Motor_ADCValueArray_Current4,MOTOR_ADC_SAMPLETIME_CURRENT,(int16_t*)AverageValueCurrent+3);
}

/*
*********************************************************************************************************
*	�� �� ��: Motor_GetPositionCurrentData
*	����˵��: һ��������ADC��������ƽ������
*	��    ��: PotenValueArray ���ڴ洢4ͨ����λ��ֵ������
            CurrentValueArray ���ڴ洢4ͨ������ֵ������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Motor_GetPositionCurrentData(uint16_t* PotenValueArray,uint16_t* CurrentValueArray)
{
  Motor_GetPotenValue();
	
	Motor_GetCurrentValue();
	
	Motor_ADCValueAverage(PotenValueArray,CurrentValueArray);
}
