#include "sys.h"
#include "delay.h"  
#include "usart.h"   
#include "led.h"
#include "key.h"   
#include "stmflash.h" 
#include "iap.h"   
#include "timer.h"


int main(void)
{ 
	SCB->VTOR = FLASH_BASE | 0x10000;              //中断向量表偏移地址
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);       //初始化延时函数
	uart_init(115200);		 //初始化串口波特率为115200
	MyDMA_Config();
	LED_Init();					   //初始化LED  
 	KEY_Init();					   //按键初始化 
	TIM3_Init(5);
	__enable_irq();
	while(1)
	{
		Iap_Write();    //对接收到的上位机指令进行处理
	}
}

