#ifndef __BSP_SPI_H
#define __BSP_SPI_H

#include "stm32f4xx_spi.h"

#define SPI_WAIT_TIME   100000

//DRV8873的读写命令格式
//Bit15                          Bit8  Bit7                 Bit0
//SDI/O   R/W     A4 A3 A2 A1 A0  X    D7 D6 D5 D4 D3 D2 D1 D0
//I=0,O=1 R=1,W=0                RSV

//DRV8873的读命令预设
//读取错误状态寄存器（只读）      0 1 00000 0  00000000
#define MotorCMD_Read_FaultStatus 0x4000

//读取诊断状态寄存器（只读）      0 1 00001 0  00000000
#define MotorCMD_Read_DiagStatus  0x4200

//读取IC1寄存器（读写）           0 1 00010 0  00000000
#define MotorCMD_Read_IC1Status   0x4400

//读取IC2寄存器（读写）           0 1 00011 0  00000000
#define MotorCMD_Read_IC2Status   0x4600

//读取IC3寄存器（读写）           0 1 00100 0  00000000
#define MotorCMD_Read_IC3Status   0x4800

//读取IC4寄存器（读写）           0 1 00101 0  00000000
#define MotorCMD_Read_IC4Status   0x4A00

//DRV8873的写命令预设
//写IC1寄存器 高8位字节           0 0 00010 0 
#define MotorCMD_Write_IC1        0x04

//写IC1寄存器 高8位字节           0 0 00011 0 
#define MotorCMD_Write_IC2        0x06

//写IC1寄存器 高8位字节           0 0 00100 0 
#define MotorCMD_Write_IC3        0x08

//写IC1寄存器 高8位字节           0 0 00101 0 
#define MotorCMD_Write_IC4        0x0A


//SPI 外设配置
//DRV8873电机驱动芯片使用STM32的SPI3，最高速度22.5MHz
#define MOTOR_SPI                           SPI3
//SPI3 时钟源为APB1 
#define MOTOR_SPI_CLK                       RCC_APB1Periph_SPI3
#define MOTOR_SPI_CLK_INIT                  RCC_APB1PeriphClockCmd

//SPI 引脚配置
//SCK PC10
#define MOTOR_SPI_SCK_PIN                   GPIO_Pin_10                  
#define MOTOR_SPI_SCK_GPIO_PORT             GPIOC                       
#define MOTOR_SPI_SCK_GPIO_CLK              RCC_AHB1Periph_GPIOC
#define MOTOR_SPI_SCK_PINSOURCE             GPIO_PinSource10
#define MOTOR_SPI_SCK_AF                    GPIO_AF_SPI3

//MISO PC11
#define MOTOR_SPI_MISO_PIN                  GPIO_Pin_11                
#define MOTOR_SPI_MISO_GPIO_PORT            GPIOC                   
#define MOTOR_SPI_MISO_GPIO_CLK             RCC_AHB1Periph_GPIOC
#define MOTOR_SPI_MISO_PINSOURCE            GPIO_PinSource11
#define MOTOR_SPI_MISO_AF                   GPIO_AF_SPI3

//MOSI PC12
#define MOTOR_SPI_MOSI_PIN                  GPIO_Pin_12                
#define MOTOR_SPI_MOSI_GPIO_PORT            GPIOC                      
#define MOTOR_SPI_MOSI_GPIO_CLK             RCC_AHB1Periph_GPIOC
#define MOTOR_SPI_MOSI_PINSOURCE            GPIO_PinSource12
#define MOTOR_SPI_MOSI_AF                   GPIO_AF_SPI3

//CS1-4使用GPIO软模拟，不用硬件片选
//CS1 PB10
#define MOTOR_CS1_PIN                        GPIO_Pin_10               
#define MOTOR_CS1_GPIO_PORT                  GPIOB                   
#define MOTOR_CS1_GPIO_CLK                   RCC_AHB1Periph_GPIOB

//CS2 PD8
#define MOTOR_CS2_PIN                        GPIO_Pin_8               
#define MOTOR_CS2_GPIO_PORT                  GPIOD                     
#define MOTOR_CS2_GPIO_CLK                   RCC_AHB1Periph_GPIOD

//CS3 PD14
#define MOTOR_CS3_PIN                        GPIO_Pin_14              
#define MOTOR_CS3_GPIO_PORT                  GPIOD                     
#define MOTOR_CS3_GPIO_CLK                   RCC_AHB1Periph_GPIOD

//CS4 PA8
#define MOTOR_CS4_PIN                        GPIO_Pin_8               
#define MOTOR_CS4_GPIO_PORT                  GPIOA                     
#define MOTOR_CS4_GPIO_CLK                   RCC_AHB1Periph_GPIOA

//片选线操作
#define MOTOR_CS1_LOW()      {MOTOR_CS1_GPIO_PORT->BSRRH=MOTOR_CS1_PIN;}
#define MOTOR_CS1_HIGH()     {MOTOR_CS1_GPIO_PORT->BSRRL=MOTOR_CS1_PIN;}

#define MOTOR_CS2_LOW()      {MOTOR_CS2_GPIO_PORT->BSRRH=MOTOR_CS2_PIN;}
#define MOTOR_CS2_HIGH()     {MOTOR_CS2_GPIO_PORT->BSRRL=MOTOR_CS2_PIN;}

#define MOTOR_CS3_LOW()      {MOTOR_CS3_GPIO_PORT->BSRRH=MOTOR_CS3_PIN;}
#define MOTOR_CS3_HIGH()     {MOTOR_CS3_GPIO_PORT->BSRRL=MOTOR_CS3_PIN;}

#define MOTOR_CS4_LOW()      {MOTOR_CS4_GPIO_PORT->BSRRH=MOTOR_CS4_PIN;}
#define MOTOR_CS4_HIGH()     {MOTOR_CS4_GPIO_PORT->BSRRL=MOTOR_CS4_PIN;}


//函数定义
void Motor_SPI_Init(void);
void SPI_Sellect(uint8_t ChipNum);
uint16_t Motor_SPI_WriteWord(uint16_t* Data);
uint8_t Motor_GetChipStatus(uint8_t ChipNum);
#endif
