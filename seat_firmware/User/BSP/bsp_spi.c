#include "bsp_spi.h"
#include "stdio.h"



/*
*********************************************************************************************************
*	�� �� ��: SPI_Delay
*	����˵��: SPI Ƭѡ�����ͺ�ʱ�Ӳ����е�ά��ʱ����ʱ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void SPI_Delay(void)
{
	uint16_t SPI_HoldTime=100;
	
	while(SPI_HoldTime--)
  {
		//DRV8873Ҫ��Ƭѡ�ߵĽ������Ӵ���ʱΪ����50ns
  }
}

/*
*********************************************************************************************************
*	�� �� ��: SPI_Sellect
*	����˵��: SPI Ƭѡ
*	��    ��: SPIƬѡ�߱�� 1 2 3 4
*	�� �� ֵ: ��
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
*	�� �� ��: SPI_UnSellect
*	����˵��: SPI �ͷ�
*	��    ��: SPIƬѡ�߱�� 1 2 3 4
*	�� �� ֵ: ��
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
*	�� �� ��: Motor_SPI_Init
*	����˵��: ��ʼ��DRV8873�����ӵ�SPI3���輰Ƭѡ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Motor_SPI_Init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* ʹ�� SPI3����3������ʱ��*/
  RCC_AHB1PeriphClockCmd (MOTOR_SPI_SCK_GPIO_CLK | MOTOR_SPI_MISO_GPIO_CLK | MOTOR_SPI_MOSI_GPIO_CLK , ENABLE);
	
	/* ʹ�� 4Ƭ DRV8873����4��CS����ʱ�ӣ�CS2��3����GPIOD*/
  RCC_AHB1PeriphClockCmd (MOTOR_CS1_GPIO_CLK | MOTOR_CS2_GPIO_CLK | MOTOR_CS4_GPIO_CLK , ENABLE);

  /* ʹ��SPI3����ʱ�� */
  MOTOR_SPI_CLK_INIT (MOTOR_SPI_CLK , ENABLE);
 
  //����SPI���Ÿ���
  GPIO_PinAFConfig(MOTOR_SPI_SCK_GPIO_PORT ,MOTOR_SPI_SCK_PINSOURCE,MOTOR_SPI_SCK_AF); 
	GPIO_PinAFConfig(MOTOR_SPI_MISO_GPIO_PORT,MOTOR_SPI_MISO_PINSOURCE,MOTOR_SPI_MISO_AF); 
	GPIO_PinAFConfig(MOTOR_SPI_MOSI_GPIO_PORT,MOTOR_SPI_MOSI_PINSOURCE,MOTOR_SPI_MOSI_AF); 
  
  /* ����SCK���� */
  GPIO_InitStructure.GPIO_Pin = MOTOR_SPI_SCK_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���踴��
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
  
  GPIO_Init(MOTOR_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);
  
	/* ����MISO���� */
  GPIO_InitStructure.GPIO_Pin = MOTOR_SPI_MISO_PIN;
  GPIO_Init(MOTOR_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);
  
	/* ����MOSI���� */
  GPIO_InitStructure.GPIO_Pin = MOTOR_SPI_MOSI_PIN;
  GPIO_Init(MOTOR_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);  

	/* ���� 4��CS���� */
	/*CS1*/
  GPIO_InitStructure.GPIO_Pin =  MOTOR_CS1_PIN; 
	MOTOR_CS1_GPIO_PORT->BSRRL=MOTOR_CS1_PIN;//�Ƚ�������ݼĴ�������Ϊ����ߵ�ƽ
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//������ģʽΪ�������ֹһ��ʼ�������0
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
	
	
  /* ����Ƭѡ���� */
   MOTOR_CS1_HIGH();  
	 MOTOR_CS2_HIGH(); 
	 MOTOR_CS3_HIGH(); 
	 MOTOR_CS4_HIGH(); 

  /* SPI ����ģʽ���� */
  /*SPI�������ã�2��ȫ˫��*/
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	/*SPIģʽ���ã�STM32Ϊ����*/
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	/*SPI����֡���ȣ�DRV8873Ҫ��Ϊ16λ*/
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
	/*SPIʱ�Ӽ��ԣ�DRV8873Ҫ�����ʱSCKΪ�͵�ƽ��CPOL=0*/
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	/*SPIʱ����λ��DRV8873��SCK�ڶ������أ�ż�����أ����в�����CPHA=1*/
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	/*SPIƬѡģʽ���������*/
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	/*SPI������Ԥ��Ƶ��DRV8873���ʱ�Ӳ�����10M��SPI3 fpclk=45M���˴�16��Ƶ*/
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	/*SPIλ����ģʽ��DRV8873Ҫ��MSB����*/
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	/*CRC����ʽ�Ĵ�������λ��Ϊ7��������ʹ��*/
  SPI_InitStructure.SPI_CRCPolynomial = 7;
	
	 /* ��ʼ�� SPI  */
  SPI_Init(MOTOR_SPI, &SPI_InitStructure);

  /* ʹ�� SPI  */
  SPI_Cmd(MOTOR_SPI, ENABLE);	
}

/*
*********************************************************************************************************
*	�� �� ��: Motor_SPI_ChipSelect
*	����˵��: DRV8873Ƭѡ
*	��    ��: ChipNum 0-3�����1-4��оƬѡ��
*	�� �� ֵ: ��
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
*	�� �� ��: Motor_SPI_ChipDiselect
*	����˵��: DRV8873��ѡ
*	��    ��: ChipNum 0-3�����1-4��оƬѡ��
*	�� �� ֵ: ��
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
*	�� �� ��: Motor_SPI_WriteWord
*	����˵��: ��SPI����дһ���ֵ����ݣ�����SPI��ͬ���ԣ�д��ͬʱʵ�����Ѿ������˶����˺���������Ƭѡ
*	��    ��: Dataָ�룬ָ���д���16λ���ݵĵ�ַ
*	�� �� ֵ: 16λ����
*********************************************************************************************************
*/
uint16_t Motor_SPI_WriteWord(uint16_t* Data)
{
	uint32_t SPI_Timeout=SPI_WAIT_TIME;
	
	
	  /* �ȴ����ͻ�����Ϊ�գ�TXE�¼� */
  while (SPI_I2S_GetFlagStatus(MOTOR_SPI , SPI_I2S_FLAG_TXE) == RESET)
   {
    if((SPI_Timeout--) == 0) 
		{
			return 0;//����DRV8873��������ܵ���0
		}
   }
	 
	 /* д�����ݼĴ�������Ҫд�������д�뷢�ͻ����� */
	SPI_I2S_SendData(MOTOR_SPI, *Data);
	 
	SPI_Timeout=SPI_WAIT_TIME; 
	 
	 /* �ȴ����ջ������ǿգ�RXNE�¼� */
  while (SPI_I2S_GetFlagStatus(MOTOR_SPI, SPI_I2S_FLAG_RXNE) == RESET)
   {
    if((SPI_Timeout--) == 0)
		{			
			return 0;//����DRV8873��������ܵ���0	
		}
   }
	 
	 return SPI_I2S_ReceiveData(MOTOR_SPI);
}

/*
*********************************************************************************************************
*	�� �� ��: Motor_GetChipStatus
*	����˵��: ��ȡDRV8873��״̬
*	��    ��: ChipNum��оƬ��ţ�0-3
*	�� �� ֵ: 8λ״̬
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
