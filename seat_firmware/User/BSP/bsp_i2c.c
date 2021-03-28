#include "bsp_i2c.h"
#include "stdio.h"
#include "stm32f4xx_gpio.h"
/*
*********************************************************************************************************
*
*	模块名称 : I2C模块(For F429 180M)
*	文件名称 : bsp_i2c.c
*	版    本 : V1.0
*	说    明 : 本模块使用GPIO模拟I2C时序，实现对NVM的读写，
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2019-04-24  WLM      草稿
*
*********************************************************************************************************
*/

/*
PB6   SCL
PB7   SDA
PB8   WP
*/
/* 定义I2C总线连接的GPIO端口 */
#define GPIO_PORT_I2C	GPIOB			              /* GPIO端口 */
#define RCC_I2C_PORT 	RCC_AHB1Periph_GPIOB		/* GPIO端口时钟 */
#define I2C_SCL_PIN		GPIO_Pin_6			        /* 连接到SCL时钟线的GPIO */
#define I2C_SDA_PIN		GPIO_Pin_7			        /* 连接到SDA数据线的GPIO */
#define I2C_WP_PIN    GPIO_Pin_8              /* 连接到WP */

/* 定义读写SCL和SDA的宏 */
#define I2C_SCL_1()  GPIO_PORT_I2C->BSRRL = I2C_SCL_PIN				/* SCL = 1 */
#define I2C_SCL_0()  GPIO_PORT_I2C->BSRRH = I2C_SCL_PIN				/* SCL = 0 */

#define I2C_SDA_1()  GPIO_PORT_I2C->BSRRL = I2C_SDA_PIN				/* SDA = 1 */
#define I2C_SDA_0()  GPIO_PORT_I2C->BSRRH = I2C_SDA_PIN				/* SDA = 0 */

#define I2C_WP_ON()  GPIO_PORT_I2C->BSRRL = I2C_WP_PIN				/* WP = 1 */
#define I2C_WP_OFF() GPIO_PORT_I2C->BSRRH = I2C_WP_PIN				/* WP = 0 */

#define I2C_SDA_READ()  ((GPIO_PORT_I2C->IDR & I2C_SDA_PIN) != 0)	/* 读SDA口线状态 */
#define I2C_SCL_READ()  ((GPIO_PORT_I2C->IDR & I2C_SCL_PIN) != 0)	/* 读SCL口线状态 */

#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))

/*FRAM初始化成功标记*/
uint8_t Flag_NVM_OK=0;


//uint8_t  RecordBuf[128];        //记录缓存
/*
*********************************************************************************************************
*	函 数 名: i2c_Delay
*	功能说明: 软延时，调整i数值使总线速度接近400KHz即可，注意SCL上升沿时间还与上拉电阻有关
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void i2c_Delay(void)
{
	uint8_t i;

	for (i = 0; i < 32; i++);
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Start
*	功能说明: CPU发起I2C总线启动信号
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_Start(void)
{
	/* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
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
*	函 数 名: i2c_Start
*	功能说明: CPU发起I2C总线停止信号
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_Stop(void)
{
	/* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
	I2C_SDA_0();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SDA_1();
	i2c_Delay();
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Init
*	功能说明: 配置I2C总线的3个GPIO
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_I2C_PORT, ENABLE);	/* 打开GPIO时钟 */

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		/* 设为输出口 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;		/* 设为开漏模式 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* 上下拉电阻不使能 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;	/* IO口最大速度 */

	GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN |  I2C_SDA_PIN | I2C_WP_PIN  ;
	GPIO_Init(GPIO_PORT_I2C, &GPIO_InitStructure);

	/* 给一个停止信号, 复位I2C总线上的所有设备到待机模式 */
	i2c_Stop();
}

/*
*********************************************************************************************************
*	函 数 名: i2c_SendByte
*	功能说明: CPU向I2C总线设备发送8bit数据
*	形    参:  _ucByte ： 等待发送的字节
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_SendByte(uint8_t _ucByte)
{
	uint8_t i;

	/* 先发送字节的高位bit7 */
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
			 I2C_SDA_1(); // 释放总线
		}
		_ucByte <<= 1;	/* 左移一个bit */
		i2c_Delay();
	}
}

/*
*********************************************************************************************************
*	函 数 名: i2c_ReadByte
*	功能说明: CPU从I2C总线设备读取8bit数据
*	形    参:  无
*	返 回 值: 读到的数据
*********************************************************************************************************
*/
uint8_t i2c_ReadByte(void)
{
	uint8_t i;
	uint8_t value;

	/* 读到第1个bit为数据的bit7 */
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
*	函 数 名: i2c_WaitAck
*	功能说明: CPU产生一个时钟，并读取器件的ACK应答信号
*	形    参:  无
*	返 回 值: 返回0表示正确应答，1表示无器件响应
*********************************************************************************************************
*/
uint8_t i2c_WaitAck(void)
{
	uint8_t re;

	I2C_SDA_1();	/* CPU释放SDA总线 */
	i2c_Delay();
	I2C_SCL_1();	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
	i2c_Delay();
	if (I2C_SDA_READ())	/* CPU读取SDA口线状态 */
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
*	函 数 名: i2c_Ack
*	功能说明: CPU产生一个ACK信号
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_Ack(void)
{
	I2C_SDA_0();	/* CPU驱动SDA = 0 */
	i2c_Delay();
	I2C_SCL_1();	/* CPU产生1个时钟 */
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
	I2C_SDA_1();	/* CPU释放SDA总线 */
}

/*
*********************************************************************************************************
*	函 数 名: i2c_NAck
*	功能说明: CPU产生1个NACK信号
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_NAck(void)
{
	I2C_SDA_1();	/* CPU驱动SDA = 1 */
	i2c_Delay();
	I2C_SCL_1();	/* CPU产生1个时钟 */
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
}

/*
*********************************************************************************************************
*	函 数 名: i2c_CheckDevice
*	功能说明: 检测I2C总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在
*	形    参:  DeviceAddress：设备的I2C总线地址
*	返 回 值: 返回值 0 表示正确， 返回1表示未探测到
*********************************************************************************************************
*/
uint8_t i2c_CheckDevice(uint8_t DeviceAddress)
{
	uint8_t ucAck;

	if (I2C_SDA_READ() && I2C_SCL_READ())
	{
		i2c_Start();		/* 发送启动信号 */

		i2c_SendByte(DeviceAddress); /*发送设备地址*/
		
		ucAck = i2c_WaitAck();	/* 检测设备的ACK应答 */

		i2c_Stop();			/* 发送停止信号 */

		return ucAck;
	}
	return 1;	/* I2C总线异常 */
}

/*
*********************************************************************************************************
*	函 数 名: NVM_I2C_Init
*	功能说明: 初始化I2C总线，检测FRAM设备是否可以正常连接上
*	形    参: 无
*	返 回 值: 返回值 1 表示正确， 返回0表示异常
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
*	函 数 名: NVM_I2C_Write
*	功能说明: 向NVM指定地址写入若干数据
*	形    参:  DistAddress : 起始地址
*			       ByteNum : 数据长度，单位为字节
*			       dataWrite : 存放待写数据的缓冲区指针
*	返 回 值: 0 表示失败，1表示成功
*********************************************************************************************************
*/
uint8_t NVM_I2C_Write(uint8_t* dataWrite,uint16_t DistAddress,uint16_t ByteNum)
{
	uint16_t i,m;
  uint8_t addr_H,addr_L;
	
	for (i = 0; i < ByteNum; i++)
	{
		/* 当发送第1个字节时，需要先发启动信号和地址 */
		if (i == 0)
		{
			/*　第０步：发停止信号，启动内部写操作　*/
			i2c_Stop();

			/* 通过检查器件应答的方式，判断内部写操作是否完成, 一般小于 10ms
				CLK频率为200KHz时，查询次数为30次左右
			*/
			for (m = 0; m < 1000; m++)
			{
				/* 第1步：发起I2C总线启动信号 */
				i2c_Start();

				/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
				i2c_SendByte(FRAM_I2C_WRITE_ADDR);	/* 此处是写指令 */

				/* 第3步：发送一个时钟，判断器件是否正确应答 */
				if (i2c_WaitAck() == 0)
				{
					break;
				}
			}
			
			if (m  == 1000)
			{
				goto cmd_fail;	/* 器件写超时 */
			}

			/* 第4步：发送字节地址，2次,先高后低 */
			addr_H=BYTE1(DistAddress);
			i2c_SendByte(addr_H);
			
			if (i2c_WaitAck() != 0)
			{
					goto cmd_fail;	/* 器件无应答 */
			}
			
      addr_L=BYTE0(DistAddress);
			i2c_SendByte(addr_L);
				
			if (i2c_WaitAck() != 0)
			{
					goto cmd_fail;	/* 器件无应答 */
			}
		}
		
		/* 第6步：开始写入数据 */
		i2c_SendByte(*(dataWrite+i));

		/* 第7步：等待ACK */
		if (i2c_WaitAck() != 0)
		{
			goto cmd_fail;	/* 器件无应答 */
		}
		
	}

	/* 命令执行成功，发送I2C总线停止信号 */
	i2c_Stop();
	return 1;

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	i2c_Stop();
	return 0;
}

/*
*********************************************************************************************************
*	函 数 名: NVM_I2C_Read
*	功能说明: 从NVM指定地址处开始读取若干数据
*	形    参: DistAddress : 起始地址
*			      ByteNum : 数据长度，单位为字节
*			      dataRead : 存放读到的数据的缓冲区指针
*	返 回 值: 0 表示失败，1表示成功
*********************************************************************************************************
*/
uint8_t NVM_I2C_Read(uint8_t* dataRead,uint16_t DistAddress,uint16_t ByteNum)
{
	uint16_t i;
  uint8_t addr_H,addr_L;
	
	/* 第1步：发起I2C总线启动信号 */
	i2c_Start();

	/* 第2步：发送设备地址（写） */
	i2c_SendByte(FRAM_I2C_WRITE_ADDR);	/* 此处是写指令 */

	/* 第3步：等待ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* 器件无应答 */
	}

	/* 第4步：写入要读的地址 */
	{ 
		addr_H=BYTE1(DistAddress);
		i2c_SendByte(addr_H);
		
		if (i2c_WaitAck() != 0)
		{
			goto cmd_fail;	/* 器件无应答 */
		}

		addr_L=BYTE0(DistAddress);
		i2c_SendByte(addr_L);
		
		if (i2c_WaitAck() != 0)
		{
			goto cmd_fail;	/* 器件无应答 */
		}
	}

	/* 第6步：重新启动I2C总线。下面开始读取数据 */
	i2c_Start();

	/* 第7步：发起读命令 */
	i2c_SendByte(FRAM_I2C_READ_ADDR);	/* 此处是读指令 */

	/* 第8步：等待ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* 器件无应答 */
	}

	/* 第9步：循环读取数据 */
	for (i = 0; i < ByteNum; i++)
	{
		*(dataRead+i) = i2c_ReadByte();	/* 读1个字节 */

		/* 每读完1个字节后，需要发送Ack， 最后一个字节不需要Ack，发Nack */
		if (i != ByteNum - 1)
		{
			i2c_Ack();	/* 中间字节读完后，CPU产生ACK信号(驱动SDA = 0) */
		}
		else
		{
			i2c_NAck();	/* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
		}
	}
	/* 发送I2C总线停止信号 */
	i2c_Stop();
	return 1;	/* 执行成功 */

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	i2c_Stop();
	return 0;
}

/*-----------------------------------------------------------------------------------------------
Record格式
0-3字节（4）   4-127字节（124=31*4）
编号           4条数据信息

数据信息格式
0-5字节（6） 第6字节（1）   7-10字节（4）  11-14字节（4）   15-22字节（8）  23-30字节（8）
时间戳        座椅状态      电机芯片状态   电机运行方向*4   电机位置ADC*4   电机电流ADC*4    
--------------------------------------------------------------------------------------------------
*/
/*
*********************************************************************************************************
*	函 数 名: SeekRecord
*	功能说明: 遍历NVM，寻找上次记录的位置及编号，以便从接下来的位置开始写入
*	形    参: RecordNum
*			      RecordStorageBlockNum
*			      
*	返 回 值: 0 表示失败，1表示成功
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
	 
	 /*编号值过0检测*/
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
*	函 数 名: WriteRecord
*	功能说明: 在指定Block处写入一条Record
*	形    参: RecordStorageBlockNum（Block号，0-63）
*			      RecordHeader（数据首地址，从此开始128字节）
*			      
*	返 回 值: 0 表示失败，1表示成功
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
