/*
*********************************************************************************************************
*
*	模块名称 : 片内Flash操作模块(For F429 180M)
*	文件名称 : bsp_flash.c
*	版    本 : V1.0
*	说    明 : 本模块使用库函数对内部Flash的Sector10进行操作，将其作为座椅标定数据的存储器，主要操作有
             写入记录
						 读取记录
						 擦除记录
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2019-04-24  WLM      草稿
*
*********************************************************************************************************
*/

#include "bsp_flash.h"
#include "stm32f4xx_flash.h"
#include "string.h"
#define  __packed

uint8_t flag_RecordData_OK=0;

uint8_t CheckSum(uint8_t * dataBuf,uint16_t bufLength)
{
	uint8_t sum=0;
	uint16_t index;
	
	for(index=0;index<bufLength;index++)
	{
	   sum+=*(dataBuf+index);
		 sum=sum&0xFF;	
	}
	
	return sum;
}

/*
*********************************************************************************************************
*	函 数 名: Flash_GetSector
*	功能说明: 根据地址计算扇区首地址
*	形    参:  无
*	返 回 值: 扇区首地址
*********************************************************************************************************
*/
uint32_t Flash_GetSector(uint32_t Address)
{
	uint32_t sector = 0;

	if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
	{
		sector = FLASH_Sector_0;
	}
	else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
	{
		sector = FLASH_Sector_1;
	}
	else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
	{
		sector = FLASH_Sector_2;
	}
	else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
	{
		sector = FLASH_Sector_3;
	}
	else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
	{
		sector = FLASH_Sector_4;
	}
	else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
	{
		sector = FLASH_Sector_5;
	}
	else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
	{
		sector = FLASH_Sector_6;
	}
	else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
	{
		sector = FLASH_Sector_7;
	}
	else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
	{
		sector = FLASH_Sector_8;
	}
	else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
	{
		sector = FLASH_Sector_9;
	}
	else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
	{
		sector = FLASH_Sector_10;
	}
	else	/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_11))*/
	{
		sector = FLASH_Sector_11;
	}

	return sector;
}

/*
*********************************************************************************************************
*	函 数 名: Flash_WriteBuf
*	功能说明: 将指定区域内指定长度数据写入指定地址开始的内部Flash，该地址所在的Sector原有数据全部清空
*	形    参: FlashAddr 待写入的Flash地址
            dataBuf   数据存放的首地址
            datalen   数据长度，注意本函数不判断长度是否超过sector大小，调用者要确保该数值小于SectorSize
*	返 回 值: 无
*********************************************************************************************************
*/
void Flash_WriteBuf(uint32_t FlashAddr,uint8_t* dataBuf,uint32_t datalen)
{
   uint8_t index;
	
	__disable_irq();//关闭全局中断响应
	
	/*Flash 解锁*/
	FLASH_Unlock();

  /* 清楚原有标志位 */
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
	
	/*擦除地址对应的Sector内数据，电压等级3*/
	FLASH_EraseSector(Flash_GetSector(FlashAddr), VoltageRange_3);
	
	
	/* 按字节模式编程 */
	for (index = 0; index<datalen; index++)
	{
		FLASH_ProgramByte(FlashAddr++ , *(dataBuf+index));
	}

  	/* Flash 加锁，禁止写Flash控制寄存器 */
  	FLASH_Lock();
	
	 /*开全局中断*/
	 __enable_irq(); 
	
	return ;
}

/*
*********************************************************************************************************
*	函 数 名: Flash_ReadBuf
*	功能说明: 将指定区域内指定长度数据写入指定地址开始的内部Flash，该地址所在的Sector原有数据全部清空
*	形    参: FlashAddr 待写入的Flash地址
            dataBuf   数据存放的首地址
            datalen   数据长度，注意本函数不判断长度是否超过sector大小，调用者要确保该数值小于SectorSize
*	返 回 值: 无
*********************************************************************************************************
*/

uint8_t Flash_ReadBuf(uint32_t FlashAddr,uint8_t* dataBuf,uint32_t datalen)
{
  uint8_t index;
	
	for(index=0;index<datalen;index++)
	{
		*(dataBuf+index)=*((uint8_t*)FlashAddr+index);
	}
	
	return index;
}

/*
*********************************************************************************************************
*	函 数 名: Flash_WriteRecord
*	功能说明: 调用Flash_WriteBuf将1条标定记录写入Sector10首地址
*	形    参: 标定记录结构体指针
*	返 回 值: 无
*********************************************************************************************************
*/
void Flash_WriteRecord(Cbrt_Data_Storage_T *CbrtRecordData,uint16_t* LinkPointArray,Axis_Cbrt_Data_T* AxisCbrtDataArray)
{ 
  memcpy(CbrtRecordData,AxisCbrtDataArray,4*sizeof(Axis_Cbrt_Data_T));
	
	//memcpy(&(CbrtRecordData->Axis_LinkPoint_B1),(u16 *)LinkPointArray,5*sizeof(uint16_t));
	
	CbrtRecordData->Cbrt_Data_CheckSum=CheckSum((uint8_t*)CbrtRecordData,(sizeof(Cbrt_Data_Storage_T)-1));
	
	Flash_WriteBuf(RECORD_DATA_ADDR,(uint8_t*)CbrtRecordData,sizeof(Cbrt_Data_Storage_T));
	
	return ;
}

/*
*********************************************************************************************************
*	函 数 名: Flash_ReadRecord
*	功能说明: 调用Flash_ReadBuf将1条标定记录从Sector10首地址读取出来并校验是否正确
*	形    参: 标定记录结构体指针
*	返 回 值: 读取成功返回1，只有校验通过才认为读取通过，其他情况返回0
*********************************************************************************************************
*/
uint8_t Flash_ReadRecord(Cbrt_Data_Storage_T *CbrtRecordData,uint16_t* LinkPointArray,Axis_Cbrt_Data_T* AxisCbrtDataArray)
{
  uint8_t readlen;
	
	readlen=Flash_ReadBuf(RECORD_DATA_ADDR,(uint8_t*)CbrtRecordData,sizeof(Cbrt_Data_Storage_T));
	
	if(readlen!=sizeof(Cbrt_Data_Storage_T))
	{
		return 0;
	}
	
	if(CbrtRecordData->Cbrt_Data_CheckSum!=CheckSum((uint8_t*)CbrtRecordData,(sizeof(Cbrt_Data_Storage_T)-1)))
	{
		return 0;
	}
	
	memcpy(AxisCbrtDataArray,CbrtRecordData,4*sizeof(Axis_Cbrt_Data_T));
	//memcpy(LinkPointArray,&(CbrtRecordData->Axis_LinkPoint_B1),5*sizeof(uint16_t));
	
	return 1;
}

