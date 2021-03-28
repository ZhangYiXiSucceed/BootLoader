#ifndef _BSP_CPU_FLASH_H_
#define _BSP_CPU_FLASH_H_


#include "user_datastructure.h"

//-----------------以下地址定义是针对F429的
#define FLASH_BASE_ADDR	0x08000000			/* Flash基地址 */
#define	FLASH_SIZE		(1*1024*1024)		/* Flash 容量 */
/* Base address of the Flash sectors */
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbytes */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbytes */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbytes */

#define RECORD_DATA_ADDR        ((uint32_t)0x080C0000) /*暂定记录存储在片内SECTOR10*/
#define RECORD_DATA_LEN         47                     /*共计47字节，46字节数据，最后1字节校验*/

uint8_t CheckSum(uint8_t * dataBuf,uint16_t bufLength);
void    Flash_WriteBuf(uint32_t FlashAddr,uint8_t* dataBuf,uint32_t datalen);
uint8_t Flash_ReadBuf(uint32_t FlashAddr,uint8_t* dataBuf,uint32_t datalen);
void    Flash_WriteRecord(Cbrt_Data_Storage_T *CbrtRecordData,uint16_t* LinkPointArray,Axis_Cbrt_Data_T* AxisCbrtDataArray);
uint8_t Flash_ReadRecord(Cbrt_Data_Storage_T *CbrtRecordData,uint16_t* LinkPointArray,Axis_Cbrt_Data_T* AxisCbrtDataArray);

#endif

