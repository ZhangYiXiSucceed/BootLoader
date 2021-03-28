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


/*系统启动任务*/
__task void Task_System_Setup(void);

/*串口接收的任务实现函数，与串口发送通过信号量同步*/
__task void Task_Uart_Receive(void);

/*串口发送的任务实现函数*/
__task void Task_Uart_Transmit(void);

/*ADC数值获取任务，主要有电机位置ADC值，电机电流ADC值*/
__task void Task_ADC_GetValue(void);

/*键值扫描任务，扫描矩阵键盘并将按键信息通过消息邮箱传递*/
__task void Task_Key_Scan(void);

/*座椅控制任务，根据按键信息、座椅标定状态、座椅姿态来输出电机动作*/
__task void Task_Seat_ControlLogic(void);

/*座椅运动信息记录任务，记录4轴运动信息到NVM*/
__task void Task_Seat_InfoRecord(void);

/*电机状态检查任务，通过SPI总线查询4个电机驱动芯片的错误寄存器信息并处理*/
__task void Task_Motor_StatusCheck(void);

/*系统看门狗任务*/
__task void Task_System_Watchdog(void);
#endif
