#include"HeadType.h"


COMM_Rec_Union_Type  MCU_Host_Rec;//MCU��Ϊ����ʱ�Ľṹ�����Ӧ�����
static u8 slave_rec_state;
u8 slaveaddr = 1;
u8 test_sms_addr = 100;
u8 device_ID_addr = 101;
u32 Mask_Low_Alarm_Time = 0;
u32 Mask_High_Alarm_Time = 0;
//=============================================================================
//��������:Respond_Host_Comm
//���ܸ�Ҫ:��Ӧ��λ���ķ�����������������Ѿ��Ӵ���һ��������
//����˵��:��
//��������:��
//ע��    :��
//=============================================================================
static void Respond_Host_Comm(void)
{
		u8 i;
		u16 crc;
	  u8 res;
		if((Usart2_Control_Data.rx_count < 18)||((Usart2_Control_Data.rxbuf[2] != slaveaddr)&&\
			(Usart2_Control_Data.rxbuf[2] != test_sms_addr)&&(Usart2_Control_Data.rxbuf[2] != device_ID_addr))){
				return;
		}
		crc=CRC_GetCCITT(Usart2_Control_Data.rxbuf,Usart2_Control_Data.rx_count - 4);
		if((Usart2_Control_Data.rxbuf[Usart2_Control_Data.rx_count-3]+\
			Usart2_Control_Data.rxbuf[Usart2_Control_Data.rx_count-4]*256 == crc)){	 
			for(i = 0;i < Usart2_Control_Data.rx_count;i++){
					MCU_Host_Rec.rec_buf[i] = Usart2_Control_Data.rxbuf[i];
			}//�����ݸ��Ƹ�����ͨѶ�ṹ��
			slave_rec_state = 1;	//�ӻ�����������ȷ
			res = Execute_Host_Comm();  
      if(res != 0){ 
        return;
      }
			PC_RE485_SEND;
			Usart2_Control_Data.tx_count = 0;	
			Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0x01;
			Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0x58;
			Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = MCU_Host_Rec.control.addr;
      Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0x01 + Sim_Send_Msg_Flag;
			crc=LRC_GetLRC16(Usart2_Control_Data.txbuf,Usart2_Control_Data.tx_count);
			Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = (crc>>8)&0xFF; 
			Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = crc&0xFF;
			Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0X0D;
			Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0X0A;
		}else{	//CRC����
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
		Usart2_Control_Data.rx_aframe = 0;	//��պ�������ͨѶ������ͨѶ����
		Usart2_Control_Data.rx_count = 0;	
}

u8 cmp_phonenum(u8 *oldnum,u8 *newnum,u8 len)
{
  u8 i;
  for(i = 0;i < len ;i++){
    if(oldnum[i] != newnum[i]){
       return 1;
    }
  }
  return 0;
}
//=============================================================================
//��������:Execute_Host_Comm
//���ܸ�Ҫ:ִ����λ������������
//����˵��:��
//��������:��
//ע��    :��
//=============================================================================
u8  Execute_Host_Comm(void)
{
  u8 i;
  u8 res = 0;
  u16 crc;
  static u8 addr_offset = ADDR_OFFSET,addr_offset_backups = ADDR_OFFSET + PAGE_SIZE;
  Sim_Send_Msg_Flag = 0;
	if(slave_rec_state == 1){//ִ���������͵�����
    if ((cmp_phonenum(MCU_Host_Rec.control.phone,send_phone_gbk,11) == 1)||\
			(MCU_Host_Rec.control.addr == 100)){
      Sim_Send_Msg_Flag = 1;
      Sim_Send_Flag = 1;
    }
    crc = CRC_GetCCITT(MCU_Host_Rec.control.phone, 11);
		if(MCU_Host_Rec.control.addr == 101){
			addr_offset = DEVICE_ID_OFFSET;
			addr_offset_backups = DEVICE_ID_OFFSET + PAGE_SIZE;
			Sim_Send_Msg_Flag = 2;
      Sim_Send_Flag = 1;
			for(i = 0;i < 5;i++){
				send_device_id[i] = MCU_Host_Rec.control.phone[i];
			}
		}else{
			addr_offset = ADDR_OFFSET;
			addr_offset_backups = ADDR_OFFSET + PAGE_SIZE;
			for(i = 0;i < 11;i++){
				send_phone_gbk[i] = MCU_Host_Rec.control.phone[i];//���������GBK����
			}
		}
    for(i = 0;i < 11;i++){
      AT24CXX_WriteOneByte(addr_offset + i,MCU_Host_Rec.control.phone[i]);
      AT24CXX_WriteOneByte(addr_offset_backups + i,MCU_Host_Rec.control.phone[i]);
    }
    AT24CXX_WriteOneByte(addr_offset + i,(crc>>8)&0xFF);
    AT24CXX_WriteOneByte(addr_offset_backups + i++,(crc>>8)&0xFF);
    
    AT24CXX_WriteOneByte(addr_offset + i,crc&0xFF);
    AT24CXX_WriteOneByte(addr_offset_backups + i,crc&0xFF);    
		slave_rec_state = 0;
	}
	return res;
}
//=============================================================================
//��������:Execute_Temp_Comm
//���ܸ�Ҫ:ִ����λ������������
//����˵��:��
//��������:��
//ע��    :��
//=============================================================================
void  Execute_Temp_Comm(void)
{
	static u8 low_temp_alarm = 0;
	static u8 high_temp_alarm = 0;
//   		TEMP_RE485_SEND;
// 			Usart3_Control_Data.tx_count = 0;	
// 			Usart3_Control_Data.txbuf[Usart3_Control_Data.tx_count++] = 0x01;
// 			Usart3_Control_Data.txbuf[Usart3_Control_Data.tx_count++] = 0x58;
// 			Usart3_Control_Data.txbuf[Usart3_Control_Data.tx_count++] = 0X55;
//       Usart3_Control_Data.txbuf[Usart3_Control_Data.tx_count++] = 0x01;
// 			Usart3_Control_Data.txbuf[Usart3_Control_Data.tx_count++] = 0X0D;
// 			Usart3_Control_Data.txbuf[Usart3_Control_Data.tx_count++] = 0X0A;
//   		Usart3_Control_Data.tx_index = 0;	
//       USART_SendData(USART3,Usart3_Control_Data.txbuf[Usart3_Control_Data.tx_index++]);
    if(Usart3_Control_Data.rx_count != 8){
        return;
    }
    if((Usart3_Control_Data.rxbuf[0] !=0xA5)||(Usart3_Control_Data.rxbuf[1] !=0x5A)||(Usart3_Control_Data.rxbuf[7] !=0x1A)){
        return;
    }
    tempperature = (Usart3_Control_Data.rxbuf[3]*100 + Usart3_Control_Data.rxbuf[4])/10.0;
		if(tempperature > 100){
			return;
		}
		if(Mask_Low_Alarm_Time == 0){
			if(Usart3_Control_Data.rxbuf[6] == 0x03){ //���±��������ع���,����λ��ֱ���ṩ
				Sim_Send_Flag = 1;
				Sim_Send_Msg_Flag = 3;
				Mask_Low_Alarm_Time = MASK_LOW_ALARM_TIME;
				return;
			}
		/**����N�ε��£���Ҫ���Ͷ��Ÿ��ͻ���������Ҫ���ϱ���֮������ʱ�����α�����
		��������������£���һֱ��N�η�һ�ζ��ű���**/
			if(tempperature < 2.0){
				low_temp_alarm++;
				if(low_temp_alarm >= 10){
					Sim_Send_Flag = 1;
					Sim_Send_Msg_Flag = 3;
					Mask_Low_Alarm_Time = MASK_LOW_ALARM_TIME;
					return;
				}
			}else{
				low_temp_alarm = 0;
			}
		}
		/**����N�θ��£���Ҫ���Ͷ��Ÿ��ͻ�**/
		if(Mask_High_Alarm_Time == 0){
			if(tempperature > 8.0){
				high_temp_alarm++;
				if(high_temp_alarm >= 10){
					Sim_Send_Flag = 1;
					Sim_Send_Msg_Flag = 4;
					Mask_High_Alarm_Time = MASK_HIGH_ALARM_TIME;
					return;
				}
			}else{
				high_temp_alarm = 0;
			}
		}
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












