#include "bsp_led.h"

void LED_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(LED_RUN_GPIO_CLK | LED_ERROR_GPIO_CLK,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=LED_RUN_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	
	GPIO_Init(LED_RUN_GPIO_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=LED_ERROR_PIN;
	GPIO_Init(LED_RUN_GPIO_PORT,&GPIO_InitStructure);
	
}
void LED_ERROR_Toggle(void)
{
 LED_ERROR_GPIO_PORT->ODR^=LED_ERROR_PIN;
}

void LED_RUN_Toggle(void)
{
 LED_RUN_GPIO_PORT->ODR^=LED_RUN_PIN;
}
void LED_ERROR_ON(void)
{
	GPIO_SetBits(LED_ERROR_GPIO_PORT,LED_ERROR_PIN);
}
	
void LED_ERROR_OFF(void)
{
	GPIO_ResetBits(LED_ERROR_GPIO_PORT,LED_ERROR_PIN);
}
	
void LED_RUN_ON(void)
{
	GPIO_SetBits(LED_RUN_GPIO_PORT,LED_RUN_PIN);
}

void LED_RUN_OFF(void)
{
	GPIO_ResetBits(LED_RUN_GPIO_PORT,LED_RUN_PIN);
}
