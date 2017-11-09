#include "sys.h"
#include "usart.h"
#include"HeadType.h"
#include "delay.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif
 
//���ڷ��ͻ����� 	
__align(8) u8 USART1_TX_BUF[USART1_MAX_SEND_LEN]; 	//���ͻ���,���USART2_MAX_SEND_LEN�ֽ�
#ifdef USART1_RX_EN   								//���ʹ���˽���   	  
//���ڽ��ջ����� 	
u8 USART1_RX_BUF[USART1_MAX_RECV_LEN]; 				//���ջ���,���USART2_MAX_RECV_LEN���ֽ�.


//ͨ���жϽ�������2���ַ�֮���ʱ������10ms�������ǲ���һ������������.
//���2���ַ����ռ������10ms,����Ϊ����1����������.Ҳ���ǳ���10msû�н��յ�
//�κ�����,���ʾ�˴ν������.
//���յ�������״̬
//[15]:0,û�н��յ�����;1,���յ���һ������.
//[14:0]:���յ������ݳ���
u16 USART1_RX_STA=0; 
 
 /*����1�ܽ���ӳ��*/
#define USART1REMAP 0

Usart_Type Usart1_Control_Data;
Usart_Type Usart2_Control_Data;
Usart_Type Usart3_Control_Data;
char Auto_Frame_Time1;
char Auto_Frame_Time2;
char Auto_Frame_Time3;

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 0
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART2->SR&0X40)==0);//ѭ������,ֱ���������   
    USART2->DR = (u8) ch;      
	return ch;
}
#endif 


//=============================================================================
//��������:Init_USART1
//���ܸ�Ҫ:USART1 ��ʼ��������ز���
//����˵��:��
//��������:��
//=============================================================================
static void Init_USART1(void)
{
    Usart1_Control_Data.tx_index = 0;
    Usart1_Control_Data.rx_index = 0;
    Usart1_Control_Data.tx_count = 0;
    Usart1_Control_Data.rx_count = 0;
    Usart1_Control_Data.rx_start = 0;
    Usart1_Control_Data.rx_aframe = 0;
    
}
//=============================================================================
//��������:Init_USART2
//���ܸ�Ҫ:USART2 ��ʼ��������ز���
//����˵��:��
//��������:��
//=============================================================================
static void Init_USART2(void)
{
    Usart2_Control_Data.tx_index = 0;
    Usart2_Control_Data.rx_index = 0;
    Usart2_Control_Data.tx_count = 0;
    Usart2_Control_Data.rx_count = 0;
    Usart2_Control_Data.rx_start = 0;
    Usart2_Control_Data.rx_aframe = 0;
    
}
//=============================================================================
//��������:Init_USART3
//���ܸ�Ҫ:USART3 ��ʼ��������ز���
//����˵��:��
//��������:��
//=============================================================================
static void Init_USART3(void)
{
    Usart3_Control_Data.tx_index = 0;
    Usart3_Control_Data.rx_index = 0;
    Usart3_Control_Data.tx_count = 0;
    Usart3_Control_Data.rx_count = 0;
    Usart3_Control_Data.rx_start = 0;
    Usart3_Control_Data.rx_aframe = 0;
    
}
//=============================================================================
//��������:USART1_Interrupts_Config
//���ܸ�Ҫ:USART1 �ж����ȼ�����
//����˵��:��
//��������:��
//=============================================================================
static void USART1_Interrupts_Config(void)
{
     NVIC_InitTypeDef NVIC_InitStructure; 
    
     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
    
     NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
     NVIC_Init(&NVIC_InitStructure);
}
//=============================================================================
//��������:USART2_Interrupts_Config
//���ܸ�Ҫ:USART2 �ж����ȼ�����
//����˵��:��
//��������:��
//=============================================================================
static void USART2_Interrupts_Config(void)
{
     NVIC_InitTypeDef NVIC_InitStructure; 
    
     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
    
     NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
     NVIC_Init(&NVIC_InitStructure);
}
//=============================================================================
//��������:USART3_Interrupts_Config
//���ܸ�Ҫ:USART3 �ж����ȼ�����
//����˵��:��
//��������:��
//=============================================================================
static void USART3_Interrupts_Config(void)
{
     NVIC_InitTypeDef NVIC_InitStructure; 
    
     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
    
     NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
     NVIC_Init(&NVIC_InitStructure);
}
//=============================================================================
//��������:USART1_Config
//���ܸ�Ҫ:USART1 ��ʼ������,����ģʽ���á�57600 8-N-1
//����˵��:��
//��������:��
//=============================================================================
#if USART1REMAP  //���USART1ʹ����ӳ��

void USART1_Config(void )
{	
    GPIO_InitTypeDef GPIO_InitStructure;	
    USART_InitTypeDef USART_InitStructure;  //���崮�ڳ�ʼ���ṹ��
    
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB | RCC_APB2Periph_USART1,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);

  /*  USART1_TX -> PB6 */
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	       //ѡ�д���Ĭ������ܽ�         
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�������������� 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����ܽ�9��ģʽ  
  GPIO_Init(GPIOB, &GPIO_InitStructure);           //���ú������ѽṹ�����������г�ʼ��	
  
  /*  USART1_RX ->	PB7*/

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	        
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  USART_InitStructure.USART_BaudRate = 19200; //������
  USART_InitStructure.USART_WordLength = USART_WordLength_8b; //����λ8λ
  USART_InitStructure.USART_StopBits = USART_StopBits_1;	//ֹͣλ1λ
  USART_InitStructure.USART_Parity = USART_Parity_No;		//У��λ ��
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��������
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//ʹ�ܽ��պͷ�������

  USART_Init(USART1, &USART_InitStructure); //�����ϸ���ֵ�Ľṹ�����⺯��USART_Init���г�ʼ��
  USART_ClearFlag(USART1,USART_FLAG_TC);
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  USART_ITConfig(USART1, USART_IT_TC, ENABLE);
    /* Enable the EXTI Interrupt */
  USART_Cmd(USART1, ENABLE);//����USART1��ע��������RCC_APB2PeriphClockCmd()���õ�����
  USART1_Interrupts_Config();
  Init_USART1();
}
#else
void USART1_Config(void)
{
	//gpio��usart��ʼ���ṹ�嶨��
	//��gpio����ʱ����������ʱ������Ҫ��ʼ��ͬʱgpio��ppp���裬��������ʹ�ܺͿ���ʱ��
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	//����gpio������ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

	//gpio��ʼ���ṹ���Ա��ֵ
	//��ʼ��PA��9�������������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//ִ�������gpio��ʼ��
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// PA.10��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//USART��ʼ��
	USART_InitStructure.USART_BaudRate = 19200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_ClearFlag(USART1,USART_FLAG_TC);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1, USART_IT_TC, ENABLE);
	/* Enable the EXTI Interrupt */
	USART_Cmd(USART1, ENABLE);//����USART1��ע��������RCC_APB2PeriphClockCmd()���õ�����
	
	USART1_Interrupts_Config();
	Init_USART1();
}

#endif
 //=============================================================================
//��������:USART2_Config
//���ܸ�Ҫ:USART2 ��ʼ������,����ģʽ���á�57600 8-N-1
//����˵��:��
//��������:��
//=============================================================================
void USART2_Config(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* Enable the USART2 Pins Software Remapping */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); 

	/* Configure USART2 Tx (PA.02) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* Configure USART2 Rx (PA.03) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //re485 IO

	USART_InitStructure.USART_BaudRate = 19200; 			 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	   
	USART_InitStructure.USART_Parity = USART_Parity_No; 	  
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  

	USART_Init(USART2, &USART_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART2, USART_IT_TC, ENABLE);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	/* Enable USART2 */
	USART_Cmd(USART2, ENABLE);
	USART2_Interrupts_Config();
	Init_USART2();
  PC_RE485_REC;

}
//=============================================================================
//��������:USART1_Config
//���ܸ�Ҫ:USART1 ��ʼ������,����ģʽ���á�57600 8-N-1
//����˵��:��
//��������:��
//=============================================================================

void USART3_Config(void )
{	
  
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* Enable the USART3 Pins Software Remapping */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); 

	/* Configure USART3 Tx (PB .10) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* Configure USART3 Rx (PB.11) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //re485 IO
      
	USART_InitStructure.USART_BaudRate = 19200; 			 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	   
	USART_InitStructure.USART_Parity = USART_Parity_No; 	  
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  

	USART_Init(USART3, &USART_InitStructure);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART3, USART_IT_TC, ENABLE);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	/* Enable USART2 */
	USART_Cmd(USART3, ENABLE);
	USART3_Interrupts_Config();
	Init_USART3();
  TEMP_RE485_REC;
}
//=============================================================================
//��������:fputc
//���ܸ�Ҫ:�ض���c�⺯��printf��USART
//����˵��:��
//��������:��
//ע��   :��printf����,��Ҫ��ѡUse Microlib
//=============================================================================

// int fputc(int ch, FILE *f)
// {
// //��Printf���ݷ������� 
//   USART_SendData(USART1, (unsigned char) ch);
//   while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); 
//   return (ch);
// }
//=============================================================================
//��������:USART1_Putc
//���ܸ�Ҫ:��USART1_Putc�������ݴ�ӡ������
//����˵��:��
//��������:��
//ע��   :��
//=============================================================================
void USART1_Putc(unsigned char c)
{
    USART_SendData(USART1, c);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET );
}

//=============================================================================
//��������:USART1_Putc
//���ܸ�Ҫ:��USART1_Putc�������ݴ�ӡ������
//����˵��:��
//��������:��
//ע��    :��
//=============================================================================
void USART1_Puts(char * str)
{
    while(*str)
    {
        USART_SendData(USART1, *str++);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    }
}
//=============================================================================
//��������:USART2_Putc
//���ܸ�Ҫ:��USART2_Putc�������ݴ�ӡ������
//����˵��:��
//��������:��
//ע��   :��
//=============================================================================
void USART2_Putc(unsigned char c)
{
    USART_SendData(USART2, c);
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET );
}

//=============================================================================
//��������:USART2_Putc
//���ܸ�Ҫ:��USART2_Putc�������ݴ�ӡ������
//����˵��:��
//��������:��
//ע��    :��
//=============================================================================
void USART2_Puts(char * str)
{
    while(*str){
        USART_SendData(USART2, *str++);
        while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    }
}
//=============================================================================
//��������:USART3_Putc
//���ܸ�Ҫ:��USART3_Putc�������ݴ�ӡ������
//����˵��:��
//��������:��
//ע��   :��
//=============================================================================
void USART3_Putc(unsigned char c)
{
    USART_SendData(USART3, c);
    while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET );
}

//=============================================================================
//��������:USART3_Putc
//���ܸ�Ҫ:��USART3_Putc�������ݴ�ӡ������
//����˵��:��
//��������:��
//ע��    :��
//=============================================================================
void USART3_Puts(char * str)
{
    while(*str){
        USART_SendData(USART3, *str++);
        while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
    }
}
//=============================================================================
//��������:USART1_Do_Tx
//���ܸ�Ҫ:������һ�����ݷ��ͳ�ȥ��ͨ���жϷ���
//����˵��:��
//��������:��
//ע��    :��
//=============================================================================
void USART1_Do_Tx(void )
{
    if (Usart1_Control_Data.tx_index < Usart1_Control_Data.tx_count) {
		USART_SendData(USART1, Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_index]);
		Usart1_Control_Data.tx_index++;
	}else{
       Usart1_Control_Data.tx_count = 0; 
       Usart1_Control_Data.tx_index = 0;
    }
}
//=============================================================================
//��������:USART2_Do_Tx
//���ܸ�Ҫ:�����ڶ������ݷ��ͳ�ȥ��ͨ���жϷ���
//����˵��:��
//��������:��
//ע��    :��
//=============================================================================
void USART2_Do_Tx(void )
{
    if (Usart2_Control_Data.tx_index < Usart2_Control_Data.tx_count) {
      USART_SendData(USART2, Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_index]);
      Usart2_Control_Data.tx_index++;
    }else{
       Usart2_Control_Data.tx_count = 0; 
       Usart2_Control_Data.tx_index = 0;
       PC_RE485_REC;
    }
}
//=============================================================================
//��������:USART3_Do_Tx
//���ܸ�Ҫ:�����ڶ������ݷ��ͳ�ȥ��ͨ���жϷ���
//����˵��:��
//��������:��
//ע��    :��
//=============================================================================
void USART3_Do_Tx(void )
{
    if (Usart3_Control_Data.tx_index < Usart3_Control_Data.tx_count) {
      USART_SendData(USART3, Usart3_Control_Data.txbuf[Usart3_Control_Data.tx_index]);
      Usart3_Control_Data.tx_index++;
    }else{
       Usart3_Control_Data.tx_count = 0; 
       Usart3_Control_Data.tx_index = 0;
       TEMP_RE485_REC;
    }
}
//=============================================================================
//��������:USART1_Do_Rx
//���ܸ�Ҫ:������һ���յ�����
//����˵��:��
//��������:��
//ע��    :��
//=============================================================================
void USART1_Do_Rx(u8 rxdata)
{       
		static u8 old_frame_end1,old_frame_end2;
    if (0 == Usart1_Control_Data.rx_aframe){
       if (0 == Usart1_Control_Data.rx_index){  //���յ�һ֡�ĵ�һ�����ݿ�����ʱ��3��ʱ���Զ���֡����
           Usart1_Control_Data.rx_start = 1;
           Auto_Frame_Time1 = AUTO_FRAME_TIMEOUT1; 
       }else {
            if (Auto_Frame_Time1 <=0){   //ʱ�䳬ʱ�Զ���֡
               Usart1_Control_Data.rx_aframe = 1; 
               Usart1_Control_Data.rx_start = 0;
               Usart1_Control_Data.rx_count = Usart1_Control_Data.rx_index;
               Usart1_Control_Data.rx_index = 0;   //�õ�һ֡���ݺ�ʱ����������
               return ;   //ֱ�ӷ��ز���������              
            }                    
        }
				Auto_Frame_Time1 = AUTO_FRAME_TIMEOUT1;
       	Usart1_Control_Data.rxbuf[Usart1_Control_Data.rx_index] = rxdata;
				Usart1_Control_Data.rx_index++;	
				if(Usart1_Control_Data.rx_index >= 2){		//��������֡β��֡
					old_frame_end1 = Usart1_Control_Data.rxbuf[Usart1_Control_Data.rx_index - 2];
					old_frame_end2 = Usart1_Control_Data.rxbuf[Usart1_Control_Data.rx_index - 1];
					if((old_frame_end1 == 0x0D)&&(old_frame_end2 == 0X09)){
							Usart1_Control_Data.rx_aframe = 1;  //�������ݳ����Զ���֡
							Usart1_Control_Data.rx_count = Usart1_Control_Data.rx_index;
						  Usart1_Control_Data.rx_index = 0;   //�õ�һ֡���ݺ�ʱ����������
							Usart1_Control_Data.rx_start = 0;
							Auto_Frame_Time1 = AUTO_FRAME_TIMEOUT1; 
					}
				}
        if (Usart1_Control_Data.rx_index > (RxBufMax - 1)){
            Usart1_Control_Data.rx_index = (RxBufMax - 1);
            Usart1_Control_Data.rx_aframe = 1;  //�������ݳ����Զ���֡
            Usart1_Control_Data.rx_count = Usart1_Control_Data.rx_index+1;
            Usart1_Control_Data.rx_index = 0;   //�õ�һ֡���ݺ�ʱ����������
            Usart1_Control_Data.rx_start = 0;
            Auto_Frame_Time1 = AUTO_FRAME_TIMEOUT1; 
        }   
    }else{  //���յ�һ֡���ݺ���봦����ſ��Լ���������һ֡����
       Usart1_Control_Data.rx_start = 0;
       Auto_Frame_Time1 = AUTO_FRAME_TIMEOUT1; 
        return;
    }           
}
//=============================================================================
//��������:USART2_Do_Rx
//���ܸ�Ҫ:�����ڶ����յ�����
//����˵��:��
//��������:��
//ע��    :��
//=============================================================================
void USART2_Do_Rx(u8 rxdata)
{       
    if (0 == Usart2_Control_Data.rx_aframe){
       if (0 == Usart2_Control_Data.rx_index){  //���յ�һ֡�ĵ�һ�����ݿ�����ʱ��3��ʱ���Զ���֡����
           Usart2_Control_Data.rx_start = 1;
           Auto_Frame_Time2 = AUTO_FRAME_TIMEOUT2; 
       }else {
            if (Auto_Frame_Time2 <=0){   //ʱ�䳬ʱ�Զ���֡
               Usart2_Control_Data.rx_aframe = 1; 
               Usart2_Control_Data.rx_start = 0;
               Usart2_Control_Data.rx_count = Usart2_Control_Data.rx_index;
               Usart2_Control_Data.rx_index = 0;   //�õ�һ֡���ݺ�ʱ����������
               return ;   //ֱ�ӷ��ز���������              
            }                    
        }
				Auto_Frame_Time2 = AUTO_FRAME_TIMEOUT2;
       	Usart2_Control_Data.rxbuf[Usart2_Control_Data.rx_index] = rxdata;
        Usart2_Control_Data.rx_index++;
        if (Usart2_Control_Data.rx_index > (RxBufMax - 1)){
            Usart2_Control_Data.rx_index = (RxBufMax - 1);
            Usart2_Control_Data.rx_aframe = 1;  //�������ݳ����Զ���֡
            Usart2_Control_Data.rx_count = Usart2_Control_Data.rx_index+1;
            Usart2_Control_Data.rx_index = 0;   //�õ�һ֡���ݺ�ʱ����������
            Usart2_Control_Data.rx_start = 0;
            Auto_Frame_Time2 = AUTO_FRAME_TIMEOUT2; 
        }   
    }else{  //���յ�һ֡���ݺ���봦����ſ��Լ���������һ֡����
       Usart2_Control_Data.rx_start = 0;
       Auto_Frame_Time2 = AUTO_FRAME_TIMEOUT2; 
        return;
    }           
}
//=============================================================================
//��������:USART3_Do_Rx
//���ܸ�Ҫ:�����ڶ����յ�����
//����˵��:��
//��������:��
//ע��    :��
//=============================================================================
void USART3_Do_Rx(u8 rxdata)
{       
    if (0 == Usart3_Control_Data.rx_aframe){
       if (0 == Usart3_Control_Data.rx_index){  //���յ�һ֡�ĵ�һ�����ݿ�����ʱ��3��ʱ���Զ���֡����
           Usart3_Control_Data.rx_start = 1;
           Auto_Frame_Time3 = AUTO_FRAME_TIMEOUT3; 
       }else {
            if (Auto_Frame_Time3 <=0){   //ʱ�䳬ʱ�Զ���֡
               Usart3_Control_Data.rx_aframe = 1; 
               Usart3_Control_Data.rx_start = 0;
               Usart3_Control_Data.rx_count = Usart3_Control_Data.rx_index;
               Usart3_Control_Data.rx_index = 0;   //�õ�һ֡���ݺ�ʱ����������
               return ;   //ֱ�ӷ��ز���������              
            }                    
        }
				Auto_Frame_Time3 = AUTO_FRAME_TIMEOUT3;
       	Usart3_Control_Data.rxbuf[Usart3_Control_Data.rx_index] = rxdata;
        Usart3_Control_Data.rx_index++;
        if (Usart3_Control_Data.rx_index > (RxBufMax - 1)){
            Usart3_Control_Data.rx_index = (RxBufMax - 1);
            Usart3_Control_Data.rx_aframe = 1;  //�������ݳ����Զ���֡
            Usart3_Control_Data.rx_count = Usart3_Control_Data.rx_index+1;
            Usart3_Control_Data.rx_index = 0;   //�õ�һ֡���ݺ�ʱ����������
            Usart3_Control_Data.rx_start = 0;
            Auto_Frame_Time3 = AUTO_FRAME_TIMEOUT3; 
        }   
    }else{  //���յ�һ֡���ݺ���봦����ſ��Լ���������һ֡����
       Usart3_Control_Data.rx_start = 0;
       Auto_Frame_Time3 = AUTO_FRAME_TIMEOUT3; 
        return;
    }           
}
   	 
 
//��ʼ��IO ����2
//pclk1:PCLK1ʱ��Ƶ��(Mhz)
//bound:������	  
void USART1_Init(u32 bound)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	// GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

 	USART_DeInit(USART1);  //��λ����1
		 //USART2_TX   PA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.2
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA2
   
    //USART2_RX	  PA.10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA3
	
	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  
	USART_Init(USART1, &USART_InitStructure); //��ʼ������	2
  
	//����������
 //	USART1->BRR=(pclk1*1000000)/(bound);// ����������	 
	//USART1->CR1|=0X200C;  	//1λֹͣ,��У��λ.
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);  	//ʹ�ܴ���1��DMA����
	UART_DMA_Config(DMA1_Channel4,(u32)&USART1->DR,(u32)USART1_TX_BUF);//DMA1ͨ��7,����Ϊ����2,�洢��ΪUSART2_TX_BUF 
	USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 
	
#ifdef USART1_RX_EN		  	//���ʹ���˽���
	//ʹ�ܽ����ж�
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�   
	USART1_Interrupts_Config();
	TIM4_Init(99,7199);		//10ms�ж�
	USART1_RX_STA=0;		//����
	TIM4_Set(0);			//�رն�ʱ��4
#endif	 									  	
}
//����1,printf ����
//ȷ��һ�η������ݲ�����USART2_MAX_SEND_LEN�ֽ�
void u1_printf(char* fmt,...)  
{  
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART1_TX_BUF,fmt,ap);
	va_end(ap);
	while(DMA1_Channel4->CNDTR!=0);	//�ȴ�ͨ��7�������   
	UART_DMA_Enable(DMA1_Channel4,strlen((const char*)USART1_TX_BUF)); 	//ͨ��dma���ͳ�ȥ
}
//��ʱ��4�жϷ������		    
void TIM4_IRQHandler(void)
{ 	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)//�Ǹ����ж�
	{	 			   
		USART1_RX_STA|=1<<15;	//��ǽ������
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx�����жϱ�־    
		TIM4_Set(0);			//�ر�TIM4  
	}   
}
//����TIM4�Ŀ���
//sta:0���ر�;1,����;
void TIM4_Set(u8 sta)
{
	if(sta)
	{
       
		TIM_SetCounter(TIM4,0);//���������
		TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx	
	}else TIM_Cmd(TIM4, DISABLE);//�رն�ʱ��4
}
//ͨ�ö�ʱ���жϳ�ʼ��
//����ʼ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��		 
void TIM4_Init(u16 arr,u16 psc)
{	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��//TIM4ʱ��ʹ��    
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM4�ж�,��������ж�

	 	  
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
}
#endif		 
///////////////////////////////////////USART2 DMA�������ò���//////////////////////////////////	   		    
//DMA1�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
//DMA_CHx:DMAͨ��CHx
//cpar:�����ַ
//cmar:�洢����ַ    
void UART_DMA_Config(DMA_Channel_TypeDef*DMA_CHx,u32 cpar,u32 cmar)
{
	DMA_InitTypeDef DMA_InitStructure;
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����
  DMA_DeInit(DMA_CHx);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  //DMA����ADC����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //���ݴ��䷽�򣬴��ڴ��ȡ���͵�����
	DMA_InitStructure.DMA_BufferSize = 0;  //DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //��������������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMAͨ�� xӵ�������ȼ� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA_CHx, &DMA_InitStructure);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���	
} 
//����һ��DMA����
void UART_DMA_Enable(DMA_Channel_TypeDef*DMA_CHx,u16 len)
{
	DMA_Cmd(DMA_CHx, DISABLE );  //�ر� ָʾ��ͨ��        
	DMA_SetCurrDataCounter(DMA_CHx,len);//DMAͨ����DMA����Ĵ�С	
	DMA_Cmd(DMA_CHx, ENABLE);           //����DMA����
}

// void USART1_IRQHandler(void)                	//����1�жϷ������
// 	{
// 	u8 Res;
// #if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
// 	OSIntEnter();    
// #endif
// 	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
// 		{
// 		Res =USART_ReceiveData(USART1);	//��ȡ���յ�������
// 		
// 		if((USART_RX_STA&0x8000)==0)//����δ���
// 			{
// 			if(USART_RX_STA&0x4000)//���յ���0x0d
// 				{
// 				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
// 				else USART_RX_STA|=0x8000;	//��������� 
// 				}
// 			else //��û�յ�0X0D
// 				{	
// 				if(Res==0x0d)USART_RX_STA|=0x4000;
// 				else
// 					{
// 					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
// 					USART_RX_STA++;
// 					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
// 					}		 
// 				}
// 			}   		 
//      } 
// #if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
// 	OSIntExit();  											 
// #endif
// } 
// #endif	

