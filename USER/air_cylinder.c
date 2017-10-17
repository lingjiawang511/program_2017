#include"HeadType.h"	

Air_Controlr_Type Air_Control;
//=============================================================================
//��������: Printer_GPIO_Config
//���ܸ�Ҫ:��ӡ����������
//��������:��
//��������:��
//ע��    :��
//=============================================================================
 void Air_Cylinder_GPIO_Config(void)
{	
	//����һ��GPIO_InitTypeDef ���͵Ľṹ�壬���ֽ�GPIO_InitStructure 
	GPIO_InitTypeDef  GPIO_InitStructure;
	//�����ϲ���������IO����
	RCC_APB2PeriphClockCmd(UPPER_REACH_RCC,ENABLE);		
	GPIO_InitStructure.GPIO_Pin = UPPER_REACH_IO;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(UPPER_REACH_PORT, &GPIO_InitStructure);
	//�������ϵ�λ����IO����
	RCC_APB2PeriphClockCmd(FIT_REACH_RCC,ENABLE);		
	GPIO_InitStructure.GPIO_Pin = FIT_REACH_IO;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(FIT_REACH_PORT, &GPIO_InitStructure);
	//Һ���ź�����IO����
	RCC_APB2PeriphClockCmd(FLUID_BAG_RCC,ENABLE);		
	GPIO_InitStructure.GPIO_Pin = FLUID_BAG_IO;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(FLUID_BAG_PORT, &GPIO_InitStructure);
	//ɨ��ǹ�ź�����IO����
	RCC_APB2PeriphClockCmd(SCANNER_RCC,ENABLE);		
	GPIO_InitStructure.GPIO_Pin = SCANNER_IO;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(SCANNER_PORT, &GPIO_InitStructure);

	//������ѹ���IO����
	RCC_APB2PeriphClockCmd(AIR_CYLINDER_RCC,ENABLE);		
	GPIO_InitStructure.GPIO_Pin = AIR_CYLINDER_IO;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(AIR_CYLINDER_PORT, &GPIO_InitStructure);
	//���״������IO����
	RCC_APB2PeriphClockCmd(AIR_BLOW_RCC,ENABLE);		
	GPIO_InitStructure.GPIO_Pin = AIR_BLOW_IO;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(AIR_BLOW_PORT, &GPIO_InitStructure);
	//�����ֽ���IO����
	RCC_APB2PeriphClockCmd(VACUUM_RCC,ENABLE);		
	GPIO_InitStructure.GPIO_Pin = VACUUM_IO;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(VACUUM_PORT, &GPIO_InitStructure);
	
}

void Air_Cylinder_Control(void)
{

	switch(Air_Control.process){
		case RESERVE:		if(Device_State == 1){  //�豸����
												Air_Control.process = READY;		
										}
										break ;
		case READY:     if((Printer.complete == 1)&&(Air_Control.air_cylinder_position ==IN_UP)&&\
											(Air_Control.delay_time == 0)){  //��ʼ��ӡ��ʱ��Ͱ�Һ�������ź���λ�����Խ�����һ���ź�����
																Air_Control.process = WORKING;
																AIR_CYLINDER_DOWM;
																Printer.complete = 0;
																Air_Control.air_cylinder_dowm_timeout = 200;
																 MCU_Host_Send.control.err_message &=0xFB;
														}		                  
					break ;
		case WORKING:   if(Control.fit_reach.state == 1){  //���ϵ�λ
												Control.upper_reach.state = 0; //�����λ����ֹ���������¼�����״̬���湦��ͬʱ����
												AIR_CYLINDER_UP;
												Air_Control.process = WORKEND;
												Control.fit_reach.state = 0;
												Air_Control.air_cylinder_position =IN_DOWN;
												Control.fluid_bag.state = 0;
												Air_Control.air_cylinder_up_timeout = 400;
												MCU_Host_Send.control.err_message &=0xEF;
										}else{
											if(Air_Control.air_cylinder_dowm_timeout == 0){ //ѹ��ȥ����λ���豸������Ҫͣ��
													Device_State = 3;
												 MCU_Host_Send.control.err_message |=0x04;
											}
										}
					break ;
		case WORKEND:   if(Control.upper_reach.state == 1){  //�ϲ���λ
												Control.fit_reach.state = 0; //�����λ����ֹ���������¼�����״̬���湦��ͬʱ����
												Air_Control.process = END;
												Air_Control.complete = 1;
												Control.upper_reach.state = 0;
												Air_Control.air_cylinder_position =IN_UP;
												VACUUM_OFF;
												AIR_BLOW_OFF;
//												Control.fluid_bag.state = 0;												
										}else{
											if(Air_Control.air_cylinder_up_timeout == 0){ //ѹ��ȥ����λ���豸������Ҫͣ��
														Device_State = 3;
													  MCU_Host_Send.control.err_message |=0x10;
												}
										}
					break;
		case END: if(Device_State == 1){  //�豸���� 
									Air_Control.process = READY; 
//									COMMUNICATION_IO2_OFF;
							}else{
									Air_Control.process = RESERVE; 
							}
					break ;
		default :break ;
	}
}




