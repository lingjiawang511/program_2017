/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h" 
#include"HeadType.h"
#include "sim900a.h"
 
void NMI_Handler(void)
{
}
 
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}
 
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

 
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}
 
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}
 
void SVC_Handler(void)
{
}
 
void DebugMon_Handler(void)
{
}
 
void PendSV_Handler(void)
{
}
 
void SysTick_Handler(void)
{
}

//=============================================================================
//��������:TIM2_IRQHandler
//���ܸ�Ҫ:TIM2 �жϺ���
//����˵��:��
//��������:��
//=============================================================================
void TIM2_IRQHandler(void)
{
	if ( TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET ) 
	{	
		if(Update_Gsm_Time > 0){
			Update_Gsm_Time--;
		}
		LED_Display();
		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);  		 
	}		 	
}

void USART1_IRQHandler(void)
{
	u8 res;	    
if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//���յ�����
	{	 
		res =USART_ReceiveData(USART1);				 
		if(USART1_RX_STA<USART1_MAX_RECV_LEN)		//�����Խ�������
		{
		  TIM_SetCounter(TIM4,0);//���������      
			if(USART1_RX_STA==0)TIM4_Set(1);	 	//ʹ�ܶ�ʱ��4���ж� 
			USART1_RX_BUF[USART1_RX_STA++]=res;		//��¼���յ���ֵ	 
		}else 
		{
			USART1_RX_STA|=1<<15;					//ǿ�Ʊ�ǽ������
		} 
	}  											 
} 
//=============================================================================
//��������:USART1_IRQHandler
//���ܸ�Ҫ:USART1 �жϺ���
//����˵��:��
//��������:��
//=============================================================================
void USART2_IRQHandler(void)
{
  
	if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE)||USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET){ //�������û������һֱ���жϵ�����
    USART2_Do_Rx(USART_ReceiveData(USART2)); 
		USART_ClearFlag(USART2,USART_FLAG_RXNE);
	}
	if(USART_GetFlagStatus(USART2, USART_FLAG_TC)){
    USART2_Do_Tx();   
		USART_ClearFlag(USART2,USART_FLAG_TC);
	}
}

//=============================================================================
//��������:USART1_IRQHandler
//���ܸ�Ҫ:USART1 �жϺ���
//����˵��:��
//��������:��
//=============================================================================
void USART3_IRQHandler(void)
{
  
	if(USART_GetFlagStatus(USART3, USART_FLAG_RXNE)||USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET){ //�������û������һֱ���жϵ�����
    USART3_Do_Rx(USART_ReceiveData(USART3));  
		USART_ClearFlag(USART3,USART_FLAG_RXNE);
	}
	if(USART_GetFlagStatus(USART3, USART_FLAG_TC)){
    USART3_Do_Tx();  
		USART_ClearFlag(USART3,USART_FLAG_TC);
	}
}


/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
