#include"HeadType.h"	
#include "usart.h"
#include "TIM.h"

void delay_ms1(u16 ms)
{
	u16  i;
	u32  j;
	for(i=ms;i>0;i--)
	{
		for(j=5000;j>0;j--)
		;
	}
}
//����ʱ����target�궨��
int main(void)
{
		delay_init();
		AT24CXX_Init();
		USART1_Config();
		LED_GPIO_Config();
    TIM2_Config();
    TIM3_Config();	
		delay_ms(100);
    while(1){
			
		}       
}




