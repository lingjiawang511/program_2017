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
		u16 crc;
	  u8 res;
		if(Usart2_Control_Data.rx_count < 8){
				return;
		}
		if(Usart2_Control_Data.rxbuf[1] != openlockflag){
				return ;
		}
		crc=CRC_GetCCITT(Usart2_Control_Data.rxbuf,Usart2_Control_Data.rx_count - 4);
		if((Usart2_Control_Data.rxbuf[Usart2_Control_Data.rx_count-3]+\
			Usart1_Control_Data.rxbuf[Usart1_Control_Data.rx_count-4]*256 == crc)){	 
			for(i = 0;i < Usart2_Control_Data.rx_count;i++){
					MCU_Host_Rec.rec_buf[i] = Usart2_Control_Data.rxbuf[i];
			}//把数据复制给主机通讯结构体,数据正确，先回应主机，记录刷写OLED状态位
			slave_rec_state = 1;	//从机接收数据正确
			res = Execute_Host_Comm();  //计算哪个锁将打开
      if(res != 0){ //不是设备拥有的锁，比如超过范围，直接返回
        return;
      }
			TEMP_RE485_SEND;
			Usart2_Control_Data.tx_count = 0;	
			Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = MCU_Host_Rec.control.frame_soh;
			Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = MCU_Host_Rec.control.frame_x;
			Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = MCU_Host_Rec.control.addr;
      Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0x01;
			crc=LRC_GetLRC16(Usart2_Control_Data.txbuf,Usart2_Control_Data.tx_count);
			Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = (crc>>8)&0xFF; 
			Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = crc&0xFF;
			Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0X0D;
			Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0X0A;
		}else{	//CRC错误
			TEMP_RE485_SEND;
			Usart2_Control_Data.tx_count = 0;	
			Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = MCU_Host_Rec.control.frame_soh;
			Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = MCU_Host_Rec.control.frame_x;
			Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = MCU_Host_Rec.control.addr;
			Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0x00;
			crc=LRC_GetLRC16(Usart2_Control_Data.txbuf,Usart2_Control_Data.tx_count);
			Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = (crc>>8)&0xFF; 
			Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = crc&0xFF;
			Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0X0D;
			Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0X0A;
		}
		Usart2_Control_Data.tx_index = 0;	
		USART_SendData(USART2,Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_index++]);
		Usart2_Control_Data.rx_aframe = 0;	//清空和主机的通讯，避免通讯错误
		Usart2_Control_Data.rx_count = 0;	
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
  u8 i;
  u8 res = 0;
  u16 crc;
  static u8 addr_offset = 0,addr_offset_backups = 128;
	if(slave_rec_state == 1){//执行主机发送的命令
    crc = CRC_GetCCITT(MCU_Host_Rec.control.phone, 11);
    for(i = 0;i < 11;i++){
      AT24CXX_WriteOneByte(addr_offset + i,MCU_Host_Rec.control.phone[i]);
      AT24CXX_WriteOneByte(addr_offset_backups + i,MCU_Host_Rec.control.phone[i]);
    }
    AT24CXX_WriteOneByte(addr_offset + i,(crc>>8)&0xFF);
    AT24CXX_WriteOneByte(addr_offset_backups + i++,(crc>>8)&0xFF);
    
    AT24CXX_WriteOneByte(addr_offset + i,crc&0xFF);
    AT24CXX_WriteOneByte(addr_offset + i,crc&0xFF);    
		slave_rec_state = 0;
	}
	return res;
}

void Communication_Process(void )
{
		if (1 == Usart2_Control_Data.rx_aframe){ 
			Respond_Host_Comm();
			Usart2_Control_Data.rx_count = 0;
			Usart2_Control_Data.rx_aframe = 0;
		}
}












