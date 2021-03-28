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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ�����2
	delay_init(168);    //��ʼ����ʱ����
	uart_init(115200);  //��ʼ�����ڲ�����Ϊ115200
	MyDMA_Config();     //DMA��������
	LED_Init();					//��ʼ��LED  
 	KEY_Init();					//������ʼ������ʱû��ʹ��
	TIM3_Init(1);       //��ʱ����ʼ����������ʱ����APP����
	__enable_irq();
	while(1)
	{	
		Iap_Write();      //APP������մ���
		IsConnect();		  //�ж��Ƿ�����λ����֮�������ز�������ʱ2s����APP����
	}
}

