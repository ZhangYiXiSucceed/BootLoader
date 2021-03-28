#include "user_controllogic.h"
#include "user_datastructure.h"
#include "stdlib.h"


extern uint16_t Seat_Position_Now[4];
extern Axis_Cbrt_Data_T Axis_Cbrt_Data[4];
extern uint16_t Axis_LinkPoint_Position[5];

Seat_Logic_Data_T Seat_Logic_Data;
extern Seat_Status_T SeatStatus;//座椅状态，指示是否在一键功能中
/*
*********************************************************************************************************
*	函 数 名: MotorSpeedLimit
*	功能说明: 临近减速，根据当前位置与目标位置给出电机速度
*	形    参: MotorNum
						CurrentPosition
            DestPosition
*	返 回 值: motorspeed 0 ±10-100
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
*	函 数 名: SeatLogicDataInit
*	功能说明: 从标定数据结构中得到逻辑运算的数值,根据电位器反接标志决定是否*-1,便于后续大小判断
            被标定过的座椅需要在RTX初始化时调用，未标定过的不需要
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void SeatLogicDataInit(void)
{
	uint8_t index;
	
	/*4轴的打开、收起、半开值*/
	for(index=0;index<4;index++)
	{
			Seat_Logic_Data.Axis_Logic_Data[index].Close_Position=Axis_Cbrt_Data[index].Axis_Close_Position*(Axis_Cbrt_Data[index].Axis_Poten_Reverse);
			Seat_Logic_Data.Axis_Logic_Data[index].Open_Position=Axis_Cbrt_Data[index].Axis_Open_Position*(Axis_Cbrt_Data[index].Axis_Poten_Reverse);
		  Seat_Logic_Data.Axis_Logic_Data[index].Half_Position=Axis_Cbrt_Data[index].Axis_Half_Position*(Axis_Cbrt_Data[index].Axis_Poten_Reverse);
	}	
	
	/*2个背靠的联动点*/
	Seat_Logic_Data.LinkPoint_B1=Axis_LinkPoint_Position[0]*(Axis_Cbrt_Data[0].Axis_Poten_Reverse);
	Seat_Logic_Data.LinkPoint_B2=Axis_LinkPoint_Position[1]*(Axis_Cbrt_Data[0].Axis_Poten_Reverse);

	/*3个腿靠的联动点*/
  Seat_Logic_Data.LinkPoint_L1=Axis_LinkPoint_Position[2]*(Axis_Cbrt_Data[1].Axis_Poten_Reverse);
  Seat_Logic_Data.LinkPoint_L2=Axis_LinkPoint_Position[3]*(Axis_Cbrt_Data[1].Axis_Poten_Reverse);
	Seat_Logic_Data.LinkPoint_L3=Axis_LinkPoint_Position[4]*(Axis_Cbrt_Data[1].Axis_Poten_Reverse);
	
	SeatStatus=Seat_Status_Idle;/*座椅初始状态为IDLE*/
}


/*
*********************************************************************************************************
*	函 数 名: ControlLogic_NonCbrtedSeat
*	功能说明: 未标定前的座椅逻辑处理
*	形    参: KeyValue 按键值，只对6个点动按键的按下，弹起有响应
*	返 回 值: 无
*********************************************************************************************************
*/
void ControlLogic_NonCbrtedSeat(uint8_t KeyValue)
{		
	switch(KeyValue)
	{ 
		case 0:Motor_Action_Normal(Motor_Channel_1 , Motor_Direction_None , 0);
			     break;/*KeyValue=0无按键按下事件，正常不会运行到*/
		
		case 1:Motor_Action_Normal(Motor_Channel_1 , Motor_Direction_Forward*Axis_Cbrt_Data[0].Axis_Motor_Reverse , DEFAULT_MOTOR_SPEED);
		       break;/*KeyValue=1,靠背打开按键按下，以默认速度正向运行*/
		case 2:Motor_Action_Normal(Motor_Channel_1 , Motor_Direction_None , 0);
		       break;/*KeyValue=2,靠背打开按键松开，驱动速度0，自行缓慢停止*/
		
		case 4:Motor_Action_Normal(Motor_Channel_1 , Motor_Direction_Reverse*Axis_Cbrt_Data[0].Axis_Motor_Reverse , DEFAULT_MOTOR_SPEED);
		       break;/*KeyValue=1,靠背收起按键按下，以默认速度反向运行*/
		case 5:Motor_Action_Normal(Motor_Channel_1 , Motor_Direction_None , 0);
		       break;/*KeyValue=1,靠背收起按键松开，停车*/
		
		case 7:Motor_Action_Normal(Motor_Channel_2 , Motor_Direction_Forward*Axis_Cbrt_Data[1].Axis_Motor_Reverse , DEFAULT_MOTOR_SPEED);
		       break;/*KeyValue=7,腿靠打开按键按下*/
		case 8:Motor_Action_Normal(Motor_Channel_2 , Motor_Direction_None , 0);
		       break;/*KeyValue=8,腿靠打开按键松开*/
		
		case 10:Motor_Action_Normal(Motor_Channel_2 , Motor_Direction_Reverse*Axis_Cbrt_Data[1].Axis_Motor_Reverse , DEFAULT_MOTOR_SPEED);
		       break;/*KeyValue=10,腿靠收起按键按下*/
		case 11:Motor_Action_Normal(Motor_Channel_2 , Motor_Direction_None , 0);
		       break;/*KeyValue=11,腿靠收起按键松开*/
	
		case 13:Motor_Action_Normal(Motor_Channel_3 , Motor_Direction_Forward*Axis_Cbrt_Data[2].Axis_Motor_Reverse , DEFAULT_MOTOR_SPEED);
		       break;/*KeyValue=13,脚踏打开按键按下*/
		case 14:Motor_Action_Normal(Motor_Channel_3 , Motor_Direction_None , 0);
		       break;/*KeyValue=14,脚踏打开按键松开*/
	
		case 16:Motor_Action_Normal(Motor_Channel_3 , Motor_Direction_Reverse*Axis_Cbrt_Data[2].Axis_Motor_Reverse , DEFAULT_MOTOR_SPEED);
		       break;/*KeyValue=16,脚踏收起按键按下*/
		case 17:Motor_Action_Normal(Motor_Channel_3 , Motor_Direction_None , 0);
		       break;/*KeyValue=17,脚踏收起按键松开*/
		
		default:/*19-27为一键功能，未标定前无效*/
			      break;		
	}
}

/*
*********************************************************************************************************
*	函 数 名: ControlLogic_CbrtedSeat
*	功能说明: 已标定过的座椅逻辑处理
*	形    参: KeyValue 按键值，对9个按键响应
*	返 回 值: 无
*********************************************************************************************************
*/
void ControlLogic_CbrtedSeat(uint8_t KeyValue)
{
	switch(KeyValue)
	{
		case 0:/*KeyValue=0无按键按下事件，正常不会运行到*/
			     break;/*不做任何处理*/
	
		       /*靠背打开按键按下及弹起*/
		case 1:CbrtedSeatLogic_BackOpen_PressDown();
			     break;
		case 2:CbrtedSeatLogic_Key_Popup();
			     break;
		       
		       /*靠背收起按键按下及弹起*/
	  case 4:CbrtedSeatLogic_BackClose_PressDown();
		       break;			      
		case 5:CbrtedSeatLogic_Key_Popup();
			     break;
		
		       /*腿靠打开按键按下及弹起*/
		case 7:CbrtedSeatLogic_LegOpen_PressDown();
			     break;
		case 8:CbrtedSeatLogic_Key_Popup();
			     break;
		
		       /*腿靠收起按键按下及弹起*/
	  case 10:CbrtedSeatLogic_LegClose_PressDown();
		        break;			      
		case 11:CbrtedSeatLogic_Key_Popup();
			      break;
					 
					 /*脚踏打开按键按下及弹起*/
		case 13:CbrtedSeatLogic_FootOpen_PressDown();
			      break;
		case 14:CbrtedSeatLogic_Key_Popup();
			      break;
					 
					 /*脚踏收起按键按下及弹起*/
	  case 16:CbrtedSeatLogic_LegClose_PressDown();
		        break;			      
		case 17:CbrtedSeatLogic_Key_Popup();
			      break;
					 
					 /*一键全躺按键按下及弹起*/
	  case 19:CbrtedSeatLogic_OneKeyOpen_PressDown();
		        break;			      
		case 20:CbrtedSeatLogic_Key_Popup();
			      break;
					 
					 /*一键半躺按键按下及弹起*/
	  case 22:CbrtedSeatLogic_OneKeyHalf_PressDown();
		        break;			      
		case 23:CbrtedSeatLogic_Key_Popup();
			      break;
					 
					/*一键全收按键按下及弹起*/
	  case 25:CbrtedSeatLogic_OneKeyClose_PressDown();
		        break;			      
		case 26:CbrtedSeatLogic_Key_Popup();
			      break;
		
		default:break;
	}	
}

/*
*********************************************************************************************************
*	函 数 名: CbrtedSeatLogic_Key_Popup
*	功能说明: 已标定座椅的点动按键松开后的动作逻辑，3轴均停止
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void CbrtedSeatLogic_Key_Popup(void)
{
	 /*三轴均停止运转*/
	Motor_Action_Normal(Motor_Channel_1 , Motor_Direction_None , 0);
	Motor_Action_Normal(Motor_Channel_2 , Motor_Direction_None , 0);
	Motor_Action_Normal(Motor_Channel_3 , Motor_Direction_None , 0);		 
	/*座椅状态切换为IDLE*/
	SeatStatus=Seat_Status_Idle;
}

/*
*********************************************************************************************************
*	函 数 名: CbrtedSeatLogic_BackOpen_PressDown
*	功能说明: 已标定座椅的背靠打开按键按下逻辑处理
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void CbrtedSeatLogic_BackOpen_PressDown(void)
{
  int16_t BackPositionNow,LegPositionNow,LegLinkPositionDest;
	float radio;
	uint8_t limitSpeed;
	
	/*如果背靠、腿靠的电位器是反装的，则比较时当前位置也转为负数*/
	BackPositionNow=Seat_Position_Now[0]*(Axis_Cbrt_Data[0].Axis_Poten_Reverse);
	LegPositionNow=Seat_Position_Now[1]*(Axis_Cbrt_Data[1].Axis_Poten_Reverse);
	
	/*B1 B2因标定或输入错误数值一样直接返回，否则后续可能发生除零错误*/
	if(Seat_Logic_Data.LinkPoint_B2==Seat_Logic_Data.LinkPoint_B1){return;}

	/*前一时刻的座椅状态为一键功能*/
	if(SeatStatus>=Seat_Status_OneKeyOpen)
	{//一键功能被点动按键终止
		Motor_Action_Normal(Motor_Channel_1 , Motor_Direction_None , 0);
		Motor_Action_Normal(Motor_Channel_2 , Motor_Direction_None , 0);
		Motor_Action_Normal(Motor_Channel_3 , Motor_Direction_None , 0);		
	  /*座椅状态切换为IDLE*/
		SeatStatus=Seat_Status_Idle;
		return;
	}
	
	/*Bnow>=Bopen,靠背抵达或越过打开极限位,实施制动*/
	if(BackPositionNow>=Seat_Logic_Data.Axis_Logic_Data[0].Open_Position)
	{
		Motor_Action_Normal(Motor_Channel_1 , Motor_Direction_None , 0);
		SeatStatus=Seat_Status_Idle;
		return;
	}
	
	if(BackPositionNow<Seat_Logic_Data.LinkPoint_B1)
	{ /*Bclos--Bnow---B1,靠背在收起位-B1联动点之间*/
		Motor_Action_Normal(Motor_Channel_1 , (Motor_Direction_Forward*Axis_Cbrt_Data[0].Axis_Motor_Reverse) , Axis_Cbrt_Data[0].Axis_Motor_Speed);
		Motor_Action_Normal(Motor_Channel_2 , Motor_Direction_None , 0);//腿靠不允许动作
		SeatStatus=Seat_Status_Back_Open;
	}
	else
	{ /*B1--Bnow---B2,靠背在两个联动点之间*/
		if((BackPositionNow>=Seat_Logic_Data.LinkPoint_B1)&&(BackPositionNow<Seat_Logic_Data.LinkPoint_B2))
		{ // Ldest=(L1-Lclose)*(Bnow-B1)/(B2-B1)+Lclose 
			radio=(abs(BackPositionNow-Seat_Logic_Data.LinkPoint_B1))*1.0/abs(Seat_Logic_Data.LinkPoint_B2-Seat_Logic_Data.LinkPoint_B1);
			radio=radio*(abs(Seat_Logic_Data.LinkPoint_L1-Seat_Logic_Data.Axis_Logic_Data[1].Close_Position));
			LegLinkPositionDest=(int16_t)radio+Seat_Logic_Data.Axis_Logic_Data[1].Close_Position;
			//腿靠需要联动
			if(LegPositionNow<LegLinkPositionDest)
			{ //腿靠以限制速度联动
				limitSpeed=MotorSpeedLimit(Motor_Channel_2 ,LegPositionNow,LegLinkPositionDest);
				Motor_Action_Normal(Motor_Channel_2 , (Motor_Direction_Forward*Axis_Cbrt_Data[1].Axis_Motor_Reverse) ,limitSpeed);	
			}
			else
			{ /*若不慎超过联动目标点，腿靠停车*/
				Motor_Action_Normal(Motor_Channel_2 , Motor_Direction_None , 0);
			}
			//背靠打开
			Motor_Action_Normal(Motor_Channel_1 , (Motor_Direction_Forward*Axis_Cbrt_Data[0].Axis_Motor_Reverse) , Axis_Cbrt_Data[0].Axis_Motor_Speed);
		  SeatStatus=Seat_Status_Back_Open;		
		}
		else
		{
			/*B2--Bnow---Bopen,靠背在B2联动点-打开位之间,速度根据与全开位的间距确定*/
			limitSpeed=MotorSpeedLimit(Motor_Channel_1 ,BackPositionNow,Seat_Logic_Data.Axis_Logic_Data[0].Open_Position);
			Motor_Action_Normal(Motor_Channel_1 , (Motor_Direction_Forward*Axis_Cbrt_Data[0].Axis_Motor_Reverse) , limitSpeed);
		  SeatStatus=Seat_Status_Back_Open;
		}
	}
}

/*
*********************************************************************************************************
*	函 数 名: CbrtedSeatLogic_BackClose_PressDown
*	功能说明: 已标定座椅的背靠收起按键按下逻辑处理
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void CbrtedSeatLogic_BackClose_PressDown(void)
{
  int16_t BackPositionNow;
	uint8_t limitSpeed;
	
	/*如果背靠电位器是反装的，则比较时当前位置也转为负数*/
	BackPositionNow=Seat_Position_Now[0]*(Axis_Cbrt_Data[0].Axis_Poten_Reverse);
	
	/*前一时刻的座椅状态为一键功能*/
	if(SeatStatus>=Seat_Status_OneKeyOpen)
	{//一键功能被点动按键终止
		Motor_Action_Normal(Motor_Channel_1 , Motor_Direction_None , 0);
		Motor_Action_Normal(Motor_Channel_2 , Motor_Direction_None , 0);
		Motor_Action_Normal(Motor_Channel_3 , Motor_Direction_None , 0);		
	  /*座椅状态切换为IDLE*/
		SeatStatus=Seat_Status_Idle;
		return;
	}
	
	/*已经到达全收位置*/
	if(BackPositionNow<=Seat_Logic_Data.Axis_Logic_Data[0].Close_Position)
	{ /*背靠停止，座椅状态切换为IDLE*/
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
*	函 数 名: CbrtedSeatLogic_LegOpen_PressDown
*	功能说明: 已标定座椅的腿靠打开按键按下逻辑处理
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void CbrtedSeatLogic_LegOpen_PressDown(void)
{
	int16_t LegPositionNow;
	uint8_t limitSpeed;
	
	LegPositionNow=Seat_Position_Now[1]*(Axis_Cbrt_Data[1].Axis_Poten_Reverse);
	
	if(SeatStatus>=Seat_Status_OneKeyOpen)
	{//一键功能被点动按键终止
		Motor_Action_Normal(Motor_Channel_1 , Motor_Direction_None , 0);
		Motor_Action_Normal(Motor_Channel_2 , Motor_Direction_None , 0);
		Motor_Action_Normal(Motor_Channel_3 , Motor_Direction_None , 0);		
	  /*座椅状态切换为IDLE*/
		SeatStatus=Seat_Status_Idle;
		return;
	}
	
	/*腿靠位置已经达到或越过全开位*/
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
*	函 数 名: CbrtedSeatLogic_LegClose_PressDown
*	功能说明: 已标定座椅的腿靠收起按键按下逻辑处理
*	形    参: 无
*	返 回 值: 无
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
	
	/*防止除0*/
	if(Seat_Logic_Data.LinkPoint_L1==Seat_Logic_Data.Axis_Logic_Data[1].Close_Position){return;}
  if(Seat_Logic_Data.LinkPoint_L3==Seat_Logic_Data.LinkPoint_L2){return;}
	
	/*一键功能被点动按键终止*/
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
		/*背靠当前实际位置大于根据当前腿靠位置计算得到的联动背靠位置，背靠需要联动收回*/
		if(BackPositionNow>BackLinkPositionDest)
		{
			limitSpeed=MotorSpeedLimit(Motor_Channel_1 ,BackPositionNow,BackLinkPositionDest);
			Motor_Action_Normal(Motor_Channel_1 , (Motor_Direction_Reverse*Axis_Cbrt_Data[0].Axis_Motor_Reverse) , limitSpeed);
		}
		else
		{
			Motor_Action_Normal(Motor_Channel_1 , Motor_Direction_None , 0);
		}
		/*腿靠收起，临近全收位时以限制速度运行*/
		limitSpeed=MotorSpeedLimit(Motor_Channel_2 ,LegPositionNow,Seat_Logic_Data.Axis_Logic_Data[1].Close_Position);
		Motor_Action_Normal(Motor_Channel_2 , (Motor_Direction_Reverse*Axis_Cbrt_Data[1].Axis_Motor_Reverse) , limitSpeed);
		SeatStatus=Seat_Status_Leg_Close;
		return;
	}
  
	/*L1--Lnow--L2*/
	if((LegPositionNow>Seat_Logic_Data.LinkPoint_L1)&&(LegPositionNow<=Seat_Logic_Data.LinkPoint_L2))
	{ /*腿靠电机正常收起，无联动*/
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
		/*脚托当前实际位置大于根据当前腿靠位置计算得到的联动脚托位置，脚托需要联动收回*/
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
	{ /*腿靠电机正常收起，无联动*/
		Motor_Action_Normal(Motor_Channel_2 , (Motor_Direction_Reverse*Axis_Cbrt_Data[1].Axis_Motor_Reverse) , Axis_Cbrt_Data[1].Axis_Motor_Speed);
		SeatStatus=Seat_Status_Leg_Close;
		return;
	}	
}

/*
*********************************************************************************************************
*	函 数 名: CbrtedSeatLogic_FootOpen_PressDown
*	功能说明: 已标定座椅的脚踏打开按键按下逻辑处理
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void CbrtedSeatLogic_FootOpen_PressDown(void)
{
	int16_t FootPositionNow;
	uint8_t limitSpeed;
	
	/*如果脚踏电位器是反装的，则比较时当前位置也转为负数*/
	FootPositionNow=Seat_Position_Now[2]*(Axis_Cbrt_Data[2].Axis_Poten_Reverse);
	
	/*前一时刻的座椅状态为一键功能*/
	if(SeatStatus>=Seat_Status_OneKeyOpen)
	{//一键功能被点动按键终止
		Motor_Action_Normal(Motor_Channel_1 , Motor_Direction_None , 0);
		Motor_Action_Normal(Motor_Channel_2 , Motor_Direction_None , 0);
		Motor_Action_Normal(Motor_Channel_3 , Motor_Direction_None , 0);		
	  /*座椅状态切换为IDLE*/
		SeatStatus=Seat_Status_Idle;
		return;
	}
	/*脚踏位置已经到达全开位*/
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
*	函 数 名: CbrtedSeatLogic_FootClose_PressDown
*	功能说明: 已标定座椅的脚踏收起按键按下逻辑处理
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void CbrtedSeatLogic_FootClose_PressDown(void)
{
	int16_t FootPositionNow;
	uint8_t limitSpeed;
	
	/*如果脚踏电位器是反装的，则比较时当前位置也转为负数*/
	FootPositionNow=Seat_Position_Now[2]*(Axis_Cbrt_Data[2].Axis_Poten_Reverse);
	
	/*前一时刻的座椅状态为一键功能*/
	if(SeatStatus>=Seat_Status_OneKeyOpen)
	{//一键功能被点动按键终止
		Motor_Action_Normal(Motor_Channel_1 , Motor_Direction_None , 0);
		Motor_Action_Normal(Motor_Channel_2 , Motor_Direction_None , 0);
		Motor_Action_Normal(Motor_Channel_3 , Motor_Direction_None , 0);		
	  /*座椅状态切换为IDLE*/
		SeatStatus=Seat_Status_Idle;
		return;
	}
	/*脚踏位置已经到达全收位*/
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
*	函 数 名: CbrtedSeatLogic_OneKeyOpen_PressDown
*	功能说明: 已标定座椅的一键全躺按键按下逻辑处理
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void CbrtedSeatLogic_OneKeyOpen_PressDown(void)
{
	int16_t BackPositionNow,LegPositionNow,FootPositionNow;
	uint8_t limitSpeed,stu_leg,stu_back,stu_foot;
	
	/*一键半躺、一键全收功能执行期间，本按键按下，则中断一键功能，全部停止*/
	/*若此前处于一键全开，又按下（或因为按键检测产生的键值连续）本按键，则不影响*/
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
	
	/*背靠运动及状态设定*/
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
	
	/*腿靠运动及状态设定*/
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
	
		/*脚踏运动及状态设定*/
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
	
  /*3轴中任意一轴在运动即认为处于一键打开功能中*/
  if((stu_back+stu_leg+stu_foot)!=0)	
	{
		SeatStatus=Seat_Status_OneKeyOpen;
	}
}

/*
*********************************************************************************************************
*	函 数 名: CbrtedSeatLogic_OneKeyClose_PressDown
*	功能说明: 已标定座椅的一键全收按键按下逻辑处理
*	形    参: 无
*	返 回 值: 无
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
	
	/*背靠运动及状态设定*/
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
	
	/*腿靠运动及状态设定*/
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
	
		/*脚踏运动及状态设定*/
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
	
  /*3轴中任意一轴在运动即认为处于一键打开功能中*/
  if((stu_back+stu_leg+stu_foot)!=0)	
	{
		SeatStatus=Seat_Status_OneKeyClose;
	}
}

/*
*********************************************************************************************************
*	函 数 名: CbrtedSeatLogic_OneKeyHalf_PressDown
*	功能说明: 已标定座椅的一键半躺按键按下逻辑处理
*	形    参: 无
*	返 回 值: 无
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
	
	/*背靠归中处理*/
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
	
	/*腿靠归中处理*/
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
	
	/*脚踏归中处理*/
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
	
	  /*3轴中任意一轴在运动即认为处于一键打开功能中*/
  if((stu_back+stu_leg+stu_foot)!=0)	
	{
		SeatStatus=Seat_Status_OneKeyHalf;
	}
}
