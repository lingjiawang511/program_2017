#include "led.h"
#include"HeadType.h"
#include "sim900a.h"

u16 Led_Flash_Time;
//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PA端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;				 //LED0-->PA.1 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.1
 GPIO_SetBits(GPIOA,GPIO_Pin_0);						 //PA.0 输出高
 GPIO_SetBits(GPIOA,GPIO_Pin_1);						 //PA.1 输出高

}
 
void LED_Display(void)
{
			Led_Flash_Time++;
		if(Gsm_Check_Flag ==0){
		if(Sim_Ready){
			if(Led_Flash_Time%100 ==0){
				LED1 = !LED1; //正常的时候闪烁蓝灯
        LED0 = 1;
			}
		}else{
				if(Led_Flash_Time%30 ==0){
					LED1 = !LED1; //异常的时候闪烁黄灯
          LED0 = 0;
				}
			}
		}else{//初始化不通过
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








