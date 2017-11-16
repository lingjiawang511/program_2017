#include"HeadType.h"

int Lock_Excute_Time;
#define LOCK_SHORT_TIME 		20
#define LOCK_LONG_TIME			300
u8 Lock1_State;
u8 Lock2_State;
u16 lock1_time;
u16 lock2_time;
u8 open_lock_count = 0;
void Write595_Alllock(u32 lockdata);
//=============================================================================
//函数名称: LED_GPIO_Config
//功能概要:LED灯引脚配置
//参数名称:无
//函数返回:无
//注意    :无
//=============================================================================
void LOCK_GPIO_Config(void)
{	
	GPIO_InitTypeDef  GPIO_InitStructure;
	//HC595移位时钟
	RCC_APB2PeriphClockCmd(SH_CLK_RCC,ENABLE);
	GPIO_InitStructure.GPIO_Pin = SH_CLK_IO;			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(SH_CLK_PORT, &GPIO_InitStructure);
	//HC595保持时钟
	RCC_APB2PeriphClockCmd(ST_CLK_RCC,ENABLE);
	GPIO_InitStructure.GPIO_Pin = ST_CLK_IO;			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(ST_CLK_PORT, &GPIO_InitStructure);
	//HC595信号输入
	RCC_APB2PeriphClockCmd(HC595_DS_RCC,ENABLE);
	GPIO_InitStructure.GPIO_Pin = HC595_DS_IO;			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(HC595_DS_PORT, &GPIO_InitStructure);
	//HC595使能
	RCC_APB2PeriphClockCmd(HC595_EN_RCC,ENABLE);
	GPIO_InitStructure.GPIO_Pin = HC595_EN_IO;			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(HC595_EN_PORT, &GPIO_InitStructure);	
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14;			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(HC595_EN_PORT, &GPIO_InitStructure);
  
//   GPIO_ResetBits(GPIOB, GPIO_Pin_13);
  GPIO_SetBits(GPIOB, GPIO_Pin_13);
  GPIO_ResetBits(GPIOB, GPIO_Pin_14);
  HC595_ENABLE;
  init595();
  Write595_Alllock(0x00000000);
}


/**************************************************************************************
* 名   称:	  Write595_byte
* 功   能: 	  写8位数据进595 ,但不输出
* 参   数: 	  需要写的数据bytedata
* 返回值:	  无
*
* 修改历史:
*   版本   日期    作者    
*   ----------------------------------------------------
*   1.0   2015.7.30   ling     
**************************************************************************************/
void Write595_byte(u8 bytedata)
{
	u8 j;  
	
		for(j=0;j<8;j++)
		{
     if((0x80>>j)&bytedata){
				SET_HC595_DS;
		}else{
				RESET_HC595_DS;
		}
			Shife595();
		}
}
void Write595_Alllock(u32 lockdata)
{
    u8 i;
    for(i = 0;i < 4;i++){
      Write595_byte((lockdata>>((3-i)*8)) & 0xFF);
    }
    Out595();
}
void Lock_control(void )
{
	static enum{
		LOCK_READY,
		LOCK_OPEN_ONE,
    LOCK_OPEN_DELAY,
		LOCK_CLOSE,
	}Lock_state = LOCK_READY;
	switch(Lock_state){
		case LOCK_READY:
      if(open_lock_count > 0){
        Lock_state = LOCK_OPEN_ONE;
      }
			break ;
		case LOCK_OPEN_ONE:	
      Write595_Alllock(0x80000000 >> (32 - open_lock_count));
    	Lock_Excute_Time = LOCK_OPEN_TIME;
			Lock_state = LOCK_OPEN_DELAY;
			break;
    case LOCK_OPEN_DELAY:
      if(Lock_Excute_Time == 0){
        Write595_Alllock(0x00000000);
        Lock_Excute_Time = LOCK_CLOSE_TIME;
        Lock_state = LOCK_CLOSE;
      }
      break;
		case LOCK_CLOSE:
      if(Lock_Excute_Time == 0){
        open_lock_count = 0;
        Lock_state = LOCK_READY;
      }
			break;
		default :
			break;

	}
}









