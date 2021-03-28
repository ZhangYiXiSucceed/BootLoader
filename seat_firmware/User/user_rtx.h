#ifndef __USER_RTX_H
#define __USER_RTX_H

#include "rtl.h"
#include "user_controllogic.h"
#include "user_comframe.h"
#include "bsp.h"

#define WTD_EVT_BIT_0	 (1 << 0)
#define WTD_EVT_BIT_1	 (1 << 1)
#define WTD_EVT_BIT_2	 (1 << 2)
#define WTD_EVT_BIT_3	 (1 << 3)
#define WTD_EVT_BIT_4	 (1 << 4)
#define WTD_EVT_BIT_5	 (1 << 5)
#define WTD_EVT_BIT_6	 (1 << 6)
#define SCG_EVT_BIT_0	 (1 << 0) 
#define WTD_EVT_BIT_ALL (WTD_EVT_BIT_0 | WTD_EVT_BIT_1 | WTD_EVT_BIT_2 | WTD_EVT_BIT_3 | WTD_EVT_BIT_4 | WTD_EVT_BIT_5 | WTD_EVT_BIT_6)


/*ϵͳ��������*/
__task void Task_System_Setup(void);

/*���ڽ��յ�����ʵ�ֺ������봮�ڷ���ͨ���ź���ͬ��*/
__task void Task_Uart_Receive(void);

/*���ڷ��͵�����ʵ�ֺ���*/
__task void Task_Uart_Transmit(void);

/*ADC��ֵ��ȡ������Ҫ�е��λ��ADCֵ���������ADCֵ*/
__task void Task_ADC_GetValue(void);

/*��ֵɨ������ɨ�������̲���������Ϣͨ����Ϣ���䴫��*/
__task void Task_Key_Scan(void);

/*���ο������񣬸��ݰ�����Ϣ�����α궨״̬��������̬������������*/
__task void Task_Seat_ControlLogic(void);

/*�����˶���Ϣ��¼���񣬼�¼4���˶���Ϣ��NVM*/
__task void Task_Seat_InfoRecord(void);

/*���״̬�������ͨ��SPI���߲�ѯ4���������оƬ�Ĵ���Ĵ�����Ϣ������*/
__task void Task_Motor_StatusCheck(void);

/*ϵͳ���Ź�����*/
__task void Task_System_Watchdog(void);
#endif
