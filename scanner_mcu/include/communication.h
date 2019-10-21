#ifndef __COMMUNICATION_H
#define __COMMUNICATION_H

#include "stm32f10x.h"

/*************************************************
			             Ԥ����
*************************************************/

#define COMM_SET_MCU_IDLE       0x01 //����ģ�����˯��ģʽ
#define EER_SET_MCU_IDLE        0xE0 

#define COMM_CONTROL_ANTENNA    0x02 //ģ�����߿��ơ��Զ�Ѱ������
#define EER_CONTROL_ANTENNA     0xE1 

#define COMM_MIFARE_SEARCH_CARD	0x03 //TYPE-AѰ��
#define EER_MIFARE_SEARCH_CARD	0xE2 

#define COMM_READ_BLOCK			0x04 //����
#define EER_READ_BLOCK			0xE3 

#define COMM_WRITE_BLOCK		0x05 //д��
#define EER_WRITE_BLOCK		    0xE4 

#define COMM_INIT_PURSE			0x06 //��ʼ��Ǯ��
#define EER_INIT_PURSE		    0xE5 

#define COMM_READ_PURSE		    0x07 //��ȡǮ��
#define EER_READ_PURSE		    0xE6 

#define COMM_INC_VALUE			0x08 //��ֵ
#define EER_INC_VALUE			0xE7
	
#define COMM_DEC_VALUE			0x09 //�ۿ�
#define EER_DEC_VALUE			0xE8 
	
#define COMM_BAK_PURSE			0x0A //����Ǯ��ֵ
#define EER_BAK_PURSE			0xE9 

#define COMM_CARD_HALT			0x0B //������
#define EER_CARD_HALT			0xEA //������

#define COMM_SELF_SEARCH_CARD	0x0C //������ID��
#define EER_SELF_SEARCH_CARD	0xEB 

#define				COMMUNICATION_IO1_IO					 GPIO_Pin_11   //�豸״̬���
#define				COMMUNICATION_IO1_PORT			   GPIOA
#define				COMMUNICATION_IO1_RCC				   RCC_APB2Periph_GPIOA
#define				COMMUNICATION_IO2_IO					 GPIO_Pin_12
#define				COMMUNICATION_IO2_PORT			   GPIOA
#define				COMMUNICATION_IO2_RCC				   RCC_APB2Periph_GPIOA
#define				COMMUNICATION_IO3_IO					 GPIO_Pin_8
#define				COMMUNICATION_IO3_PORT			   GPIOB
#define				COMMUNICATION_IO3_RCC				   RCC_APB2Periph_GPIOB

#define       READ_COMMUNICATION_IO1   		  GPIO_ReadInputDataBit(COMMUNICATION_IO1_PORT,COMMUNICATION_IO1_IO) //���ص���һ���ֽڣ�������һ��λ
#define       COMMUNICATION_IO1_OFF   		  GPIO_SetBits(COMMUNICATION_IO1_PORT, COMMUNICATION_IO1_IO)
#define       COMMUNICATION_IO1_ON  				GPIO_ResetBits(COMMUNICATION_IO1_PORT, COMMUNICATION_IO1_IO)

#define       READ_COMMUNICATION_IO2   		  GPIO_ReadInputDataBit(COMMUNICATION_IO2_PORT,COMMUNICATION_IO2_IO) //���ص���һ���ֽڣ�������һ��λ
#define       COMMUNICATION_IO2_OFF   		  GPIO_SetBits(COMMUNICATION_IO2_PORT, COMMUNICATION_IO2_IO)
#define       COMMUNICATION_IO2_ON  				GPIO_ResetBits(COMMUNICATION_IO2_PORT, COMMUNICATION_IO2_IO)

#define       READ_COMMUNICATION_IO3   		  GPIO_ReadInputDataBit(COMMUNICATION_IO3_PORT,COMMUNICATION_IO3_IO) //���ص���һ���ֽڣ�������һ��λ
#define       COMMUNICATION_IO3_OFF   		  GPIO_SetBits(COMMUNICATION_IO3_PORT, COMMUNICATION_IO3_IO)
#define       COMMUNICATION_IO3_ON  				GPIO_ResetBits(COMMUNICATION_IO3_PORT, COMMUNICATION_IO3_IO)


//================================================






void Communication_GPIO_Config(void);
void Do_Usrat2_RFIDCmd(u8 cmd );
void Do_Usrat3_RFIDCmd(u8 cmd );
void PC_Communication_Time_ISR(void );
//u8 Execute_Host_Comm(void);
void Comm_Upload_state(void);
void Respond_Host_Comm(void);
void Dispose_Data_For_Host(void);
void  Update_Err_Scanner_Data(void);
void Comm_With_Device_State(void);
#endif



