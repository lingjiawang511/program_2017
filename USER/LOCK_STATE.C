#include"HeadType.h"

u16 lock_state_decection_time;
u32 Lock_Actual_State;

//=============================================================================
//函数名称: LED_GPIO_Config
//功能概要:LED灯引脚配置
//参数名称:无
//函数返回:无
//注意    :无
//=============================================================================
void LOCK_STATE_GPIO_Config(void)
{	
	GPIO_InitTypeDef  GPIO_InitStructure;
	//HC165并行输入控制口
	RCC_APB2PeriphClockCmd(STATE_SL_RCC,ENABLE);
	GPIO_InitStructure.GPIO_Pin = STATE_SL_IO;			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(STATE_SL_PORT, &GPIO_InitStructure);
	//HC165串行移位时钟
	RCC_APB2PeriphClockCmd(STATE_CLK_RCC,ENABLE);
	GPIO_InitStructure.GPIO_Pin = STATE_CLK_IO;			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(STATE_CLK_PORT, &GPIO_InitStructure);
	//HC165串行输入
	RCC_APB2PeriphClockCmd(STATE_DS_RCC,ENABLE);
	GPIO_InitStructure.GPIO_Pin = STATE_DS_IO;			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(STATE_DS_PORT, &GPIO_InitStructure);

  init165();
	lock_state_decection_time = LOCK_STATE_DETECTION_TIME;
}


/**************************************************************************************
* 名   称:	  Read165_byte
* 功   能: 	  从165读8位数据进MCU 
* 参   数: 	  无
* 返回值:	  返回从165读到的数据
*
* 修改历史:
*   版本   日期    作者    
*   ----------------------------------------------------
*   1.0   2019.01.04   ling     
**************************************************************************************/
u8 Read165_byte(void)
{
	 u8 j;  
	 u8 rdata = 0;
// 		Shife165(); 
		for(j=0;j<8;j++){
			  rdata = rdata<<1;
			  if(READ_STATE_DS){
					rdata +=1; 
				}
				Shife165(); 
		}
		return rdata;
}
u32 Read165_Allstate(void)
{
	u8 i;
	u8 rdatabyte;
	u32 rdata;
	
	rdata = 0;
	load165data();
	for(i= 0;i<4;i++){
		rdatabyte = Read165_byte();
		rdata += rdatabyte<<(3-i)*8;
		
	}
	return rdata;
}
void Lock_Detection_Control(void )
{
	static enum{
		LOCK_DETECTION_READY,
		LOCK_DETECTION_FIRST,
    LOCK_DETECTION_SECOND,
		LOCK_DETECTION_COMPLETE,
	}Lock_detection_state = LOCK_DETECTION_READY;
	static u32 lock_state1,lock_state2;
	switch(Lock_detection_state){
		case LOCK_DETECTION_READY:
				Lock_detection_state = LOCK_DETECTION_FIRST;
		    lock_state_decection_time = LOCK_STATE_DETECTION_TIME;
			break ;
		case LOCK_DETECTION_FIRST:	
      if(lock_state_decection_time == 0){
				lock_state1 = Read165_Allstate();
				lock_state_decection_time = LOCK_STATE_DETECTION_TIME;
				Lock_detection_state = LOCK_DETECTION_SECOND;
			}
			break;
    case LOCK_DETECTION_SECOND:
      if(lock_state_decection_time == 0){
				lock_state2 = Read165_Allstate();
				Lock_detection_state = LOCK_DETECTION_COMPLETE;
			}
      break;
		case LOCK_DETECTION_COMPLETE:
				if(lock_state1 == lock_state2){
					Lock_Actual_State = lock_state1;
				}
				Lock_detection_state = LOCK_DETECTION_READY;
			break;
		default :
			Lock_detection_state = LOCK_DETECTION_READY;
			break;
	}	
	InitiativeToSend_LockState_ToPC();
}

void InitiativeToSend_LockState_ToPC(void)
{
	u8 i;
	u16 lrc;
	static u32 old_lock_actual_state = 0X00000000;
  static u8 old_lock_state_buf[32] = {0},lock_state_buf[32] = {0};
	static u8 send_count = 0;
	
	if(Check_Lock_State_Time == 0){
		if(send_count == 0){
			for(i=0;i<32;i++){
				old_lock_state_buf[i] = (old_lock_actual_state&(0x01<<i)) ? 0x31:0x30;
				lock_state_buf[i] = (Lock_Actual_State&(0x01<<i)) ? 0x31:0x30;
				if(old_lock_state_buf[i] != lock_state_buf[i]){
					send_count++;
				}
			}
			old_lock_actual_state = Lock_Actual_State;
		}
		if(send_count >0){
			for(i=0;i<32;i++){
				if(old_lock_state_buf[i] != lock_state_buf[i]){
				 while(Usart1_Control_Data.tx_count!=0);
					
					Usart1_Control_Data.tx_count = 0;	
					Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0X40;
					Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0X53;
					Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0X31;
					Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = CharToHex((i+1)/16);
					Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = CharToHex((i+1)%16);
					Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = lock_state_buf[i];
					lrc=LRC_GetLRC16(&Usart1_Control_Data.txbuf[1],Usart1_Control_Data.tx_count - 1);
					Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = (lrc>>8)&0xFF; 
					Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = lrc&0xFF;
					Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0X0D;
					Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0X09;	
					Usart1_Control_Data.tx_index = 0;	
					USART_SendData(USART1,Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_index++]);
					
					old_lock_state_buf[i] = lock_state_buf[i];
					break;
				}
			}
			send_count--;
		}
		Check_Lock_State_Time = CHECK_LOCK_STATE_TIME;
	}
}







