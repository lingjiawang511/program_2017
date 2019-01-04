#ifndef __LOCK_STATE_H
#define __LOCK_STATE_H

#include "stm32f10x.h"


#define   LOCK__STATE_TIME			400

#define		STATE_SL_IO					  GPIO_Pin_5
#define		STATE_SL_PORT				  GPIOA
#define   STATE_SL_RCC					RCC_APB2Periph_GPIOA
#define		STATE_CLK_IO					GPIO_Pin_6
#define		STATE_CLK_PORT			  GPIOA
#define   STATE_CLK_RCC					RCC_APB2Periph_GPIOA
#define		STATE_DS_IO						GPIO_Pin_7
#define		STATE_DS_PORT					GPIOA
#define   STATE_DS_RCC					RCC_APB2Periph_GPIOA


#define   SET_STATE_SL  		  	GPIO_SetBits(STATE_SL_PORT, STATE_SL_IO)
#define   RESET_STATE_SL   			GPIO_ResetBits(STATE_SL_PORT, STATE_SL_IO)
#define   SET_STATE_CLK	 			  GPIO_SetBits(STATE_CLK_PORT, STATE_CLK_IO)
#define   RESET_STATE_CLK    		GPIO_ResetBits(STATE_CLK_PORT, STATE_CLK_IO)

// #define   RESET_STATE_DS  			GPIO_ResetBits(STATE_DS_PORT, STATE_DS_IO)
// #define   SET_STATE_DS 	 				GPIO_SetBits(STATE_DS_PORT, STATE_DS_IO)

#define 	Delay_165   2
#define 	init165()	  		{ SET_STATE_SL;RESET_STATE_CLK;delay_us(Delay_165*5);}
#define 	load165data()	  { RESET_STATE_SL;RESET_STATE_CLK;delay_us(Delay_165);SET_STATE_SL;delay_us(Delay_165);}
#define 	Shife165()			{ RESET_STATE_CLK;delay_us(Delay_165); SET_STATE_CLK;delay_us(Delay_165);}

#define   READ_STATE_DS  		   GPIO_ReadInputDataBit(STATE_DS_PORT,STATE_DS_IO)


void LOCK_STATE_GPIO_Config(void);
u8 Read165_byte(void);
u32 Read165_Allstate(void);



#endif




