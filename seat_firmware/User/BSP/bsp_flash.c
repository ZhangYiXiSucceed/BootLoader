/*
*********************************************************************************************************
*
*	ģ������ : Ƭ��Flash����ģ��(For F429 180M)
*	�ļ����� : bsp_flash.c
*	��    �� : V1.0
*	˵    �� : ��ģ��ʹ�ÿ⺯�����ڲ�Flash��Sector10���в�����������Ϊ���α궨���ݵĴ洢������Ҫ������
             д���¼
						 ��ȡ��¼
						 ������¼
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2019-04-24  WLM      �ݸ�
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
*	�� �� ��: Flash_GetSector
*	����˵��: ���ݵ�ַ���������׵�ַ
*	��    ��:  ��
*	�� �� ֵ: �����׵�ַ
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
*	�� �� ��: Flash_WriteBuf
*	����˵��: ��ָ��������ָ����������д��ָ����ַ��ʼ���ڲ�Flash���õ�ַ���ڵ�Sectorԭ������ȫ�����
*	��    ��: FlashAddr ��д���Flash��ַ
            dataBuf   ���ݴ�ŵ��׵�ַ
            datalen   ���ݳ��ȣ�ע�Ȿ�������жϳ����Ƿ񳬹�sector��С��������Ҫȷ������ֵС��SectorSize
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Flash_WriteBuf(uint32_t FlashAddr,uint8_t* dataBuf,uint32_t datalen)
{
   uint8_t index;
	
	__disable_irq();//�ر�ȫ���ж���Ӧ
	
	/*Flash ����*/
	FLASH_Unlock();

  /* ���ԭ�б�־λ */
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
	
	/*������ַ��Ӧ��Sector�����ݣ���ѹ�ȼ�3*/
	FLASH_EraseSector(Flash_GetSector(FlashAddr), VoltageRange_3);
	
	
	/* ���ֽ�ģʽ��� */
	for (index = 0; index<datalen; index++)
	{
		FLASH_ProgramByte(FlashAddr++ , *(dataBuf+index));
	}

  	/* Flash ��������ֹдFlash���ƼĴ��� */
  	FLASH_Lock();
	
	 /*��ȫ���ж�*/
	 __enable_irq(); 
	
	return ;
}

/*
*********************************************************************************************************
*	�� �� ��: Flash_ReadBuf
*	����˵��: ��ָ��������ָ����������д��ָ����ַ��ʼ���ڲ�Flash���õ�ַ���ڵ�Sectorԭ������ȫ�����
*	��    ��: FlashAddr ��д���Flash��ַ
            dataBuf   ���ݴ�ŵ��׵�ַ
            datalen   ���ݳ��ȣ�ע�Ȿ�������жϳ����Ƿ񳬹�sector��С��������Ҫȷ������ֵС��SectorSize
*	�� �� ֵ: ��
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
*	�� �� ��: Flash_WriteRecord
*	����˵��: ����Flash_WriteBuf��1���궨��¼д��Sector10�׵�ַ
*	��    ��: �궨��¼�ṹ��ָ��
*	�� �� ֵ: ��
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
*	�� �� ��: Flash_ReadRecord
*	����˵��: ����Flash_ReadBuf��1���궨��¼��Sector10�׵�ַ��ȡ������У���Ƿ���ȷ
*	��    ��: �궨��¼�ṹ��ָ��
*	�� �� ֵ: ��ȡ�ɹ�����1��ֻ��У��ͨ������Ϊ��ȡͨ���������������0
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

