#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 

#define       TEMP_RE485_SEND    	    GPIO_SetBits(GPIOB, GPIO_Pin_12)
#define       TEMP_RE485_REC  				GPIO_ResetBits(GPIOB, GPIO_Pin_12)

#define       PC_RE485_SEND    	      GPIO_SetBits(GPIOA, GPIO_Pin_1)
#define       PC_RE485_REC  				  GPIO_ResetBits(GPIOA, GPIO_Pin_4)

#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收

#define USART1_MAX_RECV_LEN		1024				//最大接收缓存字节数
#define USART1_MAX_SEND_LEN		1024				//最大发送缓存字节数
#define USART1_RX_EN 			1					//0,不接收;1,接收.
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	


extern u8  USART1_RX_BUF[USART1_MAX_RECV_LEN]; 		//接收缓冲,最大USART2_MAX_RECV_LEN字节
extern u8  USART1_TX_BUF[USART1_MAX_SEND_LEN]; 		//发送缓冲,最大USART2_MAX_SEND_LEN字节
extern u16 USART1_RX_STA;   						//接收数据状态

void USART1_Init(u32 bound);				//串口2初始化 
void TIM4_Set(u8 sta);
void TIM4_Init(u16 arr,u16 psc);
void UART_DMA_Config(DMA_Channel_TypeDef*DMA_CHx,u32 cpar,u32 cmar);
void UART_DMA_Enable(DMA_Channel_TypeDef*DMA_CHx,u16 len);
void u1_printf(char* fmt, ...);

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


