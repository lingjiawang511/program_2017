#include"HeadType.h"	

#define SCANNER_ERR_TIME  220 //Һ�������ɨ�費����ά���ʱ��
#define BAG_ERR_TIME  140		//ɨ�費����ά�뵽��Һ�����ߵ�ʱ��
#define PROCESS_TIME  300
Control_Baffle_Type Baffle_Control;

//=============================================================================
//��������: Printer_GPIO_Config
//���ܸ�Ҫ:��ӡ����������
//��������:��
//��������:��
//ע��    :��
//=============================================================================
 void Baffle_GPIO_Config(void)
{	
	//����һ��GPIO_InitTypeDef ���͵Ľṹ�壬���ֽ�GPIO_InitStructure 
	GPIO_InitTypeDef  GPIO_InitStructure;
	//�����ⷭ��������IO����
	RCC_APB2PeriphClockCmd(BAFFLE_OUTER_RCC,ENABLE);		
	GPIO_InitStructure.GPIO_Pin = BAFFLE_OUTER_IO;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(BAFFLE_OUTER_PORT, &GPIO_InitStructure);
	//�����ڷ���������IO����
	RCC_APB2PeriphClockCmd(BAFFLE_INTER_RCC,ENABLE);		
	GPIO_InitStructure.GPIO_Pin = BAFFLE_INTER_IO;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(BAFFLE_INTER_PORT, &GPIO_InitStructure);


	//�����ŷ����IO����
	RCC_APB2PeriphClockCmd(BAFFLE_RCC,ENABLE);		
	GPIO_InitStructure.GPIO_Pin = BAFFLE_IO;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(BAFFLE_PORT, &GPIO_InitStructure);
	
	BAFFLE_INTER ;

}
//����û��ʱ���Ǵ��ڴ���ĵط���ɨ�贫�������ź����뿪ʼ������ʱ�䣬һ��ʱ�������ź�ʱɨ��ǹɨ����ά���ʱ�䣬��ʱ����ɨ������ά��
//����λ���ϴ�һ������Ķ�ά�룬����λ������һ�������־����һ��ʱ���ǹ�����ʱ�䣬���Ǵ��������źŵ�Һ�����е��յ�Ƥ�����е�ʱ�䣬
//���ʱ��һ���������λ��û�и�����ȷ�źţ��Ҿ͵�������
void Baffle_Control_Process(void)
{
	static u8 scannerstate=0;//��ά����������λ����������ģ��
	if(Device_State == 1){
		if(Baffle_Control.bag_input_flag == 0){
			if(Control.scanner.state == 1){
					Baffle_Control.Scanner_Err_Time = Baffle_Control.PC_send_process_time-80;		//ɨ��ǹ��������⵽��Һ���źţ�һ��ʱ����ɨ��ǹû��ɨ����ά�룬����Ϊ�Ǵ����Һ��
					Baffle_Control.process_time = Baffle_Control.PC_send_process_time;
				  Baffle_Control.bag_input_flag= 1;
					Baffle_Control.process_flag = 1;
				  Baffle_Control.scanner_result_old = 0;
			}
		}
		if(Baffle_Control.bag_input_flag ==1){   //��״̬������Ҫ��ɨ�������յ���ά���ʱ��,�ڴ�ɨ������ά����ϴ�һ��������Ϣ����λ��
				if(Baffle_Control.Scanner_Err_Time ==0){//Ȼ����λ���Ѿ�����һ�������źű�־�������������Ҳ֪������
					scannerstate = Baffle_Control.PC_send_scanner_result;
					Baffle_Control.PC_send_scanner_result = 0;
					Baffle_Control.scanner_result_old = scannerstate;
					if(scannerstate == 1){
						Baffle_Control.bag_ok_flag = 1;
	//				Update_Err_Scanner_Data();
					}else{
						Baffle_Control.bag_ok_flag = 0;
					}
					scannerstate = 0;
					Baffle_Control.bag_input_flag = 0;
					Control.scanner.state = 0;
				}
		}

		if(Baffle_Control.process_time == 0){  //ÿ�����̵�ʱ���ǹ̶���
			 if(Baffle_Control.bag_ok_flag == 1){  //ɨ��ǹɨ����ά�룬��λ��������ȷ�ϱ�־
					BAFFLE_OUTER;
				  Baffle_Control.baffle_state = 1;
				  MCU_Host_Send.control.err_message &=0xFD;
	//				Control.baffle_inter.state = 0;
	//			  Control.baffle_outer.state = 0;
//					Baffle_Control.bag_ok_flag = 0;
			}else{
					BAFFLE_INTER;	
				  Baffle_Control.baffle_state = 2;
				  MCU_Host_Send.control.err_message &=0xFE;
	//			  Control.baffle_outer.state = 0;
//					Baffle_Control.bag_ok_flag = 1;
			}
			Baffle_Control.bag_err_flag = 0;
			Baffle_Control.process_time = Baffle_Control.PC_send_process_time;
			Baffle_Control.process_flag= 0;
		}
	}else{
			Baffle_Control.bag_input_flag = 0;
			Control.scanner.state = 0;
		  Baffle_Control.process_time = 0;
			Baffle_Control.bag_err_flag = 0;
			Baffle_Control.process_time = 0;
			Baffle_Control.process_flag= 0;
	}
	
}

void Baffle_Time_Irq(void)
{
	  static u16 inter_delay_time=0;
	  static u16 outer_delay_time=0;
		if(Baffle_Control.Scanner_Err_Time > 0){
			Baffle_Control.Scanner_Err_Time--;
		}
//    if(Baffle_Control.bag_Err_Time > 0){
//			Baffle_Control.bag_Err_Time--;
//		}
   if(Baffle_Control.process_flag == 1){
		 if(Baffle_Control.process_time >0){
				Baffle_Control.process_time--;
		 }
		}
		if( Baffle_Control.baffle_state == 1){  //�ⷭ����λ����
				outer_delay_time++;
			  if(outer_delay_time >= 10){
					 if(READ_BAFFLE_OUTER == 0){
							Baffle_Control.baffle_state = 0;
						  outer_delay_time = 0;
					 }else{
							outer_delay_time = outer_delay_time;
					 }
				 }
					if(outer_delay_time >= 160){
							if(READ_BAFFLE_OUTER == 0){
								Baffle_Control.baffle_state = 0;
								outer_delay_time = 0;
							}else{
// 							  Device_State = 3;
								Baffle_Control.baffle_state = 0;
								outer_delay_time = 0;
// 						    MCU_Host_Send.control.err_message |=0x02;
					 }
					 }
		}
		if( Baffle_Control.baffle_state == 2){  //�ڷ�����λ����
				inter_delay_time++;
			  if(inter_delay_time >= 10){
					 if(READ_BAFFLE_INTER == 0){
							Baffle_Control.baffle_state = 0;
						  inter_delay_time = 0;
					 }else{
							inter_delay_time = inter_delay_time;
					 }
				 }
					 if(inter_delay_time >= 160){
							if(READ_BAFFLE_INTER == 0){
								Baffle_Control.baffle_state = 0;
								inter_delay_time = 0;
							}else{
// 							  Device_State = 3;
								Baffle_Control.baffle_state = 0;
								inter_delay_time = 0;
// 								MCU_Host_Send.control.err_message |=0x01;
					  }
					 }
		}
//		if(Control.baffle_inter.state == 1){   //����λ���ж�״̬�������൱������
//				inter_delay_time++;
//			 if(inter_delay_time >= 20){
//					inter_delay_time = 0;
//				  Control.baffle_inter.state = 0;
//				}
//		}
//		if(Control.baffle_outer.state == 1){ //�˴��������û�У���������ʱ״̬Ϊ����
//				outer_delay_time++;
//			 if(outer_delay_time >= 300){
//					outer_delay_time = 0;
//				  BAFFLE_INTER ;
//				  Control.baffle_outer.state = 0;
//				}
//		}
}












