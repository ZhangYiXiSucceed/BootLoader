#include "timer.h"
#include "iap.h"



void TIM3_Init(u32 Frequency)
{
	TIM_TimeBaseInitTypeDef    TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef           NVIC_InitStructure;
	
	u32 Prescaler=18000;
	u32 Period=SystemCoreClock/Prescaler/Frequency;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	TIM_TimeBaseInitStructure.TIM_Period=Period;
	TIM_TimeBaseInitStructure.TIM_Prescaler=Prescaler;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM3,DISABLE);
}
void TIM3_IRQHandler()
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET)
	{
		if(MotorBackAdValue>4000)
			MotorBackAdValue=0;
		else
		  MotorBackAdValue+=4;
		
		
		if(MotorLegAdValue>4000)
			MotorLegAdValue=0;
		else
			MotorLegAdValue+=8;
		
		
		if(MotorFootAdValue>4000)
			MotorFootAdValue=0;
		else
		  MotorFootAdValue+=12;
    
		MotorCurrent[0]+=5;
		MotorCurrent[1]+=15;
		MotorCurrent[2]+=25;
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}



