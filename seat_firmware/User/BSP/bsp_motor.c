#include "bsp_motor.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

//本文件内，涉及具体操作的，电机从0开始编号，仅影响名称的命名等，允许从1开始编号。
//延时是以180M主频为基础的

#define DEFAULT_MOTOR_START_SPEED 40
int8_t  MotorStage[4];
	
/*
*********************************************************************************************************
*	函 数 名: Motor_SoftDelay
*	功能说明: 行线置位到列线检测的延时,具体用不用看调试结果
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void Motor_SoftDelay(void)
{
	uint8_t HoldTime=50;
	
	while(HoldTime)
  {
		HoldTime--;
  }
}

/*
*********************************************************************************************************
*	函 数 名: Motor_ControlPin_Init
*	功能说明: 电机控制芯片的相关引脚初始化
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void Motor_ControlPin_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA ,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB ,ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC ,ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD ,ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE ,ENABLE);	
	
	GPIO_InitStructure.GPIO_Pin= Motor4_Brake_PIN ;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;

	GPIO_Init(Motor4_nFault_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=Motor1_Brake_PIN |
                              Motor2_Sleep_PIN   | Motor2_Disable_PIN|
                              Motor2_Direction_PIN;

	GPIO_Init(Motor2_Direction_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=Motor3_Brake_PIN | Motor4_Sleep_PIN |
                            	Motor4_Disable_PIN | Motor4_Direction_PIN ;
													
	GPIO_Init(Motor4_Direction_GPIO_PORT, &GPIO_InitStructure);		

  GPIO_InitStructure.GPIO_Pin=Motor2_Brake_PIN |
	                            Motor3_Sleep_PIN | Motor3_Disable_PIN |
															Motor3_Direction_PIN ;
													
	GPIO_Init(Motor3_nFault_GPIO_PORT, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin=Motor1_Sleep_PIN | Motor1_Disable_PIN |
                              Motor1_Direction_PIN;

  GPIO_Init(Motor1_Direction_GPIO_PORT, &GPIO_InitStructure);
	
	/*为避免从Sleep切换到其他状态时，nFault脚的短暂低电平触发中断，禁用Sleep模式*/
	GPIO_SetBits(Motor1_Sleep_GPIO_PORT,Motor1_Sleep_PIN);
	GPIO_SetBits(Motor2_Sleep_GPIO_PORT,Motor2_Sleep_PIN);
	GPIO_SetBits(Motor3_Sleep_GPIO_PORT,Motor3_Sleep_PIN);
	GPIO_SetBits(Motor4_Sleep_GPIO_PORT,Motor4_Sleep_PIN);
}

/*
*********************************************************************************************************
*	函 数 名: Motor_FaultPin_Init
*	功能说明: 电机错误引脚初始化，连接DRV8873错误中断脚
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void Motor_FaultPin_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;	
	
	//外部中断时钟打开
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE); //外部中断时钟打开
	//IO端口时钟在Control Pin Init中已启用，无需再次初始化
	
	//Motor1_nFault PB11
	{
	GPIO_InitStructure.GPIO_Pin= Motor1_nFault_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;           //输入模式
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;         //推挽，实际引脚为输入，不影响
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;       //外部已上拉
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;      
	GPIO_Init(Motor1_nFault_GPIO_PORT, &GPIO_InitStructure);	
	//外部中断源配置
  SYSCFG_EXTILineConfig(Motor1_nFault_EXTI_PORTSOURCE,Motor1_nFault_EXTI_PINSOURCE);	
	/* 配置外部中断线 */
	EXTI_InitStructure.EXTI_Line = Motor1_nFault_EXTI_LINE ;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  	/* 下降沿触发 */
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	/* 使能中断*/
	NVIC_InitStructure.NVIC_IRQChannel = Motor1_nFault_EXTI_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x04;//分组4,4bit抢占优先级，0bit子优先级，IO中断等级为4，串口为8
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	}
	
	//Motor2_nFault PD9 
	{
	GPIO_InitStructure.GPIO_Pin= Motor2_nFault_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;           
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;         
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;       
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;      
	GPIO_Init(Motor2_nFault_GPIO_PORT, &GPIO_InitStructure);
	//外部中断源配置
	SYSCFG_EXTILineConfig(Motor2_nFault_EXTI_PORTSOURCE,Motor2_nFault_EXTI_PINSOURCE);	
	/* 配置外部中断线 */
	EXTI_InitStructure.EXTI_Line = Motor2_nFault_EXTI_LINE ;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  	/* 下降沿触发 */
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	/* 使能中断*/
	NVIC_InitStructure.NVIC_IRQChannel = Motor2_nFault_EXTI_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x04;//也设置为4
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	}
	//Motor3_nFault PD15
	{
	GPIO_InitStructure.GPIO_Pin= Motor3_nFault_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;           
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;         
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;       
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;      
	GPIO_Init(Motor3_nFault_GPIO_PORT, &GPIO_InitStructure);
	//外部中断源配置
  SYSCFG_EXTILineConfig(Motor3_nFault_EXTI_PORTSOURCE,Motor3_nFault_EXTI_PINSOURCE);
	/* 配置外部中断线 */
	EXTI_InitStructure.EXTI_Line = Motor3_nFault_EXTI_LINE ;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  	/* 下降沿触发 */
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	/* 使能中断*/
	NVIC_InitStructure.NVIC_IRQChannel = Motor3_nFault_EXTI_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x05;//也设置为4
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	}

	//Motor4_nFault PA11暂时不做中断设置，等待后续硬件端口调整
}

/*********************************************************************************************************
*	函 数 名: EXTI9_5_IRQHandler,当前电机错误中断时只是把芯片设置成standby模式
*	功能说明: 外部中断服务程序
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void EXTI9_5_IRQHandler(void)
{
	__disable_irq();
	
if (EXTI_GetITStatus(Motor2_nFault_EXTI_LINE) != RESET)
	{
		
		/**/
		Motor_ChangeToStatus_Standby(Motor_Channel_2);
		EXTI_ClearITPendingBit(Motor2_nFault_EXTI_LINE);
		
	}
	
	__enable_irq();
}

/*
*********************************************************************************************************
*	函 数 名: EXTI15_10_IRQHandler
*	功能说明: 外部中断服务程序
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void EXTI15_10_IRQHandler(void)
{
		__disable_irq();
	
	if (EXTI_GetITStatus(Motor3_nFault_EXTI_LINE) != RESET)
	{
		
		Motor_ChangeToStatus_Standby(Motor_Channel_3);
		
		EXTI_ClearITPendingBit(Motor3_nFault_EXTI_LINE);
		
	}
	else
	{
		if(EXTI_GetITStatus(Motor1_nFault_EXTI_LINE) != RESET)
		{
			Motor_ChangeToStatus_Standby(Motor_Channel_1);
			EXTI_ClearITPendingBit(Motor1_nFault_EXTI_LINE);
		}
	}
	__enable_irq();
}


/*
*********************************************************************************************************
*	函 数 名: MotorInit
*	功能说明: 电机初始化
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/

void MotorInit(void)
{ 
	//使能、电刹车控制脚的初始化
	Motor_ControlPin_Init();
	
	//错误脚的初始化及中断配置
	Motor_FaultPin_Init();
	
	//PWM初始化
  Motor_PWM_Init();
	
	//所有通道进入Standby模式，同时抱闸
	Motor_ChangeToStatus_Standby(Motor_Channel_123);
  Motor_BrakeON(Motor_Channel_123);
}

/*
*********************************************************************************************************
*	函 数 名: Motor_BrakeON
*	功能说明: 刹车抱紧，BRAKE=1
*	形    参: ChannelNum 0-3 四个单独通道，4为0-2三通道，5为0-3四个通道
*	返 回 值: 无
*********************************************************************************************************
*/
void Motor_BrakeON(uint8_t ChannelNum)
{
  if(ChannelNum>5)
	{
		return;
	}
	
	switch(ChannelNum)
	{
		
		case 0: GPIO_SetBits(Motor1_Brake_GPIO_PORT,Motor1_Brake_PIN);
		        break;
			
		case 1: GPIO_SetBits(Motor2_Brake_GPIO_PORT,Motor2_Brake_PIN);
		        break;
			
		case 2: GPIO_SetBits(Motor3_Brake_GPIO_PORT,Motor3_Brake_PIN);
		        break;
			
		case 3: GPIO_SetBits(Motor4_Brake_GPIO_PORT,Motor4_Brake_PIN);
		        break;
		
		case 4: do
		        {
						  Motor_ChangeToStatus_Standby(--ChannelNum);
						}
			      while(ChannelNum);
		        break;
		
		case 5: Motor_ChangeToStatus_Standby(0);
						Motor_ChangeToStatus_Standby(1);
						Motor_ChangeToStatus_Standby(2);
			      break;
			
		default:break;
  }
}

/*
*********************************************************************************************************
*	函 数 名: Motor_BrakeRelease
*	功能说明: 释放刹车，BRAKE=0
*	形    参: ChannelNum 0-3 四个单独通道，4为0-2三通道，5为0-3四个通道
*	返 回 值: 无
*********************************************************************************************************
*/
void Motor_BrakeRelease(uint8_t ChannelNum)
{
  if(ChannelNum>5)
	{
		return;
	}
		
	switch(ChannelNum)
	{
		
		case 0: GPIO_ResetBits(Motor1_Brake_GPIO_PORT,Motor1_Brake_PIN);
		        break;
			
		case 1: GPIO_ResetBits(Motor2_Brake_GPIO_PORT,Motor2_Brake_PIN);
		        break;
			
		case 2: GPIO_ResetBits(Motor3_Brake_GPIO_PORT,Motor3_Brake_PIN);
		        break;
			
		case 3: GPIO_ResetBits(Motor4_Brake_GPIO_PORT,Motor4_Brake_PIN);
		        break;
		
		case 4: do
		        {
						  Motor_ChangeToStatus_Active(--ChannelNum);
						}
			      while(ChannelNum);
		        break;
		
		case 5: Motor_ChangeToStatus_Active(0);
						Motor_ChangeToStatus_Active(1);
						Motor_ChangeToStatus_Active(2);
			      break;
			
		default:break;
  }	 
}
/*
*********************************************************************************************************
*	函 数 名: Motor_ChangeToStatus_Standby
*	功能说明: 切换至待命状态，输出高阻 DISABLE=1
            此时SPI可读取相关错误、诊断信息，写设置参数寄存器
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/

void Motor_ChangeToStatus_Standby(uint8_t ChannelNum)
{
  if(ChannelNum>5)
	{
		return;
	}
	
	switch(ChannelNum)
	{
		
		case 0: GPIO_SetBits(Motor1_Disable_GPIO_PORT,Motor1_Disable_PIN);
		        break;
			
		case 1: GPIO_SetBits(Motor2_Disable_GPIO_PORT,Motor2_Disable_PIN);
		        break;
			
		case 2: GPIO_SetBits(Motor3_Disable_GPIO_PORT,Motor3_Disable_PIN);
		        break;
			
		case 3: GPIO_SetBits(Motor4_Disable_GPIO_PORT,Motor4_Disable_PIN);
		        break;
		
		case 4: do
		        {
						  Motor_ChangeToStatus_Standby(--ChannelNum);
						}
			      while(ChannelNum);
		        break;
		
		case 5: Motor_ChangeToStatus_Standby(0);
						Motor_ChangeToStatus_Standby(1);
						Motor_ChangeToStatus_Standby(2);
			      break;
			
		default:break;
  }
	
 // Motor_SoftDelay();//DRV8873进入Standby状态需要一点时间
}

/*
*********************************************************************************************************
*	函 数 名: Motor_ChangeToStatus_Active
*	功能说明: 切换至工作状态，芯片退出Standby模式，H桥工作 DISABLE=0
            IN1 IN2分别输入方向电平及PWM波即可实现电机转动
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void Motor_ChangeToStatus_Active(uint8_t ChannelNum)
{
  if(ChannelNum>5)
	{
		return;
	}
		
	switch(ChannelNum)
	{
		
		case 0: GPIO_ResetBits(Motor1_Disable_GPIO_PORT,Motor1_Disable_PIN);
		        break;
			
		case 1: GPIO_ResetBits(Motor2_Disable_GPIO_PORT,Motor2_Disable_PIN);
		        break;
			
		case 2: GPIO_ResetBits(Motor3_Disable_GPIO_PORT,Motor3_Disable_PIN);
		        break;
			
		case 3: GPIO_ResetBits(Motor4_Disable_GPIO_PORT,Motor4_Disable_PIN);
		        break;
		
		case 4: do
		        {
						  Motor_ChangeToStatus_Active(--ChannelNum);
						}
			      while(ChannelNum);
		        break;
		
		case 5: Motor_ChangeToStatus_Active(0);
						Motor_ChangeToStatus_Active(1);
						Motor_ChangeToStatus_Active(2);
			      break;
			
		default:break;
  }	 

 // Motor_SoftDelay();;//DRV8873 Enable需要2.5us	
}

/*
*********************************************************************************************************
*	函 数 名: Motor_ChangeToStatus_Manual
*	功能说明: 切换至手动状态，芯片进入Standby模式，释放刹车，H桥高阻 DISABLE=1,BRAKE=0
            用于需要人工推动或故障卡顿的情况
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void Motor_ChangeToStatus_Manual(uint8_t ChannelNum)
{
  Motor_ChangeToStatus_Standby(ChannelNum);
	Motor_BrakeRelease(ChannelNum);
}

/*
*********************************************************************************************************
*	函 数 名: Motor_SetMoveDirection
*	功能说明: 设定指定电机的运行方向
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void Motor_SetMoveDirection(uint8_t ChannelNum,int8_t MoveDirection)
{
  if(ChannelNum>5)
	{
		return;
	}
	
	switch(ChannelNum)
	{
		
		case 0: MoveDirection>0?GPIO_SetBits(Motor1_Direction_GPIO_PORT,Motor1_Direction_PIN):GPIO_ResetBits(Motor1_Direction_GPIO_PORT,Motor1_Direction_PIN);
		        break;       
	
		case 1: MoveDirection>0?GPIO_SetBits(Motor2_Direction_GPIO_PORT,Motor2_Direction_PIN):GPIO_ResetBits(Motor2_Direction_GPIO_PORT,Motor2_Direction_PIN);
            break;
			
		case 2: MoveDirection>0?GPIO_SetBits(Motor3_Direction_GPIO_PORT,Motor3_Direction_PIN):GPIO_ResetBits(Motor3_Direction_GPIO_PORT,Motor3_Direction_PIN);
		        break;
			
		case 3: MoveDirection>0?GPIO_SetBits(Motor4_Direction_GPIO_PORT,Motor4_Direction_PIN):GPIO_ResetBits(Motor4_Direction_GPIO_PORT,Motor4_Direction_PIN);
			      break;
		
		case 4: Motor_SetMoveDirection(0,MoveDirection);
		        Motor_SetMoveDirection(1,MoveDirection);
		        Motor_SetMoveDirection(2,MoveDirection);
		        Motor_SetMoveDirection(3,MoveDirection);	  
		        break;
		
		case 5: Motor_SetMoveDirection(0,MoveDirection);
		        Motor_SetMoveDirection(1,MoveDirection);
		        Motor_SetMoveDirection(2,MoveDirection);
			      break;
			
		default:break;
  }	  
}

/*
*********************************************************************************************************
*	函 数 名: Motor_DriveByStage
*	功能说明: 该函数根据当前阶段数值来控制电机按照指定规律运行,其中初始速度为30，该数值下电机基本上可以
            克服静摩擦，马上运动。形参传递来的0-100将比例化到0-70，再叠加到初始速度上
*	形    参: ChannelNum 电机通道编号 0 1 2 3
            StageNum   -100-+100，阶段编码，正负指示方向，阶段代表速度值
*	返 回 值: 无
*********************************************************************************************************
*/
void Motor_DriveByStage(uint8_t ChannelNum,int8_t StageNum)
{
	uint8_t postiveSpeed;
	uint8_t nagetiveSpeed;
	//float speedRatio=0;
	
	if((StageNum<-100)||(StageNum>100)||(ChannelNum>3))
	{
		return;
	}
	else if(StageNum==0)
	{
		Motor_ChangeToStatus_Standby(ChannelNum);
		return;
	}
	else
	{
		Motor_ChangeToStatus_Active(ChannelNum);
		
		if(StageNum>0)//0——100,正向驱动
		{
		  Motor_SetMoveDirection(ChannelNum,Motor_Direction_Forward);
//			speedRatio=StageNum;
//			speedRatio=speedRatio*0.7;
			postiveSpeed=StageNum;
			postiveSpeed=postiveSpeed*2;
			postiveSpeed=200-postiveSpeed;
			Motor_PWM_Output(ChannelNum,postiveSpeed);
		}
		else if(StageNum<0)//反向驱动
		{
			Motor_SetMoveDirection(ChannelNum,Motor_Direction_Reverse);
//			speedRatio=abs(StageNum);
//			speedRatio=speedRatio*0.7;
			nagetiveSpeed=abs(StageNum);
			nagetiveSpeed=nagetiveSpeed*2;
			Motor_PWM_Output(ChannelNum,nagetiveSpeed);
		}
	}  	
}
/*
*********************************************************************************************************
*	函 数 名: Abs_MinusOne
*	功能说明: 对绝对值减一，符号不变
*	形    参: Value被操作数的指针
*	返 回 值: 无
*********************************************************************************************************
*/

void Abs_MinusOne(int8_t* Value)
{
	if(*Value>0)
	{
		*Value-=1;
	}
	else
	{
		if(*Value<0)
		{
			*Value+=1;
		}
	}
}

/*
*********************************************************************************************************
*	函 数 名: Abs_PlusOne
*	功能说明: 对绝对值加一，符号不变
*	形    参: Value被操作数的指针
*	返 回 值: 无
*********************************************************************************************************
*/

void Abs_PlusOne(int8_t* Value)
{
	if(*Value>0)
	{
		*Value+=1;
	}
	else
	{
		if(*Value<0)
		{
			*Value-=1;
		}
	}
}
/*
*********************************************************************************************************
*	函 数 名: Motor_Action_Normal
*	功能说明: 正常情况下的电机操纵
*	形    参: MotorNum         电机编号，0 1 2 3
            MotorDirection   运转方向 1为正向，-1为反向
            MotorSpeed       电机速度，0-100，内部*2转为0-200的占空比,设定默认为30起驱动
*	返 回 值: 无
*********************************************************************************************************
*/
void Motor_Action_Normal(uint8_t MotorNum,int8_t MotorDirection,uint8_t MotorSpeed)
{ 
	int8_t DirectionFlag;
	
	if((MotorNum>3)||(MotorSpeed>100))
	{
		return;
	}
	
	/*输入的速度为0，即要求停止运行*/
	if(MotorSpeed==0)
	{
    if(MotorStage[MotorNum]==0)	
		{
			Motor_BrakeON(MotorNum);//刹车
		}
    else
		{  /*下面代码是为了缓慢减速减少冲击*/
//			/*绝对值-1*/
//			Abs_MinusOne(&MotorStage[MotorNum]);
//			
//			if(abs(MotorStage[MotorNum])<DEFAULT_MOTOR_START_SPEED)
//      {
//				MotorStage[MotorNum]=0;
//			}	
			
			/*直接减速到0*/
			MotorStage[MotorNum]=0;
			/*驱动电机*/
      Motor_DriveByStage(MotorNum,MotorStage[MotorNum]);	
		}			
	}
	else/*输入速度不为0，即要求调速*/
	{ 
   	/*先释放刹车*/
		Motor_BrakeRelease(MotorNum);
		
		DirectionFlag=MotorDirection*MotorStage[MotorNum];//前一运动状态方向与当前要求运动方向是否一致
		
		if(DirectionFlag>0)/*同向调速，直接进行速度逐步调整*/
		{ 
			if(MotorSpeed>abs(MotorStage[MotorNum]))/*要求的速度比当前速度大*/
			{
				Abs_PlusOne(&MotorStage[MotorNum]);//速度绝对值+1
			}
			else if(MotorSpeed<abs(MotorStage[MotorNum]))/*要求的速度比当前速度小*/
			{
				Abs_MinusOne(&MotorStage[MotorNum]);//速度绝对值-1
			}	
			
			Motor_DriveByStage(MotorNum,MotorStage[MotorNum]);		
		}
		else
		{
			/*调速过零点*/ 
			if(DirectionFlag==0)
			{ /*目标速度是正向的*/
				if(MotorDirection>0)
				{
					MotorStage[MotorNum]+=DEFAULT_MOTOR_START_SPEED;
				}
				else if(MotorDirection<0)/*目标速度是反向的*/
				{
					MotorStage[MotorNum]-=DEFAULT_MOTOR_START_SPEED;
				}
			}
			else
			{ /*反向调速，先同向减速，再反向加速*/
				if(DirectionFlag<0)
				{ 		
				  Abs_MinusOne(&MotorStage[MotorNum]);		
				  /*驱动电机*/
					Motor_DriveByStage(MotorNum,MotorStage[MotorNum]);				
				}
			}
		}
	}	
}

/*
*********************************************************************************************************
*	函 数 名: Motor_Action_Emergency
*	功能说明: 电机紧急制动
*	形    参: 电机编号 0 1 2 3 
*	返 回 值: 无
*********************************************************************************************************
*/
void Motor_Action_Emergency(uint8_t MotorNum)
{
	if((MotorNum>3))
	{ /*只允许0 1 2 3，不允许多通道*/
		return;
	}
	/*芯片切换为Standby模式，H桥立刻输出高阻*/
	Motor_ChangeToStatus_Standby(MotorNum);
	
	/*PWM停止输出*/
  Motor_PWM_Output(MotorNum,0);
	/*刹车启用*/
	Motor_BrakeON(MotorNum);
	/*stage置0*/
	MotorStage[MotorNum]=0;
}

/*
*********************************************************************************************************
*	函 数 名: Motor_Error_Handle
*	功能说明: 处理电机驱动芯片错误
*	形    参: MotorErrorCode
*	返 回 值: 无
*********************************************************************************************************
*/
void Motor_Error_Handle(uint8_t MotorNum,uint8_t MotorErrorCode)
{
  uint8_t ErrorBit[8];
	uint8_t index;
	
	for(index=0;index<8;index++)
	{
		ErrorBit[index]=(MotorErrorCode>>index)&0x01;
	}
	
	/*Global=0,No Fault Occur*/
	if(ErrorBit[6]==0)
  {
	 Motor_ChangeToStatus_Active(MotorNum);
	 return;
	}
	
	/*负载开路，不处理*/
	if(ErrorBit[0]==1){return;}
	
	/*过热警告，不处理*/
	if(ErrorBit[5]==1){return;}
	
	/*过热错误，过流错误,芯片禁能，停止PWM，释放刹车*/
	if(ErrorBit[1]==1||ErrorBit[2]==1)
  {
		Motor_ChangeToStatus_Manual(MotorNum);
		return;
	}
		/*UVLO（电机供电不足），CPUV（电荷泵升压不足），停止PWM，芯片禁能*/
	if(ErrorBit[3]==1||ErrorBit[4]==1)
  {
		Motor_ChangeToStatus_Standby(MotorNum);
		return;
	}
}
