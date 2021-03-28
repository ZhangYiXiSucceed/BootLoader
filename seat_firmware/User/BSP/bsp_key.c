#include "bsp_key.h"



static KEY_T      Control_Key[KEY_COUNT];     /* 9�������Ľṹ������ */


#define Key_RowCode_HZ 0x11 //�������߶�����Ϊ����״̬
#define Key_RowCode_01 0x01 //Row1 ����״̬��Row2����͵�ƽ��������
#define Key_RowCode_10 0x10 //Row1����͵�ƽ����������Row2����״̬

#define KEY_PRESSDOWN  1
#define KEY_POPUP      0
#define KEY_IDLE       2
os_mbx_declare(mbx_KeyValue,10);//���ڸ��߼����������͵���Ϣ����

_declare_box(KeyValuePool,1,10);//���ڴ洢��Ϣ��������ݵ��ڴ��


/*
*********************************************************************************************************
*	�� �� ��: Key_SoftDelay
*	����˵��: ������λ�����߼�����ʱ,�����ò��ÿ����Խ��
*	��    ��: ��
*	�� �� ֵ: ��
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
*	�� �� ��: Key_SetRowValue
*	����˵��: ���ߵ�ɨ��ǰ����
*	��    ��: �������ߵ�״̬��
*	�� �� ֵ: ��
*********************************************************************************************************
*/

void Key_SetRowValue(uint8_t RowCode)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
	switch(RowCode)
	{ /*0x11,���߾�������������ģʽ*/
	  case Key_RowCode_HZ: GPIO_InitStructure.GPIO_Pin=  KEY_ROW1_PIN |KEY_ROW2_PIN;       
                         GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	    
	                       GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		
	                       GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	                       GPIO_Init(KEY_GPIO_PORT, &GPIO_InitStructure);
			                  
	                       break;
	
	/*0x01,����1������������������2��������͵�ƽ*/
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
	
	/*0x10,����2��������״̬����Ϊ�Ǹ��裬����1���ģʽ������͵�ƽ*/
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
/*9�������İ�����⺯��*/
//---------------------------------------������-----------------------------------------
static uint8_t IsKeyDown0(void) 
{ 
	uint8_t KEY_Status=0;
	
	/*ROW2����COL1*/
	/*��ȡ����״̬���͵�ƽΪ����*/
	KEY_Status=(KEY_GPIO_PORT ->IDR & KEY_COL1_PIN)?KEY_POPUP:KEY_PRESSDOWN;	
	
	/*����ֵ�ǰ����Ƿ��£��Ƕ˿��߼��ķ�*/	
	return KEY_Status;
}

//---------------------------------------��������-----------------------------------------
static uint8_t IsKeyDown1(void) 
{ 
	uint8_t KEY_Status=0;
		
	/*ROW1����COL1*/
	KEY_Status=(KEY_GPIO_PORT ->IDR & KEY_COL1_PIN)?KEY_POPUP:KEY_PRESSDOWN;
	
	return KEY_Status;
}
//---------------------------------------���д�-----------------------------------------
static uint8_t IsKeyDown2(void) 
{ 
	uint8_t KEY_Status=0;
		
	/*ROW2����COL3*/
	KEY_Status=(KEY_GPIO_PORT ->IDR & KEY_COL3_PIN)?KEY_POPUP:KEY_PRESSDOWN;	
	 
	return KEY_Status;
}
//---------------------------------------��������-----------------------------------------
static uint8_t IsKeyDown3(void) 
{ 
	uint8_t KEY_Status=0;

  /*ROW1����COL3*/
	KEY_Status=(KEY_GPIO_PORT ->IDR & KEY_COL3_PIN)?KEY_POPUP:KEY_PRESSDOWN;	

	return KEY_Status;
}
//---------------------------------------���д�-----------------------------------------
static uint8_t IsKeyDown4(void) 
{ 
	uint8_t KEY_Status=0;

  /*ROW2����COL2*/
	KEY_Status=(KEY_GPIO_PORT ->IDR & KEY_COL2_PIN)?KEY_POPUP:KEY_PRESSDOWN;	
	
	return KEY_Status;
}
//---------------------------------------��������-----------------------------------------
static uint8_t IsKeyDown5(void) 
{ 
	uint8_t KEY_Status=0;
	
  /*ROW1����COL2*/
	KEY_Status=(KEY_GPIO_PORT ->IDR & KEY_COL2_PIN)?KEY_POPUP:KEY_PRESSDOWN;	

	return KEY_Status;
}
//---------------------------------------һ��ȫ��-----------------------------------------
static uint8_t IsKeyDown6(void) 
{ 
	uint8_t KEY_Status=0;
	
  /*ROW1����COL5*/
	KEY_Status=(KEY_GPIO_PORT ->IDR & KEY_COL5_PIN)?KEY_POPUP:KEY_PRESSDOWN;	
	
	return KEY_Status;
}
//---------------------------------------һ������-----------------------------------------
static uint8_t IsKeyDown7(void) 
{ 
	uint8_t KEY_Status=0;
	
  /*ROW1����COL4*/
	KEY_Status=(KEY_GPIO_PORT ->IDR & KEY_COL4_PIN)?KEY_POPUP:KEY_PRESSDOWN;	
	
	return KEY_Status;
}
//---------------------------------------һ��ȫ��-----------------------------------------
static uint8_t IsKeyDown8(void) 
{ 
	uint8_t KEY_Status=0;
	
  /*ROW2����COL5*/
	KEY_Status=(KEY_GPIO_PORT ->IDR & KEY_COL5_PIN)?KEY_POPUP:KEY_PRESSDOWN;	
	
	return KEY_Status;
}
/*
*********************************************************************************************************
*	�� �� ��: Key_GPIO_Init
*	����˵��: ������̵�GPIO��ʼ����������������Ϊ����������������Ϊ��������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Key_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ��GPIOE�Ķ˿�ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	/* �������е�KEY��GPIOΪ��������ģʽ */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		/* ��Ϊ����� */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* ��Ϊ����ģʽ */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	/* �������������� */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* 2MHz���� */

	GPIO_InitStructure.GPIO_Pin = KEY_COL1_PIN | 
	                              KEY_COL2_PIN |
	                              KEY_COL3_PIN |
	                              KEY_COL4_PIN |
	                              KEY_COL5_PIN |
	                              KEY_ROW1_PIN |
	                              KEY_ROW2_PIN;
	                                       
	//����Ҳ������Ϊ����״̬������Ҫ����ɨ��ʱ���л�ģʽ	
	GPIO_Init(KEY_GPIO_PORT, &GPIO_InitStructure);
	
}

/*
*********************************************************************************************************
*	�� �� ��: Key_Struct_Init
*	����˵��: ������̵Ľṹ���ʼ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Key_Struct_Init()
{
	uint8_t i;
	
	/*9�������ṹ���Ա����ֵ*/
	for(i=0;i<KEY_COUNT;i++)
	{
		Control_Key[i].LongTime=KEY_LONGPRESS_TIME;//0��ʾ����ⳤ��
		Control_Key[i].Count=KEY_FILTER_TIME;
		Control_Key[i].State=KEY_POPUP;//ȱʡ״̬Ϊ����
		Control_Key[i].RepeatCount=0;
		Control_Key[i].RepeatPeriod=1;	//��������10ms��Ҳ��������ÿ10ms��һ�ΰ��¼�ֵ
	}
	/*Ϊ9������ָ�����µĺ���*/
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
*	�� �� ��: Key_Init
*	����˵��: ������̳�ʼ���������ṩ����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Key_Init()
{
	Key_GPIO_Init();
	Key_Struct_Init();
}

/*
*********************************************************************************************************
*	�� �� ��: RTX_Key_Init
*	����˵��: ������̳�ʼ���������ṩ����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RTX_Key_Init(void)
{
	/*������Ϣ���ݵ��ڴ��*/
	_init_box(KeyValuePool,sizeof(KeyValuePool),1);
	
	/*������Ϣ�����ʼ��*/
	os_mbx_init(&mbx_KeyValue,sizeof(mbx_KeyValue));
}

/*
*********************************************************************************************************
*	�� �� ��: Key_PutIntoFIFO
*	����˵��: ��1����ֵ�ֱ�ѹ�밴����Ϣ�������¼��Ϣ����Ϣ���䣬ǰ�������߼������������ڼ�¼��
*	��    ��: ����ֵ
*	�� �� ֵ: 
*********************************************************************************************************
*/
uint8_t Key_PutIntoFIFO(uint8_t KeyValue)
{
   uint8_t* msg_KeyValue;
	 
	 /*���ڴ������һ���ڴ�*/
	 msg_KeyValue=_calloc_box(KeyValuePool);
	
	 /*��ַ����ʧ��*/
	 if(msg_KeyValue==NULL)
   {
		 return 0;
	 }
	 
	 *msg_KeyValue=KeyValue;
	
	 //��ʱ�ȴ�������Ҫ��Ҫȥ��
	 if(os_mbx_send(&mbx_KeyValue,msg_KeyValue,1)!=OS_R_OK)
	 {
		 _free_box(KeyValuePool,msg_KeyValue);//����ʧ���ͷ��ڴ�
		 return 0;
	 }
	 
	 return 1;
}

/*
*********************************************************************************************************
*	�� �� ��: Key_GetFromFIFO
*	����˵��: ����Ϣ�����ȡһ����ֵ�����߼�����������
*	��    ��:  ��
*	�� �� ֵ: ��ȡ��ȷʱ���ذ������루�Ǹ����������ȴ���ʱ����-1
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
*	�� �� ��: Key_DetectOneStatus
*	����˵��: ���һ��������������״̬�����뱻�����Եĵ��á�
            
            Count = 1/2 KEY_FILTER_TIME               Count��ʼֵ
            0<Count<KEY_FILTER_TIME                   �����˲���������0ʱ��Ϊȷʵ���ɿ���
            Count = KEY_FILTER_TIME                   �ж��а������µĳ�ʼ����ֵ
            KEY_FILTER_TIME <Count<2*KEY_FILTER_TIME  �����˲���������2��ʱ��Ϊȷʵ�а���������

*	��    ��:  ָ�������ڰ����ṹ�������ڵ��±�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void Key_DetectOneStatus(uint8_t i)
{
	KEY_T *pBtn;

	pBtn = &Control_Key[i];
	
	/*�������������*/
	if (pBtn->IsKeyDownFunc())
	{
		/*Ϊ�����ϵ�ʱ���⵽����Чֵ����ʼֵ��1/2�˲�ʱ��*/
		if (pBtn->Count < KEY_FILTER_TIME)
		{
			pBtn->Count = KEY_FILTER_TIME;
		}
		else if(pBtn->Count < 2 * KEY_FILTER_TIME)
		{
			pBtn->Count++;
		}
		else /*����ʱ�䳬���˲�ʱ�䣬��Ϊ��һ�����������¼�*/
		{ 
			if (pBtn->State == KEY_POPUP)
			{ /*��Ϊ��һ���µİ����¼�*/
				pBtn->State = KEY_PRESSDOWN;		
			}
			
			/* ��ֵ�������Ӧ�����Ĺ�ϵΪ ��ֵ����=�����±�*3+���尴��״ֵ̬ */
			Key_PutIntoFIFO((uint8_t)(3 * i + 1));
		}
	}//end of if pBtn->IsKeyDownFunc()
	else //�����¼����
	{ /*���pBtn->Count > KEY_FILTER_TIME������������⵽���¹�*/
		if(pBtn->Count > KEY_FILTER_TIME)
		{ /*����ֵ���»ص�KEY_FILTER_TIME�����������˲�*/
			pBtn->Count = KEY_FILTER_TIME;
		}
		else if(pBtn->Count != 0)
		{ /*pBtn->Count��0-KEY_FILTER_TIMEʱִ�еݼ�*/
			pBtn->Count--;
		}
		else
		{ /* pBtn->Count==0ʱ�����֮ǰ�ǰ��£��������ǵ���*/
			if (pBtn->State == KEY_PRESSDOWN)
			{
				pBtn->State = KEY_POPUP;

				/* ���Ͱ�ť�������Ϣ */
				Key_PutIntoFIFO((uint8_t)(3 * i + 2));
			}
		}
	}
}

/*
*********************************************************************************************************
*	�� �� ��: Key_ScanAll
*	����˵��: ɨ�����а���������������RTX�������,�Ӷ�������ִ��
*	��    ��:  ��
*	�� �� ֵ: ��
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
