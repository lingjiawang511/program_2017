#include"HeadType.h"	

#if selectboard
#define	DEVICE1_KEY_IO					GPIO_Pin_3
#define	DEVICE1_KEY_PORT				GPIOA
#define	DEVICE1_KEY_RCC				  RCC_APB2Periph_GPIOA
#else
#define	DEVICE1_KEY_IO					GPIO_Pin_13
#define	DEVICE1_KEY_PORT				GPIOC
#define	DEVICE1_KEY_RCC				  RCC_APB2Periph_GPIOC
#endif 
#define READ_DEVICE1_KEY    		GPIO_ReadInputDataBit(DEVICE1_KEY_PORT,DEVICE1_KEY_IO) //���ص���һ���ֽڣ�������һ��λ


#define KEY_SHORT_TIME 		    15
#define KEY_LONG_TIME			    100
#define KEY_LONGLONG_TIME			400
u8 Key_ScanNum;
u8 Key_SetParamFlag;

u16  timeflag;
u8   AdrrOK_Flag;
u16  Menu_Exit_Time;
//=============================================================================
//��������: KEY_GPIO_Config
//���ܸ�Ҫ:LED����������
//��������:��
//��������:��
//ע��    :��
//=============================================================================
void KEY_GPIO_Config(void)
{	
	//����һ��GPIO_InitTypeDef ���͵Ľṹ�壬���ֽ�GPIO_InitStructure 
	GPIO_InitTypeDef  GPIO_InitStructure;
	//ʹ��GPIOC������ʱ��
	RCC_APB2PeriphClockCmd(DEVICE1_KEY_RCC,ENABLE);
	//ѡ��Ҫ�õ�GPIO����		
	GPIO_InitStructure.GPIO_Pin = DEVICE1_KEY_IO;
	///��������ģʽΪ�������ģʽ			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
	//���������ٶ�Ϊ50MHZ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	//���ÿ⺯������ʼ��GPIO
	GPIO_Init(DEVICE1_KEY_PORT, &GPIO_InitStructure);

}

u8 Key_Scan(void)
{
	 u8 key_num;
	 static u8 key1_triggerstate;
	 static u16 key1_timercount;
	 static u8 longlongkey_flag = 0;
	  key_num = 0;
	 //key1
		if(READ_DEVICE1_KEY == READLOW){
			if(longlongkey_flag == 0){
				if(key1_triggerstate == 0){
					key1_triggerstate = 1;
				}else{
					key1_timercount++;
					if(key1_timercount >= KEY_LONGLONG_TIME){
						key_num |=0xFF;
						longlongkey_flag = 1;
				  }
				}
			}
		}else{
			if(longlongkey_flag == 1){
				longlongkey_flag = 0;
				key1_timercount = 0;
				key1_triggerstate =0;
			}else{
				if(key1_triggerstate == 1){
					if(key1_timercount <KEY_SHORT_TIME){
						key_num &=0xfe;
					}else if((key1_timercount >=KEY_SHORT_TIME)&&(key1_timercount <KEY_LONG_TIME)){
							key_num |=0x01;
					}else{
							key_num |=0x11;
					}
					key1_triggerstate =0;				
				}
				key1_timercount = 0;
			}
		}
	
	 return key_num;
}





