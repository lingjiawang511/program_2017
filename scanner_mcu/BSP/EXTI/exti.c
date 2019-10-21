#include"HeadType.h"	

 Control_Input_Type Control;

//=============================================================================
//��������:EXTIX0_Init
//���ܸ�Ҫ:�ⲿ�ж�0��ʼ��
//��������:��
//��������:��
//ע��    :��
//=============================================================================
static void  EXTIX0_Init(void )
{
	 	EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		GPIO_InitTypeDef  GPIO_InitStructure;
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��
	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;			 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	
  	EXTI_InitStructure.EXTI_Line=EXTI_Line0;	//KEY2
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	
	  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//ʹ�ܰ���KEY2���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//�����ȼ�2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);
}
////=============================================================================
////��������:EXTIX3_Init
////���ܸ�Ҫ:�ⲿ�ж�3��ʼ��
////��������:��
////��������:��
////ע��    :��
////=============================================================================
//static void  EXTIX3_Init(void )
//{
//	 	EXTI_InitTypeDef EXTI_InitStructure;
//		NVIC_InitTypeDef NVIC_InitStructure;
//		GPIO_InitTypeDef  GPIO_InitStructure;
//	
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��
////	  GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
//		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
//	
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;			 
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//		GPIO_Init(GPIOB, &GPIO_InitStructure);
//	
//	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource3);
//			
//  	EXTI_InitStructure.EXTI_Line=EXTI_Line3;	//KEY2
//  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
//  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
//	
//	  NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			//ʹ�ܰ���KEY2���ڵ��ⲿ�ж�ͨ��
//  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
//  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//�����ȼ�2
//  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
//  	NVIC_Init(&NVIC_InitStructure);
//}
////=============================================================================
////��������:EXTIX4_Init
////���ܸ�Ҫ:�ⲿ�ж�4��ʼ��
////��������:��
////��������:��
////ע��    :��
////=============================================================================
//static void  EXTIX4_Init(void )
//{
//	 	EXTI_InitTypeDef EXTI_InitStructure;
//		NVIC_InitTypeDef NVIC_InitStructure;
//		GPIO_InitTypeDef  GPIO_InitStructure;
//	
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��
////	  GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
//		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
//	
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;			 
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//		GPIO_Init(GPIOB, &GPIO_InitStructure);
//	
//	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource4);
//	
//  	EXTI_InitStructure.EXTI_Line=EXTI_Line4;	//KEY2
//  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
//  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
//	
//	  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//ʹ�ܰ���KEY2���ڵ��ⲿ�ж�ͨ��
//  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
//  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//�����ȼ�2
//  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
//  	NVIC_Init(&NVIC_InitStructure);
//}
////=============================================================================
////��������:EXTIX9_5_Init
////���ܸ�Ҫ:�ⲿ�ж�9_5��ʼ��
////��������:��
////��������:��
////ע��    :��
////=============================================================================
//static void  EXTIX9_5_Init(void )
//{
//	 	EXTI_InitTypeDef EXTI_InitStructure;
//		NVIC_InitTypeDef NVIC_InitStructure;
//		GPIO_InitTypeDef  GPIO_InitStructure;
//	
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
//	
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_8;			 
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//		GPIO_Init(GPIOC, &GPIO_InitStructure);
//	
//		GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource6); 
// 	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource8);
//  	EXTI_InitStructure.EXTI_Line=EXTI_Line6|EXTI_Line8;
//  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
//  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
//	
//	  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//ʹ�ܰ���KEY2���ڵ��ⲿ�ж�ͨ��
//  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
//  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//�����ȼ�2
//  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
//  	NVIC_Init(&NVIC_InitStructure);
//}
//=============================================================================
//��������:EXTIX15_10_Init
//���ܸ�Ҫ:�ⲿ�ж�15_10��ʼ��
//��������:��
//��������:��
//ע��    :��
//=============================================================================
static void  EXTIX15_10_Init(void )
{
	 	EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		GPIO_InitTypeDef  GPIO_InitStructure;
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;			 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource10); 
 	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource11);
	
  	EXTI_InitStructure.EXTI_Line=EXTI_Line10|EXTI_Line11;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	
	  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//ʹ�ܰ���KEY2���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//�����ȼ�2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);
}

//=============================================================================
//��������:EXTIX_Init
//���ܸ�Ҫ:�����ⲿ�жϳ�ʼ��
//��������:��
//��������:��
//ע��    :��
//=============================================================================
void EXTIX_Init(void)
{
		EXTIX0_Init( );
//		EXTIX3_Init( );
//		EXTIX4_Init( );
//		EXTIX9_5_Init( );
		EXTIX15_10_Init( );
  	
}
//=============================================================================
//��������:EXTI0_IRQHandler
//���ܸ�Ҫ:�ⲿ�ж�0������� 
//��������:��
//��������:��
//ע��    :��
//=============================================================================
void EXTI0_IRQHandler(void)
{		
		if(EXTI_GetITStatus(EXTI_Line0) != RESET){
			if(Control.scanner.irqstate == 0){//��ʱ����ʹ�ö�ʱ����ʱ���жϽ�����״̬��8MS���ж�״̬�Ƿ�����					
					Control.scanner.irqstate = 1;
					Control.scanner.irqtime = 0;					
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line0); //���LINE0�ϵ��жϱ�־λ  
		
}

//=============================================================================
//��������:EXTI3_IRQHandler
//���ܸ�Ҫ:�ⲿ�ж�3������� 
//��������:��
//��������:��
//ע��    :��
//=============================================================================
void EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3) != RESET){

		}
		EXTI_ClearITPendingBit(EXTI_Line3); //���LINE0�ϵ��жϱ�־λ  
	 
}
//=============================================================================
//��������:EXTI4_IRQHandler
//���ܸ�Ҫ:�ⲿ�ж�4������� 
//��������:��
//��������:��
//ע��    :��
//=============================================================================
void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4) != RESET){
		EXTI_ClearITPendingBit(EXTI_Line4); //���LINE0�ϵ��жϱ�־λ  
	}  
}
//=============================================================================
//��������:EXTI9_5_IRQHandler
//���ܸ�Ҫ:�ⲿ�ж�9-5������� 
//��������:��
//��������:��
//ע��    :��
//=============================================================================
void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line6) != RESET){
			
			EXTI_ClearITPendingBit(EXTI_Line6);
	}else if(EXTI_GetITStatus(EXTI_Line8) != RESET){

		EXTI_ClearITPendingBit(EXTI_Line8);
	}else{
		EXTI_ClearITPendingBit(EXTI_Line5);
		EXTI_ClearITPendingBit(EXTI_Line7);
		EXTI_ClearITPendingBit(EXTI_Line9);
	}

}
//=============================================================================
//��������:EXTI15_10_IRQHandler
//���ܸ�Ҫ:�ⲿ�ж�15-10������� 
//��������:��
//��������:��
//ע��    :��
//=============================================================================
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line10) != RESET){
	if(Control.scanner.irqstate == 0){//��ʱ����ʹ�ö�ʱ����ʱ���жϽ�����״̬��8MS���ж�״̬�Ƿ�����					
					Control.baffle_outer.irqstate = 1;
					Control.baffle_outer.irqtime = 0;					
			}
		EXTI_ClearITPendingBit(EXTI_Line10);
	}else if(EXTI_GetITStatus(EXTI_Line11) != RESET){
	if(Control.scanner.irqstate == 0){//��ʱ����ʹ�ö�ʱ����ʱ���жϽ�����״̬��8MS���ж�״̬�Ƿ�����					
					Control.baffle_inter.irqstate = 1;
					Control.baffle_inter.irqtime = 0;					
			}
			  EXTI_ClearITPendingBit(EXTI_Line11);
	}else if(EXTI_GetITStatus(EXTI_Line14) != RESET){

		EXTI_ClearITPendingBit(EXTI_Line14);
	}else{
		EXTI_ClearITPendingBit(EXTI_Line12);
		EXTI_ClearITPendingBit(EXTI_Line13);
		EXTI_ClearITPendingBit(EXTI_Line15);
	}
}

//=============================================================================
//��������:Control_Input_IRQTimer
//���ܸ�Ҫ:��ӡ������״̬�������������ʱ���
//��������:��
//��������:��
//ע��    :��
//=============================================================================
void 	Control_Input_IRQTimer(void)
{
		if(Control.scanner.irqstate == 1){//��ʱ����ʹ�ö�ʱ����ʱ���жϽ�����״̬��8MS���ж�״̬�Ƿ�����
					Control.scanner.irqtime++;
					if(IRQ_TIMEOUT <= Control.scanner.irqtime){
						if(READ_SCANNER == RESET){
							Control.scanner.state = 1;
						}
						Control.scanner.irqstate = 0;
						Control.scanner.irqtime = 0;
					}
		}
		if(Control.baffle_inter.irqstate == 1){//��ʱ����ʹ�ö�ʱ����ʱ���жϽ�����״̬��8MS���ж�״̬�Ƿ�����
					Control.baffle_inter.irqtime++;
					if(IRQ_TIMEOUT <= Control.baffle_inter.irqtime){
						if(READ_BAFFLE_INTER == RESET){

							Control.baffle_inter.state = 1;
//							if(Air_Control.delay_time == 0){
//								Air_Control.delay_time = 200;   //�Ժ�Ҫ�ĳɿɱ��ʱ��  
//							}
						}
						Control.baffle_inter.irqstate = 0;
						Control.baffle_inter.irqtime = 0;
					}
		}
		if(Control.baffle_outer.irqstate == 1){//��ʱ����ʹ�ö�ʱ����ʱ���жϽ�����״̬��8MS���ж�״̬�Ƿ�����
					Control.baffle_outer.irqtime++;
					if(IRQ_TIMEOUT <= Control.baffle_outer.irqtime){
						if(READ_BAFFLE_OUTER  == RESET){
							Control.baffle_outer.state = 1;
						}
						Control.baffle_outer.irqstate = 0;
						Control.baffle_outer.irqtime = 0;
					}
		}

		
}





