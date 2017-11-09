#include "led.h"
#include"HeadType.h"
#include "sim900a.h"

u16 Led_Flash_Time;
//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PA�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;				 //LED0-->PA.1 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA.1
 GPIO_SetBits(GPIOA,GPIO_Pin_0);						 //PA.0 �����
 GPIO_SetBits(GPIOA,GPIO_Pin_1);						 //PA.1 �����

}
 
void LED_Display(void)
{
			Led_Flash_Time++;
		if(Gsm_Check_Flag ==0){
		if(Sim_Ready){
			if(Led_Flash_Time%100 ==0){
				LED1 = !LED1; //������ʱ����˸����
        LED0 = 1;
			}
		}else{
				if(Led_Flash_Time%30 ==0){
					LED1 = !LED1; //�쳣��ʱ����˸�Ƶ�
          LED0 = 0;
				}
			}
		}else{//��ʼ����ͨ��
      LED1 = 1;
			if(Led_Flash_Time<=60){
				 LED0=0;
			}else if((60<Led_Flash_Time)&&(Led_Flash_Time < 400)){
				 LED0=1;
			}else if(Led_Flash_Time > 400){
				Led_Flash_Time = 0;
				LED0=0;
      }
		}
}








