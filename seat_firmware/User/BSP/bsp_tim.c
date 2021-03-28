#include "bsp_tim.h"
#include "stdio.h"
//由于DRV8873内部有MOS控制逻辑，因此只需要简单产生指定频率，可调占空比的PWM即可
//这里TIM1只用到基本定时器的功能


/*全局变量定义*/
/*定时器基础参数设置结构体，所有定时器都有*/
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
/*定时器输出比较结构体，4通道占空比不同，故需要配置*/
TIM_OCInitTypeDef  TIM_OCInitStructure;



/*
*********************************************************************************************************
*	函 数 名: Motor_PWM_GPIO_Config
*	功能说明: TIM1所用的引脚初始化
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void Motor_PWM_GPIO_Config(void)
{
/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*开启PE端口的GPIO外设时钟*/
	RCC_AHB1PeriphClockCmd (MOTOR_PWM_PIN_CLK, ENABLE); 
	
  /* 指定引脚复用功能 */
	GPIO_PinAFConfig(MOTOR_PWM_PORT,MOTOR1_PWM_PINSOURCE, MOTOR_PWM_PIN_AF); 
	GPIO_PinAFConfig(MOTOR_PWM_PORT,MOTOR2_PWM_PINSOURCE, MOTOR_PWM_PIN_AF);
	GPIO_PinAFConfig(MOTOR_PWM_PORT,MOTOR3_PWM_PINSOURCE, MOTOR_PWM_PIN_AF);
	GPIO_PinAFConfig(MOTOR_PWM_PORT,MOTOR4_PWM_PINSOURCE, MOTOR_PWM_PIN_AF);
	
	/* 定时器功能引脚初始化 */															   
	GPIO_InitStructure.GPIO_Pin = MOTOR1_PWM_PIN |
	                              MOTOR2_PWM_PIN |
																MOTOR3_PWM_PIN |
	                              MOTOR4_PWM_PIN ;
	
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF;    
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	
	GPIO_Init(MOTOR_PWM_PORT, &GPIO_InitStructure);	
}

/*
*********************************************************************************************************
*	函 数 名: Motor_PWM_TIM_Mode_Config
*	功能说明: TIM1工作模式初始化
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void Motor_PWM_TIM_Mode_Config(void)
{
 	// 开启TIM1时钟
  RCC_APB2PeriphClockCmd(MOTOR_PWM_TIM_CLK, ENABLE); 
	
  // 高级控制定时器时钟源TIM1CLK = HCLK=180MHz 
	// 设定分频系数，使定时器时钟频率为TIM1CLK/(TIM_Prescaler+1)=10MHz
  TIM_TimeBaseStructure.TIM_Prescaler = 18-1;		
	
  //当定时器从0计数到199，即为200次，为一个定时周期,使事件周期为10M/200=50KHz
  TIM_TimeBaseStructure.TIM_Period = 200-1;
	 
  // 采样时钟分频，不影响
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
  // 计数方式
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
  // 重复计数器
  TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// 初始化定时器TIM1
	TIM_TimeBaseInit(MOTOR_PWM_TIM, &TIM_TimeBaseStructure);
	
  /*PWM模式配置*/
	//配置为PWM模式1
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	//比较输出使能（比较结果输出到引脚），先不使能
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;	
	//互补输出禁止，不需要互补信号
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	//初始的占空比为50%
  TIM_OCInitStructure.TIM_Pulse = MOTOR_PWM_DutyInitValue ;
	//有效输出为高电平
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  //死区电平设置，不影响
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	
  //使能通道1
  TIM_OC1Init(MOTOR_PWM_TIM, &TIM_OCInitStructure);	  
	/* 使能通道1自动重载 */
	TIM_OC1PreloadConfig(MOTOR_PWM_TIM, TIM_OCPreload_Enable);
	
  TIM_OC2Init(MOTOR_PWM_TIM, &TIM_OCInitStructure);	  	
	TIM_OC2PreloadConfig(MOTOR_PWM_TIM, TIM_OCPreload_Enable);
	
  TIM_OC3Init(MOTOR_PWM_TIM, &TIM_OCInitStructure);	  
	TIM_OC3PreloadConfig(MOTOR_PWM_TIM, TIM_OCPreload_Enable);
	
  TIM_OC4Init(MOTOR_PWM_TIM, &TIM_OCInitStructure);	  
	TIM_OC4PreloadConfig(MOTOR_PWM_TIM, TIM_OCPreload_Enable);


	// 使能定时器
	TIM_Cmd(MOTOR_PWM_TIM, ENABLE);	
	
	/* 主动输出使能 */
  TIM_CtrlPWMOutputs(MOTOR_PWM_TIM, ENABLE);
}

/*
*********************************************************************************************************
*	函 数 名: MOTOR_PWM_Init
*	功能说明: 初始化TIM，使之可以产生指定频率的PWM波，默认初始化后4路均为50KHz，占空比50%，但不输出
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void Motor_PWM_Init(void)
{
	Motor_PWM_GPIO_Config();
	Motor_PWM_TIM_Mode_Config();
}

/*
*********************************************************************************************************
*	函 数 名: MOTOR_PWM_Output
*	功能说明: 指定一通道以某个占空比输出
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void Motor_PWM_Output(uint8_t MotorNum,uint8_t DutyValue)
{
  if(DutyValue>200)
	{	
		return;
	}
	
	TIM_CtrlPWMOutputs(MOTOR_PWM_TIM, ENABLE);
	
	switch(MotorNum)
	{
		
		case 0:	  
		          TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	        
		          TIM_OC1Init(MOTOR_PWM_TIM, &TIM_OCInitStructure);
		          TIM_SetCompare1(MOTOR_PWM_TIM,DutyValue);
							break;
		
		case 1:		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	        
		          TIM_OC2Init(MOTOR_PWM_TIM, &TIM_OCInitStructure);
		          TIM_SetCompare2(MOTOR_PWM_TIM,DutyValue);
							break;
			
		case 2:   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	        
		          TIM_OC3Init(MOTOR_PWM_TIM, &TIM_OCInitStructure);
		          TIM_SetCompare3(MOTOR_PWM_TIM,DutyValue);
							break;
			
		case 3:   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	        
		          TIM_OC4Init(MOTOR_PWM_TIM, &TIM_OCInitStructure);
		          TIM_SetCompare4(MOTOR_PWM_TIM,DutyValue);
							break;
			
		default: break;
	}
  
	return;
}

void Motor_PWM_SetDutyValue(uint8_t MotorNum,uint8_t DutyValue)
{
	if(DutyValue>200)
	{	
		return;
	}
	
	switch(MotorNum)
	{
		
		case 0:	  TIM_SetCompare1(MOTOR_PWM_TIM,DutyValue);
							break;
		
		case 1:		TIM_SetCompare2(MOTOR_PWM_TIM,DutyValue);
							break;
			
		case 2:   TIM_SetCompare3(MOTOR_PWM_TIM,DutyValue);
							break;
			
		case 3:   TIM_SetCompare4(MOTOR_PWM_TIM,DutyValue);
							break;
			
		default: break;
	}
  
	return;
}

void Motor_PWM_DisableAllChannel()
{
	TIM_CtrlPWMOutputs(MOTOR_PWM_TIM, DISABLE);
}

void Motor_PWM_DisableOneChannel(uint8_t MotorNum)
{
	switch(MotorNum)
	{
		
		case 0:	  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;	        
		          TIM_OC1Init(MOTOR_PWM_TIM, &TIM_OCInitStructure);
							break;
		
		case 1:		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;	        
		          TIM_OC2Init(MOTOR_PWM_TIM, &TIM_OCInitStructure);
							break;
			
		case 2:   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;	        
		          TIM_OC3Init(MOTOR_PWM_TIM, &TIM_OCInitStructure);
							break;
			
		case 3:   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;	        
		          TIM_OC4Init(MOTOR_PWM_TIM, &TIM_OCInitStructure);
							break;
			
		default: break;
	}
}

