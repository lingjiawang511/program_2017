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
static u32 lock_state=0;
//����ʱ����target�궨��
int main(void)
{
		delay_init();
  	LED_GPIO_Config();
    LOCK_GPIO_Config();
		LOCK_STATE_GPIO_Config();
		USART1_Config();
    USART2_Config();
    TIM2_Config();
    TIM3_Config();	
		delay_ms(100);
    while(1){
      Communication_Process();
      Lock_control();
			Lock_Detection_Control();
// 			lock_state = Read165_Allstate();
// 			delay_ms(10);
		}       
}




