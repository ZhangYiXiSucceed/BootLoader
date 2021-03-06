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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组2
	delay_init(168);    //初始化延时函数
	uart_init(115200);  //初始化串口波特率为115200
	MyDMA_Config();     //DMA接收配置
	LED_Init();					//初始化LED  
 	KEY_Init();					//按键初始化，暂时没有使用
	TIM3_Init(1);       //定时器初始化，用于延时进入APP程序
	__enable_irq();
	while(1)
	{	
		Iap_Write();      //APP程序接收处理
		IsConnect();		  //判断是否有上位机，之后进行相关操作（延时2s进入APP程序）
	}
}

