#ifndef __BSP_RS485_H
#define __BSP_RS485_H

#include "stm32f4xx_usart.h"
#include <stdio.h>
#include "string.h"
#include "RTL.h"



//波特率115200
#define UART1_BAUD			   115200

//使用FIFO机制，发送及接收缓冲256字节
#define UART1_TX_BUF_SIZE	 512
#define UART1_RX_BUF_SIZE	 128

//使用USART1 引脚复用为PA9（TX），PA10（RX）
#define UART1_TX_PORT      GPIOA
#define UART1_TX_PIN       GPIO_Pin_9
#define UART1_TX_CLK       RCC_AHB1Periph_GPIOA

//外设复用指定引脚时必须使用PinSource9而不是PIN_9
#define UART1_TX_SOURCE    GPIO_PinSource9

#define UART1_RX_PORT      GPIOA
#define UART1_RX_PIN       GPIO_Pin_10
#define UART1_RX_CLK       RCC_AHB1Periph_GPIOA
#define UART1_RX_SOURCE    GPIO_PinSource10

//RS485模块自动流控，故不需要TR控制脚

#define UART_DMA_CHANNEL               DMA_Channel_4
#define UART_RX_DMA_STREAM             DMA2_Stream5 
#define UART_TX_DMA_STREAM             DMA2_Stream7

#define DMA_RECV_LEN              128
//#define DMA_TRAN_LEN            512

#define event_uart_received     (1<<0)                                    //事件标志定义 串口收到数据事件
#define event_uart_received     (1<<0)                                       //事件标志定义 串口收到数据事件
#define event_uart_transmitted  (1<<1)                                       //事件标志定义 串口发送完成事件
#define event_uart_needtrans    (1<<2)
#define event_uart_readytotrans (event_uart_needtrans |event_uart_transmitted)//事件标志定义 串口收发均完成事件

//UART硬件配置初始化
static void InitHardUart(void);
//UART中断配置
static void ConfigUartNVIC(void);

//串口总初始化函数
void RS485_UART_Init(void);

//块发送函数
uint8_t UartSendBuf(uint8_t *_ucaBuf, uint16_t _usLen);

//字节发送函数
uint8_t UartSendChar(uint8_t _ucByte);

//字节接收函数
uint8_t UartGetChar(uint8_t *_pByte);

/**/
uint8_t DmaSendDataProc(void);

void DmaReceiveReopen(void);

#endif
