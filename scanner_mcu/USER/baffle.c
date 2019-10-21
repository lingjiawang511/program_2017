#include"HeadType.h"	

#define SCANNER_ERR_TIME  140 //Һ�������ɨ�費����ά���ʱ��
#define BAG_ERR_TIME  140		//ɨ�費����ά�뵽��Һ�����ߵ�ʱ��
#define PROCESS_TIME  200
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
	//ɨ��ǹ�ź�����IO����
	RCC_APB2PeriphClockCmd(SCANNER_RCC,ENABLE);		
	GPIO_InitStructure.GPIO_Pin = SCANNER_IO;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(SCANNER_PORT, &GPIO_InitStructure);

	//�����ŷ����IO����
	RCC_APB2PeriphClockCmd(BAFFLE_RCC,ENABLE);		
	GPIO_InitStructure.GPIO_Pin = BAFFLE_IO;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(BAFFLE_PORT, &GPIO_InitStructure);

}
//����û��ʱ���Ǵ��ڴ���ĵط���ɨ�贫�������ź����뿪ʼ������ʱ�䣬һ��ʱ�������ź�ʱɨ��ǹɨ����ά���ʱ�䣬��ʱ����ɨ������ά��
//����λ���ϴ�һ������Ķ�ά�룬����λ������һ�������־����һ��ʱ���ǹ�����ʱ�䣬���Ǵ��������źŵ�Һ�����е��յ�Ƥ�����е�ʱ�䣬
//���ʱ��һ���������λ��û�и�����ȷ�źţ��Ҿ͵�������
void Baffle_Control_Process(void)
{
	if(Control.scanner.state == 1){
			Baffle_Control.Scanner_Err_Time = SCANNER_ERR_TIME;		//ɨ��ǹ��������⵽��Һ���źţ�һ��ʱ����ɨ��ǹû��ɨ����ά�룬����Ϊ�Ǵ����Һ��
			Baffle_Control.bag_input_flag= 1;
		  Baffle_Control.process_flag = 1;
			Baffle_Control.process_time = 0;
		  Control.scanner.state = 0;
	}
	if(Baffle_Control.bag_input_flag ==1){   //��״̬������Ҫ��ɨ�������յ���ά���ʱ��,�ڴ�ɨ������ά����ϴ�һ��������Ϣ����λ��
			if(Baffle_Control.Scanner_Err_Time ==0){//Ȼ����λ���Ѿ�����һ�������źű�־�������������Ҳ֪������
				Baffle_Control.bag_err_flag = 1;
//				Baffle_Control.bag_Err_Time = BAG_ERR_TIME;
				Update_Err_Scanner_Data();
				BAFFLE_INTER ;																	//�����Ȳ������ĵط��ȴ�
				Baffle_Control.bag_input_flag = 0;
			}
	}
//	if(Baffle_Control.bag_err_flag == 1){
//			if(Baffle_Control.bag_Err_Time == 0){  //Һ�����е�����
//					if(Control.baffle_inter.state == 1){
//             BAFFLE_ON;
//						 Control.baffle_inter.state = 0;
//						 Baffle_Control.bag_err_flag = 0;
//					}
//			}
//	}
	if(Baffle_Control.process_time >= PROCESS_TIME){  //ÿ�����̵�ʱ���ǹ̶���
		 if(Baffle_Control.bag_ok_flag == 1){  //ɨ��ǹɨ����ά�룬��λ��������ȷ�ϱ�־
				BAFFLE_OUTER;
				Control.baffle_inter.state = 0;
			  Control.baffle_outer.state = 0;
			  Baffle_Control.bag_ok_flag = 0;
		}else{
				BAFFLE_INTER ;	
			  Control.baffle_outer.state = 0;
			  Baffle_Control.bag_ok_flag = 1;
		}
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
			Baffle_Control.process_time++;
		}
		
		if(Control.baffle_inter.state == 1){   //����λ���ж�״̬�������൱������
				inter_delay_time++;
			 if(inter_delay_time >= 20){
					inter_delay_time = 0;
				  Control.baffle_inter.state = 0;
				}
		}
		if(Control.baffle_outer.state == 1){ //�˴��������û�У���������ʱ״̬Ϊ����
				outer_delay_time++;
			 if(outer_delay_time >= 300){
					outer_delay_time = 0;
				  BAFFLE_INTER ;
				  Control.baffle_outer.state = 0;
				}
		}
}












