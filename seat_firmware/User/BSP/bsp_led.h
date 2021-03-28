#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "stm32f4xx_gpio.h"

// 绿色运行指示灯
#define LED_RUN_PIN         GPIO_Pin_5    
#define LED_RUN_GPIO_PORT   GPIOB
#define LED_RUN_GPIO_CLK    RCC_AHB1Periph_GPIOB

// 红色错误指示灯
#define LED_ERROR_PIN       GPIO_Pin_4    
#define LED_ERROR_GPIO_PORT   GPIOB
#define LED_ERROR_GPIO_CLK    RCC_AHB1Periph_GPIOB



void LED_GPIO_Config(void);
void LED_ERROR_Toggle(void);
void LED_RUN_Toggle(void);
void LED_ERROR_ON(void);
void LED_ERROR_OFF(void);
void LED_RUN_ON(void);
void LED_RUN_OFF(void);

#endif
