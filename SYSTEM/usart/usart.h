#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 

#define       RE485_SEND    	  GPIO_SetBits(GPIOA, GPIO_Pin_11)
#define       RE485_REC  				GPIO_ResetBits(GPIOA, GPIO_Pin_11)


#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	
//如果想串口中断接收，请不要注释以下宏定义
void uart_init(u32 bound);

void USART1_IRQHandler(void);	
int fputc(int ch, FILE *f);
void USART1_Putc(unsigned char c);
void USART1_Puts(char * str);
void USART1_Config(void);
void USART2_Config(void);
void USART2_Putc(unsigned char c);
void USART2_Puts(char * str);
void USART3_Config(void);
void USART3_Putc(unsigned char c);
void USART3_Puts(char * str);

void USART1_Do_Tx(void );
void USART2_Do_Tx(void );
void USART3_Do_Tx(void );
void USART1_Do_Rx(u8 rxdata);
void USART2_Do_Rx(u8 rxdata);
void USART3_Do_Rx(u8 rxdata);
#endif


