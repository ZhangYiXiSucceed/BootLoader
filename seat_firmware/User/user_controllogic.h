#ifndef __USER_CONTROLLOGIC_H
#define __USER_CONTROLLOGIC_H

#include "bsp_motor.h"

#define DEFAULT_MOTOR_SPEED 80           //δ�궨ǰĬ���ٶ�
#define DEFAULT_AXIS_DISTANCE_SLOW 100    //Ĭ�ϵļ�����ǰ��
#define DEFAULT_AXIS_DISTANCE_STOP 50    //Ĭ�ϵ�ɲ����ǰ��

void SeatLogicDataInit(void);//������������±궨�������Ҫ������
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

