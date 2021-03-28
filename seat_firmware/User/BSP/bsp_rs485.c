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
*	函 数 名: InitUartVar
*	功能说明: 串口的全局变量初始化
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void InitUartVar(void)
{ 
	UART1_ReceiveSize=0;        /* 接收到的实际数据字节数清零*/
	UART1_TransmitSize=0;
	memset(g_TxBuf,0,UART1_TX_BUF_SIZE*sizeof(uint8_t));
	memset(g_RxBuf,0,UART1_RX_BUF_SIZE*sizeof(uint8_t));
}

/*
*********************************************************************************************************
*	函 数 名: InitUartGPIO
*	功能说明: 配置串口的硬件引脚PA9 PA10
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void InitUartGPIO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 打开 GPIO 时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/* 打开 UART 时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	/* 将 PA9 映射为 USART1_TX */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);

	/* 将 PA10 映射为 USART1_RX */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

	/* 配置 USART Tx 为复用功能 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	/* 输出类型为推挽 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	/* 内部上拉电阻使能 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	/* 复用模式 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* 配置 USART Rx 为复用功能 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*
*********************************************************************************************************
*	函 数 名: InitUartMode
*	功能说明: 配置USART的工作模式为收发全双工，115200,8N1，配置为DMA传输，开启发送完成中断，接收空闲中断
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void InitUartMode(void)
{
	USART_InitTypeDef USART_InitStructure;
	
	USART_DeInit(USART1);//复位串口1设置
	
	/*  配置串口硬件参数为8N1,无流控，TX RX双工 */
 	USART_InitStructure.USART_BaudRate = UART1_BAUD;	
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_ITConfig(USART1, USART_IT_TC, DISABLE); 
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);		/* 使能串口 */
	
	/*STM32会发一个空帧作为第一次数据发送*/
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET)
  {
        ; //等待空闲帧发送完成后  再清零发送标志
  }
 
	USART_ClearFlag(USART1,USART_FLAG_TC);
	USART_ClearITPendingBit(USART1, USART_IT_TC);//清除标志位
	
	/*  使能串口 配置为DMA收发，空闲中断 */
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);//开启空闲中断，一旦接收过程中数据断流即表示接收完成
	//USART_ITConfig(USART1, USART_IT_TC, ENABLE);  //开启发送完成中断	
	USART_DMACmd(USART1,USART_DMAReq_Tx|USART_DMAReq_Rx,ENABLE);//开启串口DMA收发模式
}

/*
*********************************************************************************************************
*	函 数 名: InitDMAUart
*	功能说明: 配置USART的DMA操作，将USART1的TX及RX分别使用DMA进行传输，开启DMA完成中断
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void InitDMAUart(void)
{
  //DMA初始化结构体
	DMA_InitTypeDef DMA_InitStructure;
	//用于DMA中断配置
	NVIC_InitTypeDef NVIC_InitStructure;
	// 开启DMA时钟
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 , ENABLE);
	
	/*RXDMA设置*/
	DMA_DeInit(UART_RX_DMA_STREAM);//复位RX数据流设置
	while(DMA_GetCmdStatus(UART_RX_DMA_STREAM)!=DISABLE);//等待DMA可配置
	
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;                   //通道选择，通道4数据流5是USART1_RX
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART1->DR;     //设置DMA外设基址为USART1数据寄存器
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)g_RxBuf;           //接收内存基址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;          //数据传输方向，从外设读取发送到内存
	DMA_InitStructure.DMA_BufferSize = DMA_RECV_LEN;                 //DMA接收缓存大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设地址不递增
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;          // 存储器地址递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外设数据为8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 //存储器数据大小也8位	
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                    // 正常传输模式,禁止循环 
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;              // DMA 传输通道优先级为高
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;           // 禁止DMA FIFO	，使用直连模式 
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;    //禁用FIFO时本行及以下两条无意义
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;      //存储器突发单次传输
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
	DMA_Init(UART_RX_DMA_STREAM, &DMA_InitStructure);                //初始化DMA流
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream5_IRQn;          //接收DMA中断号
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=7;          //抢占优先级为7，数值越小优先级越高
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
  DMA_Cmd(UART_RX_DMA_STREAM, ENABLE);                             // 使能DMA流
	//DMA_ITConfig(UART_RX_DMA_STREAM,DMA_IT_TC,ENABLE);               //开启DMA接收中断（数据从串口到接收缓存转移完毕中断）
	 
	/*TXDMA设置*/
	DMA_DeInit(UART_TX_DMA_STREAM);//复位TX数据流设置
	while (DMA_GetCmdStatus(UART_TX_DMA_STREAM) != DISABLE);         //等待DMA可配置
	
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;                   //通道选择，通道4数据流7是USART1_TX
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART1->DR;     //设置DMA外设基址为USART1数据寄存器
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)g_TxBuf;            //发送内存基址
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;          //数据传输方向，从内存发送到外设
	DMA_InitStructure.DMA_BufferSize = 0;                            //DMA发送数据大小需要根据数据包实时修改,这里先写0
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设地址不递增
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;          // 存储器地址递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外设数据为8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 //存储器数据大小也8位	
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                    // 正常传输模式,禁止循环 
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;              // DMA 传输通道优先级为高
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;           // 禁止DMA FIFO	，使用直连模式
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;      //存储器突发单次传输
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
	DMA_Init(UART_TX_DMA_STREAM, &DMA_InitStructure);                //初始化DMA流
	
  NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn;          //发送DMA中断号
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 8;        //抢占优先级为8
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
	NVIC_Init(&NVIC_InitStructure);  
	//DMA_ITConfig(UART_TX_DMA_STREAM,DMA_IT_TC,ENABLE);               //使能DMA发送中断（数据从发送缓存转移到串口）
	/*发送此处先不使能，需要发送时再打开*/
  //DMA_Cmd(UART_TX_DMA_STREAM, ENABLE);                             // 使能DMA流
}

/*
*********************************************************************************************************
*	函 数 名: ConfigUartNVIC
*	功能说明: 配置串口硬件中断，分组4，全部为抢占优先级，无子优先级
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static void ConfigUartNVIC(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
 
	/* 使能串口1中断 */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=8;//抢占优先级为8，数值越小优先级越高
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*
*********************************************************************************************************
*	函 数 名: bsp_InitUart
*	功能说明: 调用上述三个函数，进行初始化串口硬件，并对全局变量赋初值.
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void RS485_UART_Init(void)
{ //uint8_t strlen;
	//uint8_t comInfo[40];
	
	InitUartVar();    /* 配置串口的全局变量 */
	
	InitUartGPIO();		/* 配置串口的GPIO */
	
	InitUartMode();   /* 配置串口的工作模式*/
	
	ConfigUartNVIC();	/* 配置串口中断 */

	InitDMAUart();    /* 配置DMA相关通道 */	
	
	//strlen=sprintf((char*)comInfo,"USATR1 Init OK\n");//串口助手看不到这个信息说明串口有问题
	//UartSendBuf(comInfo, strlen);
}


/*
*********************************************************************************************************
*	函 数 名: DmaReceiveReopen
*	功能说明: 重启串口接受的DMA通道，用于串口接收到数据处理完后重新打开
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void DmaReceiveReopen(void)
{ 
	/*清DMA标记位*/
	DMA_ClearFlag(UART_RX_DMA_STREAM,DMA_FLAG_TCIF5 | DMA_FLAG_FEIF5 | DMA_FLAG_DMEIF5 | DMA_FLAG_TEIF5 | DMA_FLAG_HTIF5);//清除DMA2_Steam5传输完成标志
  /*重填接收长度*/
	DMA_SetCurrDataCounter(UART_RX_DMA_STREAM, UART1_RX_BUF_SIZE);
	/*打开DMA*/
	DMA_Cmd(UART_RX_DMA_STREAM, ENABLE);     //打开DMA,	
}
	
/*
*********************************************************************************************************
*	函 数 名: DmaSendDataProc
*	功能说明: 填写数据到UART发送缓冲区,并启动发送中断。中断处理函数发送完毕后，自动关闭发送中断
*	形    参: 无
*	返 回 值: 执行结果
*********************************************************************************************************
*/
uint8_t DmaSendDataProc(void)
{

	DMA_Cmd(UART_TX_DMA_STREAM, DISABLE);                      //关闭DMA传输 
	
	if(DMA_GetCmdStatus(UART_TX_DMA_STREAM) != DISABLE)
	{
		return 1;//DMA无法设置时直接返回，不阻塞
	} 
		
	DMA_SetCurrDataCounter(UART_TX_DMA_STREAM,UART1_TransmitSize);          //数据传输量  
  /*开启DMA*/
	DMA_Cmd(UART_TX_DMA_STREAM, ENABLE);
	/*使能串口发送寄存器空中断，这里不打开发送完成中断*/
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	
	return 0;
}

/*
*********************************************************************************************************
*	函 数 名: UartSendBuf
*	功能说明: 串口buf发送函数
*	形    参: 发送数据指针，数据长度
*	返 回 值: ret值
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
*	函 数 名: UartSendChar
*	功能说明: 该函数待完善
*	形    参: ucByte: 待发送的数据
*	返 回 值: ret值
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
*	函 数 名: USART1_IRQHandler  
*	功能说明: USART中断服务程序，名称不能修改，否则不能重载startup_stm32f429_439xx.s中的同名函数
            处理收到、发送完成、发送寄存器空中断
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void USART1_IRQHandler(void)
{
	//uint16_t data;
	//USART1接收空闲中断被置位，表明本次接收完成
	if(USART_GetITStatus(USART1,USART_IT_IDLE) != RESET)
	{
		DMA_Cmd(UART_RX_DMA_STREAM, DISABLE); //关闭DMA,防止处理其间有数据
    /*
		//必须依照下面顺序读取SR DR以清除IDLE标志位
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
	
	//USART1发送缓冲区空
	if(USART_GetITStatus(USART1,USART_IT_TXE)!= RESET)
	{
		/* 发送缓冲区的数据已取完时， 禁止发送缓冲区空中断 （注意：此时最后1个数据还未真正发送完毕）*/
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
			
		/* 使能数据发送完毕中断 */
			USART_ITConfig(USART1, USART_IT_TC, ENABLE);
	}
	
	//USART1发送完成中断被置位，表明本次发送结束
	if(USART_GetITStatus(USART1,USART_IT_TC)!= RESET)
	{
	    USART_ClearITPendingBit(USART1, USART_IT_TC);//清除标志位
		
		 /*关闭发送完成中断*/
		  USART_ITConfig(USART1, USART_IT_TC, DISABLE);
		
		 //关闭串口发送DMA通道
	    DMA_Cmd(UART_TX_DMA_STREAM, DISABLE); 
		  //数据长度清零
		  UART_TX_DMA_STREAM->NDTR=0;
		  //清除DMA2_Steam7传输完成标志
		  DMA_ClearFlag(UART_TX_DMA_STREAM,DMA_FLAG_TCIF7);
	   	
			memset(g_TxBuf,0,UART1_TX_BUF_SIZE*sizeof(uint8_t));
			UART1_TransmitSize=0;

		//发送完成事件标志处理		
	    if(HandleTaskUartTsmt)
			{
				//isr_evt_set(event_uart_transmitted ,HandleTaskUartTsmt);
				isr_sem_send(UartTrans);			
			}			
	}
}

/*
*********************************************************************************************************
*	函 数 名: fputc
*	功能说明: 重定义putc函数，这样可以使用printf函数从串口1打印输出
            由于采用了DMA，单字节传输效率低，因此最好不要使用本函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
int fputc(int ch, FILE *f)
{
	UartSendChar(ch);
	return ch;
}


