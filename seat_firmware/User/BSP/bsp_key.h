#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#include "stm32f4xx_gpio.h"
#include "stdio.h"
#include "stdint.h"
#include "RTL.h"

/*����������������Ĵ򿪺�������OPEN��CLOSE��������״̬ͳһ��UP DOWN������ΪLONGPRESS*/

/* ����ID */
typedef enum
{
	KID_KEY_BackOpen = 0,
	KID_KEY_BackClose,
	KID_KEY_LegOpen,
	KID_KEY_LegClose,
	KID_JKEY_FeetOpen,
	KID_KEY_FeetClose,
	KID_KEY_OneKeyOpen,
	KID_KEY_OneKeyHalfOpen,//һ������
	KID_KEY_OneKeyClose,
}KEY_ID_E;


/*��ʱ������10msΪ��λ����*/
#define KEY_FILTER_TIME        2  //5
#define KEY_LONGPRESS_TIME     10 //100			/* ����1�룬��Ϊ�����¼� */


/*�����Ľṹ����������ڼ�¼���������������Ϣ��ÿ������һ�� */


typedef __packed struct
{
	uint8_t (*IsKeyDownFunc)(void); /* ���ָ�������Ƿ��µĺ���ָ��*/
	uint8_t  Count;			/* �˲��������� */
	uint16_t LongCount;		/* ���������� */
	uint16_t LongTime;		/* ������ֵʱ��, 0��ʾ����ⳤ�� */
	uint8_t  State;			/* ������ǰ״̬�����»��ǵ��� */
	uint8_t  RepeatPeriod;	/* ������������ */
	uint8_t  RepeatCount;	/* �������������� */
}KEY_T;

/*
�����ֵ����, ���밴���´���ʱÿ�����İ��¡�����ͳ����¼�
ʹ��enum, ����������ֵ,�������˳�򣬱����ֵ�ظ���
*/
typedef enum
{
	KEY_NONE = 0,			            /* 0 ��ʾ�ް����¼� */

	KEY_BackOpen_Down,				    /* �����򿪼����� */
	KEY_BackOpen_Up,				      /* �����򿪼����� */
	KEY_BackOpen_LongPress,				/* �����򿪼����� */

	KEY_BackClose_Down,				    /* ������������� */
	KEY_BackClose_Up,				      /* ������������� */
	KEY_BackClose_LongPress,			/* ������������� */
	
	KEY_LegOpen_Down,				      /* ���д򿪼����� */
	KEY_LegOpen_Up,				        /* ���д򿪼����� */
	KEY_LegOpen_LongPress,				/* ���д򿪼����� */
	
	KEY_LegClose_Down,				    /* ������������� */
	KEY_LegClose_Up,				      /* ������������� */
	KEY_LegClose_LongPress,				/* ������������� */
	
	KEY_FeetOpen_Down,				    /* ���д򿪼����� */
	KEY_FeetOpen_Up,				      /* ���д򿪼����� */
	KEY_FeetOpen_LongPress,				/* ���д򿪼����� */
	
	KEY_FeetClose_Down,				    /* ������������� */
	KEY_FeetClose_Up,				      /* ������������� */
	KEY_FeetClose_LongPress,			/* ������������� */
	
	KEY_OneKeyOpen_Down,				  /* һ��ȫ�ɼ����� */
	KEY_OneKeyOpen_Up,				    /* һ��ȫ�ɼ����� */
	KEY_OneKeyOpen_LongPress,		  /* һ��ȫ�ɼ����� */
	
	KEY_OneKeyHalfOpen_Down,			/* һ�����ɼ����� */
	KEY_OneKeyHalfOpen_Up,				/* һ�����ɼ����� */
	KEY_OneKeyHalfOpen_LongPress,	/* һ�����ɼ����� */
	
	KEY_OneKeyClose_Down,				  /* һ��ȫ�ռ����� */
	KEY_OneKeyClose_Up,				    /* һ��ȫ�ռ����� */
	KEY_OneKeyClose_LongPress,		/* һ��ȫ�ռ����� */
		
  /* ��ϼ���Ԥ�� */
}KEY_ENUM;


/* ����FIFO���� */
#define KEY_FIFO_SIZE	 16

/* �������� ��ǰ����������Ч����9��*/
#define KEY_COUNT	     9  

/* ����FIFO�ṹ�� */
typedef __packed struct
{
	uint8_t Buf[KEY_FIFO_SIZE];		/* ��ֵ������ */
	uint8_t Read;					        /* ��������ָ��1 */
	uint8_t Write;					      /* ������дָ�� */
}KEY_FIFO_T;

/*������̵������� KEY1-KEY7ΪPE0-PE6*/
/*ʱ�ӣ��˿ڣ����Ŷ���*/
#define KEY_GPIO_CLK             RCC_AHB1Periph_GPIOE
#define KEY_GPIO_PORT            GPIOE

#define KEY_ROW1_PIN             GPIO_Pin_0 //Line-KEY1
#define KEY_ROW2_PIN             GPIO_Pin_1 //Line-KEY2

#define KEY_COL1_PIN             GPIO_Pin_2 //Line-KEY3
#define KEY_COL2_PIN             GPIO_Pin_3 //Line-KEY4
#define KEY_COL3_PIN             GPIO_Pin_4 //Line-KEY5
#define KEY_COL4_PIN             GPIO_Pin_5 //Line-KEY6
#define KEY_COL5_PIN             GPIO_Pin_6 //Line-KEY7



void Key_Init(void);
void RTX_Key_Init(void);

uint8_t Key_PutIntoFIFO(uint8_t KeyValue);
int8_t Key_GetFromFIFO(void);

void Key_ScanAll(void);

#endif
