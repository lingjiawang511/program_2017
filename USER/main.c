#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "malloc.h"   
#include "sim900a.h"		
#include "usart2.h"	
#include "led.h"
#include"HeadType.h"

u8 Gsm_Check_Flag;
u16 Update_Gsm_Time = UPDATE_GSM_TIME;
u8 Sim_Send_Flag;
/* 注意：使用软件生成Unicode代码时注意去掉空格*/
const u8* sim900a_msg="60A8597D002C8BBE59076E295EA65DF28D856E29FF0C5F53524D6E295EA6FF1A";  
/* sim900a_msg:将所要发的中文短信内容转换成Unicode代码后替代以上变量双引号内容即可*/
const u8* sim900a_myphone_num="00310038003500310036003700370030003700330032";
//const u8* sim900a_myphone_num="00310038003600320031003100330037003300360039";
/* sim900a_myphone_num:短信发送给对方的手机号码，需要转换成Unicode代码后替代以上变量双引号内容即可*/
float tempperature;
 /* tempperature:短信发送给对方的手机号码，需要转换成Unicode代码后替代以上变量双引号内容即可*/
u8 send_phone_gbk[11] = {'1','8','5','1','6','7','7','0','7','3','2',};  //GBK格式的短信发送号码
 int main(void)
 {  
   static u8 buf0[10] = {'1','A','B',0x34,0x35,0x36,0x37,0x38,0x39,0x30};
   static u8 buf1[256] = {0};
	delay_init();	    	 //延时函数初始化
  mem_init();				//初始化内存池
	LED_Init();	 
  AT24CXX_Init(); 
  sim900a_unigbk_exchange(buf0,buf1,1);
	USART1_Init(115200);
  USART2_Config();
  USART3_Config();
	Gsm_Check_Flag = 0;
	if(sim900a_start_test() >0){
		Gsm_Check_Flag = 1;
	}
	TIM2_Config();
  TIM3_Config();
	Sim_Send_Flag = 1;
	tempperature = 96.8;
	while(1){	 
		sim900a_disproce();
		sim900a_update_state();
    Communication_Process();
	}
}


