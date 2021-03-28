#include "bsp_i2c.h"
#include "stdio.h"
#include "stm32f4xx_gpio.h"
/*
*********************************************************************************************************
*
*	ģ������ : I2Cģ��(For F429 180M)
*	�ļ����� : bsp_i2c.c
*	��    �� : V1.0
*	˵    �� : ��ģ��ʹ��GPIOģ��I2Cʱ��ʵ�ֶ�NVM�Ķ�д��
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2019-04-24  WLM      �ݸ�
*
*********************************************************************************************************
*/

/*
PB6   SCL
PB7   SDA
PB8   WP
*/
/* ����I2C�������ӵ�GPIO�˿� */
#define GPIO_PORT_I2C	GPIOB			              /* GPIO�˿� */
#define RCC_I2C_PORT 	RCC_AHB1Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define I2C_SCL_PIN		GPIO_Pin_6			        /* ���ӵ�SCLʱ���ߵ�GPIO */
#define I2C_SDA_PIN		GPIO_Pin_7			        /* ���ӵ�SDA�����ߵ�GPIO */
#define I2C_WP_PIN    GPIO_Pin_8              /* ���ӵ�WP */

/* �����дSCL��SDA�ĺ� */
#define I2C_SCL_1()  GPIO_PORT_I2C->BSRRL = I2C_SCL_PIN				/* SCL = 1 */
#define I2C_SCL_0()  GPIO_PORT_I2C->BSRRH = I2C_SCL_PIN				/* SCL = 0 */

#define I2C_SDA_1()  GPIO_PORT_I2C->BSRRL = I2C_SDA_PIN				/* SDA = 1 */
#define I2C_SDA_0()  GPIO_PORT_I2C->BSRRH = I2C_SDA_PIN				/* SDA = 0 */

#define I2C_WP_ON()  GPIO_PORT_I2C->BSRRL = I2C_WP_PIN				/* WP = 1 */
#define I2C_WP_OFF() GPIO_PORT_I2C->BSRRH = I2C_WP_PIN				/* WP = 0 */

#define I2C_SDA_READ()  ((GPIO_PORT_I2C->IDR & I2C_SDA_PIN) != 0)	/* ��SDA����״̬ */
#define I2C_SCL_READ()  ((GPIO_PORT_I2C->IDR & I2C_SCL_PIN) != 0)	/* ��SCL����״̬ */

#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))

/*FRAM��ʼ���ɹ����*/
uint8_t Flag_NVM_OK=0;


//uint8_t  RecordBuf[128];        //��¼����
/*
*********************************************************************************************************
*	�� �� ��: i2c_Delay
*	����˵��: ����ʱ������i��ֵʹ�����ٶȽӽ�400KHz���ɣ�ע��SCL������ʱ�仹�����������й�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void i2c_Delay(void)
{
	uint8_t i;

	for (i = 0; i < 32; i++);
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_Start
*	����˵��: CPU����I2C���������ź�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_Start(void)
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C���������ź� */
	I2C_SDA_1();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SDA_0();
	i2c_Delay();
	
	I2C_SCL_0();
	i2c_Delay();
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_Start
*	����˵��: CPU����I2C����ֹͣ�ź�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_Stop(void)
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� */
	I2C_SDA_0();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SDA_1();
	i2c_Delay();
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_Init
*	����˵��: ����I2C���ߵ�3��GPIO
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_I2C_PORT, ENABLE);	/* ��GPIOʱ�� */

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		/* ��Ϊ����� */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;		/* ��Ϊ��©ģʽ */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* ���������費ʹ�� */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;	/* IO������ٶ� */

	GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN |  I2C_SDA_PIN | I2C_WP_PIN  ;
	GPIO_Init(GPIO_PORT_I2C, &GPIO_InitStructure);

	/* ��һ��ֹͣ�ź�, ��λI2C�����ϵ������豸������ģʽ */
	i2c_Stop();
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_SendByte
*	����˵��: CPU��I2C�����豸����8bit����
*	��    ��:  _ucByte �� �ȴ����͵��ֽ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_SendByte(uint8_t _ucByte)
{
	uint8_t i;

	/* �ȷ����ֽڵĸ�λbit7 */
	for (i = 0; i < 8; i++)
	{
		if (_ucByte & 0x80)
		{
			I2C_SDA_1();
		}
		else
		{
			I2C_SDA_0();
		}
		i2c_Delay();
		I2C_SCL_1();
		i2c_Delay();
		I2C_SCL_0();
		if (i == 7)
		{
			 I2C_SDA_1(); // �ͷ�����
		}
		_ucByte <<= 1;	/* ����һ��bit */
		i2c_Delay();
	}
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_ReadByte
*	����˵��: CPU��I2C�����豸��ȡ8bit����
*	��    ��:  ��
*	�� �� ֵ: ����������
*********************************************************************************************************
*/
uint8_t i2c_ReadByte(void)
{
	uint8_t i;
	uint8_t value;

	/* ������1��bitΪ���ݵ�bit7 */
	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		I2C_SCL_1();
		i2c_Delay();
		if (I2C_SDA_READ())
		{
			value++;
		}
		I2C_SCL_0();
		i2c_Delay();
	}
	return value;
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_WaitAck
*	����˵��: CPU����һ��ʱ�ӣ�����ȡ������ACKӦ���ź�
*	��    ��:  ��
*	�� �� ֵ: ����0��ʾ��ȷӦ��1��ʾ��������Ӧ
*********************************************************************************************************
*/
uint8_t i2c_WaitAck(void)
{
	uint8_t re;

	I2C_SDA_1();	/* CPU�ͷ�SDA���� */
	i2c_Delay();
	I2C_SCL_1();	/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
	i2c_Delay();
	if (I2C_SDA_READ())	/* CPU��ȡSDA����״̬ */
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	I2C_SCL_0();
	i2c_Delay();
	return re;
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_Ack
*	����˵��: CPU����һ��ACK�ź�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_Ack(void)
{
	I2C_SDA_0();	/* CPU����SDA = 0 */
	i2c_Delay();
	I2C_SCL_1();	/* CPU����1��ʱ�� */
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
	I2C_SDA_1();	/* CPU�ͷ�SDA���� */
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_NAck
*	����˵��: CPU����1��NACK�ź�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_NAck(void)
{
	I2C_SDA_1();	/* CPU����SDA = 1 */
	i2c_Delay();
	I2C_SCL_1();	/* CPU����1��ʱ�� */
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_CheckDevice
*	����˵��: ���I2C�����豸��CPU�����豸��ַ��Ȼ���ȡ�豸Ӧ�����жϸ��豸�Ƿ����
*	��    ��:  DeviceAddress���豸��I2C���ߵ�ַ
*	�� �� ֵ: ����ֵ 0 ��ʾ��ȷ�� ����1��ʾδ̽�⵽
*********************************************************************************************************
*/
uint8_t i2c_CheckDevice(uint8_t DeviceAddress)
{
	uint8_t ucAck;

	if (I2C_SDA_READ() && I2C_SCL_READ())
	{
		i2c_Start();		/* ���������ź� */

		i2c_SendByte(DeviceAddress); /*�����豸��ַ*/
		
		ucAck = i2c_WaitAck();	/* ����豸��ACKӦ�� */

		i2c_Stop();			/* ����ֹͣ�ź� */

		return ucAck;
	}
	return 1;	/* I2C�����쳣 */
}

/*
*********************************************************************************************************
*	�� �� ��: NVM_I2C_Init
*	����˵��: ��ʼ��I2C���ߣ����FRAM�豸�Ƿ��������������
*	��    ��: ��
*	�� �� ֵ: ����ֵ 1 ��ʾ��ȷ�� ����0��ʾ�쳣
*********************************************************************************************************
*/
uint8_t NVM_I2C_Init(void)
{
  uint8_t FRAM_status=1;
	
	i2c_Init();
	
	FRAM_status=i2c_CheckDevice(FRAM_I2C_WRITE_ADDR);
	
	Flag_NVM_OK=!FRAM_status;
	
	return Flag_NVM_OK;
}

/*
*********************************************************************************************************
*	�� �� ��: NVM_I2C_Write
*	����˵��: ��NVMָ����ַд����������
*	��    ��:  DistAddress : ��ʼ��ַ
*			       ByteNum : ���ݳ��ȣ���λΪ�ֽ�
*			       dataWrite : ��Ŵ�д���ݵĻ�����ָ��
*	�� �� ֵ: 0 ��ʾʧ�ܣ�1��ʾ�ɹ�
*********************************************************************************************************
*/
uint8_t NVM_I2C_Write(uint8_t* dataWrite,uint16_t DistAddress,uint16_t ByteNum)
{
	uint16_t i,m;
  uint8_t addr_H,addr_L;
	
	for (i = 0; i < ByteNum; i++)
	{
		/* �����͵�1���ֽ�ʱ����Ҫ�ȷ������źź͵�ַ */
		if (i == 0)
		{
			/*���ڣ�������ֹͣ�źţ������ڲ�д������*/
			i2c_Stop();

			/* ͨ���������Ӧ��ķ�ʽ���ж��ڲ�д�����Ƿ����, һ��С�� 10ms
				CLKƵ��Ϊ200KHzʱ����ѯ����Ϊ30������
			*/
			for (m = 0; m < 1000; m++)
			{
				/* ��1��������I2C���������ź� */
				i2c_Start();

				/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
				i2c_SendByte(FRAM_I2C_WRITE_ADDR);	/* �˴���дָ�� */

				/* ��3��������һ��ʱ�ӣ��ж������Ƿ���ȷӦ�� */
				if (i2c_WaitAck() == 0)
				{
					break;
				}
			}
			
			if (m  == 1000)
			{
				goto cmd_fail;	/* ����д��ʱ */
			}

			/* ��4���������ֽڵ�ַ��2��,�ȸߺ�� */
			addr_H=BYTE1(DistAddress);
			i2c_SendByte(addr_H);
			
			if (i2c_WaitAck() != 0)
			{
					goto cmd_fail;	/* ������Ӧ�� */
			}
			
      addr_L=BYTE0(DistAddress);
			i2c_SendByte(addr_L);
				
			if (i2c_WaitAck() != 0)
			{
					goto cmd_fail;	/* ������Ӧ�� */
			}
		}
		
		/* ��6������ʼд������ */
		i2c_SendByte(*(dataWrite+i));

		/* ��7�����ȴ�ACK */
		if (i2c_WaitAck() != 0)
		{
			goto cmd_fail;	/* ������Ӧ�� */
		}
		
	}

	/* ����ִ�гɹ�������I2C����ֹͣ�ź� */
	i2c_Stop();
	return 1;

cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
	i2c_Stop();
	return 0;
}

/*
*********************************************************************************************************
*	�� �� ��: NVM_I2C_Read
*	����˵��: ��NVMָ����ַ����ʼ��ȡ��������
*	��    ��: DistAddress : ��ʼ��ַ
*			      ByteNum : ���ݳ��ȣ���λΪ�ֽ�
*			      dataRead : ��Ŷ��������ݵĻ�����ָ��
*	�� �� ֵ: 0 ��ʾʧ�ܣ�1��ʾ�ɹ�
*********************************************************************************************************
*/
uint8_t NVM_I2C_Read(uint8_t* dataRead,uint16_t DistAddress,uint16_t ByteNum)
{
	uint16_t i;
  uint8_t addr_H,addr_L;
	
	/* ��1��������I2C���������ź� */
	i2c_Start();

	/* ��2���������豸��ַ��д�� */
	i2c_SendByte(FRAM_I2C_WRITE_ADDR);	/* �˴���дָ�� */

	/* ��3�����ȴ�ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* ������Ӧ�� */
	}

	/* ��4����д��Ҫ���ĵ�ַ */
	{ 
		addr_H=BYTE1(DistAddress);
		i2c_SendByte(addr_H);
		
		if (i2c_WaitAck() != 0)
		{
			goto cmd_fail;	/* ������Ӧ�� */
		}

		addr_L=BYTE0(DistAddress);
		i2c_SendByte(addr_L);
		
		if (i2c_WaitAck() != 0)
		{
			goto cmd_fail;	/* ������Ӧ�� */
		}
	}

	/* ��6������������I2C���ߡ����濪ʼ��ȡ���� */
	i2c_Start();

	/* ��7������������� */
	i2c_SendByte(FRAM_I2C_READ_ADDR);	/* �˴��Ƕ�ָ�� */

	/* ��8�����ȴ�ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* ������Ӧ�� */
	}

	/* ��9����ѭ����ȡ���� */
	for (i = 0; i < ByteNum; i++)
	{
		*(dataRead+i) = i2c_ReadByte();	/* ��1���ֽ� */

		/* ÿ����1���ֽں���Ҫ����Ack�� ���һ���ֽڲ���ҪAck����Nack */
		if (i != ByteNum - 1)
		{
			i2c_Ack();	/* �м��ֽڶ����CPU����ACK�ź�(����SDA = 0) */
		}
		else
		{
			i2c_NAck();	/* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
		}
	}
	/* ����I2C����ֹͣ�ź� */
	i2c_Stop();
	return 1;	/* ִ�гɹ� */

cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
	i2c_Stop();
	return 0;
}

/*-----------------------------------------------------------------------------------------------
Record��ʽ
0-3�ֽڣ�4��   4-127�ֽڣ�124=31*4��
���           4��������Ϣ

������Ϣ��ʽ
0-5�ֽڣ�6�� ��6�ֽڣ�1��   7-10�ֽڣ�4��  11-14�ֽڣ�4��   15-22�ֽڣ�8��  23-30�ֽڣ�8��
ʱ���        ����״̬      ���оƬ״̬   ������з���*4   ���λ��ADC*4   �������ADC*4    
--------------------------------------------------------------------------------------------------
*/
/*
*********************************************************************************************************
*	�� �� ��: SeekRecord
*	����˵��: ����NVM��Ѱ���ϴμ�¼��λ�ü���ţ��Ա�ӽ�������λ�ÿ�ʼд��
*	��    ��: RecordNum
*			      RecordStorageBlockNum
*			      
*	�� �� ֵ: 0 ��ʾʧ�ܣ�1��ʾ�ɹ�
*********************************************************************************************************
*/
uint8_t SeekRecord(uint32_t* RecordNum,uint8_t* RecordStorageBlockNum)
{ 
	uint32_t RecordNumArray[64];
	uint8_t  RecordNumByte[4];
	uint8_t  index;
  uint32_t MaxRecordNum,MinRecordNum,MaxIndex,MinIndex;
	
	if(!Flag_NVM_OK)
	 {
		 return 0;
	 }
	 
	for(index=0;index<64;index++)
	 {
	   if(index<0x30)
		 {
			 continue;
		 }
		 
		 NVM_I2C_Read(RecordNumByte,BLOCKADDR(index),4);
		 RecordNumArray[index]=*((uint32_t *)RecordNumByte);
	 }

	 MaxRecordNum=RecordNumArray[0];
	 MinRecordNum=RecordNumArray[0];
	 MaxIndex=0;
	 MinIndex=0;
	 
	 for(index=1;index<64;index++)
	 {
	   if(RecordNumArray[index]>MaxRecordNum)
		 {
			 MaxRecordNum=RecordNumArray[index];
			 MaxIndex=index;
		 }
		 
		 if(RecordNumArray[index]<MaxRecordNum)
		 {
		   MinRecordNum=RecordNumArray[index];
			 MinIndex=index;
		 }
	 }
	 
	 /*���ֵ��0���*/
	 if((MaxRecordNum==0xFFFFFFFF-64)&&(MinRecordNum<64))
	 {
		 *RecordNum=MinRecordNum;
     *RecordStorageBlockNum= MinIndex; 
	 }
	 else
	 {
		 *RecordNum=MaxRecordNum;
     *RecordStorageBlockNum= MaxIndex; 
	 }
	 
	 return 1;
}

/*
*********************************************************************************************************
*	�� �� ��: WriteRecord
*	����˵��: ��ָ��Block��д��һ��Record
*	��    ��: RecordStorageBlockNum��Block�ţ�0-63��
*			      RecordHeader�������׵�ַ���Ӵ˿�ʼ128�ֽڣ�
*			      
*	�� �� ֵ: 0 ��ʾʧ�ܣ�1��ʾ�ɹ�
*********************************************************************************************************
*/
uint8_t WriteRecord(uint8_t RecordStorageBlockNum,uint8_t* RecordHeader)
{
     uint16_t RecordAddr;
	   uint8_t ret=0;
	   
	   if(RecordStorageBlockNum>63)
		 {
			 return 0;
		 }
		 
	   RecordAddr=RecordStorageBlockNum*128;
	
	   ret=NVM_I2C_Write(RecordHeader,RecordAddr,128);
	
	   return ret;
}
