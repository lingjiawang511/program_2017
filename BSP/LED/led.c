#include "led.h"
#include"HeadType.h"
#include "sim900a.h"

u16 Led_Flash_Time;
//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PA,PD�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				 //LED0-->PA.8 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA.8
 GPIO_SetBits(GPIOC,GPIO_Pin_13);						 //PA.8 �����

// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	    		 //LED1-->PD.2 �˿�����, �������
// GPIO_Init(GPIOD, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
// GPIO_SetBits(GPIOD,GPIO_Pin_2); 						 //PD.2 ����� 
}
 
void LED_Display(void)
{
			Led_Flash_Time++;
		if(Gsm_Check_Flag ==0){
		if(Sim_Ready){
			if(Led_Flash_Time%100 ==0){
				LED0=!LED0;
			}
		}else{
				if(Led_Flash_Time%30 ==0){
					LED0=!LED0;
				}
			}
		}else{//��ʼ����ͨ��
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








