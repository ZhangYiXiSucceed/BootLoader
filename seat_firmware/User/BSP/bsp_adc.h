#ifndef __BSP_ADC_H
#define __BSP_ADC_H

#include "stm32f4xx_adc.h"



//ADC外设、端口、时钟定义
#define MOTOR_ADC_GPIO_PORT       GPIOA  
#define MOTOR_ADC_GPIO_CLK        RCC_AHB1Periph_GPIOA
#define MOTOR_ADC                 ADC1
#define MOTOR_ADC_CLK             RCC_APB2Periph_ADC1

//ADC1数据寄存器地址为ADC1外设基址+0x4c（偏址）
#define MOTOR_ADC_DR_ADDR         ((u32)ADC1+0x4c)

//ADC 通道数定义（位置和电流都是4）
#define MOTOR_ADC_CHANNEL_NUM              4

//ADC转换次数
#define MOTOR_ADC_SAMPLETIME_POTEN         10
#define MOTOR_ADC_SAMPLETIME_CURRENT       100

//PA0-3  电位器4-1位置 PA4-7 电机1-4电流传感
//电机4电位器位置 PA0
#define MOTOR4_Position_ADC                 GPIO_Pin_0                
#define Motor_Pos_CH1                       ADC_Channel_0

//电机3电位器位置 PA1
#define MOTOR3_Position_ADC                 GPIO_Pin_1                
#define Motor_Pos_CH2                       ADC_Channel_1

//电机2电位器位置 PA2
#define MOTOR2_Position_ADC                 GPIO_Pin_2                
#define Motor_Pos_CH3                       ADC_Channel_2

//电机1电位器位置 PA3
#define MOTOR1_Position_ADC                 GPIO_Pin_3                
#define Motor_Pos_CH4                       ADC_Channel_3

//电机电流传感
//电机1电流采样电压 PA4
#define MOTOR1_Current_ADC                  GPIO_Pin_4                
#define Motor_Crt_CH1                       ADC_Channel_4

//电机2电流采样电压 PA5
#define MOTOR2_Current_ADC                  GPIO_Pin_5                
#define Motor_Crt_CH2                       ADC_Channel_5

//电机3电流采样电压 PA6
#define MOTOR3_Current_ADC                  GPIO_Pin_6                
#define Motor_Crt_CH3                       ADC_Channel_6

//电机4电流采样电压 PA7
#define MOTOR4_Current_ADC                  GPIO_Pin_7                
#define Motor_Crt_CH4                       ADC_Channel_7



//DMA相关宏定义
//ADC1使用DMA2，数据流0，通道0
#define MOTOR_ADC_DMA_CLK                   RCC_AHB1Periph_DMA2
#define MOTOR_ADC_DMA_CHANNEL               DMA_Channel_0
#define MOTOR_ADC_DMA_STREAM                DMA2_Stream0 

#define RHEOSTAT_ADC_DR_ADDR                ((u32)ADC1+0x4c)

//SRAM中用于存储DMA发送的ADC数据的数组的全局变量声明，定义在adc.c中


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
