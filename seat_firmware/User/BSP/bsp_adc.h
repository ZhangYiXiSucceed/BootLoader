#ifndef __BSP_ADC_H
#define __BSP_ADC_H

#include "stm32f4xx_adc.h"



//ADC���衢�˿ڡ�ʱ�Ӷ���
#define MOTOR_ADC_GPIO_PORT       GPIOA  
#define MOTOR_ADC_GPIO_CLK        RCC_AHB1Periph_GPIOA
#define MOTOR_ADC                 ADC1
#define MOTOR_ADC_CLK             RCC_APB2Periph_ADC1

//ADC1���ݼĴ�����ַΪADC1�����ַ+0x4c��ƫַ��
#define MOTOR_ADC_DR_ADDR         ((u32)ADC1+0x4c)

//ADC ͨ�������壨λ�ú͵�������4��
#define MOTOR_ADC_CHANNEL_NUM              4

//ADCת������
#define MOTOR_ADC_SAMPLETIME_POTEN         10
#define MOTOR_ADC_SAMPLETIME_CURRENT       100

//PA0-3  ��λ��4-1λ�� PA4-7 ���1-4��������
//���4��λ��λ�� PA0
#define MOTOR4_Position_ADC                 GPIO_Pin_0                
#define Motor_Pos_CH1                       ADC_Channel_0

//���3��λ��λ�� PA1
#define MOTOR3_Position_ADC                 GPIO_Pin_1                
#define Motor_Pos_CH2                       ADC_Channel_1

//���2��λ��λ�� PA2
#define MOTOR2_Position_ADC                 GPIO_Pin_2                
#define Motor_Pos_CH3                       ADC_Channel_2

//���1��λ��λ�� PA3
#define MOTOR1_Position_ADC                 GPIO_Pin_3                
#define Motor_Pos_CH4                       ADC_Channel_3

//�����������
//���1����������ѹ PA4
#define MOTOR1_Current_ADC                  GPIO_Pin_4                
#define Motor_Crt_CH1                       ADC_Channel_4

//���2����������ѹ PA5
#define MOTOR2_Current_ADC                  GPIO_Pin_5                
#define Motor_Crt_CH2                       ADC_Channel_5

//���3����������ѹ PA6
#define MOTOR3_Current_ADC                  GPIO_Pin_6                
#define Motor_Crt_CH3                       ADC_Channel_6

//���4����������ѹ PA7
#define MOTOR4_Current_ADC                  GPIO_Pin_7                
#define Motor_Crt_CH4                       ADC_Channel_7



//DMA��غ궨��
//ADC1ʹ��DMA2��������0��ͨ��0
#define MOTOR_ADC_DMA_CLK                   RCC_AHB1Periph_DMA2
#define MOTOR_ADC_DMA_CHANNEL               DMA_Channel_0
#define MOTOR_ADC_DMA_STREAM                DMA2_Stream0 

#define RHEOSTAT_ADC_DR_ADDR                ((u32)ADC1+0x4c)

//SRAM�����ڴ洢DMA���͵�ADC���ݵ������ȫ�ֱ���������������adc.c��


void Motor_ADC_Init(void);
void Motor_DMAConfig_PotenMode(void);
void Motor_DMAConfig_CurrentMode(void);
void Motor_ADC_Start(void);
void Motor_ADC_Stop(void);
void Motor_GetPotenValue(void);
void Motor_GetCurrentValue(void);
void Motor_ADCValueAverage(uint16_t* AverageValuePoten,uint16_t* AverageValueCurrent);
void Motor_GetPositionCurrentData(uint16_t* PotenValueArray,uint16_t* CurrentValueArray);

#endif
