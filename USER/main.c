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
u8 Sim_Send_Msg_Flag;
/* 注意：使用软件生成Unicode代码时注意去掉空格*/
const u8* sim900a_msg = "60A8597D002C8BBE59076E295EA65DF28D856E29FF0C5F53524D6E295EA6FF1A";  
const u8* msg_phone_change = "8BBE59077BA1740653F7780166F46362FF0C8FD9662F4E0067616D4B8BD54FE1606FFF015F53524D6E295EA6FF1A"; //更换号码测试信息

const u8* msg_device_change = "8BBE59077F1653F753D866F4FF0C5F53524D";	//设备编号变更，当前
const u8* msg_device = "8BBE59077F1653F7003A";	//设备编号
const u8* msg_temp_low = "4F4E6E2962A58B66FF0C5F53524D6E295EA6003A";	//低温报警，当前温度:
const u8* msg_temp_high = "9AD86E2962A58B66FF0C5F53524D6E295EA6003A";	//高温报警，当前温度:
/* sim900a_msg:将所要发的中文短信内容转换成Unicode代码后替代以上变量双引号内容即可*/
const u8* sim900a_myphone_num="00310038003500310036003700370030003700330032";
//const u8* sim900a_myphone_num="00310038003600320031003100330037003300360039";
/* sim900a_myphone_num:短信发送给对方的手机号码，需要转换成Unicode代码后替代以上变量双引号内容即可*/
float tempperature;
 /* tempperature:短信发送给对方的手机号码，需要转换成Unicode代码后替代以上变量双引号内容即可*/
u8 send_phone_gbk[11] = {'1','8','5','1','6','7','7','0','7','3','2',};  //GBK格式的短信发送号码
 

int main(void)
 {  
	delay_init();	    	 //延时函数初始化
  mem_init();				//初始化内存池
	LED_Init();	 
  AT24CXX_Init(); 
	USART1_Init(115200);
  USART2_Config();
  USART3_Config();
	Gsm_Check_Flag = 0;
	if(sim900a_start_test() >0){
		Gsm_Check_Flag = 1;
	}
	TIM2_Config();
  TIM3_Config();
//  	Sim_Send_Flag = 1;
	tempperature = 96.8;
	while(1){	 
		sim900a_disproce();
		sim900a_update_state();
    Communication_Process();
	}
}


