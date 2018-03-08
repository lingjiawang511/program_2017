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
/* ע�⣺ʹ���������Unicode����ʱע��ȥ���ո�*/
const u8* sim900a_msg = "60A8597D002C8BBE59076E295EA65DF28D856E29FF0C5F53524D6E295EA6FF1A";  
const u8* msg_phone_change = "8BBE59077BA1740653F7780166F46362FF0C8FD9662F4E0067616D4B8BD54FE1606FFF015F53524D6E295EA6FF1A"; //�������������Ϣ

const u8* msg_device_change = "8BBE59077F1653F753D866F4FF0C5F53524D";	//�豸��ű������ǰ
const u8* msg_device = "8BBE59077F1653F7003A";	//�豸���
const u8* msg_temp_low = "4F4E6E2962A58B66FF0C5F53524D6E295EA6003A";	//���±�������ǰ�¶�:
const u8* msg_temp_high = "9AD86E2962A58B66FF0C5F53524D6E295EA6003A";	//���±�������ǰ�¶�:
/* sim900a_msg:����Ҫ�������Ķ�������ת����Unicode�����������ϱ���˫�������ݼ���*/
const u8* sim900a_myphone_num="00310038003500310036003700370030003700330032";
//const u8* sim900a_myphone_num="00310038003600320031003100330037003300360039";
/* sim900a_myphone_num:���ŷ��͸��Է����ֻ����룬��Ҫת����Unicode�����������ϱ���˫�������ݼ���*/
float tempperature;
 /* tempperature:���ŷ��͸��Է����ֻ����룬��Ҫת����Unicode�����������ϱ���˫�������ݼ���*/
u8 send_phone_gbk[11] = {'1','8','5','1','6','7','7','0','7','3','2',};  //GBK��ʽ�Ķ��ŷ��ͺ���
 

int main(void)
 {  
	delay_init();	    	 //��ʱ������ʼ��
  mem_init();				//��ʼ���ڴ��
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


