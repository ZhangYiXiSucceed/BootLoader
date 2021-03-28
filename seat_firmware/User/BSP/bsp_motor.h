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
//#define CHIP_DRV8842 其他芯片 
#define MotorNum_4   1
#define MotorNum_3   0


#if defined(BrushedDCMotor)

//DRV8873与MCU的引脚连接定义

//电机1引脚定义
//错误提示脚 PB11
#define Motor1_nFault_PIN              GPIO_Pin_11    
#define Motor1_nFault_GPIO_PORT        GPIOB
#define Motor1_nFault_GPIO_CLK         RCC_AHB1Periph_GPIOB
#define Motor1_nFault_EXTI_PORTSOURCE  EXTI_PortSourceGPIOB
#define Motor1_nFault_EXTI_PINSOURCE   EXTI_PinSource11
#define Motor1_nFault_EXTI_LINE        EXTI_Line11
#define Motor1_nFault_EXTI_IRQ         EXTI15_10_IRQn
#define Motor1_nFault_EXTI_Handler     EXTI15_10_IRQHandler

//刹车使能脚 PB12
#define Motor1_Brake_PIN               GPIO_Pin_12    
#define Motor1_Brake_GPIO_PORT         GPIOB
#define Motor1_Brake_GPIO_CLK          RCC_AHB1Periph_GPIOB
//方向控制脚 PE15
#define Motor1_Direction_PIN           GPIO_Pin_15    
#define Motor1_Direction_GPIO_PORT     GPIOE
#define Motor1_Direction_GPIO_CLK      RCC_AHB1Periph_GPIOE
//芯片禁能脚 PE12
#define Motor1_Disable_PIN             GPIO_Pin_12    
#define Motor1_Disable_GPIO_PORT       GPIOE
#define Motor1_Disable_GPIO_CLK        RCC_AHB1Periph_GPIOE
//芯片睡眠脚 PE10
#define Motor1_Sleep_PIN               GPIO_Pin_10    
#define Motor1_Sleep_GPIO_PORT         GPIOE
#define Motor1_Sleep_GPIO_CLK          RCC_AHB1Periph_GPIOE
//以下引脚在其他文件
//电机1调速脚PE9 
//电机1电流感应脚PA4
//电机1位置感应脚PA3


//电机2引脚定义
//错误提示脚 PD9
#define Motor2_nFault_PIN              GPIO_Pin_9    
#define Motor2_nFault_GPIO_PORT        GPIOD
#define Motor2_nFault_GPIO_CLK         RCC_AHB1Periph_GPIOD
#define Motor2_nFault_EXTI_PORTSOURCE  EXTI_PortSourceGPIOD
#define Motor2_nFault_EXTI_PINSOURCE   EXTI_PinSource9
#define Motor2_nFault_EXTI_LINE        EXTI_Line9
#define Motor2_nFault_EXTI_IRQ         EXTI9_5_IRQn
#define Motor2_nFault_EXTI_Handler     EXTI9_5_IRQHandler

//刹车使能脚 PD10
#define Motor2_Brake_PIN               GPIO_Pin_10    
#define Motor2_Brake_GPIO_PORT         GPIOD
#define Motor2_Brake_GPIO_CLK          RCC_AHB1Periph_GPIOD
//方向控制脚 PB15
#define Motor2_Direction_PIN           GPIO_Pin_15    
#define Motor2_Direction_GPIO_PORT     GPIOB
#define Motor2_Direction_GPIO_CLK      RCC_AHB1Periph_GPIOB
//芯片禁能脚 PB14
#define Motor2_Disable_PIN             GPIO_Pin_14    
#define Motor2_Disable_GPIO_PORT       GPIOB
#define Motor2_Disable_GPIO_CLK        RCC_AHB1Periph_GPIOB
//芯片睡眠脚 PB13
#define Motor2_Sleep_PIN               GPIO_Pin_13    
#define Motor2_Sleep_GPIO_PORT         GPIOB
#define Motor2_Sleep_GPIO_CLK          RCC_AHB1Periph_GPIOB
//以下引脚在其他文件
//电机2调速脚PE11 
//电机2电流感应脚PA5
//电机2位置感应脚PA2


//电机3引脚定义
//错误提示脚 PD15
#define Motor3_nFault_PIN              GPIO_Pin_15    
#define Motor3_nFault_GPIO_PORT        GPIOD
#define Motor3_nFault_GPIO_CLK         RCC_AHB1Periph_GPIOD
#define Motor3_nFault_EXTI_PORTSOURCE  EXTI_PortSourceGPIOD
#define Motor3_nFault_EXTI_PINSOURCE   EXTI_PinSource15
#define Motor3_nFault_EXTI_LINE        EXTI_Line15
#define Motor3_nFault_EXTI_IRQ         EXTI15_10_IRQn
#define Motor3_nFault_EXTI_Handler     EXTI15_10_IRQHandler

//刹车使能脚 PC6
#define Motor3_Brake_PIN               GPIO_Pin_6    
#define Motor3_Brake_GPIO_PORT         GPIOC
#define Motor3_Brake_GPIO_CLK          RCC_AHB1Periph_GPIOC
//方向控制脚 PD13
#define Motor3_Direction_PIN           GPIO_Pin_13    
#define Motor3_Direction_GPIO_PORT     GPIOD
#define Motor3_Direction_GPIO_CLK      RCC_AHB1Periph_GPIOD
//芯片禁能脚 PD12
#define Motor3_Disable_PIN             GPIO_Pin_12    
#define Motor3_Disable_GPIO_PORT       GPIOD
#define Motor3_Disable_GPIO_CLK        RCC_AHB1Periph_GPIOD
//芯片睡眠脚 PD11
#define Motor3_Sleep_PIN               GPIO_Pin_11    
#define Motor3_Sleep_GPIO_PORT         GPIOD
#define Motor3_Sleep_GPIO_CLK          RCC_AHB1Periph_GPIOD
//以下引脚在其他文件
//电机3调速脚PE13 
//电机3电流感应脚PA6
//电机3位置感应脚PA1


//电机4引脚定义
//错误提示脚 PA11
#define Motor4_nFault_PIN              GPIO_Pin_11    
#define Motor4_nFault_GPIO_PORT        GPIOA
#define Motor4_nFault_GPIO_CLK         RCC_AHB1Periph_GPIOA
//电机4的错误脚PA11与1号的PB11使用了同组中断，后续修改引脚

//刹车使能脚 PA12
#define Motor4_Brake_PIN               GPIO_Pin_12    
#define Motor4_Brake_GPIO_PORT         GPIOA
#define Motor4_Brake_GPIO_CLK          RCC_AHB1Periph_GPIOA
//方向控制脚 PC9
#define Motor4_Direction_PIN           GPIO_Pin_9    
#define Motor4_Direction_GPIO_PORT     GPIOC
#define Motor4_Direction_GPIO_CLK      RCC_AHB1Periph_GPIOC
//芯片禁能脚 PC8
#define Motor4_Disable_PIN             GPIO_Pin_8   
#define Motor4_Disable_GPIO_PORT       GPIOC
#define Motor4_Disable_GPIO_CLK        RCC_AHB1Periph_GPIOC
//芯片睡眠脚 PC7
#define Motor4_Sleep_PIN               GPIO_Pin_7    
#define Motor4_Sleep_GPIO_PORT         GPIOC
#define Motor4_Sleep_GPIO_CLK          RCC_AHB1Periph_GPIOC
//以下引脚在其他文件
//电机4调速脚PE14
//电机4电流感应脚PA7
//电机4位置感应脚PA0

#else
//针对无刷电机的定义，预留
#endif

//电机通道定义
#define Motor_Channel_1           0
#define Motor_Channel_2           1
#define Motor_Channel_3           2
#define Motor_Channel_4           3
#define Motor_Channel_ALL         4
#define Motor_Channel_123         5

//电机方向定义

#define Motor_Direction_Forward   1   //OUT1--->OUT2--->GND
#define Motor_Direction_None      0   //OUT2--->OUT1--->GND
#define Motor_Direction_Reverse  -1   //OUT2--->OUT1--->GND

#define Motor_nSleepCtrl_Sleeping      0
#define Motor_nSleepCtrl_Wakeup      1   

#define Motor_DisableCtrl_Enable       0
#define Motor_DisableCtrl_Disable      1

#define Motor_BrakeCtrl_Released       0
#define Motor_BrakeCtrl_Braked         1


//函数定义
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
