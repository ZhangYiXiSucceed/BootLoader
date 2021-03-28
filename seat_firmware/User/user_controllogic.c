#include "user_controllogic.h"
#include "user_datastructure.h"
#include "stdlib.h"


extern uint16_t Seat_Position_Now[4];
extern Axis_Cbrt_Data_T Axis_Cbrt_Data[4];
extern uint16_t Axis_LinkPoint_Position[5];

Seat_Logic_Data_T Seat_Logic_Data;
extern Seat_Status_T SeatStatus;//����״̬��ָʾ�Ƿ���һ��������
/*
*********************************************************************************************************
*	�� �� ��: MotorSpeedLimit
*	����˵��: �ٽ����٣����ݵ�ǰλ����Ŀ��λ�ø�������ٶ�
*	��    ��: MotorNum
						CurrentPosition
            DestPosition
*	�� �� ֵ: motorspeed 0 ��10-100
*********************************************************************************************************
*/
uint8_t MotorSpeedLimit(uint8_t MotorNum,int16_t CurrentPosition,int16_t DestPosition)
{
	int16_t offset;
	uint8_t MotorSpeedLimited;
	
	offset=abs(DestPosition-CurrentPosition);
	
	if(offset>DEFAULT_AXIS_DISTANCE_SLOW)
	{
		return Axis_Cbrt_Data[MotorNum].Axis_Motor_Speed;
	}
	
	if((offset<=DEFAULT_AXIS_DISTANCE_SLOW)&&(offset>DEFAULT_AXIS_DISTANCE_STOP))
	{
		MotorSpeedLimited=(Axis_Cbrt_Data[MotorNum].Axis_Motor_Speed)/2;
		
		if(MotorSpeedLimited<=10)
		{
			MotorSpeedLimited=0;
		}
	}
	else
	{
		if(offset<=DEFAULT_AXIS_DISTANCE_STOP)
		{
			MotorSpeedLimited=0;
		}
	}
	
	return MotorSpeedLimited;
}

/*
*********************************************************************************************************
*	�� �� ��: SeatLogicDataInit
*	����˵��: �ӱ궨���ݽṹ�еõ��߼��������ֵ,���ݵ�λ�����ӱ�־�����Ƿ�*-1,���ں�����С�ж�
            ���궨����������Ҫ��RTX��ʼ��ʱ���ã�δ�궨���Ĳ���Ҫ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void SeatLogicDataInit(void)
{
	uint8_t index;
	
	/*4��Ĵ򿪡����𡢰뿪ֵ*/
	for(index=0;index<4;index++)
	{
			Seat_Logic_Data.Axis_Logic_Data[index].Close_Position=Axis_Cbrt_Data[index].Axis_Close_Position*(Axis_Cbrt_Data[index].Axis_Poten_Reverse);
			Seat_Logic_Data.Axis_Logic_Data[index].Open_Position=Axis_Cbrt_Data[index].Axis_Open_Position*(Axis_Cbrt_Data[index].Axis_Poten_Reverse);
		  Seat_Logic_Data.Axis_Logic_Data[index].Half_Position=Axis_Cbrt_Data[index].Axis_Half_Position*(Axis_Cbrt_Data[index].Axis_Poten_Reverse);
	}	
	
	/*2��������������*/
	Seat_Logic_Data.LinkPoint_B1=Axis_LinkPoint_Position[0]*(Axis_Cbrt_Data[0].Axis_Poten_Reverse);
	Seat_Logic_Data.LinkPoint_B2=Axis_LinkPoint_Position[1]*(Axis_Cbrt_Data[0].Axis_Poten_Reverse);

	/*3���ȿ���������*/
  Seat_Logic_Data.LinkPoint_L1=Axis_LinkPoint_Position[2]*(Axis_Cbrt_Data[1].Axis_Poten_Reverse);
  Seat_Logic_Data.LinkPoint_L2=Axis_LinkPoint_Position[3]*(Axis_Cbrt_Data[1].Axis_Poten_Reverse);
	Seat_Logic_Data.LinkPoint_L3=Axis_LinkPoint_Position[4]*(Axis_Cbrt_Data[1].Axis_Poten_Reverse);
	
	SeatStatus=Seat_Status_Idle;/*���γ�ʼ״̬ΪIDLE*/
}


/*
*********************************************************************************************************
*	�� �� ��: ControlLogic_NonCbrtedSeat
*	����˵��: δ�궨ǰ�������߼�����
*	��    ��: KeyValue ����ֵ��ֻ��6���㶯�����İ��£���������Ӧ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void ControlLogic_NonCbrtedSeat(uint8_t KeyValue)
{		
	switch(KeyValue)
	{ 
		case 0:Motor_Action_Normal(Motor_Channel_1 , Motor_Direction_None , 0);
			     break;/*KeyValue=0�ް��������¼��������������е�*/
		
		case 1:Motor_Action_Normal(Motor_Channel_1 , Motor_Direction_Forward*Axis_Cbrt_Data[0].Axis_Motor_Reverse , DEFAULT_MOTOR_SPEED);
		       break;/*KeyValue=1,�����򿪰������£���Ĭ���ٶ���������*/
		case 2:Motor_Action_Normal(Motor_Channel_1 , Motor_Direction_None , 0);
		       break;/*KeyValue=2,�����򿪰����ɿ��������ٶ�0�����л���ֹͣ*/
		
		case 4:Motor_Action_Normal(Motor_Channel_1 , Motor_Direction_Reverse*Axis_Cbrt_Data[0].Axis_Motor_Reverse , DEFAULT_MOTOR_SPEED);
		       break;/*KeyValue=1,�������𰴼����£���Ĭ���ٶȷ�������*/
		case 5:Motor_Action_Normal(Motor_Channel_1 , Motor_Direction_None , 0);
		       break;/*KeyValue=1,�������𰴼��ɿ���ͣ��*/
		
		case 7:Motor_Action_Normal(Motor_Channel_2 , Motor_Direction_Forward*Axis_Cbrt_Data[1].Axis_Motor_Reverse , DEFAULT_MOTOR_SPEED);
		       break;/*KeyValue=7,�ȿ��򿪰�������*/
		case 8:Motor_Action_Normal(Motor_Channel_2 , Motor_Direction_None , 0);
		       break;/*KeyValue=8,�ȿ��򿪰����ɿ�*/
		
		case 10:Motor_Action_Normal(Motor_Channel_2 , Motor_Direction_Reverse*Axis_Cbrt_Data[1].Axis_Motor_Reverse , DEFAULT_MOTOR_SPEED);
		       break;/*KeyValue=10,�ȿ����𰴼�����*/
		case 11:Motor_Action_Normal(Motor_Channel_2 , Motor_Direction_None , 0);
		       break;/*KeyValue=11,�ȿ����𰴼��ɿ�*/
	
		case 13:Motor_Action_Normal(Motor_Channel_3 , Motor_Direction_Forward*Axis_Cbrt_Data[2].Axis_Motor_Reverse , DEFAULT_MOTOR_SPEED);
		       break;/*KeyValue=13,��̤�򿪰�������*/
		case 14:Motor_Action_Normal(Motor_Channel_3 , Motor_Direction_None , 0);
		       break;/*KeyValue=14,��̤�򿪰����ɿ�*/
	
		case 16:Motor_Action_Normal(Motor_Channel_3 , Motor_Direction_Reverse*Axis_Cbrt_Data[2].Axis_Motor_Reverse , DEFAULT_MOTOR_SPEED);
		       break;/*KeyValue=16,��̤���𰴼�����*/
		case 17:Motor_Action_Normal(Motor_Channel_3 , Motor_Direction_None , 0);
		       break;/*KeyValue=17,��̤���𰴼��ɿ�*/
		
		default:/*19-27Ϊһ�����ܣ�δ�궨ǰ��Ч*/
			      break;		
	}
}

/*
*********************************************************************************************************
*	�� �� ��: ControlLogic_CbrtedSeat
*	����˵��: �ѱ궨���������߼�����
*	��    ��: KeyValue ����ֵ����9��������Ӧ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void ControlLogic_CbrtedSeat(uint8_t KeyValue)
{
	switch(KeyValue)
	{
		case 0:/*KeyValue=0�ް��������¼��������������е�*/
			     break;/*�����κδ���*/
	
		       /*�����򿪰������¼�����*/
		case 1:CbrtedSeatLogic_BackOpen_PressDown();
			     break;
		case 2:CbrtedSeatLogic_Key_Popup();
			     break;
		       
		       /*�������𰴼����¼�����*/
	  case 4:CbrtedSeatLogic_BackClose_PressDown();
		       break;			      
		case 5:CbrtedSeatLogic_Key_Popup();
			     break;
		
		       /*�ȿ��򿪰������¼�����*/
		case 7:CbrtedSeatLogic_LegOpen_PressDown();
			     break;
		case 8:CbrtedSeatLogic_Key_Popup();
			     break;
		
		       /*�ȿ����𰴼����¼�����*/
	  case 10:CbrtedSeatLogic_LegClose_PressDown();
		        break;			      
		case 11:CbrtedSeatLogic_Key_Popup();
			      break;
					 
					 /*��̤�򿪰������¼�����*/
		case 13:CbrtedSeatLogic_FootOpen_PressDown();
			      break;
		case 14:CbrtedSeatLogic_Key_Popup();
			      break;
					 
					 /*��̤���𰴼����¼�����*/
	  case 16:CbrtedSeatLogic_LegClose_PressDown();
		        break;			      
		case 17:CbrtedSeatLogic_Key_Popup();
			      break;
					 
					 /*һ��ȫ�ɰ������¼�����*/
	  case 19:CbrtedSeatLogic_OneKeyOpen_PressDown();
		        break;			      
		case 20:CbrtedSeatLogic_Key_Popup();
			      break;
					 
					 /*һ�����ɰ������¼�����*/
	  case 22:CbrtedSeatLogic_OneKeyHalf_PressDown();
		        break;			      
		case 23:CbrtedSeatLogic_Key_Popup();
			      break;
					 
					/*һ��ȫ�հ������¼�����*/
	  case 25:CbrtedSeatLogic_OneKeyClose_PressDown();
		        break;			      
		case 26:CbrtedSeatLogic_Key_Popup();
			      break;
		
		default:break;
	}	
}

/*
*********************************************************************************************************
*	�� �� ��: CbrtedSeatLogic_Key_Popup
*	����˵��: �ѱ궨���εĵ㶯�����ɿ���Ķ����߼���3���ֹͣ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void CbrtedSeatLogic_Key_Popup(void)
{
	 /*�����ֹͣ��ת*/
	Motor_Action_Normal(Motor_Channel_1 , Motor_Direction_None , 0);
	Motor_Action_Normal(Motor_Channel_2 , Motor_Direction_None , 0);
	Motor_Action_Normal(Motor_Channel_3 , Motor_Direction_None , 0);		 
	/*����״̬�л�ΪIDLE*/
	SeatStatus=Seat_Status_Idle;
}

/*
*********************************************************************************************************
*	�� �� ��: CbrtedSeatLogic_BackOpen_PressDown
*	����˵��: �ѱ궨���εı����򿪰��������߼�����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void CbrtedSeatLogic_BackOpen_PressDown(void)
{
  int16_t BackPositionNow,LegPositionNow,LegLinkPositionDest;
	float radio;
	uint8_t limitSpeed;
	
	/*����������ȿ��ĵ�λ���Ƿ�װ�ģ���Ƚ�ʱ��ǰλ��ҲתΪ����*/
	BackPositionNow=Seat_Position_Now[0]*(Axis_Cbrt_Data[0].Axis_Poten_Reverse);
	LegPositionNow=Seat_Position_Now[1]*(Axis_Cbrt_Data[1].Axis_Poten_Reverse);
	
	/*B1 B2��궨�����������ֵһ��ֱ�ӷ��أ�����������ܷ����������*/
	if(Seat_Logic_Data.LinkPoint_B2==Seat_Logic_Data.LinkPoint_B1){return;}

	/*ǰһʱ�̵�����״̬Ϊһ������*/
	if(SeatStatus>=Seat_Status_OneKeyOpen)
	{//һ�����ܱ��㶯������ֹ
		Motor_Action_Normal(Motor_Channel_1 , Motor_Direction_None , 0);
		Motor_Action_Normal(Motor_Channel_2 , Motor_Direction_None , 0);
		Motor_Action_Normal(Motor_Channel_3 , Motor_Direction_None , 0);		
	  /*����״̬�л�ΪIDLE*/
		SeatStatus=Seat_Status_Idle;
		return;
	}
	
	/*Bnow>=Bopen,�����ִ��Խ���򿪼���λ,ʵʩ�ƶ�*/
	if(BackPositionNow>=Seat_Logic_Data.Axis_Logic_Data[0].Open_Position)
	{
		Motor_Action_Normal(Motor_Channel_1 , Motor_Direction_None , 0);
		SeatStatus=Seat_Status_Idle;
		return;
	}
	
	if(BackPositionNow<Seat_Logic_Data.LinkPoint_B1)
	{ /*Bclos--Bnow---B1,����������λ-B1������֮��*/
		Motor_Action_Normal(Motor_Channel_1 , (Motor_Direction_Forward*Axis_Cbrt_Data[0].Axis_Motor_Reverse) , Axis_Cbrt_Data[0].Axis_Motor_Speed);
		Motor_Action_Normal(Motor_Channel_2 , Motor_Direction_None , 0);//�ȿ���������
		SeatStatus=Seat_Status_Back_Open;
	}
	else
	{ /*B1--Bnow---B2,����������������֮��*/
		if((BackPositionNow>=Seat_Logic_Data.LinkPoint_B1)&&(BackPositionNow<Seat_Logic_Data.LinkPoint_B2))
		{ // Ldest=(L1-Lclose)*(Bnow-B1)/(B2-B1)+Lclose 
			radio=(abs(BackPositionNow-Seat_Logic_Data.LinkPoint_B1))*1.0/abs(Seat_Logic_Data.LinkPoint_B2-Seat_Logic_Data.LinkPoint_B1);
			radio=radio*(abs(Seat_Logic_Data.LinkPoint_L1-Seat_Logic_Data.Axis_Logic_Data[1].Close_Position));
			LegLinkPositionDest=(int16_t)radio+Seat_Logic_Data.Axis_Logic_Data[1].Close_Position;
			//�ȿ���Ҫ����
			if(LegPositionNow<LegLinkPositionDest)
			{ //�ȿ��������ٶ�����
				limitSpeed=MotorSpeedLimit(Motor_Channel_2 ,LegPositionNow,LegLinkPositionDest);
				Motor_Action_Normal(Motor_Channel_2 , (Motor_Direction_Forward*Axis_Cbrt_Data[1].Axis_Motor_Reverse) ,limitSpeed);	
			}
			else
			{ /*��������������Ŀ��㣬�ȿ�ͣ��*/
				Motor_Action_Normal(Motor_Channel_2 , Motor_Direction_None , 0);
			}
			//������
			Motor_Action_Normal(Motor_Channel_1 , (Motor_Direction_Forward*Axis_Cbrt_Data[0].Axis_Motor_Reverse) , Axis_Cbrt_Data[0].Axis_Motor_Speed);
		  SeatStatus=Seat_Status_Back_Open;		
		}
		else
		{
			/*B2--Bnow---Bopen,������B2������-��λ֮��,�ٶȸ�����ȫ��λ�ļ��ȷ��*/
			limitSpeed=MotorSpeedLimit(Motor_Channel_1 ,BackPositionNow,Seat_Logic_Data.Axis_Logic_Data[0].Open_Position);
			Motor_Action_Normal(Motor_Channel_1 , (Motor_Direction_Forward*Axis_Cbrt_Data[0].Axis_Motor_Reverse) , limitSpeed);
		  SeatStatus=Seat_Status_Back_Open;
		}
	}
}

/*
*********************************************************************************************************
*	�� �� ��: CbrtedSeatLogic_BackClose_PressDown
*	����˵��: �ѱ궨���εı������𰴼������߼�����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void CbrtedSeatLogic_BackClose_PressDown(void)
{
  int16_t BackPositionNow;
	uint8_t limitSpeed;
	
	/*���������λ���Ƿ�װ�ģ���Ƚ�ʱ��ǰλ��ҲתΪ����*/
	BackPositionNow=Seat_Position_Now[0]*(Axis_Cbrt_Data[0].Axis_Poten_Reverse);
	
	/*ǰһʱ�̵�����״̬Ϊһ������*/
	if(SeatStatus>=Seat_Status_OneKeyOpen)
	{//һ�����ܱ��㶯������ֹ
		Motor_Action_Normal(Motor_Channel_1 , Motor_Direction_None , 0);
		Motor_Action_Normal(Motor_Channel_2 , Motor_Direction_None , 0);
		Motor_Action_Normal(Motor_Channel_3 , Motor_Direction_None , 0);		
	  /*����״̬�л�ΪIDLE*/
		SeatStatus=Seat_Status_Idle;
		return;
	}
	
	/*�Ѿ�����ȫ��λ��*/
	if(BackPositionNow<=Seat_Logic_Data.Axis_Logic_Data[0].Close_Position)
	{ /*����ֹͣ������״̬�л�ΪIDLE*/
		Motor_Action_Normal(Motor_Channel_1 , Motor_Direction_None , 0);
		SeatStatus=Seat_Status_Idle;
		return;
	}
	else
	{ /*Bclose--Bnow---Bopen*/
		limitSpeed=MotorSpeedLimit(Motor_Channel_1 ,BackPositionNow,Seat_Logic_Data.Axis_Logic_Data[0].Close_Position);
		Motor_Action_Normal(Motor_Channel_1 , (Motor_Direction_Reverse*Axis_Cbrt_Data[0].Axis_Motor_Reverse) , limitSpeed);
		SeatStatus=Seat_Status_Back_Close;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: CbrtedSeatLogic_LegOpen_PressDown
*	����˵��: �ѱ궨���ε��ȿ��򿪰��������߼�����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void CbrtedSeatLogic_LegOpen_PressDown(void)
{
	int16_t LegPositionNow;
	uint8_t limitSpeed;
	
	LegPositionNow=Seat_Position_Now[1]*(Axis_Cbrt_Data[1].Axis_Poten_Reverse);
	
	if(SeatStatus>=Seat_Status_OneKeyOpen)
	{//һ�����ܱ��㶯������ֹ
		Motor_Action_Normal(Motor_Channel_1 , Motor_Direction_None , 0);
		Motor_Action_Normal(Motor_Channel_2 , Motor_Direction_None , 0);
		Motor_Action_Normal(Motor_Channel_3 , Motor_Direction_None , 0);		
	  /*����״̬�л�ΪIDLE*/
		SeatStatus=Seat_Status_Idle;
		return;
	}
	
	/*�ȿ�λ���Ѿ��ﵽ��Խ��ȫ��λ*/
	if(LegPositionNow>=Seat_Logic_Data.Axis_Logic_Data[1].Open_Position)
	{
		Motor_Action_Normal(Motor_Channel_2 , Motor_Direction_None , 0);
		SeatStatus=Seat_Status_Idle;
		return;
	}
	else
	{ /*Lclose--Lnow---Lopen*/
		limitSpeed=MotorSpeedLimit(Motor_Channel_2 ,LegPositionNow,Seat_Logic_Data.Axis_Logic_Data[1].Open_Position);
		Motor_Action_Normal(Motor_Channel_2 , (Motor_Direction_Reverse*Axis_Cbrt_Data[1].Axis_Motor_Reverse) , limitSpeed);
		SeatStatus=Seat_Status_Leg_Open;
		return;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: CbrtedSeatLogic_LegClose_PressDown
*	����˵��: �ѱ궨���ε��ȿ����𰴼������߼�����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void CbrtedSeatLogic_LegClose_PressDown(void)
{
	int16_t BackPositionNow,LegPositionNow,FootPositionNow,BackLinkPositionDest,FootLinkPositionDest;
	float radio;
	uint8_t limitSpeed;
	
	BackPositionNow=Seat_Position_Now[0]*(Axis_Cbrt_Data[0].Axis_Poten_Reverse);
	LegPositionNow=Seat_Position_Now[1]*(Axis_Cbrt_Data[1].Axis_Poten_Reverse);
	FootPositionNow=Seat_Position_Now[2]*(Axis_Cbrt_Data[2].Axis_Poten_Reverse);
	
	/*��ֹ��0*/
	if(Seat_Logic_Data.LinkPoint_L1==Seat_Logic_Data.Axis_Logic_Data[1].Close_Position){return;}
  if(Seat_Logic_Data.LinkPoint_L3==Seat_Logic_Data.LinkPoint_L2){return;}
	
	/*һ�����ܱ��㶯������ֹ*/
	if(SeatStatus>=Seat_Status_OneKeyOpen)
	{
		Motor_Action_Normal(Motor_Channel_1 , Motor_Direction_None , 0);
		Motor_Action_Normal(Motor_Channel_2 , Motor_Direction_None , 0);
		Motor_Action_Normal(Motor_Channel_3 , Motor_Direction_None , 0);		
		SeatStatus=Seat_Status_Idle;
		return;
	}
	
	/*Lnow<Lclose*/
	if(LegPositionNow<=Seat_Logic_Data.Axis_Logic_Data[1].Close_Position)
	{
		Motor_Action_Normal(Motor_Channel_2 , Motor_Direction_None , 0);
		SeatStatus=Seat_Status_Idle;
		return;
	}
	
	/*Lclose--Lnow--L1*/
	if((LegPositionNow>Seat_Logic_Data.Axis_Logic_Data[1].Close_Position)&&(LegPositionNow<=Seat_Logic_Data.LinkPoint_L1))
	{
		radio=abs(LegPositionNow-Seat_Logic_Data.Axis_Logic_Data[1].Close_Position)*1.0/abs(Seat_Logic_Data.LinkPoint_L1-Seat_Logic_Data.Axis_Logic_Data[1].Close_Position);
		radio=radio*abs(Seat_Logic_Data.LinkPoint_B2-Seat_Logic_Data.LinkPoint_B1);
		BackLinkPositionDest=(int16_t)radio+Seat_Logic_Data.LinkPoint_B1;
		/*������ǰʵ��λ�ô��ڸ��ݵ�ǰ�ȿ�λ�ü���õ�����������λ�ã�������Ҫ�����ջ�*/
		if(BackPositionNow>BackLinkPositionDest)
		{
			limitSpeed=MotorSpeedLimit(Motor_Channel_1 ,BackPositionNow,BackLinkPositionDest);
			Motor_Action_Normal(Motor_Channel_1 , (Motor_Direction_Reverse*Axis_Cbrt_Data[0].Axis_Motor_Reverse) , limitSpeed);
		}
		else
		{
			Motor_Action_Normal(Motor_Channel_1 , Motor_Direction_None , 0);
		}
		/*�ȿ������ٽ�ȫ��λʱ�������ٶ�����*/
		limitSpeed=MotorSpeedLimit(Motor_Channel_2 ,LegPositionNow,Seat_Logic_Data.Axis_Logic_Data[1].Close_Position);
		Motor_Action_Normal(Motor_Channel_2 , (Motor_Direction_Reverse*Axis_Cbrt_Data[1].Axis_Motor_Reverse) , limitSpeed);
		SeatStatus=Seat_Status_Leg_Close;
		return;
	}
  
	/*L1--Lnow--L2*/
	if((LegPositionNow>Seat_Logic_Data.LinkPoint_L1)&&(LegPositionNow<=Seat_Logic_Data.LinkPoint_L2))
	{ /*�ȿ������������������*/
		Motor_Action_Normal(Motor_Channel_2 , (Motor_Direction_Reverse*Axis_Cbrt_Data[1].Axis_Motor_Reverse) , Axis_Cbrt_Data[1].Axis_Motor_Speed);
		SeatStatus=Seat_Status_Leg_Close;
		return;
	}
	
	/*L2--Lnow--L3*/
	if((LegPositionNow>Seat_Logic_Data.LinkPoint_L2)&&(LegPositionNow<=Seat_Logic_Data.LinkPoint_L3))
	{
		radio=abs(LegPositionNow-Seat_Logic_Data.LinkPoint_L2)*1.0/abs(Seat_Logic_Data.LinkPoint_L3-Seat_Logic_Data.LinkPoint_L2);
		radio=radio*abs(Seat_Logic_Data.Axis_Logic_Data[2].Open_Position-Seat_Logic_Data.Axis_Logic_Data[2].Close_Position);
		FootLinkPositionDest=(int16_t)radio+Seat_Logic_Data.Axis_Logic_Data[2].Close_Position;
		/*���е�ǰʵ��λ�ô��ڸ��ݵ�ǰ�ȿ�λ�ü���õ�����������λ�ã�������Ҫ�����ջ�*/
		if(FootPositionNow>FootLinkPositionDest)
		{
			limitSpeed=MotorSpeedLimit(Motor_Channel_3 ,FootPositionNow,FootLinkPositionDest);
			Motor_Action_Normal(Motor_Channel_3 , (Motor_Direction_Reverse*Axis_Cbrt_Data[2].Axis_Motor_Reverse) , limitSpeed);
		}
		else
		{
			Motor_Action_Normal(Motor_Channel_3 , Motor_Direction_None , 0);
		}
		
		Motor_Action_Normal(Motor_Channel_2 , (Motor_Direction_Reverse*Axis_Cbrt_Data[1].Axis_Motor_Reverse) , Axis_Cbrt_Data[1].Axis_Motor_Speed);
		SeatStatus=Seat_Status_Leg_Close;
		return;
	}
	
	/*L3--Lnow--Lopen*/
	if((LegPositionNow>Seat_Logic_Data.LinkPoint_L3)&&(LegPositionNow<=Seat_Logic_Data.Axis_Logic_Data[1].Open_Position))
	{ /*�ȿ������������������*/
		Motor_Action_Normal(Motor_Channel_2 , (Motor_Direction_Reverse*Axis_Cbrt_Data[1].Axis_Motor_Reverse) , Axis_Cbrt_Data[1].Axis_Motor_Speed);
		SeatStatus=Seat_Status_Leg_Close;
		return;
	}	
}

/*
*********************************************************************************************************
*	�� �� ��: CbrtedSeatLogic_FootOpen_PressDown
*	����˵��: �ѱ궨���εĽ�̤�򿪰��������߼�����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void CbrtedSeatLogic_FootOpen_PressDown(void)
{
	int16_t FootPositionNow;
	uint8_t limitSpeed;
	
	/*�����̤��λ���Ƿ�װ�ģ���Ƚ�ʱ��ǰλ��ҲתΪ����*/
	FootPositionNow=Seat_Position_Now[2]*(Axis_Cbrt_Data[2].Axis_Poten_Reverse);
	
	/*ǰһʱ�̵�����״̬Ϊһ������*/
	if(SeatStatus>=Seat_Status_OneKeyOpen)
	{//һ�����ܱ��㶯������ֹ
		Motor_Action_Normal(Motor_Channel_1 , Motor_Direction_None , 0);
		Motor_Action_Normal(Motor_Channel_2 , Motor_Direction_None , 0);
		Motor_Action_Normal(Motor_Channel_3 , Motor_Direction_None , 0);		
	  /*����״̬�л�ΪIDLE*/
		SeatStatus=Seat_Status_Idle;
		return;
	}
	/*��̤λ���Ѿ�����ȫ��λ*/
	if(FootPositionNow>=Seat_Logic_Data.Axis_Logic_Data[2].Open_Position)
	{
		Motor_Action_Normal(Motor_Channel_3 , Motor_Direction_None , 0);		
		SeatStatus=Seat_Status_Idle;
	}
	else
	{
		limitSpeed=MotorSpeedLimit(Motor_Channel_3 ,FootPositionNow,Seat_Logic_Data.Axis_Logic_Data[2].Open_Position);
		Motor_Action_Normal(Motor_Channel_3 , (Motor_Direction_Forward*Axis_Cbrt_Data[2].Axis_Motor_Reverse) , limitSpeed);
		SeatStatus=Seat_Status_Foot_Open;
	}

	return;
}

/*
*********************************************************************************************************
*	�� �� ��: CbrtedSeatLogic_FootClose_PressDown
*	����˵��: �ѱ궨���εĽ�̤���𰴼������߼�����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void CbrtedSeatLogic_FootClose_PressDown(void)
{
	int16_t FootPositionNow;
	uint8_t limitSpeed;
	
	/*�����̤��λ���Ƿ�װ�ģ���Ƚ�ʱ��ǰλ��ҲתΪ����*/
	FootPositionNow=Seat_Position_Now[2]*(Axis_Cbrt_Data[2].Axis_Poten_Reverse);
	
	/*ǰһʱ�̵�����״̬Ϊһ������*/
	if(SeatStatus>=Seat_Status_OneKeyOpen)
	{//һ�����ܱ��㶯������ֹ
		Motor_Action_Normal(Motor_Channel_1 , Motor_Direction_None , 0);
		Motor_Action_Normal(Motor_Channel_2 , Motor_Direction_None , 0);
		Motor_Action_Normal(Motor_Channel_3 , Motor_Direction_None , 0);		
	  /*����״̬�л�ΪIDLE*/
		SeatStatus=Seat_Status_Idle;
		return;
	}
	/*��̤λ���Ѿ�����ȫ��λ*/
	if(FootPositionNow<=Seat_Logic_Data.Axis_Logic_Data[2].Close_Position)
	{
		Motor_Action_Normal(Motor_Channel_3 , Motor_Direction_None , 0);		
		SeatStatus=Seat_Status_Idle;
	}
	else
	{
		limitSpeed=MotorSpeedLimit(Motor_Channel_3 ,FootPositionNow,Seat_Logic_Data.Axis_Logic_Data[2].Close_Position);
		Motor_Action_Normal(Motor_Channel_3 , (Motor_Direction_Reverse*Axis_Cbrt_Data[2].Axis_Motor_Reverse) , limitSpeed);
		SeatStatus=Seat_Status_Foot_Close;
	}

	return;
}

/*
*********************************************************************************************************
*	�� �� ��: CbrtedSeatLogic_OneKeyOpen_PressDown
*	����˵��: �ѱ궨���ε�һ��ȫ�ɰ��������߼�����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void CbrtedSeatLogic_OneKeyOpen_PressDown(void)
{
	int16_t BackPositionNow,LegPositionNow,FootPositionNow;
	uint8_t limitSpeed,stu_leg,stu_back,stu_foot;
	
	/*һ�����ɡ�һ��ȫ�չ���ִ���ڼ䣬���������£����ж�һ�����ܣ�ȫ��ֹͣ*/
	/*����ǰ����һ��ȫ�����ְ��£�����Ϊ�����������ļ�ֵ����������������Ӱ��*/
	if((SeatStatus==Seat_Status_OneKeyHalf)||(SeatStatus==Seat_Status_OneKeyClose))
	{
		Motor_Action_Normal(Motor_Channel_1 , Motor_Direction_None , 0);
		Motor_Action_Normal(Motor_Channel_2 , Motor_Direction_None , 0);
		Motor_Action_Normal(Motor_Channel_3 , Motor_Direction_None , 0);		
		SeatStatus=Seat_Status_Idle;
		return;
	}
	
	BackPositionNow=Seat_Position_Now[0]*(Axis_Cbrt_Data[0].Axis_Poten_Reverse);
	LegPositionNow =Seat_Position_Now[1]*(Axis_Cbrt_Data[1].Axis_Poten_Reverse);
	FootPositionNow=Seat_Position_Now[2]*(Axis_Cbrt_Data[2].Axis_Poten_Reverse);
	stu_leg=0;
	stu_foot=0;
	stu_back=0;
	
	/*�����˶���״̬�趨*/
	if(BackPositionNow>=Seat_Logic_Data.Axis_Logic_Data[0].Open_Position)
	{
		Motor_Action_Normal(Motor_Channel_1 , Motor_Direction_None , 0);
		stu_back=0;
	}
	else
	{
		limitSpeed=MotorSpeedLimit(Motor_Channel_1 ,BackPositionNow,Seat_Logic_Data.Axis_Logic_Data[0].Open_Position);
		if(limitSpeed!=0)
		{
			Motor_Action_Normal(Motor_Channel_1 , (Motor_Direction_Forward*Axis_Cbrt_Data[0].Axis_Motor_Reverse) , limitSpeed);
			stu_back=1;
		}	
	}
	
	/*�ȿ��˶���״̬�趨*/
	if(LegPositionNow>=Seat_Logic_Data.Axis_Logic_Data[1].Open_Position)
	{
		Motor_Action_Normal(Motor_Channel_2 , Motor_Direction_None , 0);
		stu_leg=0;
	}
	else
	{
		limitSpeed=MotorSpeedLimit(Motor_Channel_2 ,LegPositionNow,Seat_Logic_Data.Axis_Logic_Data[1].Open_Position);
		if(limitSpeed!=0)
		{
			Motor_Action_Normal(Motor_Channel_2 , (Motor_Direction_Forward*Axis_Cbrt_Data[1].Axis_Motor_Reverse) , limitSpeed);
			stu_leg=1;
		}		
	}
	
		/*��̤�˶���״̬�趨*/
	if(FootPositionNow>=Seat_Logic_Data.Axis_Logic_Data[2].Open_Position)
	{
		Motor_Action_Normal(Motor_Channel_3 , Motor_Direction_None , 0);
		stu_foot=0;
	}
	else
	{
		limitSpeed=MotorSpeedLimit(Motor_Channel_3 ,FootPositionNow,Seat_Logic_Data.Axis_Logic_Data[2].Open_Position);
		if(limitSpeed!=0)
		{
			Motor_Action_Normal(Motor_Channel_3 , (Motor_Direction_Forward*Axis_Cbrt_Data[2].Axis_Motor_Reverse) , limitSpeed);
			stu_foot=1;
		}		
	}
	
  /*3��������һ�����˶�����Ϊ����һ���򿪹�����*/
  if((stu_back+stu_leg+stu_foot)!=0)	
	{
		SeatStatus=Seat_Status_OneKeyOpen;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: CbrtedSeatLogic_OneKeyClose_PressDown
*	����˵��: �ѱ궨���ε�һ��ȫ�հ��������߼�����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void CbrtedSeatLogic_OneKeyClose_PressDown(void)
{
	int16_t BackPositionNow,LegPositionNow,FootPositionNow;
	uint8_t limitSpeed,stu_leg,stu_back,stu_foot;
	
	if((SeatStatus==Seat_Status_OneKeyHalf)||(SeatStatus==Seat_Status_OneKeyOpen))
	{
		Motor_Action_Normal(Motor_Channel_1 , Motor_Direction_None , 0);
		Motor_Action_Normal(Motor_Channel_2 , Motor_Direction_None , 0);
		Motor_Action_Normal(Motor_Channel_3 , Motor_Direction_None , 0);		
		SeatStatus=Seat_Status_Idle;
		return;
	}
	
	BackPositionNow=Seat_Position_Now[0]*(Axis_Cbrt_Data[0].Axis_Poten_Reverse);
	LegPositionNow =Seat_Position_Now[1]*(Axis_Cbrt_Data[1].Axis_Poten_Reverse);
	FootPositionNow=Seat_Position_Now[2]*(Axis_Cbrt_Data[2].Axis_Poten_Reverse);
	stu_leg=0;
	stu_foot=0;
	stu_back=0;
	
	/*�����˶���״̬�趨*/
	if(BackPositionNow<=Seat_Logic_Data.Axis_Logic_Data[0].Close_Position)
	{
		Motor_Action_Normal(Motor_Channel_1 , Motor_Direction_None , 0);
		stu_back=0;
	}
	else
	{
		limitSpeed=MotorSpeedLimit(Motor_Channel_1 ,BackPositionNow,Seat_Logic_Data.Axis_Logic_Data[0].Close_Position);
		if(limitSpeed!=0)
		{
			Motor_Action_Normal(Motor_Channel_1 , (Motor_Direction_Reverse*Axis_Cbrt_Data[0].Axis_Motor_Reverse) , limitSpeed);
			stu_back=1;
		}	
	}
	
	/*�ȿ��˶���״̬�趨*/
	if(LegPositionNow<=Seat_Logic_Data.Axis_Logic_Data[1].Close_Position)
	{
		Motor_Action_Normal(Motor_Channel_2 , Motor_Direction_None , 0);
		stu_leg=0;
	}
	else
	{
		limitSpeed=MotorSpeedLimit(Motor_Channel_2 ,LegPositionNow,Seat_Logic_Data.Axis_Logic_Data[1].Close_Position);
		if(limitSpeed!=0)
		{
			Motor_Action_Normal(Motor_Channel_2 , (Motor_Direction_Reverse*Axis_Cbrt_Data[1].Axis_Motor_Reverse) , limitSpeed);
			stu_leg=1;
		}		
	}
	
		/*��̤�˶���״̬�趨*/
	if(FootPositionNow<=Seat_Logic_Data.Axis_Logic_Data[2].Close_Position)
	{
		Motor_Action_Normal(Motor_Channel_3 , Motor_Direction_None , 0);
		stu_foot=0;
	}
	else
	{
		limitSpeed=MotorSpeedLimit(Motor_Channel_3 ,FootPositionNow,Seat_Logic_Data.Axis_Logic_Data[2].Close_Position);
		if(limitSpeed!=0)
		{
			Motor_Action_Normal(Motor_Channel_3 , (Motor_Direction_Reverse*Axis_Cbrt_Data[2].Axis_Motor_Reverse) , limitSpeed);
			stu_foot=1;
		}	
	}
	
  /*3��������һ�����˶�����Ϊ����һ���򿪹�����*/
  if((stu_back+stu_leg+stu_foot)!=0)	
	{
		SeatStatus=Seat_Status_OneKeyClose;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: CbrtedSeatLogic_OneKeyHalf_PressDown
*	����˵��: �ѱ궨���ε�һ�����ɰ��������߼�����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void CbrtedSeatLogic_OneKeyHalf_PressDown(void)
{
	int16_t BackPositionNow,LegPositionNow,FootPositionNow;
	uint8_t limitSpeed,stu_leg,stu_back,stu_foot;
	
	if((SeatStatus==Seat_Status_OneKeyOpen)||(SeatStatus==Seat_Status_OneKeyClose))
	{
		Motor_Action_Normal(Motor_Channel_1 , Motor_Direction_None , 0);
		Motor_Action_Normal(Motor_Channel_2 , Motor_Direction_None , 0);
		Motor_Action_Normal(Motor_Channel_3 , Motor_Direction_None , 0);		
		SeatStatus=Seat_Status_Idle;
		return;
	}
	
	BackPositionNow=Seat_Position_Now[0]*(Axis_Cbrt_Data[0].Axis_Poten_Reverse);
	LegPositionNow =Seat_Position_Now[1]*(Axis_Cbrt_Data[1].Axis_Poten_Reverse);
	FootPositionNow=Seat_Position_Now[2]*(Axis_Cbrt_Data[2].Axis_Poten_Reverse);
	stu_leg=0;
	stu_foot=0;
	stu_back=0;
	
	/*�������д���*/
	if(BackPositionNow==Seat_Logic_Data.Axis_Logic_Data[0].Half_Position)
	{
		Motor_Action_Normal(Motor_Channel_1 , Motor_Direction_None , 0);
		stu_back=0;
	}
	else
	{
		limitSpeed=MotorSpeedLimit(Motor_Channel_1 ,BackPositionNow,Seat_Logic_Data.Axis_Logic_Data[0].Half_Position);
		
		if(limitSpeed!=0)
		 {
			 if(BackPositionNow>Seat_Logic_Data.Axis_Logic_Data[0].Half_Position)
		   {		
		     Motor_Action_Normal(Motor_Channel_1 , (Motor_Direction_Reverse*Axis_Cbrt_Data[0].Axis_Motor_Reverse) , limitSpeed);		
		   }
			 else
			 {
				 Motor_Action_Normal(Motor_Channel_1 , (Motor_Direction_Forward*Axis_Cbrt_Data[0].Axis_Motor_Reverse) , limitSpeed);
			 }
			 
		   stu_back=1;
		 }
	}
	
	/*�ȿ����д���*/
	if(LegPositionNow==Seat_Logic_Data.Axis_Logic_Data[1].Half_Position)
	{
		Motor_Action_Normal(Motor_Channel_2 , Motor_Direction_None , 0);
		stu_leg=0;
	}
	else
	{
		limitSpeed=MotorSpeedLimit(Motor_Channel_2 ,LegPositionNow,Seat_Logic_Data.Axis_Logic_Data[1].Half_Position);
		
		if(limitSpeed!=0)
		 {
			 if(LegPositionNow>Seat_Logic_Data.Axis_Logic_Data[1].Half_Position)
		   {		
		     Motor_Action_Normal(Motor_Channel_2 , (Motor_Direction_Reverse*Axis_Cbrt_Data[1].Axis_Motor_Reverse) , limitSpeed);		
		   }
			 else
			 {
				 Motor_Action_Normal(Motor_Channel_2 , (Motor_Direction_Forward*Axis_Cbrt_Data[1].Axis_Motor_Reverse) , limitSpeed);
			 }
			 
		   stu_leg=1;
		 }
	}
	
	/*��̤���д���*/
	if(FootPositionNow==Seat_Logic_Data.Axis_Logic_Data[2].Half_Position)
	{
		Motor_Action_Normal(Motor_Channel_3 , Motor_Direction_None , 0);
		stu_foot=0;
	}
	else
	{
		limitSpeed=MotorSpeedLimit(Motor_Channel_3 ,FootPositionNow,Seat_Logic_Data.Axis_Logic_Data[2].Half_Position);
		
		if(limitSpeed!=0)
		 {
			 if(FootPositionNow>Seat_Logic_Data.Axis_Logic_Data[2].Half_Position)
		   {		
		     Motor_Action_Normal(Motor_Channel_3 , (Motor_Direction_Reverse*Axis_Cbrt_Data[2].Axis_Motor_Reverse) , limitSpeed);		
		   }
			 else
			 {
				 Motor_Action_Normal(Motor_Channel_3 , (Motor_Direction_Forward*Axis_Cbrt_Data[2].Axis_Motor_Reverse) , limitSpeed);
			 }
			 
		   stu_foot=1;
		 }
	}
	
	  /*3��������һ�����˶�����Ϊ����һ���򿪹�����*/
  if((stu_back+stu_leg+stu_foot)!=0)	
	{
		SeatStatus=Seat_Status_OneKeyHalf;
	}
}
