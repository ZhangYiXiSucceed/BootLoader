#ifndef __BSP_TIM_H
#define __BSP_TIM_H

#include "stm32f4xx_tim.h"

//DRV8873所接的PWM来自TIM1 
#define MOTOR_PWM_TIM              TIM1
#define MOTOR_PWM_TIM_CLK          RCC_APB2Periph_TIM1


//4个电机芯片（1->4）的PWM输入脚分别为PE9，PE11，PE13，PE14
#define MOTOR_PWM_PORT             GPIOE
#define MOTOR_PWM_PIN_CLK          RCC_AHB1Periph_GPIOE
#define MOTOR_PWM_PIN_AF           GPIO_AF_TIM1

#define MOTOR1_PWM_PIN             GPIO_Pin_9
#define MOTOR1_PWM_PINSOURCE       GPIO_PinSource9


#define MOTOR2_PWM_PIN             GPIO_Pin_11
#define MOTOR2_PWM_PINSOURCE       GPIO_PinSource11


#define MOTOR3_PWM_PIN             GPIO_Pin_13
#define MOTOR3_PWM_PINSOURCE       GPIO_PinSource13


#define MOTOR4_PWM_PIN             GPIO_Pin_14
#define MOTOR4_PWM_PINSOURCE       GPIO_PinSource14


//DRV8873的推荐PWM输入最大值为100KHz
#define MOTOR_PWM_FRQ              50000
#define MOTOR_PWM_DutyInitValue    100



//PWM初始化函数
void Motor_PWM_Init(void);

//PWM停止一路输出
void Motor_PWM_DisableOneChannel(uint8_t MotorNum);

//PWM停止所有输出
void Motor_PWM_DisableAllChannel(void);

//PWM输出
void Motor_PWM_Output(uint8_t MotorNum,uint8_t DutyValue);

//改变占空比
void Motor_PWM_SetDutyValue(uint8_t MotorNum,uint8_t DutyValue);
#endif
