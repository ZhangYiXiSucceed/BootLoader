#include "stm32f4xx.h"
#include "bsp.h"
#include "RTL.h"
#include "user_datastructure.h"
#include "user_rtx.h"
//-------------------------------------全局变量--------------------------------------
/*座椅当前位置*/
 uint16_t Seat_Position_Now[4];
 uint16_t Seat_Position_Last[4];
 /*4轴运动速度（以电位器数值作为量纲）*/
 int16_t Axis_MoveVelocity[4];
/*电机当前电流*/
 uint16_t Motor_Current_Now[4];
/*DRV8873错误状态寄存器值*/
uint8_t  Motor_Status_Flag[4];
/*nFault引脚中断标志*/
uint8_t  Motor_Status_IT[4];
/*5个联动点ADC值*/
uint16_t Axis_LinkPoint_Position[5];
/*4轴标定数据*/
Axis_Cbrt_Data_T Axis_Cbrt_Data[4];
/*记录数据，用于RAM与Flash之间提供临时存储*/
Cbrt_Data_Storage_T SeatCbrtData;
/*电机运动控制结构体数组*/
Motor_Ctrl_Struct_T Motor_Ctrl_Info[4];
/*FRAM中的记录号*/
uint32_t RecordNum;
/*FRAM中记录存储的自定义Block号*/
uint8_t RecordStorageBlockNum;
/*FRAM遍历成功标志*/
uint8_t RecordSeekOK;
/*标定数据成功获取标志*/
uint8_t CbrtDataOK;
/*座椅状态*/
Seat_Status_T SeatStatus;
/*写入NVM的记录*/
uint8_t SeatInfoRecordMemory[128];
/*RTC内时间*/
RTC_Time_T TimeOfRtc;
/*控制程序版本*/
uint8_t AppVer=10;
//------------------------------------RTX系统相关变量---------------------------------
/*启动任务的任务栈*/
static uint64_t TaskStk_SystemSetup[512/8];

//------------------------------------主函数------------------------------------------
int main(void)
{ 
	//RCC_DeInit();
	
	
	
	//__enable_irq();
	
	/*硬件初始化*/	
	bsp_Init();
	
	/*RTX系统启动前的数据初始化*/
	data_Init();
	
	NVIC_SetVectorTable(NVIC_VectTab_FLASH,NVIC_VECTOR_TABLE_OFFSET);
	
	/*初始化RTX系统并创建启动任务，启动任务优先级为9,（当前最高）*/
  os_sys_init_user(Task_System_Setup,9,&TaskStk_SystemSetup,sizeof(TaskStk_SystemSetup));
	
  /*死循环*/
	while(1){};
}

