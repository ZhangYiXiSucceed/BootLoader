#include "bsp_adc.h"
#include "string.h"
#include "arm_math.h"
/*
*********************************************************************************************************
*
*	模块名称 : ADC模块
*	文件名称 : bsp_i2c.h
*	版    本 : V1.0
*	说    明 : 本模块使用ADC1进行电机电流及电位器数值的采集，ADC1均处于独立扫描模式，DMA传输，但分为2个4通道，
             电流通道因波动比较大，采集100次后取平均，电位器通道只采集10次。这里用的先扫描后分离，如果速度慢
						 后续可以试试先连续然后切换ADC通道
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2019-04-24  WLM      草稿
*
*********************************************************************************************************
*/

//ADC数组全局变量定义，该数组元素为volatile类型,电位器值采集10组，电流值采集100组
__IO uint16_t Motor_ADC_PotenValue[MOTOR_ADC_CHANNEL_NUM*MOTOR_ADC_SAMPLETIME_POTEN]={0};
__IO uint16_t Motor_ADC_CurrentValue[MOTOR_ADC_CHANNEL_NUM*MOTOR_ADC_SAMPLETIME_CURRENT]={0};


int16_t Motor_ADCValueArray_Poten1[MOTOR_ADC_SAMPLETIME_POTEN];
int16_t Motor_ADCValueArray_Poten2[MOTOR_ADC_SAMPLETIME_POTEN];
int16_t Motor_ADCValueArray_Poten3[MOTOR_ADC_SAMPLETIME_POTEN];
int16_t Motor_ADCValueArray_Poten4[MOTOR_ADC_SAMPLETIME_POTEN];

int16_t Motor_ADCValueArray_Current1[MOTOR_ADC_SAMPLETIME_CURRENT];
int16_t Motor_ADCValueArray_Current2[MOTOR_ADC_SAMPLETIME_CURRENT];
int16_t Motor_ADCValueArray_Current3[MOTOR_ADC_SAMPLETIME_CURRENT];
int16_t Motor_ADCValueArray_Current4[MOTOR_ADC_SAMPLETIME_CURRENT];
/*
*********************************************************************************************************
*	函 数 名: Motor_ADC_GPIO_Config
*	功能说明: 电机电流，位置传感所用的8个ADC引脚的引脚初始化
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void Motor_ADC_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	
	// 使能 GPIO 时钟
	RCC_AHB1PeriphClockCmd( MOTOR_ADC_GPIO_CLK,ENABLE);		
	
	// 需要配置的GPIO为PA0-7
  GPIO_InitStructure.GPIO_Pin = MOTOR1_Position_ADC |
	                              MOTOR2_Position_ADC |
																MOTOR3_Position_ADC |
																MOTOR4_Position_ADC |
																MOTOR1_Current_ADC  |
																MOTOR2_Current_ADC  |
																MOTOR3_Current_ADC  |
																MOTOR4_Current_ADC  ;
  
	// 模式配置为模拟输入AIN
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
 
	// 用作AIN时不上拉也不下拉
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	
	//初始化GPIO
	GPIO_Init(MOTOR_ADC_GPIO_PORT, &GPIO_InitStructure);
}

/*
*********************************************************************************************************
*	函 数 名: Motor_ADC_Mode_Config
*	功能说明: ADC模式初始化，设置为ADC1，独立 4通道，DMA模式
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void Motor_ADC_Mode_Config(void)
{
	//ADC初始化结构体
	ADC_InitTypeDef ADC_InitStructure;
	//ADC通用初始化结构体
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
	
		// 开启ADC时钟
	RCC_APB2PeriphClockCmd(MOTOR_ADC_CLK  , ENABLE);
	// 开启DMA时钟
  RCC_AHB1PeriphClockCmd(MOTOR_ADC_DMA_CLK , ENABLE);
	
  // -------------------ADC Common 结构体 参数 初始化------------------------
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);
	// 独立ADC模式
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  // 时钟为fpclk 4分频	ADC_CLK=22.5MHz
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
  // 禁止DMA直接访问模式，独立模式下不需要，只有双重及三重时候需要	
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  // 采样时间间隔，独立模式下不影响，只有双重及三重时候有关系
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_10Cycles;  
  ADC_CommonInit(&ADC_CommonInitStructure);
	
  // -------------------ADC Init 结构体 参数 初始化--------------------------
  // ADC 12Bit分辨率
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  // 扫描模式，多通道采集需要	
  ADC_InitStructure.ADC_ScanConvMode = ENABLE; 
  // 连续转换	
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
  //禁止外部边沿触发
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  //外部触发通道，本例子使用软件触发，但此值赋值=((uint32_t)0x00000000)
	//此处必须赋值，否则初始值0xFF会导致ADC进入连续转换模式
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  //数据右对齐	
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  //转换通道 8个
  ADC_InitStructure.ADC_NbrOfConversion = MOTOR_ADC_CHANNEL_NUM ;                                    
  ADC_Init(MOTOR_ADC, &ADC_InitStructure);
}

/*
*********************************************************************************************************
*	函 数 名: Motor_DMAConfig_PotenMode
*	功能说明: 设置DMA参数，将40个电位器ADC值传输到电位器数组
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void Motor_DMAConfig_PotenMode(void)
{
	//DMA初始化结构体
	DMA_InitTypeDef DMA_InitStructure;
		
	//复位DMA配置
	DMA_DeInit(MOTOR_ADC_DMA_STREAM);	 
	// 选择 DMA 通道
  DMA_InitStructure.DMA_Channel = MOTOR_ADC_DMA_CHANNEL ; 	
	// DMA外设地址为ADC1数据寄存器地址
	DMA_InitStructure.DMA_PeripheralBaseAddr = MOTOR_ADC_DR_ADDR;	
  // DMA目标地址为数组地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)Motor_ADC_PotenValue;  
  // 数据传输方向为外设到存储器	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	
	// 缓冲区大小,4*10 uint16
	DMA_InitStructure.DMA_BufferSize = MOTOR_ADC_CHANNEL_NUM*MOTOR_ADC_SAMPLETIME_POTEN;	
	// 外设寄存器只有一个，地址不递增
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  // 存储器地址需要递增
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
  // // 外设数据大小为半字，即两个字节 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
  //	存储器数据大小也为半字，跟外设数据大小相同
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	
	
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
 	  // DMA 传输通道优先级为高，当使用一个DMA通道时，优先级设置不影响
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  // 禁止DMA FIFO	，使用直连模式
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;  
  // FIFO模式禁止时，以下配置不影响	
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  

  //初始化DMA流
	DMA_Init(MOTOR_ADC_DMA_STREAM, &DMA_InitStructure);
	// 使能DMA流
  DMA_Cmd(MOTOR_ADC_DMA_STREAM, ENABLE);
}	
/*
*********************************************************************************************************
*	函 数 名: Motor_DMAConfig_CurrentMode
*	功能说明: 设置DMA参数，将400个电位器ADC值传输到电流数组
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
 void Motor_DMAConfig_CurrentMode(void)
{
	//DMA初始化结构体
	DMA_InitTypeDef DMA_InitStructure;
		
	//复位DMA配置
	DMA_DeInit(MOTOR_ADC_DMA_STREAM);	 
	// 选择 DMA 通道
  DMA_InitStructure.DMA_Channel = MOTOR_ADC_DMA_CHANNEL ; 	
	// DMA外设地址为ADC1数据寄存器地址
	DMA_InitStructure.DMA_PeripheralBaseAddr = MOTOR_ADC_DR_ADDR;	
  // DMA目标地址为数组地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)Motor_ADC_CurrentValue;  
  // 数据传输方向为外设到存储器	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	
	// 缓冲区大小,4*100 uint16
	DMA_InitStructure.DMA_BufferSize = MOTOR_ADC_CHANNEL_NUM*MOTOR_ADC_SAMPLETIME_CURRENT;	
	// 外设寄存器只有一个，地址不递增
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  // 存储器地址需要递增
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
  // // 外设数据大小为半字，即两个字节 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
  //	存储器数据大小也为半字，跟外设数据大小相同
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	
	
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
 	  // DMA 传输通道优先级为高，当使用一个DMA通道时，优先级设置不影响
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  // 禁止DMA FIFO	，使用直连模式
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;  
  // FIFO模式禁止时，以下配置不影响	
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  

  //初始化DMA流
	DMA_Init(MOTOR_ADC_DMA_STREAM, &DMA_InitStructure);
	// 使能DMA流
  DMA_Cmd(MOTOR_ADC_DMA_STREAM, ENABLE);
}	
/*
*********************************************************************************************************
*	函 数 名: Motor_ADC_SetPotenChannel
*	功能说明: 设置ADC1的4通道为4个电位器通道，使能DMA传输
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void Motor_ADC_SetPotenChannel(void)
{
  /*设置DMA通道参数为电位器模式*/ 
	Motor_DMAConfig_PotenMode();
	
	// 配置 ADC 通道转换顺序和采样时间周期，4通道电位器位置（电位器1->4）
	ADC_RegularChannelConfig(MOTOR_ADC, Motor_Pos_CH4, 1,  ADC_SampleTime_56Cycles);//PA3
	ADC_RegularChannelConfig(MOTOR_ADC, Motor_Pos_CH3, 2,  ADC_SampleTime_56Cycles);//PA2
	ADC_RegularChannelConfig(MOTOR_ADC, Motor_Pos_CH2, 3,  ADC_SampleTime_56Cycles);//PA1
	ADC_RegularChannelConfig(MOTOR_ADC, Motor_Pos_CH1, 4,  ADC_SampleTime_56Cycles);//PA0
	
  // 该位置1后只要发生数据转换且使能了ADC DMA，就会发出DMA请求
  ADC_DMARequestAfterLastTransferCmd(MOTOR_ADC, ENABLE);
  // 使能ADC DMA
  ADC_DMACmd(MOTOR_ADC, ENABLE); 
}

/*
*********************************************************************************************************
*	函 数 名: Motor_ADC_SetCurrentChannel
*	功能说明: ADC模式初始化，设置为ADC1，独立 4通道，DMA模式
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void Motor_ADC_SetCurrentChannel(void)
{
  /*设置DMA通道参数为电位器模式*/ 
	Motor_DMAConfig_CurrentMode();
	
	// 配置 ADC 通道转换顺序和采样时间周期，4通道电流（电机1->4）
  ADC_RegularChannelConfig(MOTOR_ADC, Motor_Crt_CH1, 1,  ADC_SampleTime_56Cycles);//PA4
	ADC_RegularChannelConfig(MOTOR_ADC, Motor_Crt_CH2, 2,  ADC_SampleTime_56Cycles);//PA5                       
  ADC_RegularChannelConfig(MOTOR_ADC, Motor_Crt_CH3, 3,  ADC_SampleTime_56Cycles);//PA6
  ADC_RegularChannelConfig(MOTOR_ADC, Motor_Crt_CH4, 4,  ADC_SampleTime_56Cycles);//PA7
	
  // 该位置1后只要发生数据转换且使能了ADC DMA，就会发出DMA请求
  ADC_DMARequestAfterLastTransferCmd(MOTOR_ADC, ENABLE);
  // 使能ADC DMA
  ADC_DMACmd(MOTOR_ADC, ENABLE); 
}



/*
*********************************************************************************************************
*	函 数 名: Motor_ADC_Init
*	功能说明: 电机电流及位置使用ADC采样前的初始化工作，包括ADC初始化，引脚初始化，DMA初始化
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void Motor_ADC_Init(void)
{
		
	//ADC引脚初始化
	Motor_ADC_GPIO_Config();
	
	//ADC模式初始化
	Motor_ADC_Mode_Config();
	
	//ADC数组初始化
	memset((void*)&Motor_ADC_PotenValue[0],0,40*sizeof(uint16_t));
	memset((void*)&Motor_ADC_CurrentValue[0],0,400*sizeof(uint16_t));
}

/*
*********************************************************************************************************
*	函 数 名: Motor_ADC_Start
*	功能说明: 开始ADC转换，重新使能ADC，然后使用软件触发
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void Motor_ADC_Start(void)
{
	// 使能ADC
  ADC_Cmd(MOTOR_ADC, ENABLE); 
	DMA_Cmd(MOTOR_ADC_DMA_STREAM, ENABLE);
	//开始adc转换，软件触发
	ADC_SoftwareStartConv(MOTOR_ADC);
  
}

/*
*********************************************************************************************************
*	函 数 名: Motor_ADC_Stop
*	功能说明: 停止ADC转换，直接关闭ADC
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void Motor_ADC_Stop(void)
{
	ADC_Cmd(MOTOR_ADC, DISABLE); 
}

/*
*********************************************************************************************************
*	函 数 名: Motor_GetPotenValue
*	功能说明: 获取电位器ADC数值，即设置ADC通道为4个电位器通道，开启ADC，转换完成后关闭ADC
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void Motor_GetPotenValue(void)
{
	Motor_ADC_SetPotenChannel();
	Motor_ADC_Start();
	
	while(DMA_GetFlagStatus(MOTOR_ADC_DMA_STREAM,DMA_FLAG_TCIF0) == RESET);
	
	Motor_ADC_Stop();
}

/*
*********************************************************************************************************
*	函 数 名: Motor_GetCurrentValue
*	功能说明: 获取电流ADC数值，即设置ADC通道为4个电流通道，开启ADC，转换完成后关闭ADC
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void Motor_GetCurrentValue(void)
{
	Motor_ADC_SetCurrentChannel();
	Motor_ADC_Start();
	
	while(DMA_GetFlagStatus(MOTOR_ADC_DMA_STREAM,DMA_FLAG_TCIF0) == RESET);
	
	Motor_ADC_Stop();
}

/*
*********************************************************************************************************
*	函 数 名: Motor_ADCValueAverage
*	功能说明: 计算电流、电位器ADC数值平均值，调用DSP实现
*	形    参: AverageValuePoten 存储4通道电位器值的数组
            AverageValueCurrent 存储4通道电流值的数组
*	返 回 值: 无
*********************************************************************************************************
*/
void Motor_ADCValueAverage(uint16_t* AverageValuePoten,uint16_t* AverageValueCurrent)
{
  uint16_t index;
	
	/*将电位器采样值转到4个电位器求平均值数组中*/
	for(index=0;index<MOTOR_ADC_SAMPLETIME_POTEN;index++)
	{
	   Motor_ADCValueArray_Poten1[index]=Motor_ADC_PotenValue[MOTOR_ADC_CHANNEL_NUM*index];
     Motor_ADCValueArray_Poten2[index]=Motor_ADC_PotenValue[MOTOR_ADC_CHANNEL_NUM*index+1];
     Motor_ADCValueArray_Poten3[index]=Motor_ADC_PotenValue[MOTOR_ADC_CHANNEL_NUM*index+2];
     Motor_ADCValueArray_Poten4[index]=Motor_ADC_PotenValue[MOTOR_ADC_CHANNEL_NUM*index+3];	   
	}
	
		/*将电流采样值转到4个电位器求平均值数组中*/
	for(index=0;index<MOTOR_ADC_SAMPLETIME_CURRENT;index++)
	{
	   Motor_ADCValueArray_Current1[index]=Motor_ADC_CurrentValue[MOTOR_ADC_CHANNEL_NUM*index];
     Motor_ADCValueArray_Current2[index]=Motor_ADC_CurrentValue[MOTOR_ADC_CHANNEL_NUM*index+1];
     Motor_ADCValueArray_Current3[index]=Motor_ADC_CurrentValue[MOTOR_ADC_CHANNEL_NUM*index+2];
     Motor_ADCValueArray_Current4[index]=Motor_ADC_CurrentValue[MOTOR_ADC_CHANNEL_NUM*index+3];	   
	}

	 /*使用DSP库计算平均值*/
	arm_mean_q15(Motor_ADCValueArray_Poten1,MOTOR_ADC_SAMPLETIME_POTEN,(int16_t*)AverageValuePoten);
	arm_mean_q15(Motor_ADCValueArray_Poten2,MOTOR_ADC_SAMPLETIME_POTEN,(int16_t*)AverageValuePoten+1);
	arm_mean_q15(Motor_ADCValueArray_Poten3,MOTOR_ADC_SAMPLETIME_POTEN,(int16_t*)AverageValuePoten+2);
	arm_mean_q15(Motor_ADCValueArray_Poten4,MOTOR_ADC_SAMPLETIME_POTEN,(int16_t*)AverageValuePoten+3);
	
	arm_mean_q15(Motor_ADCValueArray_Current1,MOTOR_ADC_SAMPLETIME_CURRENT,(int16_t*)AverageValueCurrent);
	arm_mean_q15(Motor_ADCValueArray_Current2,MOTOR_ADC_SAMPLETIME_CURRENT,(int16_t*)AverageValueCurrent+1);
	arm_mean_q15(Motor_ADCValueArray_Current3,MOTOR_ADC_SAMPLETIME_CURRENT,(int16_t*)AverageValueCurrent+2);
	arm_mean_q15(Motor_ADCValueArray_Current4,MOTOR_ADC_SAMPLETIME_CURRENT,(int16_t*)AverageValueCurrent+3);
}

/*
*********************************************************************************************************
*	函 数 名: Motor_GetPositionCurrentData
*	功能说明: 一个完整的ADC采样、求平均过程
*	形    参: PotenValueArray 用于存储4通道电位器值的数组
            CurrentValueArray 用于存储4通道电流值的数组
*	返 回 值: 无
*********************************************************************************************************
*/
void Motor_GetPositionCurrentData(uint16_t* PotenValueArray,uint16_t* CurrentValueArray)
{
  Motor_GetPotenValue();
	
	Motor_GetCurrentValue();
	
	Motor_ADCValueAverage(PotenValueArray,CurrentValueArray);
}
