#ifndef __COMMUNICATION_H
#define __COMMUNICATION_H

#include "stm32f10x.h"

//#define				COMMUNICATION_IO1_IO					 GPIO_Pin_11   //�豸״̬���
//#define				COMMUNICATION_IO1_PORT			   GPIOA
//#define				COMMUNICATION_IO1_RCC				   RCC_APB2Periph_GPIOA
//#define				COMMUNICATION_IO2_IO					 GPIO_Pin_12
//#define				COMMUNICATION_IO2_PORT			   GPIOA
//#define				COMMUNICATION_IO2_RCC				   RCC_APB2Periph_GPIOA
//#define				COMMUNICATION_IO3_IO					 GPIO_Pin_8
//#define				COMMUNICATION_IO3_PORT			   GPIOB
//#define				COMMUNICATION_IO3_RCC				   RCC_APB2Periph_GPIOB

//#define       READ_COMMUNICATION_IO1   		  GPIO_ReadInputDataBit(COMMUNICATION_IO1_PORT,COMMUNICATION_IO1_IO) //���ص���һ���ֽڣ�������һ��λ
//#define       COMMUNICATION_IO1_OFF   		  GPIO_SetBits(COMMUNICATION_IO1_PORT, COMMUNICATION_IO1_IO)
//#define       COMMUNICATION_IO1_ON  				GPIO_ResetBits(COMMUNICATION_IO1_PORT, COMMUNICATION_IO1_IO)

//#define       READ_COMMUNICATION_IO2   		  GPIO_ReadInputDataBit(COMMUNICATION_IO2_PORT,COMMUNICATION_IO2_IO) //���ص���һ���ֽڣ�������һ��λ
//#define       COMMUNICATION_IO2_OFF   		  GPIO_SetBits(COMMUNICATION_IO2_PORT, COMMUNICATION_IO2_IO)
//#define       COMMUNICATION_IO2_ON  				GPIO_ResetBits(COMMUNICATION_IO2_PORT, COMMUNICATION_IO2_IO)

//#define       READ_COMMUNICATION_IO3   		  GPIO_ReadInputDataBit(COMMUNICATION_IO3_PORT,COMMUNICATION_IO3_IO) //���ص���һ���ֽڣ�������һ��λ
//#define       COMMUNICATION_IO3_OFF   		  GPIO_SetBits(COMMUNICATION_IO3_PORT, COMMUNICATION_IO3_IO)
//#define       COMMUNICATION_IO3_ON  				GPIO_ResetBits(COMMUNICATION_IO3_PORT, COMMUNICATION_IO3_IO)



void Communication_GPIO_Config(void);
void Respond_Host_Comm(void);
void Dispose_Data_For_Host(void);

#endif 






