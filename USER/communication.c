#include"HeadType.h"


COMM_Rec_Union_Type  MCU_Host_Rec;//MCU作为主机时的结构体接收应答变量
static u8 slave_rec_state;
u8 slaveaddr = 3;
static u8 openlockflag = 0x41;
//=============================================================================
//函数名称:Respond_Host_Comm
//功能概要:响应上位机的发出的数据命令，数据已经从串口一接收完整
//参数说明:无
//函数返回:无
//注意    :无
//=============================================================================
static void Respond_Host_Comm(void)
{
		u8 i;
		u16 lrc;
	  u8 res,detection_lock,lock_state;
		if(Usart1_Control_Data.rx_count < 8){
				return;
		}
		if(Usart1_Control_Data.rxbuf[1] != openlockflag){
				return ;
		}
		lrc=LRC_GetLRC16(&Usart1_Control_Data.rxbuf[1],Usart1_Control_Data.rx_count-5);
		if((Usart1_Control_Data.rxbuf[Usart1_Control_Data.rx_count-3]+\
			Usart1_Control_Data.rxbuf[Usart1_Control_Data.rx_count-4]*256 == lrc)){	 
			for(i = 0;i < Usart1_Control_Data.rx_count;i++){
					MCU_Host_Rec.rec_buf[i] = Usart1_Control_Data.rxbuf[i];
			}//把数据复制给主机通讯结构体,数据正确，先回应主机，记录刷写OLED状态位
				slave_rec_state = 1;	//从机接收数据正确
		 if(MCU_Host_Rec.control.comm == 0x41){
				res = Execute_Host_Comm();  //计算哪个锁将打开
				if(res != 0){ //不是设备拥有的锁，比如超过范围，直接返回
					return;
				}
				RE485_SEND;
				Usart1_Control_Data.tx_count = 0;	
				Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = MCU_Host_Rec.control.frame_start;
				Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = MCU_Host_Rec.control.comm;
				Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = MCU_Host_Rec.control.addr;
				Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = MCU_Host_Rec.control.lockH;
				Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = MCU_Host_Rec.control.lockL;
				lrc=LRC_GetLRC16(&Usart1_Control_Data.txbuf[1],Usart1_Control_Data.tx_count - 1);
				Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = (lrc>>8)&0xFF; 
				Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = lrc&0xFF;
				Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0X0D;
				Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0X09;
			}else if(MCU_Host_Rec.control.comm == 0x4C){
				RE485_SEND;
				Usart1_Control_Data.tx_count = 0;	
				Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = MCU_Host_Rec.control.frame_start;
				Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = MCU_Host_Rec.control.comm;
				Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = MCU_Host_Rec.control.addr;
				Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = (Lock_Actual_State>>24)&0xFF;
				Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = (Lock_Actual_State>>16)&0xFF;
				Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = (Lock_Actual_State>>8)&0xFF;
				Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = Lock_Actual_State&0xFF;
				lrc=LRC_GetLRC16(&Usart1_Control_Data.txbuf[1],Usart1_Control_Data.tx_count - 1);
				Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = (lrc>>8)&0xFF; 
				Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = lrc&0xFF;
				Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0X0D;
				Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0X09;				
			}else if(MCU_Host_Rec.control.comm == 0x4D){
					
			  detection_lock = HexToChar(MCU_Host_Rec.control.lockH)*16 + HexToChar(MCU_Host_Rec.control.lockL);
				if(detection_lock <= 32){
					if((Lock_Actual_State>>(detection_lock-1))&0x01){
							lock_state = 0x31;
					}else{
							lock_state = 0x30;
					}
				}else{
					return;
				}
				RE485_SEND;
				Usart1_Control_Data.tx_count = 0;	
				Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = MCU_Host_Rec.control.frame_start;
				Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = MCU_Host_Rec.control.comm;
				Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = MCU_Host_Rec.control.addr;
				Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = MCU_Host_Rec.control.lockH;
				Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = MCU_Host_Rec.control.lockL;
				Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = lock_state;
				lrc=LRC_GetLRC16(&Usart1_Control_Data.txbuf[1],Usart1_Control_Data.tx_count - 1);
				Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = (lrc>>8)&0xFF; 
				Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = lrc&0xFF;
				Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0X0D;
				Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0X09;	
			}
		}else{	//LRC错误
			RE485_SEND;
			Usart1_Control_Data.tx_count = 0;	
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = MCU_Host_Rec.control.frame_start;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = MCU_Host_Rec.control.comm;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = MCU_Host_Rec.control.addr;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0x00;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0x00;
			lrc=LRC_GetLRC16(&Usart1_Control_Data.txbuf[1],Usart1_Control_Data.tx_count - 1);
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = (lrc>>8)&0xFF; 
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = lrc&0xFF;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0X0D;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0X09;
      //校验错误不返回
      Usart1_Control_Data.tx_count = 0;	
      Usart1_Control_Data.tx_index = 0;
      Usart1_Control_Data.rx_aframe = 0;	//清空和主机的通讯，避免通讯错误
      Usart1_Control_Data.rx_count = 0;	
      return;
		}
		Usart1_Control_Data.tx_index = 0;	
		USART_SendData(USART1,Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_index++]);
		Usart1_Control_Data.rx_aframe = 0;	//清空和主机的通讯，避免通讯错误
		Usart1_Control_Data.rx_count = 0;	
}


//=============================================================================
//函数名称:Execute_Host_Comm
//功能概要:执行上位机发出的命令
//参数说明:无
//函数返回:无
//注意    :无
//=============================================================================
u8  Execute_Host_Comm(void)
{
  u8 res = 0;
	if(slave_rec_state == 1){//执行主机发送的命令
    open_lock_count = HexToChar(MCU_Host_Rec.control.lockH)*16 + HexToChar(MCU_Host_Rec.control.lockL);
    if(open_lock_count <= 32){
      res = 0;
    }else{
      open_lock_count = 0;
      res = 1;
    }
		slave_rec_state = 0;
	}
	return res;
}

void Communication_Process(void )
{
		if ((1 == Usart1_Control_Data.rx_aframe)&&(open_lock_count == 0)){ 
			Respond_Host_Comm();
			Usart1_Control_Data.rx_count = 0;
			Usart1_Control_Data.rx_aframe = 0;
		}
}












