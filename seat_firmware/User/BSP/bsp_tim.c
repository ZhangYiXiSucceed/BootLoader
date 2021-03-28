#include "bsp_tim.h"
#include "stdio.h"
//����DRV8873�ڲ���MOS�����߼������ֻ��Ҫ�򵥲���ָ��Ƶ�ʣ��ɵ�ռ�ձȵ�PWM����
//����TIM1ֻ�õ�������ʱ���Ĺ���


/*ȫ�ֱ�������*/
/*��ʱ�������������ýṹ�壬���ж�ʱ������*/
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
/*��ʱ������ȽϽṹ�壬4ͨ��ռ�ձȲ�ͬ������Ҫ����*/
TIM_OCInitTypeDef  TIM_OCInitStructure;



/*
*********************************************************************************************************
*	�� �� ��: Motor_PWM_GPIO_Config
*	����˵��: TIM1���õ����ų�ʼ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void Motor_PWM_GPIO_Config(void)
{
/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*����PE�˿ڵ�GPIO����ʱ��*/
	RCC_AHB1PeriphClockCmd (MOTOR_PWM_PIN_CLK, ENABLE); 
	
  /* ָ�����Ÿ��ù��� */
	GPIO_PinAFConfig(MOTOR_PWM_PORT,MOTOR1_PWM_PINSOURCE, MOTOR_PWM_PIN_AF); 
	GPIO_PinAFConfig(MOTOR_PWM_PORT,MOTOR2_PWM_PINSOURCE, MOTOR_PWM_PIN_AF);
	GPIO_PinAFConfig(MOTOR_PWM_PORT,MOTOR3_PWM_PINSOURCE, MOTOR_PWM_PIN_AF);
	GPIO_PinAFConfig(MOTOR_PWM_PORT,MOTOR4_PWM_PINSOURCE, MOTOR_PWM_PIN_AF);
	
	/* ��ʱ���������ų�ʼ�� */															   
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
*	�� �� ��: Motor_PWM_TIM_Mode_Config
*	����˵��: TIM1����ģʽ��ʼ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void Motor_PWM_TIM_Mode_Config(void)
{
 	// ����TIM1ʱ��
  RCC_APB2PeriphClockCmd(MOTOR_PWM_TIM_CLK, ENABLE); 
	
  // �߼����ƶ�ʱ��ʱ��ԴTIM1CLK = HCLK=180MHz 
	// �趨��Ƶϵ����ʹ��ʱ��ʱ��Ƶ��ΪTIM1CLK/(TIM_Prescaler+1)=10MHz
  TIM_TimeBaseStructure.TIM_Prescaler = 18-1;		
	
  //����ʱ����0������199����Ϊ200�Σ�Ϊһ����ʱ����,ʹ�¼�����Ϊ10M/200=50KHz
  TIM_TimeBaseStructure.TIM_Period = 200-1;
	 
  // ����ʱ�ӷ�Ƶ����Ӱ��
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
  // ������ʽ
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
  // �ظ�������
  TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// ��ʼ����ʱ��TIM1
	TIM_TimeBaseInit(MOTOR_PWM_TIM, &TIM_TimeBaseStructure);
	
  /*PWMģʽ����*/
	//����ΪPWMģʽ1
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	//�Ƚ����ʹ�ܣ��ȽϽ����������ţ����Ȳ�ʹ��
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;	
	//���������ֹ������Ҫ�����ź�
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	//��ʼ��ռ�ձ�Ϊ50%
  TIM_OCInitStructure.TIM_Pulse = MOTOR_PWM_DutyInitValue ;
	//��Ч���Ϊ�ߵ�ƽ
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  //������ƽ���ã���Ӱ��
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	
  //ʹ��ͨ��1
  TIM_OC1Init(MOTOR_PWM_TIM, &TIM_OCInitStructure);	  
	/* ʹ��ͨ��1�Զ����� */
	TIM_OC1PreloadConfig(MOTOR_PWM_TIM, TIM_OCPreload_Enable);
	
  TIM_OC2Init(MOTOR_PWM_TIM, &TIM_OCInitStructure);	  	
	TIM_OC2PreloadConfig(MOTOR_PWM_TIM, TIM_OCPreload_Enable);
	
  TIM_OC3Init(MOTOR_PWM_TIM, &TIM_OCInitStructure);	  
	TIM_OC3PreloadConfig(MOTOR_PWM_TIM, TIM_OCPreload_Enable);
	
  TIM_OC4Init(MOTOR_PWM_TIM, &TIM_OCInitStructure);	  
	TIM_OC4PreloadConfig(MOTOR_PWM_TIM, TIM_OCPreload_Enable);


	// ʹ�ܶ�ʱ��
	TIM_Cmd(MOTOR_PWM_TIM, ENABLE);	
	
	/* �������ʹ�� */
  TIM_CtrlPWMOutputs(MOTOR_PWM_TIM, ENABLE);
}

/*
*********************************************************************************************************
*	�� �� ��: MOTOR_PWM_Init
*	����˵��: ��ʼ��TIM��ʹ֮���Բ���ָ��Ƶ�ʵ�PWM����Ĭ�ϳ�ʼ����4·��Ϊ50KHz��ռ�ձ�50%���������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Motor_PWM_Init(void)
{
	Motor_PWM_GPIO_Config();
	Motor_PWM_TIM_Mode_Config();
}

/*
*********************************************************************************************************
*	�� �� ��: MOTOR_PWM_Output
*	����˵��: ָ��һͨ����ĳ��ռ�ձ����
*	��    ��: ��
*	�� �� ֵ: ��
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

