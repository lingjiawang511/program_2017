#include"HeadType.h"

// int Lock_Excute_Time;
// #define LOCK_SHORT_TIME 		20
// #define LOCK_LONG_TIME			300
// u8 Lock1_State;
// u8 Lock2_State;
// u16 lock1_time;
// u16 lock2_time;
// u8 open_lock_count = 0;
// void Write595_Alllock(u32 lockdata);
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
	static u8 rdatabyte;
	static u32 rdata;
	
	rdata = 0;
	load165data();
	for(i= 0;i<4;i++){
		rdatabyte = Read165_byte();
		rdata += rdatabyte<<(3-i)*8;
		
	}
	return rdata;
}
// void Lock_control(void )
// {
// 	static enum{
// 		LOCK_READY,
// 		LOCK_OPEN_ONE,
//     LOCK_OPEN_DELAY,
// 		LOCK_CLOSE,
// 	}Lock_state = LOCK_READY;
// 	switch(Lock_state){
// 		case LOCK_READY:
//       if(open_lock_count > 0){
//         Lock_state = LOCK_OPEN_ONE;
//       }
// 			break ;
// 		case LOCK_OPEN_ONE:	
//       Write595_Alllock(0x80000000 >> (32 - open_lock_count));
//     	Lock_Excute_Time = LOCK_OPEN_TIME;
// 			Lock_state = LOCK_OPEN_DELAY;
// 			break;
//     case LOCK_OPEN_DELAY:
//       if(Lock_Excute_Time == 0){
//         Write595_Alllock(0x00000000);
//         Lock_Excute_Time = LOCK_CLOSE_TIME;
//         Lock_state = LOCK_CLOSE;
//       }
//       break;
// 		case LOCK_CLOSE:
//       if(Lock_Excute_Time == 0){
//         open_lock_count = 0;
//         Lock_state = LOCK_READY;
//       }
// 			break;
// 		default :
// 			break;

// 	}
// }









