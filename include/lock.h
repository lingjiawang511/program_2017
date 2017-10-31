#ifndef __LOCK_H
#define __LOCK_H

#include "stm32f10x.h"

#define		LOCK1_IO					GPIO_Pin_12
#define		LOCK1_PORT				GPIOB
#define   LOCK1_RCC					RCC_APB2Periph_GPIOB
#define		LOCK2_IO					GPIO_Pin_13
#define		LOCK2_PORT				GPIOB
#define   LOCK2_RCC					RCC_APB2Periph_GPIOB

#define   LOCK1_OFF   			GPIO_ResetBits(LOCK1_PORT, LOCK1_IO)
#define   LOCK1_ON  				GPIO_SetBits(LOCK1_PORT, LOCK1_IO)
#define   LOCK2_OFF   			GPIO_ResetBits(LOCK2_PORT, LOCK2_IO)
#define   LOCK2_ON 	 				GPIO_SetBits(LOCK2_PORT, LOCK2_IO)

#define		LOCK1_CHECK_IO					GPIO_Pin_8
#define		LOCK1_CHECK_PORT				GPIOB
#define   LOCK1_CHECK_RCC					RCC_APB2Periph_GPIOB
#define		LOCK2_CHECK_IO					GPIO_Pin_9
#define		LOCK2_CHECK_PORT				GPIOB
#define   LOCK2_CHECK_RCC					RCC_APB2Periph_GPIOB

#define   READ_LOCK1_CHECK   			GPIO_ReadInputDataBit(LOCK1_CHECK_PORT,LOCK1_CHECK_IO)
#define   READ_LOCK2_CHECK  			GPIO_ReadInputDataBit(LOCK2_CHECK_PORT,LOCK2_CHECK_IO)

#define		LOCK1_LIGHT1_IO					GPIO_Pin_5
#define		LOCK1_LIGHT1_PORT				GPIOA
#define   LOCK1_LIGHT1_RCC				RCC_APB2Periph_GPIOA
#define		LOCK1_LIGHT2_IO					GPIO_Pin_7
#define		LOCK1_LIGHT2_PORT				GPIOA
#define   LOCK1_LIGHT2_RCC				RCC_APB2Periph_GPIOA

#define		LOCK2_LIGHT1_IO					GPIO_Pin_4
#define		LOCK2_LIGHT1_PORT				GPIOA
#define   LOCK2_LIGHT1_RCC				RCC_APB2Periph_GPIOA
#define		LOCK2_LIGHT2_IO					GPIO_Pin_6
#define		LOCK2_LIGHT2_PORT				GPIOA
#define   LOCK2_LIGHT2_RCC				RCC_APB2Periph_GPIOA

#define   LOCK1_LIGHT1_OFF   			GPIO_ResetBits(LOCK1_LIGHT1_PORT, LOCK1_LIGHT1_IO)
#define   LOCK1_LIGHT1_ON  				GPIO_SetBits(LOCK1_LIGHT1_PORT, LOCK1_LIGHT1_IO)
#define   LOCK1_LIGHT2_OFF   			GPIO_ResetBits(LOCK1_LIGHT2_PORT, LOCK1_LIGHT2_IO)
#define   LOCK1_LIGHT2_ON  				GPIO_SetBits(LOCK1_LIGHT2_PORT, LOCK1_LIGHT2_IO)

#define   LOCK2_LIGHT1_OFF   			GPIO_ResetBits(LOCK2_LIGHT1_PORT, LOCK2_LIGHT1_IO)
#define   LOCK2_LIGHT1_ON  				GPIO_SetBits(LOCK2_LIGHT1_PORT, LOCK2_LIGHT1_IO)
#define   LOCK2_LIGHT2_OFF   			GPIO_ResetBits(LOCK2_LIGHT2_PORT, LOCK2_LIGHT2_IO)
#define   LOCK2_LIGHT2_ON  				GPIO_SetBits(LOCK2_LIGHT2_PORT, LOCK2_LIGHT2_IO)


#define   LOCK1_LIGHT_OFF()   		{LOCK1_LIGHT1_OFF;LOCK1_LIGHT2_OFF;}
#define   LOCK1_LIGHT_ON()  			{LOCK1_LIGHT1_ON;LOCK1_LIGHT2_ON;}	
#define   LOCK2_LIGHT_OFF()   		{LOCK2_LIGHT1_OFF;LOCK2_LIGHT2_OFF;}	
#define   LOCK2_LIGHT_ON() 	 			{LOCK2_LIGHT1_ON;LOCK2_LIGHT2_ON;}		

#define   LOCK_TIME			400

#define		SH_CLK_IO					 GPIO_Pin_12
#define		SH_CLK_PORT				GPIOB
#define   SH_CLK_RCC					RCC_APB2Periph_GPIOB
#define		ST_CLK_IO					GPIO_Pin_13
#define		ST_CLK_PORT				GPIOB
#define   ST_CLK_RCC					RCC_APB2Periph_GPIOB
#define		HC595_DS_IO					GPIO_Pin_13
#define		HC595_DS_PORT				GPIOB
#define   HC595_DS_RCC					RCC_APB2Periph_GPIOB

#define   SET_SH_CLK   			GPIO_ResetBits(SH_CLK_PORT, SH_CLK_IO)
#define   RESET_SH_CLK  				GPIO_SetBits(SH_CLK_PORT, SH_CLK_IO)
#define   SET_ST_CLK   			GPIO_ResetBits(ST_CLK_PORT, ST_CLK_IO)
#define   RESET_ST_CLK 	 				GPIO_SetBits(ST_CLK_PORT, ST_CLK_IO)
#define   SET_HC595_DS   			GPIO_ResetBits(HC595_DS_PORT, HC595_DS_IO)
#define   RESET_HC595_DS 	 				GPIO_SetBits(HC595_DS_PORT, HC595_DS_IO)

#define Delay_595   10
#define init595()	{RESET_SH_CLK;RESET_ST_CLK;Delay_us(Delay_595);}
#define Shife595()	{ SET_SH_CLK;delay_us(Delay_595); RESET_SH_CLK;delay_us(Delay_595);}
#define Out595()	{ SET_ST_CLK;delay_us(Delay_595); RESET_ST_CLK;delay_us(Delay_595);}

extern u8 Lock1_State;
extern u8 Lock2_State;
extern u16 lock1_time;
extern u16 lock2_time;

void LOCK_GPIO_Config(void);
void Lock_control(void );



#endif




