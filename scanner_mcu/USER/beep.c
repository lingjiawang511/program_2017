#include"HeadType.h"

u16 Beep_Time;
u8 Beep_Num;
u8 ERR_Beep_State;
//=============================================================================
//��������:BEEP_GPIO_Config
//���ܸ�Ҫ:��������������
//��������:��
//��������:��
//ע��    :��
//=============================================================================
void BEEP_GPIO_Config(void)
{	
	//����һ��GPIO_InitTypeDef ���͵Ľṹ�壬���ֽ�GPIO_InitStructure 
	GPIO_InitTypeDef  GPIO_InitStructure;
	//ʹ��GPIOC������ʱ��
	RCC_APB2PeriphClockCmd(BEEP_RCC,ENABLE);
	//ѡ��Ҫ�õ�GPIO����		
	GPIO_InitStructure.GPIO_Pin = BEEP_IO;
	///��������ģʽΪ�������ģʽ			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	//���������ٶ�Ϊ50MHZ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//���ÿ⺯������ʼ��GPIO
	GPIO_Init(BEEP_PORT, &GPIO_InitStructure);
	BEEP_ON;
	ERR_Beep_State = 0;
	Beep_Num = 0;
}

//=============================================================================
//��������:Beep_Response
//���ܸ�Ҫ:�������죬ʹ�ö�ʱ����ʵ��
//��������:��
//��������:��
//ע��    :�����ڶ�ʱ������
//=============================================================================

void Beep_Response(void )
{
	static u8 beep_state=1;
	static u8 retry = 50; //led��˸ʱ����
	if(Beep_Num > 0){
		retry--;
		if(retry <=0){
			if(beep_state){
			   BEEP_ON;
			   beep_state = 0;
			}else{
			   BEEP_OFF;
			   Beep_Num--;
			   beep_state = 1;
			}
			retry = 50;
		}
	}
	
}



