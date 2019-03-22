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
extern void Communication_GPIO_Config(void);
static u32 lock_state=0;
void test_lrc(void);
//配置时钟在target宏定义
int main(void)
{
		delay_init();
  	LED_GPIO_Config();
    LOCK_GPIO_Config();
		LOCK_STATE_GPIO_Config();
	  Communication_GPIO_Config();
		USART1_Config();
    USART2_Config();
    TIM2_Config();
    TIM3_Config();
		test_lrc();
		delay_ms(100);
    while(1){
      Communication_Process();
      Lock_control();
			Lock_Detection_Control();
// 			lock_state = Read165_Allstate();
// 			delay_ms(10);
		}       
}

void test_lrc(void)
{
	static u16 testlrc = 0;
	static u8 testbuf[20];
	static u8 testcount = 0;
  u8 i,j;
	testcount = testcount;
	testlrc =testlrc;
	for(j=0;j<4;j++){
		for(i=0;i<10;i++){
			testcount = 0;
			testbuf[testcount++] = 0x40;
			testbuf[testcount++] = 0x4D;
			testbuf[testcount++] = 0x31;
			testbuf[testcount++] = 0x32 +j;
			testbuf[testcount++] = 0x30 + i;
			testlrc=LRC_GetLRC16(&testbuf[1],testcount - 1);
		}
	}

}


