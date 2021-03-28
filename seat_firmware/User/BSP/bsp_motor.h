#ifndef __BSP_MOTOR_H
#define __BSP_MOTOR_H

#include "bsp_spi.h"
#include "bsp_tim.h"
#include "bsp_adc.h"
#include "stm32f4xx_gpio.h"
#include "string.h"
#include "stdbool.h"
#include "stm32f4xx_exti.h"
#include "bsp_led.h"
#define     BrushedDCMotor 
//define    BrushlessDCMotor
#define CHIP_DRV8873 
//#define CHIP_DRV8842 ����оƬ 
#define MotorNum_4   1
#define MotorNum_3   0


#if defined(BrushedDCMotor)

//DRV8873��MCU���������Ӷ���

//���1���Ŷ���
//������ʾ�� PB11
#define Motor1_nFault_PIN              GPIO_Pin_11    
#define Motor1_nFault_GPIO_PORT        GPIOB
#define Motor1_nFault_GPIO_CLK         RCC_AHB1Periph_GPIOB
#define Motor1_nFault_EXTI_PORTSOURCE  EXTI_PortSourceGPIOB
#define Motor1_nFault_EXTI_PINSOURCE   EXTI_PinSource11
#define Motor1_nFault_EXTI_LINE        EXTI_Line11
#define Motor1_nFault_EXTI_IRQ         EXTI15_10_IRQn
#define Motor1_nFault_EXTI_Handler     EXTI15_10_IRQHandler

//ɲ��ʹ�ܽ� PB12
#define Motor1_Brake_PIN               GPIO_Pin_12    
#define Motor1_Brake_GPIO_PORT         GPIOB
#define Motor1_Brake_GPIO_CLK          RCC_AHB1Periph_GPIOB
//������ƽ� PE15
#define Motor1_Direction_PIN           GPIO_Pin_15    
#define Motor1_Direction_GPIO_PORT     GPIOE
#define Motor1_Direction_GPIO_CLK      RCC_AHB1Periph_GPIOE
//оƬ���ܽ� PE12
#define Motor1_Disable_PIN             GPIO_Pin_12    
#define Motor1_Disable_GPIO_PORT       GPIOE
#define Motor1_Disable_GPIO_CLK        RCC_AHB1Periph_GPIOE
//оƬ˯�߽� PE10
#define Motor1_Sleep_PIN               GPIO_Pin_10    
#define Motor1_Sleep_GPIO_PORT         GPIOE
#define Motor1_Sleep_GPIO_CLK          RCC_AHB1Periph_GPIOE
//���������������ļ�
//���1���ٽ�PE9 
//���1������Ӧ��PA4
//���1λ�ø�Ӧ��PA3


//���2���Ŷ���
//������ʾ�� PD9
#define Motor2_nFault_PIN              GPIO_Pin_9    
#define Motor2_nFault_GPIO_PORT        GPIOD
#define Motor2_nFault_GPIO_CLK         RCC_AHB1Periph_GPIOD
#define Motor2_nFault_EXTI_PORTSOURCE  EXTI_PortSourceGPIOD
#define Motor2_nFault_EXTI_PINSOURCE   EXTI_PinSource9
#define Motor2_nFault_EXTI_LINE        EXTI_Line9
#define Motor2_nFault_EXTI_IRQ         EXTI9_5_IRQn
#define Motor2_nFault_EXTI_Handler     EXTI9_5_IRQHandler

//ɲ��ʹ�ܽ� PD10
#define Motor2_Brake_PIN               GPIO_Pin_10    
#define Motor2_Brake_GPIO_PORT         GPIOD
#define Motor2_Brake_GPIO_CLK          RCC_AHB1Periph_GPIOD
//������ƽ� PB15
#define Motor2_Direction_PIN           GPIO_Pin_15    
#define Motor2_Direction_GPIO_PORT     GPIOB
#define Motor2_Direction_GPIO_CLK      RCC_AHB1Periph_GPIOB
//оƬ���ܽ� PB14
#define Motor2_Disable_PIN             GPIO_Pin_14    
#define Motor2_Disable_GPIO_PORT       GPIOB
#define Motor2_Disable_GPIO_CLK        RCC_AHB1Periph_GPIOB
//оƬ˯�߽� PB13
#define Motor2_Sleep_PIN               GPIO_Pin_13    
#define Motor2_Sleep_GPIO_PORT         GPIOB
#define Motor2_Sleep_GPIO_CLK          RCC_AHB1Periph_GPIOB
//���������������ļ�
//���2���ٽ�PE11 
//���2������Ӧ��PA5
//���2λ�ø�Ӧ��PA2


//���3���Ŷ���
//������ʾ�� PD15
#define Motor3_nFault_PIN              GPIO_Pin_15    
#define Motor3_nFault_GPIO_PORT        GPIOD
#define Motor3_nFault_GPIO_CLK         RCC_AHB1Periph_GPIOD
#define Motor3_nFault_EXTI_PORTSOURCE  EXTI_PortSourceGPIOD
#define Motor3_nFault_EXTI_PINSOURCE   EXTI_PinSource15
#define Motor3_nFault_EXTI_LINE        EXTI_Line15
#define Motor3_nFault_EXTI_IRQ         EXTI15_10_IRQn
#define Motor3_nFault_EXTI_Handler     EXTI15_10_IRQHandler

//ɲ��ʹ�ܽ� PC6
#define Motor3_Brake_PIN               GPIO_Pin_6    
#define Motor3_Brake_GPIO_PORT         GPIOC
#define Motor3_Brake_GPIO_CLK          RCC_AHB1Periph_GPIOC
//������ƽ� PD13
#define Motor3_Direction_PIN           GPIO_Pin_13    
#define Motor3_Direction_GPIO_PORT     GPIOD
#define Motor3_Direction_GPIO_CLK      RCC_AHB1Periph_GPIOD
//оƬ���ܽ� PD12
#define Motor3_Disable_PIN             GPIO_Pin_12    
#define Motor3_Disable_GPIO_PORT       GPIOD
#define Motor3_Disable_GPIO_CLK        RCC_AHB1Periph_GPIOD
//оƬ˯�߽� PD11
#define Motor3_Sleep_PIN               GPIO_Pin_11    
#define Motor3_Sleep_GPIO_PORT         GPIOD
#define Motor3_Sleep_GPIO_CLK          RCC_AHB1Periph_GPIOD
//���������������ļ�
//���3���ٽ�PE13 
//���3������Ӧ��PA6
//���3λ�ø�Ӧ��PA1


//���4���Ŷ���
//������ʾ�� PA11
#define Motor4_nFault_PIN              GPIO_Pin_11    
#define Motor4_nFault_GPIO_PORT        GPIOA
#define Motor4_nFault_GPIO_CLK         RCC_AHB1Periph_GPIOA
//���4�Ĵ����PA11��1�ŵ�PB11ʹ����ͬ���жϣ������޸�����

//ɲ��ʹ�ܽ� PA12
#define Motor4_Brake_PIN               GPIO_Pin_12    
#define Motor4_Brake_GPIO_PORT         GPIOA
#define Motor4_Brake_GPIO_CLK          RCC_AHB1Periph_GPIOA
//������ƽ� PC9
#define Motor4_Direction_PIN           GPIO_Pin_9    
#define Motor4_Direction_GPIO_PORT     GPIOC
#define Motor4_Direction_GPIO_CLK      RCC_AHB1Periph_GPIOC
//оƬ���ܽ� PC8
#define Motor4_Disable_PIN             GPIO_Pin_8   
#define Motor4_Disable_GPIO_PORT       GPIOC
#define Motor4_Disable_GPIO_CLK        RCC_AHB1Periph_GPIOC
//оƬ˯�߽� PC7
#define Motor4_Sleep_PIN               GPIO_Pin_7    
#define Motor4_Sleep_GPIO_PORT         GPIOC
#define Motor4_Sleep_GPIO_CLK          RCC_AHB1Periph_GPIOC
//���������������ļ�
//���4���ٽ�PE14
//���4������Ӧ��PA7
//���4λ�ø�Ӧ��PA0

#else
//�����ˢ����Ķ��壬Ԥ��
#endif

//���ͨ������
#define Motor_Channel_1           0
#define Motor_Channel_2           1
#define Motor_Channel_3           2
#define Motor_Channel_4           3
#define Motor_Channel_ALL         4
#define Motor_Channel_123         5

//���������

#define Motor_Direction_Forward   1   //OUT1--->OUT2--->GND
#define Motor_Direction_None      0   //OUT2--->OUT1--->GND
#define Motor_Direction_Reverse  -1   //OUT2--->OUT1--->GND

#define Motor_nSleepCtrl_Sleeping      0
#define Motor_nSleepCtrl_Wakeup      1   

#define Motor_DisableCtrl_Enable       0
#define Motor_DisableCtrl_Disable      1

#define Motor_BrakeCtrl_Released       0
#define Motor_BrakeCtrl_Braked         1


//��������
void MotorInit(void);

void Motor_BrakeRelease(uint8_t ChannelNum);
void Motor_BrakeON(uint8_t ChannelNum);

void Motor_ChangeToStatus_Standby(uint8_t ChannelNum);
void Motor_ChangeToStatus_Active(uint8_t ChannelNum);
void Motor_ChangeToStatus_Manual(uint8_t ChannelNum);

void Motor_Action_Normal(uint8_t MotorNum,int8_t MotorDirection,uint8_t MotorSpeed);
void Motor_Action_Emergency(uint8_t MotorNum);
void Motor_Action_Manual(uint8_t Manual);

void Motor_Error_Handle(uint8_t MotorNum,uint8_t MotorErrorCode);
#endif
