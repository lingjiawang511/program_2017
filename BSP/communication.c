#include"HeadType.h"


COMM_Rec_Union_Type  MCU_Host_Rec;//MCU作为主机时的结构体接收应答变量
static u8 slave_rec_state;
u8 slaveaddr = 1;
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
		if((Usart2_Control_Data.rx_count < 18)||(Usart2_Control_Data.rxbuf[2] != slaveaddr)){
				return;
		}
		crc=CRC_GetCCITT(Usart2_Control_Data.rxbuf,Usart2_Control_Data.rx_count - 4);
		if((Usart2_Control_Data.rxbuf[Usart2_Control_Data.rx_count-3]+\
			Usart2_Control_Data.rxbuf[Usart2_Control_Data.rx_count-4]*256 == crc)){	 
			for(i = 0;i < Usart2_Control_Data.rx_count;i++){
					MCU_Host_Rec.rec_buf[i] = Usart2_Control_Data.rxbuf[i];
			}//把数据复制给主机通讯结构体
			slave_rec_state = 1;	//从机接收数据正确
			res = Execute_Host_Comm();  
      if(res != 0){ 
        return;
      }
			PC_RE485_SEND;
			Usart2_Control_Data.tx_count = 0;	
			Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0x01;
			Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0x58;
			Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = MCU_Host_Rec.control.addr;
      Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0x01;
			crc=LRC_GetLRC16(Usart2_Control_Data.txbuf,Usart2_Control_Data.tx_count);
			Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = (crc>>8)&0xFF; 
			Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = crc&0xFF;
			Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0X0D;
			Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0X0A;
		}else{	//CRC错误
			PC_RE485_SEND;
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
  static u8 addr_offset = ADDR_OFFSET,addr_offset_backups = ADDR_OFFSET + PAGE_SIZE;
	if(slave_rec_state == 1){//执行主机发送的命令
    crc = CRC_GetCCITT(MCU_Host_Rec.control.phone, 11);
    for(i = 0;i < 11;i++){
      send_phone_gbk[i] = MCU_Host_Rec.control.phone[i];//将号码存入GBK数组 
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
//=============================================================================
//函数名称:Execute_Temp_Comm
//功能概要:执行上位机发出的命令
//参数说明:无
//函数返回:无
//注意    :无
//=============================================================================
void  Execute_Temp_Comm(void)
{
  		TEMP_RE485_SEND;
			Usart3_Control_Data.tx_count = 0;	
			Usart3_Control_Data.txbuf[Usart3_Control_Data.tx_count++] = 0x01;
			Usart3_Control_Data.txbuf[Usart3_Control_Data.tx_count++] = 0x58;
			Usart3_Control_Data.txbuf[Usart3_Control_Data.tx_count++] = 0X55;
      Usart3_Control_Data.txbuf[Usart3_Control_Data.tx_count++] = 0x01;
			Usart3_Control_Data.txbuf[Usart3_Control_Data.tx_count++] = 0X0D;
			Usart3_Control_Data.txbuf[Usart3_Control_Data.tx_count++] = 0X0A;
  		Usart3_Control_Data.tx_index = 0;	
      USART_SendData(USART3,Usart3_Control_Data.txbuf[Usart3_Control_Data.tx_index++]);
      Usart3_Control_Data.rx_aframe = 0;	//清空和主机的通讯，避免通讯错误
      Usart3_Control_Data.rx_count = 0;	
}
void Communication_Process(void )
{
		if (1 == Usart2_Control_Data.rx_aframe){ 
			Respond_Host_Comm();
			Usart2_Control_Data.rx_count = 0;
			Usart2_Control_Data.rx_aframe = 0;
		}
    if (1 == Usart3_Control_Data.rx_aframe){ 
			Execute_Temp_Comm();
			Usart3_Control_Data.rx_count = 0;
			Usart3_Control_Data.rx_aframe = 0;
		}
}












