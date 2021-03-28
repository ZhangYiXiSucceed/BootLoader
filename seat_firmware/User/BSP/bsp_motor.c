#include "bsp_motor.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

//���ļ��ڣ��漰��������ģ������0��ʼ��ţ���Ӱ�����Ƶ������ȣ������1��ʼ��š�
//��ʱ����180M��ƵΪ������

#define DEFAULT_MOTOR_START_SPEED 40
int8_t  MotorStage[4];
	
/*
*********************************************************************************************************
*	�� �� ��: Motor_SoftDelay
*	����˵��: ������λ�����߼�����ʱ,�����ò��ÿ����Խ��
*	��    ��: ��
*	�� �� ֵ: ��
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
*	�� �� ��: Motor_ControlPin_Init
*	����˵��: �������оƬ��������ų�ʼ��
*	��    ��: ��
*	�� �� ֵ: ��
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
	
	/*Ϊ�����Sleep�л�������״̬ʱ��nFault�ŵĶ��ݵ͵�ƽ�����жϣ�����Sleepģʽ*/
	GPIO_SetBits(Motor1_Sleep_GPIO_PORT,Motor1_Sleep_PIN);
	GPIO_SetBits(Motor2_Sleep_GPIO_PORT,Motor2_Sleep_PIN);
	GPIO_SetBits(Motor3_Sleep_GPIO_PORT,Motor3_Sleep_PIN);
	GPIO_SetBits(Motor4_Sleep_GPIO_PORT,Motor4_Sleep_PIN);
}

/*
*********************************************************************************************************
*	�� �� ��: Motor_FaultPin_Init
*	����˵��: ����������ų�ʼ��������DRV8873�����жϽ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Motor_FaultPin_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;	
	
	//�ⲿ�ж�ʱ�Ӵ�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE); //�ⲿ�ж�ʱ�Ӵ�
	//IO�˿�ʱ����Control Pin Init�������ã������ٴγ�ʼ��
	
	//Motor1_nFault PB11
	{
	GPIO_InitStructure.GPIO_Pin= Motor1_nFault_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;           //����ģʽ
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;         //���죬ʵ������Ϊ���룬��Ӱ��
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;       //�ⲿ������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;      
	GPIO_Init(Motor1_nFault_GPIO_PORT, &GPIO_InitStructure);	
	//�ⲿ�ж�Դ����
  SYSCFG_EXTILineConfig(Motor1_nFault_EXTI_PORTSOURCE,Motor1_nFault_EXTI_PINSOURCE);	
	/* �����ⲿ�ж��� */
	EXTI_InitStructure.EXTI_Line = Motor1_nFault_EXTI_LINE ;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  	/* �½��ش��� */
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	/* ʹ���ж�*/
	NVIC_InitStructure.NVIC_IRQChannel = Motor1_nFault_EXTI_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x04;//����4,4bit��ռ���ȼ���0bit�����ȼ���IO�жϵȼ�Ϊ4������Ϊ8
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
	//�ⲿ�ж�Դ����
	SYSCFG_EXTILineConfig(Motor2_nFault_EXTI_PORTSOURCE,Motor2_nFault_EXTI_PINSOURCE);	
	/* �����ⲿ�ж��� */
	EXTI_InitStructure.EXTI_Line = Motor2_nFault_EXTI_LINE ;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  	/* �½��ش��� */
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	/* ʹ���ж�*/
	NVIC_InitStructure.NVIC_IRQChannel = Motor2_nFault_EXTI_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x04;//Ҳ����Ϊ4
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
	//�ⲿ�ж�Դ����
  SYSCFG_EXTILineConfig(Motor3_nFault_EXTI_PORTSOURCE,Motor3_nFault_EXTI_PINSOURCE);
	/* �����ⲿ�ж��� */
	EXTI_InitStructure.EXTI_Line = Motor3_nFault_EXTI_LINE ;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  	/* �½��ش��� */
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	/* ʹ���ж�*/
	NVIC_InitStructure.NVIC_IRQChannel = Motor3_nFault_EXTI_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x05;//Ҳ����Ϊ4
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	}

	//Motor4_nFault PA11��ʱ�����ж����ã��ȴ�����Ӳ���˿ڵ���
}

/*********************************************************************************************************
*	�� �� ��: EXTI9_5_IRQHandler,��ǰ��������ж�ʱֻ�ǰ�оƬ���ó�standbyģʽ
*	����˵��: �ⲿ�жϷ������
*	��    �Σ���
*	�� �� ֵ: ��
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
*	�� �� ��: EXTI15_10_IRQHandler
*	����˵��: �ⲿ�жϷ������
*	��    �Σ���
*	�� �� ֵ: ��
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
*	�� �� ��: MotorInit
*	����˵��: �����ʼ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/

void MotorInit(void)
{ 
	//ʹ�ܡ���ɲ�����ƽŵĳ�ʼ��
	Motor_ControlPin_Init();
	
	//����ŵĳ�ʼ�����ж�����
	Motor_FaultPin_Init();
	
	//PWM��ʼ��
  Motor_PWM_Init();
	
	//����ͨ������Standbyģʽ��ͬʱ��բ
	Motor_ChangeToStatus_Standby(Motor_Channel_123);
  Motor_BrakeON(Motor_Channel_123);
}

/*
*********************************************************************************************************
*	�� �� ��: Motor_BrakeON
*	����˵��: ɲ��������BRAKE=1
*	��    ��: ChannelNum 0-3 �ĸ�����ͨ����4Ϊ0-2��ͨ����5Ϊ0-3�ĸ�ͨ��
*	�� �� ֵ: ��
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
*	�� �� ��: Motor_BrakeRelease
*	����˵��: �ͷ�ɲ����BRAKE=0
*	��    ��: ChannelNum 0-3 �ĸ�����ͨ����4Ϊ0-2��ͨ����5Ϊ0-3�ĸ�ͨ��
*	�� �� ֵ: ��
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
*	�� �� ��: Motor_ChangeToStatus_Standby
*	����˵��: �л�������״̬��������� DISABLE=1
            ��ʱSPI�ɶ�ȡ��ش��������Ϣ��д���ò����Ĵ���
*	��    ��: ��
*	�� �� ֵ: ��
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
	
 // Motor_SoftDelay();//DRV8873����Standby״̬��Ҫһ��ʱ��
}

/*
*********************************************************************************************************
*	�� �� ��: Motor_ChangeToStatus_Active
*	����˵��: �л�������״̬��оƬ�˳�Standbyģʽ��H�Ź��� DISABLE=0
            IN1 IN2�ֱ����뷽���ƽ��PWM������ʵ�ֵ��ת��
*	��    ��: ��
*	�� �� ֵ: ��
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

 // Motor_SoftDelay();;//DRV8873 Enable��Ҫ2.5us	
}

/*
*********************************************************************************************************
*	�� �� ��: Motor_ChangeToStatus_Manual
*	����˵��: �л����ֶ�״̬��оƬ����Standbyģʽ���ͷ�ɲ����H�Ÿ��� DISABLE=1,BRAKE=0
            ������Ҫ�˹��ƶ�����Ͽ��ٵ����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Motor_ChangeToStatus_Manual(uint8_t ChannelNum)
{
  Motor_ChangeToStatus_Standby(ChannelNum);
	Motor_BrakeRelease(ChannelNum);
}

/*
*********************************************************************************************************
*	�� �� ��: Motor_SetMoveDirection
*	����˵��: �趨ָ����������з���
*	��    ��: ��
*	�� �� ֵ: ��
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
*	�� �� ��: Motor_DriveByStage
*	����˵��: �ú������ݵ�ǰ�׶���ֵ�����Ƶ������ָ����������,���г�ʼ�ٶ�Ϊ30������ֵ�µ�������Ͽ���
            �˷���Ħ���������˶����βδ�������0-100����������0-70���ٵ��ӵ���ʼ�ٶ���
*	��    ��: ChannelNum ���ͨ����� 0 1 2 3
            StageNum   -100-+100���׶α��룬����ָʾ���򣬽׶δ����ٶ�ֵ
*	�� �� ֵ: ��
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
		
		if(StageNum>0)//0����100,��������
		{
		  Motor_SetMoveDirection(ChannelNum,Motor_Direction_Forward);
//			speedRatio=StageNum;
//			speedRatio=speedRatio*0.7;
			postiveSpeed=StageNum;
			postiveSpeed=postiveSpeed*2;
			postiveSpeed=200-postiveSpeed;
			Motor_PWM_Output(ChannelNum,postiveSpeed);
		}
		else if(StageNum<0)//��������
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
*	�� �� ��: Abs_MinusOne
*	����˵��: �Ծ���ֵ��һ�����Ų���
*	��    ��: Value����������ָ��
*	�� �� ֵ: ��
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
*	�� �� ��: Abs_PlusOne
*	����˵��: �Ծ���ֵ��һ�����Ų���
*	��    ��: Value����������ָ��
*	�� �� ֵ: ��
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
*	�� �� ��: Motor_Action_Normal
*	����˵��: ��������µĵ������
*	��    ��: MotorNum         �����ţ�0 1 2 3
            MotorDirection   ��ת���� 1Ϊ����-1Ϊ����
            MotorSpeed       ����ٶȣ�0-100���ڲ�*2תΪ0-200��ռ�ձ�,�趨Ĭ��Ϊ30������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Motor_Action_Normal(uint8_t MotorNum,int8_t MotorDirection,uint8_t MotorSpeed)
{ 
	int8_t DirectionFlag;
	
	if((MotorNum>3)||(MotorSpeed>100))
	{
		return;
	}
	
	/*������ٶ�Ϊ0����Ҫ��ֹͣ����*/
	if(MotorSpeed==0)
	{
    if(MotorStage[MotorNum]==0)	
		{
			Motor_BrakeON(MotorNum);//ɲ��
		}
    else
		{  /*���������Ϊ�˻������ټ��ٳ��*/
//			/*����ֵ-1*/
//			Abs_MinusOne(&MotorStage[MotorNum]);
//			
//			if(abs(MotorStage[MotorNum])<DEFAULT_MOTOR_START_SPEED)
//      {
//				MotorStage[MotorNum]=0;
//			}	
			
			/*ֱ�Ӽ��ٵ�0*/
			MotorStage[MotorNum]=0;
			/*�������*/
      Motor_DriveByStage(MotorNum,MotorStage[MotorNum]);	
		}			
	}
	else/*�����ٶȲ�Ϊ0����Ҫ�����*/
	{ 
   	/*���ͷ�ɲ��*/
		Motor_BrakeRelease(MotorNum);
		
		DirectionFlag=MotorDirection*MotorStage[MotorNum];//ǰһ�˶�״̬�����뵱ǰҪ���˶������Ƿ�һ��
		
		if(DirectionFlag>0)/*ͬ����٣�ֱ�ӽ����ٶ��𲽵���*/
		{ 
			if(MotorSpeed>abs(MotorStage[MotorNum]))/*Ҫ����ٶȱȵ�ǰ�ٶȴ�*/
			{
				Abs_PlusOne(&MotorStage[MotorNum]);//�ٶȾ���ֵ+1
			}
			else if(MotorSpeed<abs(MotorStage[MotorNum]))/*Ҫ����ٶȱȵ�ǰ�ٶ�С*/
			{
				Abs_MinusOne(&MotorStage[MotorNum]);//�ٶȾ���ֵ-1
			}	
			
			Motor_DriveByStage(MotorNum,MotorStage[MotorNum]);		
		}
		else
		{
			/*���ٹ����*/ 
			if(DirectionFlag==0)
			{ /*Ŀ���ٶ��������*/
				if(MotorDirection>0)
				{
					MotorStage[MotorNum]+=DEFAULT_MOTOR_START_SPEED;
				}
				else if(MotorDirection<0)/*Ŀ���ٶ��Ƿ����*/
				{
					MotorStage[MotorNum]-=DEFAULT_MOTOR_START_SPEED;
				}
			}
			else
			{ /*������٣���ͬ����٣��ٷ������*/
				if(DirectionFlag<0)
				{ 		
				  Abs_MinusOne(&MotorStage[MotorNum]);		
				  /*�������*/
					Motor_DriveByStage(MotorNum,MotorStage[MotorNum]);				
				}
			}
		}
	}	
}

/*
*********************************************************************************************************
*	�� �� ��: Motor_Action_Emergency
*	����˵��: ��������ƶ�
*	��    ��: ������ 0 1 2 3 
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Motor_Action_Emergency(uint8_t MotorNum)
{
	if((MotorNum>3))
	{ /*ֻ����0 1 2 3���������ͨ��*/
		return;
	}
	/*оƬ�л�ΪStandbyģʽ��H�������������*/
	Motor_ChangeToStatus_Standby(MotorNum);
	
	/*PWMֹͣ���*/
  Motor_PWM_Output(MotorNum,0);
	/*ɲ������*/
	Motor_BrakeON(MotorNum);
	/*stage��0*/
	MotorStage[MotorNum]=0;
}

/*
*********************************************************************************************************
*	�� �� ��: Motor_Error_Handle
*	����˵��: ����������оƬ����
*	��    ��: MotorErrorCode
*	�� �� ֵ: ��
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
	
	/*���ؿ�·��������*/
	if(ErrorBit[0]==1){return;}
	
	/*���Ⱦ��棬������*/
	if(ErrorBit[5]==1){return;}
	
	/*���ȴ��󣬹�������,оƬ���ܣ�ֹͣPWM���ͷ�ɲ��*/
	if(ErrorBit[1]==1||ErrorBit[2]==1)
  {
		Motor_ChangeToStatus_Manual(MotorNum);
		return;
	}
		/*UVLO��������粻�㣩��CPUV����ɱ���ѹ���㣩��ֹͣPWM��оƬ����*/
	if(ErrorBit[3]==1||ErrorBit[4]==1)
  {
		Motor_ChangeToStatus_Standby(MotorNum);
		return;
	}
}
