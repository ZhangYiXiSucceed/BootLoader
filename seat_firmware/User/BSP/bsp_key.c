#include "bsp_key.h"



static KEY_T      Control_Key[KEY_COUNT];     /* 9个按键的结构体数组 */


#define Key_RowCode_HZ 0x11 //两根行线都设置为输入状态
#define Key_RowCode_01 0x01 //Row1 输入状态，Row2输出低电平（下拉）
#define Key_RowCode_10 0x10 //Row1输出低电平（下拉），Row2输入状态

#define KEY_PRESSDOWN  1
#define KEY_POPUP      0
#define KEY_IDLE       2
os_mbx_declare(mbx_KeyValue,10);//用于给逻辑控制任务发送的消息邮箱

_declare_box(KeyValuePool,1,10);//用于存储消息邮箱的数据的内存池


/*
*********************************************************************************************************
*	函 数 名: Key_SoftDelay
*	功能说明: 行线置位到列线检测的延时,具体用不用看调试结果
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void Key_SoftDelay(void)
{
	uint8_t Key_HoldTime=50;
	
	while(Key_HoldTime>1)
  {
		Key_HoldTime--;
  }
}


/*
*********************************************************************************************************
*	函 数 名: Key_SetRowValue
*	功能说明: 行线的扫描前设置
*	形    参: 两根行线的状态码
*	返 回 值: 无
*********************************************************************************************************
*/

void Key_SetRowValue(uint8_t RowCode)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
	switch(RowCode)
	{ /*0x11,行线均处于输入上拉模式*/
	  case Key_RowCode_HZ: GPIO_InitStructure.GPIO_Pin=  KEY_ROW1_PIN |KEY_ROW2_PIN;       
                         GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	    
	                       GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		
	                       GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	                       GPIO_Init(KEY_GPIO_PORT, &GPIO_InitStructure);
			                  
	                       break;
	
	/*0x01,行线1处于输入上拉，行线2推挽输出低电平*/
		case Key_RowCode_01: GPIO_InitStructure.GPIO_Pin=  KEY_ROW1_PIN ;      	 
                         GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		
	                       GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		
	                       GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	                       GPIO_Init(KEY_GPIO_PORT, &GPIO_InitStructure);
	
	                       GPIO_InitStructure.GPIO_Pin=  KEY_ROW2_PIN ;
                         KEY_GPIO_PORT->BSRRH= KEY_ROW2_PIN ;  	
                         GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	                       GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;   
	                       GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL; 
	                       GPIO_Init(KEY_GPIO_PORT, &GPIO_InitStructure);
												 											
		                     break;
	
	/*0x10,行线2处于输入状态，认为是高阻，行线1输出模式，输出低电平*/
		case Key_RowCode_10: GPIO_InitStructure.GPIO_Pin=  KEY_ROW1_PIN ;
                         KEY_GPIO_PORT->BSRRH= KEY_ROW1_PIN ;  	
                         GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	                       GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;   
	                       GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL; 
	                       GPIO_Init(KEY_GPIO_PORT, &GPIO_InitStructure);
												 
												 GPIO_InitStructure.GPIO_Pin=  KEY_ROW2_PIN ;      	 
                         GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		
	                       GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		
	                       GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	                       GPIO_Init(KEY_GPIO_PORT, &GPIO_InitStructure);
											
												 break;
	
		default:             break;	
			            
	}

}
/*9个按键的按键检测函数*/
//---------------------------------------靠背打开-----------------------------------------
static uint8_t IsKeyDown0(void) 
{ 
	uint8_t KEY_Status=0;
	
	/*ROW2——COL1*/
	/*读取列线状态，低电平为按下*/
	KEY_Status=(KEY_GPIO_PORT ->IDR & KEY_COL1_PIN)?KEY_POPUP:KEY_PRESSDOWN;	
	
	/*返回值是按键是否按下，是端口逻辑的反*/	
	return KEY_Status;
}

//---------------------------------------靠背收起-----------------------------------------
static uint8_t IsKeyDown1(void) 
{ 
	uint8_t KEY_Status=0;
		
	/*ROW1——COL1*/
	KEY_Status=(KEY_GPIO_PORT ->IDR & KEY_COL1_PIN)?KEY_POPUP:KEY_PRESSDOWN;
	
	return KEY_Status;
}
//---------------------------------------腿托打开-----------------------------------------
static uint8_t IsKeyDown2(void) 
{ 
	uint8_t KEY_Status=0;
		
	/*ROW2——COL3*/
	KEY_Status=(KEY_GPIO_PORT ->IDR & KEY_COL3_PIN)?KEY_POPUP:KEY_PRESSDOWN;	
	 
	return KEY_Status;
}
//---------------------------------------腿托收起-----------------------------------------
static uint8_t IsKeyDown3(void) 
{ 
	uint8_t KEY_Status=0;

  /*ROW1——COL3*/
	KEY_Status=(KEY_GPIO_PORT ->IDR & KEY_COL3_PIN)?KEY_POPUP:KEY_PRESSDOWN;	

	return KEY_Status;
}
//---------------------------------------脚托打开-----------------------------------------
static uint8_t IsKeyDown4(void) 
{ 
	uint8_t KEY_Status=0;

  /*ROW2——COL2*/
	KEY_Status=(KEY_GPIO_PORT ->IDR & KEY_COL2_PIN)?KEY_POPUP:KEY_PRESSDOWN;	
	
	return KEY_Status;
}
//---------------------------------------脚托收起-----------------------------------------
static uint8_t IsKeyDown5(void) 
{ 
	uint8_t KEY_Status=0;
	
  /*ROW1——COL2*/
	KEY_Status=(KEY_GPIO_PORT ->IDR & KEY_COL2_PIN)?KEY_POPUP:KEY_PRESSDOWN;	

	return KEY_Status;
}
//---------------------------------------一键全躺-----------------------------------------
static uint8_t IsKeyDown6(void) 
{ 
	uint8_t KEY_Status=0;
	
  /*ROW1——COL5*/
	KEY_Status=(KEY_GPIO_PORT ->IDR & KEY_COL5_PIN)?KEY_POPUP:KEY_PRESSDOWN;	
	
	return KEY_Status;
}
//---------------------------------------一键半躺-----------------------------------------
static uint8_t IsKeyDown7(void) 
{ 
	uint8_t KEY_Status=0;
	
  /*ROW1——COL4*/
	KEY_Status=(KEY_GPIO_PORT ->IDR & KEY_COL4_PIN)?KEY_POPUP:KEY_PRESSDOWN;	
	
	return KEY_Status;
}
//---------------------------------------一键全收-----------------------------------------
static uint8_t IsKeyDown8(void) 
{ 
	uint8_t KEY_Status=0;
	
  /*ROW2——COL5*/
	KEY_Status=(KEY_GPIO_PORT ->IDR & KEY_COL5_PIN)?KEY_POPUP:KEY_PRESSDOWN;	
	
	return KEY_Status;
}
/*
*********************************************************************************************************
*	函 数 名: Key_GPIO_Init
*	功能说明: 矩阵键盘的GPIO初始化，两根行线设置为输出，五根列线设置为悬空输入
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void Key_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 打开GPIOE的端口时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	/* 配置所有的KEY线GPIO为浮动输入模式 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		/* 设为输入口 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* 设为推挽模式 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	/* 无需上下拉电阻 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* 2MHz即可 */

	GPIO_InitStructure.GPIO_Pin = KEY_COL1_PIN | 
	                              KEY_COL2_PIN |
	                              KEY_COL3_PIN |
	                              KEY_COL4_PIN |
	                              KEY_COL5_PIN |
	                              KEY_ROW1_PIN |
	                              KEY_ROW2_PIN;
	                                       
	//行线也先配置为输入状态，等需要按键扫描时再切换模式	
	GPIO_Init(KEY_GPIO_PORT, &GPIO_InitStructure);
	
}

/*
*********************************************************************************************************
*	函 数 名: Key_Struct_Init
*	功能说明: 矩阵键盘的结构体初始化
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void Key_Struct_Init()
{
	uint8_t i;
	
	/*9个按键结构体成员赋初值*/
	for(i=0;i<KEY_COUNT;i++)
	{
		Control_Key[i].LongTime=KEY_LONGPRESS_TIME;//0表示不检测长按
		Control_Key[i].Count=KEY_FILTER_TIME;
		Control_Key[i].State=KEY_POPUP;//缺省状态为弹起
		Control_Key[i].RepeatCount=0;
		Control_Key[i].RepeatPeriod=1;	//连发周期10ms，也即长按后，每10ms发一次按下键值
	}
	/*为9个按键指定按下的函数*/
	Control_Key[0].IsKeyDownFunc=IsKeyDown0;
	Control_Key[1].IsKeyDownFunc=IsKeyDown1;
	Control_Key[2].IsKeyDownFunc=IsKeyDown2;
	Control_Key[3].IsKeyDownFunc=IsKeyDown3;
	Control_Key[4].IsKeyDownFunc=IsKeyDown4;
	Control_Key[5].IsKeyDownFunc=IsKeyDown5;
	Control_Key[6].IsKeyDownFunc=IsKeyDown6;
	Control_Key[7].IsKeyDownFunc=IsKeyDown7;
	Control_Key[8].IsKeyDownFunc=IsKeyDown8;
}

/*
*********************************************************************************************************
*	函 数 名: Key_Init
*	功能说明: 矩阵键盘初始化，对外提供调用
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void Key_Init()
{
	Key_GPIO_Init();
	Key_Struct_Init();
}

/*
*********************************************************************************************************
*	函 数 名: RTX_Key_Init
*	功能说明: 矩阵键盘初始化，对外提供调用
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void RTX_Key_Init(void)
{
	/*按键消息传递的内存池*/
	_init_box(KeyValuePool,sizeof(KeyValuePool),1);
	
	/*按键消息邮箱初始化*/
	os_mbx_init(&mbx_KeyValue,sizeof(mbx_KeyValue));
}

/*
*********************************************************************************************************
*	函 数 名: Key_PutIntoFIFO
*	功能说明: 将1个键值分别压入按键消息邮箱与记录信息的消息邮箱，前者用于逻辑处理，后者用于记录。
*	形    参: 按键值
*	返 回 值: 
*********************************************************************************************************
*/
uint8_t Key_PutIntoFIFO(uint8_t KeyValue)
{
   uint8_t* msg_KeyValue;
	 
	 /*向内存池申请一块内存*/
	 msg_KeyValue=_calloc_box(KeyValuePool);
	
	 /*地址分配失败*/
	 if(msg_KeyValue==NULL)
   {
		 return 0;
	 }
	 
	 *msg_KeyValue=KeyValue;
	
	 //超时等待后续看要不要去掉
	 if(os_mbx_send(&mbx_KeyValue,msg_KeyValue,1)!=OS_R_OK)
	 {
		 _free_box(KeyValuePool,msg_KeyValue);//发送失败释放内存
		 return 0;
	 }
	 
	 return 1;
}

/*
*********************************************************************************************************
*	函 数 名: Key_GetFromFIFO
*	功能说明: 从消息邮箱读取一个键值，供逻辑处理任务处理
*	形    参:  无
*	返 回 值: 获取正确时返回按键代码（非负整数），等待超时返回-1
*********************************************************************************************************
*/
int8_t Key_GetFromFIFO(void)
{
	void* msg_KeyValue;
	uint8_t KeyValue;
	
	OS_RESULT msgResult;
	
  msgResult=os_mbx_wait(&mbx_KeyValue,&msg_KeyValue,0);
	
	if(msgResult!=OS_R_OK)
	{
	  return -1;
	}
	else
	{
		KeyValue = *((uint8_t*)msg_KeyValue);
		_free_box(KeyValuePool,msg_KeyValue);
		return KeyValue;
	}
}

/*
*********************************************************************************************************
*	函 数 名: Key_DetectOneStatus
*	功能说明: 检测一个按键。非阻塞状态，必须被周期性的调用。
            
            Count = 1/2 KEY_FILTER_TIME               Count初始值
            0<Count<KEY_FILTER_TIME                   弹起滤波计数，到0时认为确实是松开了
            Count = KEY_FILTER_TIME                   判断有按键按下的初始计数值
            KEY_FILTER_TIME <Count<2*KEY_FILTER_TIME  按下滤波计数，到2倍时认为确实有按键按下了

*	形    参:  指定按键在按键结构体数组内的下标
*	返 回 值: 无
*********************************************************************************************************
*/
static void Key_DetectOneStatus(uint8_t i)
{
	KEY_T *pBtn;

	pBtn = &Control_Key[i];
	
	/*如果按键被按下*/
	if (pBtn->IsKeyDownFunc())
	{
		/*为避免上电时候检测到的无效值，初始值是1/2滤波时间*/
		if (pBtn->Count < KEY_FILTER_TIME)
		{
			pBtn->Count = KEY_FILTER_TIME;
		}
		else if(pBtn->Count < 2 * KEY_FILTER_TIME)
		{
			pBtn->Count++;
		}
		else /*按下时间超过滤波时间，认为是一个按键按下事件*/
		{ 
			if (pBtn->State == KEY_POPUP)
			{ /*认为是一个新的按下事件*/
				pBtn->State = KEY_PRESSDOWN;		
			}
			
			/* 键值代码与对应按键的关系为 键值代码=按键下标*3+具体按键状态值 */
			Key_PutIntoFIFO((uint8_t)(3 * i + 1));
		}
	}//end of if pBtn->IsKeyDownFunc()
	else //弹起事件检测
	{ /*如果pBtn->Count > KEY_FILTER_TIME，表明曾经检测到按下过*/
		if(pBtn->Count > KEY_FILTER_TIME)
		{ /*计数值重新回到KEY_FILTER_TIME，继续按键滤波*/
			pBtn->Count = KEY_FILTER_TIME;
		}
		else if(pBtn->Count != 0)
		{ /*pBtn->Count在0-KEY_FILTER_TIME时执行递减*/
			pBtn->Count--;
		}
		else
		{ /* pBtn->Count==0时，如果之前是按下，则现在是弹起*/
			if (pBtn->State == KEY_PRESSDOWN)
			{
				pBtn->State = KEY_POPUP;

				/* 发送按钮弹起的消息 */
				Key_PutIntoFIFO((uint8_t)(3 * i + 2));
			}
		}
	}
}

/*
*********************************************************************************************************
*	函 数 名: Key_ScanAll
*	功能说明: 扫描所有按键。非阻塞，被RTX任务调用,从而周期性执行
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void Key_ScanAll(void)
{
	Key_SetRowValue(Key_RowCode_10);
	
  Key_SoftDelay();
	
	Key_DetectOneStatus(1);
		
	Key_DetectOneStatus(5);
		     
	Key_DetectOneStatus(3);
		
	Key_DetectOneStatus(7);
	
  Key_DetectOneStatus(6);
	
	
  Key_SetRowValue(Key_RowCode_01);
	
	Key_SoftDelay();
	
	Key_DetectOneStatus(0);

  Key_DetectOneStatus(4);

  Key_DetectOneStatus(2);
	
  Key_DetectOneStatus(8);
		  
}
