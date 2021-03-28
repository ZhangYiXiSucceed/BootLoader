#ifndef __BSP__H
#define __BSP__H

#include "bsp_led.h"
#include "bsp_rs485.h"
#include "bsp_i2c.h"
#include "bsp_rtc.h"
#include "bsp_spi.h"
#include "bsp_adc.h"
#include "bsp_tim.h"
#include "bsp_motor.h"
#include "bsp_key.h"
#include "bsp_flash.h"

#define NVIC_VECTOR_TABLE_OFFSET  0x10000

void bsp_Init(void);
void data_Init(void);
void bsp_SetDefaultControlValue(void);
void bsp_WatchdogInit(uint32_t WDGTime);
#endif

