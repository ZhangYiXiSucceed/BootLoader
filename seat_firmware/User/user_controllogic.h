#ifndef __USER_CONTROLLOGIC_H
#define __USER_CONTROLLOGIC_H

#include "bsp_motor.h"

#define DEFAULT_MOTOR_SPEED 80           //未标定前默认速度
#define DEFAULT_AXIS_DISTANCE_SLOW 100    //默认的减速提前量
#define DEFAULT_AXIS_DISTANCE_STOP 50    //默认的刹车提前量

void SeatLogicDataInit(void);//这个函数在重新标定保存后需要被调用
void CbrtedSeatLogic_Key_Popup(void);
void CbrtedSeatLogic_BackOpen_PressDown(void);
void CbrtedSeatLogic_BackClose_PressDown(void);
void CbrtedSeatLogic_LegOpen_PressDown(void);
void CbrtedSeatLogic_LegClose_PressDown(void);
void CbrtedSeatLogic_FootOpen_PressDown(void);
void CbrtedSeatLogic_FootClose_PressDown(void);
void CbrtedSeatLogic_OneKeyOpen_PressDown(void);
void CbrtedSeatLogic_OneKeyClose_PressDown(void);
void CbrtedSeatLogic_OneKeyHalf_PressDown(void);

void ControlLogic_NonCbrtedSeat(uint8_t KeyValue);
void ControlLogic_CbrtedSeat(uint8_t KeyValue);

#endif

