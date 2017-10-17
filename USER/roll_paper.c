#include"HeadType.h"	


#define	ROLL_PAPER_IO			  	GPIO_Pin_7
#define	ROLL_PAPER_PORT			  GPIOA
#define	ROLL_PAPER_RCC				RCC_APB2Periph_GPIOA

#define ROLL_PAPER_ON  		 		GPIO_SetBits(ROLL_PAPER_PORT, ROLL_PAPER_IO)
#define ROLL_PAPER_OFF  			GPIO_ResetBits(ROLL_PAPER_PORT, ROLL_PAPER_IO)



//=============================================================================
//��������: ROLL_PAPER_GPIO_Config
//���ܸ�Ҫ:��ֽ���������������
//��������:��
//��������:��
//ע��    :��
//=============================================================================
void ROLL_PAPER_GPIO_Config(void)
{	
	//����һ��GPIO_InitTypeDef ���͵Ľṹ�壬���ֽ�GPIO_InitStructure 
	GPIO_InitTypeDef  GPIO_InitStructure;
	//ֹͣ����IO����
	RCC_APB2PeriphClockCmd(ROLL_PAPER_RCC,ENABLE);		
	GPIO_InitStructure.GPIO_Pin = ROLL_PAPER_IO;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(ROLL_PAPER_PORT, &GPIO_InitStructure);

}

void Roll_Paper_Control(void)
{
	 if(Device_State == 1){//Device_State=0������״̬��Device_State=1������״̬��Device_State=2��ֹͣ״̬
			ROLL_PAPER_ON;
		 	belt.state = READY;			//����Ƥ������
//		  COMMUNICATION_IO1_ON;
	}else if(Device_State == 2){
			ROLL_PAPER_OFF;
		  AIR_CYLINDER_UP;
		  AIR_BLOW_OFF;
		  VACUUM_OFF;
			belt.state = END ;     //����Ƥ��ֹͣ
			param_init();
	}else if(Device_State == 3){
			Printer.complete = 0;
			Printer.process = PRINTER_RESERVE;
			PRINTER_START_OFF;
		  Air_Control.process = RESERVE;
			belt.state = END ;     //����Ƥ��ֹͣ
		  AIR_BLOW_OFF;
		  VACUUM_OFF;
//		  COMMUNICATION_IO1_OFF;
		}else{
      ROLL_PAPER_ON;
			belt.state = END ;     //����Ƥ��ֹͣ
			param_init();
			if(READ_UPPER_REACH==0){
				AIR_CYLINDER_UP;
		  }
	}

}









