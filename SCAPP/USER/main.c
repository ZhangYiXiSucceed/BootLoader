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
	SCB->VTOR = FLASH_BASE | 0x10000;              //�ж�������ƫ�Ƶ�ַ
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);       //��ʼ����ʱ����
	uart_init(115200);		 //��ʼ�����ڲ�����Ϊ115200
	MyDMA_Config();
	LED_Init();					   //��ʼ��LED  
 	KEY_Init();					   //������ʼ�� 
	TIM3_Init(5);
	__enable_irq();
	while(1)
	{
		Iap_Write();    //�Խ��յ�����λ��ָ����д���
	}
}

