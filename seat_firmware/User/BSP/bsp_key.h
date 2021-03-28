#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#include "stm32f4xx_gpio.h"
#include "stdio.h"
#include "stdint.h"
#include "RTL.h"

/*命名规则，座椅组件的打开和收起用OPEN，CLOSE，按键的状态统一用UP DOWN，长按为LONGPRESS*/

/* 按键ID */
typedef enum
{
	KID_KEY_BackOpen = 0,
	KID_KEY_BackClose,
	KID_KEY_LegOpen,
	KID_KEY_LegClose,
	KID_JKEY_FeetOpen,
	KID_KEY_FeetClose,
	KID_KEY_OneKeyOpen,
	KID_KEY_OneKeyHalfOpen,//一键半躺
	KID_KEY_OneKeyClose,
}KEY_ID_E;


/*定时器将以10ms为单位工作*/
#define KEY_FILTER_TIME        2  //5
#define KEY_LONGPRESS_TIME     10 //100			/* 持续1秒，认为长按事件 */


/*按键的结构体变量，用于记录按键的相关配置信息，每个按键一个 */


typedef __packed struct
{
	uint8_t (*IsKeyDownFunc)(void); /* 检测指定按键是否按下的函数指针*/
	uint8_t  Count;			/* 滤波器计数器 */
	uint16_t LongCount;		/* 长按计数器 */
	uint16_t LongTime;		/* 长按阈值时间, 0表示不检测长按 */
	uint8_t  State;			/* 按键当前状态（按下还是弹起） */
	uint8_t  RepeatPeriod;	/* 连续按键周期 */
	uint8_t  RepeatCount;	/* 连续按键计数器 */
}KEY_T;

/*
定义键值代码, 必须按如下次序定时每个键的按下、弹起和长按事件
使用enum, 便于新增键值,方便调整顺序，避免键值重复。
*/
typedef enum
{
	KEY_NONE = 0,			            /* 0 表示无按键事件 */

	KEY_BackOpen_Down,				    /* 靠背打开键按下 */
	KEY_BackOpen_Up,				      /* 靠背打开键弹起 */
	KEY_BackOpen_LongPress,				/* 靠背打开键长按 */

	KEY_BackClose_Down,				    /* 靠背收起键按下 */
	KEY_BackClose_Up,				      /* 靠背收起键弹起 */
	KEY_BackClose_LongPress,			/* 靠背收起键长按 */
	
	KEY_LegOpen_Down,				      /* 腿托打开键按下 */
	KEY_LegOpen_Up,				        /* 腿托打开键弹起 */
	KEY_LegOpen_LongPress,				/* 腿托打开键长按 */
	
	KEY_LegClose_Down,				    /* 腿托收起键按下 */
	KEY_LegClose_Up,				      /* 腿托收起键弹起 */
	KEY_LegClose_LongPress,				/* 腿托收起键长按 */
	
	KEY_FeetOpen_Down,				    /* 脚托打开键按下 */
	KEY_FeetOpen_Up,				      /* 脚托打开键弹起 */
	KEY_FeetOpen_LongPress,				/* 脚托打开键长按 */
	
	KEY_FeetClose_Down,				    /* 脚托收起键按下 */
	KEY_FeetClose_Up,				      /* 脚托收起键弹起 */
	KEY_FeetClose_LongPress,			/* 脚托收起键长按 */
	
	KEY_OneKeyOpen_Down,				  /* 一键全躺键按下 */
	KEY_OneKeyOpen_Up,				    /* 一键全躺键弹起 */
	KEY_OneKeyOpen_LongPress,		  /* 一键全躺键长按 */
	
	KEY_OneKeyHalfOpen_Down,			/* 一键半躺键按下 */
	KEY_OneKeyHalfOpen_Up,				/* 一键半躺键弹起 */
	KEY_OneKeyHalfOpen_LongPress,	/* 一键半躺键长按 */
	
	KEY_OneKeyClose_Down,				  /* 一键全收键按下 */
	KEY_OneKeyClose_Up,				    /* 一键全收键弹起 */
	KEY_OneKeyClose_LongPress,		/* 一键全收键长按 */
		
  /* 组合键，预留 */
}KEY_ENUM;


/* 按键FIFO容量 */
#define KEY_FIFO_SIZE	 16

/* 按键数量 当前按键板上有效按键9个*/
#define KEY_COUNT	     9  

/* 按键FIFO结构体 */
typedef __packed struct
{
	uint8_t Buf[KEY_FIFO_SIZE];		/* 键值缓冲区 */
	uint8_t Read;					        /* 缓冲区读指针1 */
	uint8_t Write;					      /* 缓冲区写指针 */
}KEY_FIFO_T;

/*矩阵键盘的物理定义 KEY1-KEY7为PE0-PE6*/
/*时钟，端口，引脚定义*/
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
