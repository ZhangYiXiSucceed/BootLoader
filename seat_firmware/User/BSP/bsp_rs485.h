#ifndef __BSP_RS485_H
#define __BSP_RS485_H

#include "stm32f4xx_usart.h"
#include <stdio.h>
#include "string.h"
#include "RTL.h"



//������115200
#define UART1_BAUD			   115200

//ʹ��FIFO���ƣ����ͼ����ջ���256�ֽ�
#define UART1_TX_BUF_SIZE	 512
#define UART1_RX_BUF_SIZE	 128

//ʹ��USART1 ���Ÿ���ΪPA9��TX����PA10��RX��
#define UART1_TX_PORT      GPIOA
#define UART1_TX_PIN       GPIO_Pin_9
#define UART1_TX_CLK       RCC_AHB1Periph_GPIOA

//���踴��ָ������ʱ����ʹ��PinSource9������PIN_9
#define UART1_TX_SOURCE    GPIO_PinSource9

#define UART1_RX_PORT      GPIOA
#define UART1_RX_PIN       GPIO_Pin_10
#define UART1_RX_CLK       RCC_AHB1Periph_GPIOA
#define UART1_RX_SOURCE    GPIO_PinSource10

//RS485ģ���Զ����أ��ʲ���ҪTR���ƽ�

#define UART_DMA_CHANNEL               DMA_Channel_4
#define UART_RX_DMA_STREAM             DMA2_Stream5 
#define UART_TX_DMA_STREAM             DMA2_Stream7

#define DMA_RECV_LEN              128
//#define DMA_TRAN_LEN            512

#define event_uart_received     (1<<0)                                    //�¼���־���� �����յ������¼�
#define event_uart_received     (1<<0)                                       //�¼���־���� �����յ������¼�
#define event_uart_transmitted  (1<<1)                                       //�¼���־���� ���ڷ�������¼�
#define event_uart_needtrans    (1<<2)
#define event_uart_readytotrans (event_uart_needtrans |event_uart_transmitted)//�¼���־���� �����շ�������¼�

//UARTӲ�����ó�ʼ��
static void InitHardUart(void);
//UART�ж�����
static void ConfigUartNVIC(void);

//�����ܳ�ʼ������
void RS485_UART_Init(void);

//�鷢�ͺ���
uint8_t UartSendBuf(uint8_t *_ucaBuf, uint16_t _usLen);

//�ֽڷ��ͺ���
uint8_t UartSendChar(uint8_t _ucByte);

//�ֽڽ��պ���
uint8_t UartGetChar(uint8_t *_pByte);

/**/
uint8_t DmaSendDataProc(void);

void DmaReceiveReopen(void);

#endif
