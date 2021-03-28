#ifndef __BSP_I2C_H
#define __BSP_I2C_H

//---------��Ӳ��I2C����������EV2EV5����λ��Ч���ʾ���ʹ��GPIOģ��I2C���
#include "stdint.h"

//FRAM����ض���
/*FM24CL64 ����ַ 10100001*/
#define FRAM_I2C_READ_ADDR 0xA1
/*FM24CL64 д��ַ 10100000*/
#define FRAM_I2C_WRITE_ADDR 0xA0

#define BLOCKADDR(N)  N*128

uint8_t NVM_I2C_Init(void);

uint8_t NVM_I2C_Write(uint8_t* dataWrite,uint16_t DistAddress,uint16_t ByteNum);

uint8_t NVM_I2C_Read(uint8_t* dataRead,uint16_t DistAddress,uint16_t ByteNum);

uint8_t SeekRecord(uint32_t* RecordNum,uint8_t* RecordStorageBlockNum);

uint8_t WriteRecord(uint8_t RecordStorageBlockNum,uint8_t* RecordHeader);

#endif

