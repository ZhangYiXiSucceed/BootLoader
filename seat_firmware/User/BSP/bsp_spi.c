#include "bsp_spi.h"
#include "stdio.h"



/*
*********************************************************************************************************
*	函 数 名: SPI_Delay
*	功能说明: SPI 片选线拉低后到时钟产生中的维持时间延时
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void SPI_Delay(void)
{
	uint16_t SPI_HoldTime=100;
	
	while(SPI_HoldTime--)
  {
		//DRV8873要求片选线的建立及接触延时为至少50ns
  }
}

/*
*********************************************************************************************************
*	函 数 名: SPI_Sellect
*	功能说明: SPI 片选
*	形    参: SPI片选线编号 1 2 3 4
*	返 回 值: 无
*********************************************************************************************************
*/
void SPI_Sellect(uint8_t ChipNum)
{
	switch(ChipNum)
	{
		case 0:   MOTOR_CS1_LOW(); 
		          break;			
		case 1:   MOTOR_CS2_LOW(); 
		          break;		
		case 2:   MOTOR_CS3_LOW(); 
		          break;		
		case 3:   MOTOR_CS4_LOW(); 
		          break;			
		default: break;
	}
	
	SPI_Delay();	
}

/*
*********************************************************************************************************
*	函 数 名: SPI_UnSellect
*	功能说明: SPI 释放
*	形    参: SPI片选线编号 1 2 3 4
*	返 回 值: 无
*********************************************************************************************************
*/
void SPI_UnSellect(uint8_t ChipNum)
{
	
	SPI_Delay();
	
	switch(ChipNum)
	{
		case 0:   MOTOR_CS1_HIGH(); 
		          break;			
		case 1:   MOTOR_CS2_HIGH(); 
		          break;		
		case 2:   MOTOR_CS3_HIGH(); 
		          break;		
		case 3:   MOTOR_CS4_HIGH(); 
		          break;			
		default: break;
	}	
}

/*
*********************************************************************************************************
*	函 数 名: Motor_SPI_Init
*	功能说明: 初始化DRV8873所连接的SPI3外设及片选线
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void Motor_SPI_Init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* 使能 SPI3所用3个引脚时钟*/
  RCC_AHB1PeriphClockCmd (MOTOR_SPI_SCK_GPIO_CLK | MOTOR_SPI_MISO_GPIO_CLK | MOTOR_SPI_MOSI_GPIO_CLK , ENABLE);
	
	/* 使能 4片 DRV8873所用4个CS引脚时钟，CS2与3都在GPIOD*/
  RCC_AHB1PeriphClockCmd (MOTOR_CS1_GPIO_CLK | MOTOR_CS2_GPIO_CLK | MOTOR_CS4_GPIO_CLK , ENABLE);

  /* 使能SPI3外设时钟 */
  MOTOR_SPI_CLK_INIT (MOTOR_SPI_CLK , ENABLE);
 
  //设置SPI引脚复用
  GPIO_PinAFConfig(MOTOR_SPI_SCK_GPIO_PORT ,MOTOR_SPI_SCK_PINSOURCE,MOTOR_SPI_SCK_AF); 
	GPIO_PinAFConfig(MOTOR_SPI_MISO_GPIO_PORT,MOTOR_SPI_MISO_PINSOURCE,MOTOR_SPI_MISO_AF); 
	GPIO_PinAFConfig(MOTOR_SPI_MOSI_GPIO_PORT,MOTOR_SPI_MOSI_PINSOURCE,MOTOR_SPI_MOSI_AF); 
  
  /* 配置SCK引脚 */
  GPIO_InitStructure.GPIO_Pin = MOTOR_SPI_SCK_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//外设复用
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
  
  GPIO_Init(MOTOR_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);
  
	/* 配置MISO引脚 */
  GPIO_InitStructure.GPIO_Pin = MOTOR_SPI_MISO_PIN;
  GPIO_Init(MOTOR_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);
  
	/* 配置MOSI引脚 */
  GPIO_InitStructure.GPIO_Pin = MOTOR_SPI_MOSI_PIN;
  GPIO_Init(MOTOR_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);  

	/* 配置 4个CS引脚 */
	/*CS1*/
  GPIO_InitStructure.GPIO_Pin =  MOTOR_CS1_PIN; 
	MOTOR_CS1_GPIO_PORT->BSRRL=MOTOR_CS1_PIN;//先将输出数据寄存器设置为输出高电平
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//再设置模式为输出，防止一初始化就输出0
	GPIO_Init(MOTOR_CS1_GPIO_PORT, &GPIO_InitStructure);
  /*CS2*/
  GPIO_InitStructure.GPIO_Pin =  MOTOR_CS2_PIN ;
	MOTOR_CS2_GPIO_PORT->BSRRL=MOTOR_CS2_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(MOTOR_CS2_GPIO_PORT, &GPIO_InitStructure);
	
	/*CS3*/
  GPIO_InitStructure.GPIO_Pin =  MOTOR_CS3_PIN;
	MOTOR_CS3_GPIO_PORT->BSRRL=MOTOR_CS3_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(MOTOR_CS3_GPIO_PORT, &GPIO_InitStructure);
	
	/*CS4*/
  GPIO_InitStructure.GPIO_Pin =  MOTOR_CS4_PIN;
	MOTOR_CS4_GPIO_PORT->BSRRL=MOTOR_CS4_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(MOTOR_CS4_GPIO_PORT, &GPIO_InitStructure);
	
	
  /* 所有片选拉高 */
   MOTOR_CS1_HIGH();  
	 MOTOR_CS2_HIGH(); 
	 MOTOR_CS3_HIGH(); 
	 MOTOR_CS4_HIGH(); 

  /* SPI 外设模式配置 */
  /*SPI方向设置，2线全双工*/
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	/*SPI模式设置，STM32为主控*/
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	/*SPI数据帧长度，DRV8873要求为16位*/
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
	/*SPI时钟极性，DRV8873要求空闲时SCK为低电平，CPOL=0*/
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	/*SPI时钟相位，DRV8873在SCK第二个边沿（偶数边沿）进行采样，CPHA=1*/
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	/*SPI片选模式，软件控制*/
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	/*SPI波特率预分频，DRV8873最高时钟不超过10M，SPI3 fpclk=45M，此处16分频*/
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	/*SPI位先行模式，DRV8873要求MSB先行*/
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	/*CRC多项式寄存器，复位后为7，本程序不使用*/
  SPI_InitStructure.SPI_CRCPolynomial = 7;
	
	 /* 初始化 SPI  */
  SPI_Init(MOTOR_SPI, &SPI_InitStructure);

  /* 使能 SPI  */
  SPI_Cmd(MOTOR_SPI, ENABLE);	
}

/*
*********************************************************************************************************
*	函 数 名: Motor_SPI_ChipSelect
*	功能说明: DRV8873片选
*	形    参: ChipNum 0-3，电机1-4的芯片选择
*	返 回 值: 无
*********************************************************************************************************
*/
void Motor_SPI_ChipSelect(uint8_t ChipNum)
{
	switch(ChipNum)
	{
		case 0:MOTOR_CS1_LOW(); 
		       break;
		case 1:MOTOR_CS2_LOW(); 
		       break;
    case 2:MOTOR_CS3_LOW(); 
		       break;
    case 3:MOTOR_CS4_LOW(); 
		       break;
		default:break;
	}
}

/*
*********************************************************************************************************
*	函 数 名: Motor_SPI_ChipDiselect
*	功能说明: DRV8873弃选
*	形    参: ChipNum 0-3，电机1-4的芯片选择
*	返 回 值: 无
*********************************************************************************************************
*/
void Motor_SPI_ChipDiselect(uint8_t ChipNum)
{
	switch(ChipNum)
	{
		case 0:MOTOR_CS1_HIGH(); 
		       break;
		case 1:MOTOR_CS2_HIGH(); 
		       break;
    case 2:MOTOR_CS3_HIGH(); 
		       break;
    case 3:MOTOR_CS4_HIGH(); 
		       break;
		default:break;
	}
}
/*
*********************************************************************************************************
*	函 数 名: Motor_SPI_WriteWord
*	功能说明: 向SPI总线写一个字的数据，由于SPI的同步性，写的同时实际上已经进行了读，此函数不考虑片选
*	形    参: Data指针，指向待写入的16位数据的地址
*	返 回 值: 16位数据
*********************************************************************************************************
*/
uint16_t Motor_SPI_WriteWord(uint16_t* Data)
{
	uint32_t SPI_Timeout=SPI_WAIT_TIME;
	
	
	  /* 等待发送缓冲区为空，TXE事件 */
  while (SPI_I2S_GetFlagStatus(MOTOR_SPI , SPI_I2S_FLAG_TXE) == RESET)
   {
    if((SPI_Timeout--) == 0) 
		{
			return 0;//正常DRV8873输出不可能等于0
		}
   }
	 
	 /* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
	SPI_I2S_SendData(MOTOR_SPI, *Data);
	 
	SPI_Timeout=SPI_WAIT_TIME; 
	 
	 /* 等待接收缓冲区非空，RXNE事件 */
  while (SPI_I2S_GetFlagStatus(MOTOR_SPI, SPI_I2S_FLAG_RXNE) == RESET)
   {
    if((SPI_Timeout--) == 0)
		{			
			return 0;//正常DRV8873输出不可能等于0	
		}
   }
	 
	 return SPI_I2S_ReceiveData(MOTOR_SPI);
}

/*
*********************************************************************************************************
*	函 数 名: Motor_GetChipStatus
*	功能说明: 获取DRV8873的状态
*	形    参: ChipNum，芯片编号，0-3
*	返 回 值: 8位状态
*********************************************************************************************************
*/
uint8_t Motor_GetChipStatus(uint8_t ChipNum)
{ 
  uint16_t SPI_ReadData,SPI_WriteData;
	uint8_t FaultStatus=0xFF;
	
	Motor_SPI_ChipSelect(ChipNum);
			
	SPI_WriteData=MotorCMD_Read_FaultStatus;//0x4000
	
	SPI_ReadData=Motor_SPI_WriteWord(&SPI_WriteData);
	
	
	FaultStatus=(*(uint8_t*)(&SPI_ReadData));
	
	Motor_SPI_ChipDiselect(ChipNum);
	
	return FaultStatus;
}
