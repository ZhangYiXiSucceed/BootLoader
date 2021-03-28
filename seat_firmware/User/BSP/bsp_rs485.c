#include "bsp_rs485.h"


uint8_t g_TxBuf[UART1_TX_BUF_SIZE];		
uint8_t g_RxBuf[UART1_RX_BUF_SIZE];	  

uint16_t  UART1_ReceiveSize;
uint16_t  UART1_TransmitSize;

extern OS_TID HandleTaskUartRcv;
extern OS_TID HandleTaskUartTsmt;
extern OS_SEM UartTrans;

/*
*********************************************************************************************************
*	�� �� ��: InitUartVar
*	����˵��: ���ڵ�ȫ�ֱ�����ʼ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void InitUartVar(void)
{ 
	UART1_ReceiveSize=0;        /* ���յ���ʵ�������ֽ�������*/
	UART1_TransmitSize=0;
	memset(g_TxBuf,0,UART1_TX_BUF_SIZE*sizeof(uint8_t));
	memset(g_RxBuf,0,UART1_RX_BUF_SIZE*sizeof(uint8_t));
}

/*
*********************************************************************************************************
*	�� �� ��: InitUartGPIO
*	����˵��: ���ô��ڵ�Ӳ������PA9 PA10
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void InitUartGPIO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* �� GPIO ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/* �� UART ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	/* �� PA9 ӳ��Ϊ USART1_TX */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);

	/* �� PA10 ӳ��Ϊ USART1_RX */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

	/* ���� USART Tx Ϊ���ù��� */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	/* �������Ϊ���� */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	/* �ڲ���������ʹ�� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	/* ����ģʽ */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* ���� USART Rx Ϊ���ù��� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*
*********************************************************************************************************
*	�� �� ��: InitUartMode
*	����˵��: ����USART�Ĺ���ģʽΪ�շ�ȫ˫����115200,8N1������ΪDMA���䣬������������жϣ����տ����ж�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void InitUartMode(void)
{
	USART_InitTypeDef USART_InitStructure;
	
	USART_DeInit(USART1);//��λ����1����
	
	/*  ���ô���Ӳ������Ϊ8N1,�����أ�TX RX˫�� */
 	USART_InitStructure.USART_BaudRate = UART1_BAUD;	
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_ITConfig(USART1, USART_IT_TC, DISABLE); 
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);		/* ʹ�ܴ��� */
	
	/*STM32�ᷢһ����֡��Ϊ��һ�����ݷ���*/
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET)
  {
        ; //�ȴ�����֡������ɺ�  �����㷢�ͱ�־
  }
 
	USART_ClearFlag(USART1,USART_FLAG_TC);
	USART_ClearITPendingBit(USART1, USART_IT_TC);//�����־λ
	
	/*  ʹ�ܴ��� ����ΪDMA�շ��������ж� */
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);//���������жϣ�һ�����չ��������ݶ�������ʾ�������
	//USART_ITConfig(USART1, USART_IT_TC, ENABLE);  //������������ж�	
	USART_DMACmd(USART1,USART_DMAReq_Tx|USART_DMAReq_Rx,ENABLE);//��������DMA�շ�ģʽ
}

/*
*********************************************************************************************************
*	�� �� ��: InitDMAUart
*	����˵��: ����USART��DMA��������USART1��TX��RX�ֱ�ʹ��DMA���д��䣬����DMA����ж�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void InitDMAUart(void)
{
  //DMA��ʼ���ṹ��
	DMA_InitTypeDef DMA_InitStructure;
	//����DMA�ж�����
	NVIC_InitTypeDef NVIC_InitStructure;
	// ����DMAʱ��
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 , ENABLE);
	
	/*RXDMA����*/
	DMA_DeInit(UART_RX_DMA_STREAM);//��λRX����������
	while(DMA_GetCmdStatus(UART_RX_DMA_STREAM)!=DISABLE);//�ȴ�DMA������
	
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;                   //ͨ��ѡ��ͨ��4������5��USART1_RX
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART1->DR;     //����DMA�����ַΪUSART1���ݼĴ���
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)g_RxBuf;           //�����ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;          //���ݴ��䷽�򣬴������ȡ���͵��ڴ�
	DMA_InitStructure.DMA_BufferSize = DMA_RECV_LEN;                 //DMA���ջ����С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //�����ַ������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;          // �洢����ַ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //��������Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 //�洢�����ݴ�СҲ8λ	
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                    // ��������ģʽ,��ֹѭ�� 
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;              // DMA ����ͨ�����ȼ�Ϊ��
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;           // ��ֹDMA FIFO	��ʹ��ֱ��ģʽ 
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;    //����FIFOʱ���м���������������
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;      //�洢��ͻ�����δ���
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//����ͻ�����δ���
	DMA_Init(UART_RX_DMA_STREAM, &DMA_InitStructure);                //��ʼ��DMA��
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream5_IRQn;          //����DMA�жϺ�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=7;          //��ռ���ȼ�Ϊ7����ֵԽС���ȼ�Խ��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
  DMA_Cmd(UART_RX_DMA_STREAM, ENABLE);                             // ʹ��DMA��
	//DMA_ITConfig(UART_RX_DMA_STREAM,DMA_IT_TC,ENABLE);               //����DMA�����жϣ����ݴӴ��ڵ����ջ���ת������жϣ�
	 
	/*TXDMA����*/
	DMA_DeInit(UART_TX_DMA_STREAM);//��λTX����������
	while (DMA_GetCmdStatus(UART_TX_DMA_STREAM) != DISABLE);         //�ȴ�DMA������
	
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;                   //ͨ��ѡ��ͨ��4������7��USART1_TX
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART1->DR;     //����DMA�����ַΪUSART1���ݼĴ���
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)g_TxBuf;            //�����ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;          //���ݴ��䷽�򣬴��ڴ淢�͵�����
	DMA_InitStructure.DMA_BufferSize = 0;                            //DMA�������ݴ�С��Ҫ�������ݰ�ʵʱ�޸�,������д0
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //�����ַ������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;          // �洢����ַ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //��������Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 //�洢�����ݴ�СҲ8λ	
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                    // ��������ģʽ,��ֹѭ�� 
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;              // DMA ����ͨ�����ȼ�Ϊ��
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;           // ��ֹDMA FIFO	��ʹ��ֱ��ģʽ
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;      //�洢��ͻ�����δ���
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//����ͻ�����δ���
	DMA_Init(UART_TX_DMA_STREAM, &DMA_InitStructure);                //��ʼ��DMA��
	
  NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn;          //����DMA�жϺ�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 8;        //��ռ���ȼ�Ϊ8
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
	NVIC_Init(&NVIC_InitStructure);  
	//DMA_ITConfig(UART_TX_DMA_STREAM,DMA_IT_TC,ENABLE);               //ʹ��DMA�����жϣ����ݴӷ��ͻ���ת�Ƶ����ڣ�
	/*���ʹ˴��Ȳ�ʹ�ܣ���Ҫ����ʱ�ٴ�*/
  //DMA_Cmd(UART_TX_DMA_STREAM, ENABLE);                             // ʹ��DMA��
}

/*
*********************************************************************************************************
*	�� �� ��: ConfigUartNVIC
*	����˵��: ���ô���Ӳ���жϣ�����4��ȫ��Ϊ��ռ���ȼ����������ȼ�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void ConfigUartNVIC(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
 
	/* ʹ�ܴ���1�ж� */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=8;//��ռ���ȼ�Ϊ8����ֵԽС���ȼ�Խ��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitUart
*	����˵��: ���������������������г�ʼ������Ӳ��������ȫ�ֱ�������ֵ.
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RS485_UART_Init(void)
{ //uint8_t strlen;
	//uint8_t comInfo[40];
	
	InitUartVar();    /* ���ô��ڵ�ȫ�ֱ��� */
	
	InitUartGPIO();		/* ���ô��ڵ�GPIO */
	
	InitUartMode();   /* ���ô��ڵĹ���ģʽ*/
	
	ConfigUartNVIC();	/* ���ô����ж� */

	InitDMAUart();    /* ����DMA���ͨ�� */	
	
	//strlen=sprintf((char*)comInfo,"USATR1 Init OK\n");//�������ֿ����������Ϣ˵������������
	//UartSendBuf(comInfo, strlen);
}


/*
*********************************************************************************************************
*	�� �� ��: DmaReceiveReopen
*	����˵��: �������ڽ��ܵ�DMAͨ�������ڴ��ڽ��յ����ݴ���������´�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void DmaReceiveReopen(void)
{ 
	/*��DMA���λ*/
	DMA_ClearFlag(UART_RX_DMA_STREAM,DMA_FLAG_TCIF5 | DMA_FLAG_FEIF5 | DMA_FLAG_DMEIF5 | DMA_FLAG_TEIF5 | DMA_FLAG_HTIF5);//���DMA2_Steam5������ɱ�־
  /*������ճ���*/
	DMA_SetCurrDataCounter(UART_RX_DMA_STREAM, UART1_RX_BUF_SIZE);
	/*��DMA*/
	DMA_Cmd(UART_RX_DMA_STREAM, ENABLE);     //��DMA,	
}
	
/*
*********************************************************************************************************
*	�� �� ��: DmaSendDataProc
*	����˵��: ��д���ݵ�UART���ͻ�����,�����������жϡ��жϴ�����������Ϻ��Զ��رշ����ж�
*	��    ��: ��
*	�� �� ֵ: ִ�н��
*********************************************************************************************************
*/
uint8_t DmaSendDataProc(void)
{

	DMA_Cmd(UART_TX_DMA_STREAM, DISABLE);                      //�ر�DMA���� 
	
	if(DMA_GetCmdStatus(UART_TX_DMA_STREAM) != DISABLE)
	{
		return 1;//DMA�޷�����ʱֱ�ӷ��أ�������
	} 
		
	DMA_SetCurrDataCounter(UART_TX_DMA_STREAM,UART1_TransmitSize);          //���ݴ�����  
  /*����DMA*/
	DMA_Cmd(UART_TX_DMA_STREAM, ENABLE);
	/*ʹ�ܴ��ڷ��ͼĴ������жϣ����ﲻ�򿪷�������ж�*/
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	
	return 0;
}

/*
*********************************************************************************************************
*	�� �� ��: UartSendBuf
*	����˵��: ����buf���ͺ���
*	��    ��: ��������ָ�룬���ݳ���
*	�� �� ֵ: retֵ
*********************************************************************************************************
*/
uint8_t UartSendBuf(uint8_t *_ucaBuf, uint16_t _usLen)
{

  if((UART1_TransmitSize+_usLen)<=UART1_TX_BUF_SIZE)
	{
		memcpy((g_TxBuf+UART1_TransmitSize),_ucaBuf,_usLen);
		UART1_TransmitSize+=_usLen;
		return 1;
	}
  else
	{
		return 0;
	}	
}

/*
*********************************************************************************************************
*	�� �� ��: UartSendChar
*	����˵��: �ú���������
*	��    ��: ucByte: �����͵�����
*	�� �� ֵ: retֵ
*********************************************************************************************************
*/
uint8_t UartSendChar(uint8_t _ucByte)
{
	uint8_t ret=3;
	uint8_t* ch;
	
	*ch=_ucByte;
	
	ret=UartSendBuf(ch,1);
	
	return ret;
}

/*
*********************************************************************************************************
*	�� �� ��: USART1_IRQHandler  
*	����˵��: USART�жϷ���������Ʋ����޸ģ�����������startup_stm32f429_439xx.s�е�ͬ������
            �����յ���������ɡ����ͼĴ������ж�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void USART1_IRQHandler(void)
{
	//uint16_t data;
	//USART1���տ����жϱ���λ���������ν������
	if(USART_GetITStatus(USART1,USART_IT_IDLE) != RESET)
	{
		DMA_Cmd(UART_RX_DMA_STREAM, DISABLE); //�ر�DMA,��ֹ�������������
    /*
		//������������˳���ȡSR DR�����IDLE��־λ
		data = USART1->SR;
		data = USART1->DR;
		*/
		USART_ReceiveData(USART1);
		
		UART1_ReceiveSize =UART1_RX_BUF_SIZE - DMA_GetCurrDataCounter(UART_RX_DMA_STREAM);
		
		if(UART1_ReceiveSize !=0)
		{
			if(HandleTaskUartRcv)
			{
				isr_evt_set(event_uart_received ,HandleTaskUartRcv);
			}			
		}	
	}
	
	//USART1���ͻ�������
	if(USART_GetITStatus(USART1,USART_IT_TXE)!= RESET)
	{
		/* ���ͻ�������������ȡ��ʱ�� ��ֹ���ͻ��������ж� ��ע�⣺��ʱ���1�����ݻ�δ����������ϣ�*/
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
			
		/* ʹ�����ݷ�������ж� */
			USART_ITConfig(USART1, USART_IT_TC, ENABLE);
	}
	
	//USART1��������жϱ���λ���������η��ͽ���
	if(USART_GetITStatus(USART1,USART_IT_TC)!= RESET)
	{
	    USART_ClearITPendingBit(USART1, USART_IT_TC);//�����־λ
		
		 /*�رշ�������ж�*/
		  USART_ITConfig(USART1, USART_IT_TC, DISABLE);
		
		 //�رմ��ڷ���DMAͨ��
	    DMA_Cmd(UART_TX_DMA_STREAM, DISABLE); 
		  //���ݳ�������
		  UART_TX_DMA_STREAM->NDTR=0;
		  //���DMA2_Steam7������ɱ�־
		  DMA_ClearFlag(UART_TX_DMA_STREAM,DMA_FLAG_TCIF7);
	   	
			memset(g_TxBuf,0,UART1_TX_BUF_SIZE*sizeof(uint8_t));
			UART1_TransmitSize=0;

		//��������¼���־����		
	    if(HandleTaskUartTsmt)
			{
				//isr_evt_set(event_uart_transmitted ,HandleTaskUartTsmt);
				isr_sem_send(UartTrans);			
			}			
	}
}

/*
*********************************************************************************************************
*	�� �� ��: fputc
*	����˵��: �ض���putc��������������ʹ��printf�����Ӵ���1��ӡ���
            ���ڲ�����DMA�����ֽڴ���Ч�ʵͣ������ò�Ҫʹ�ñ�����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
int fputc(int ch, FILE *f)
{
	UartSendChar(ch);
	return ch;
}


