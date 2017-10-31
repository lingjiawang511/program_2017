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

//================================================


extern u8 slaveaddr;




void Do_Usrat2_RFIDCmd(u8 cmd );
void Do_Usrat3_RFIDCmd(u8 cmd );
void PC_Communication_Time_ISR(void );
u8 Execute_Host_Comm(void);


void Communication_Process(void );
#endif



