#include"HeadType.h"


COMM_Rec_Union_Type  MCU_Host_Rec;//MCU��Ϊ����ʱ�Ľṹ�����Ӧ�����
static u8 slave_rec_state;
u8 slaveaddr = 3;
static u8 openlockflag = 0x41;
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
			}//�����ݸ��Ƹ�����ͨѶ�ṹ��,������ȷ���Ȼ�Ӧ��������¼ˢдOLED״̬λ
				slave_rec_state = 1;	//�ӻ�����������ȷ
		 if(MCU_Host_Rec.control.comm == 0x41){
				res = Execute_Host_Comm();  //�����ĸ�������
				if(res != 0){ //�����豸ӵ�е��������糬����Χ��ֱ�ӷ���
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
		}else{	//LRC����
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
      //У����󲻷���
      Usart1_Control_Data.tx_count = 0;	
      Usart1_Control_Data.tx_index = 0;
      Usart1_Control_Data.rx_aframe = 0;	//��պ�������ͨѶ������ͨѶ����
      Usart1_Control_Data.rx_count = 0;	
      return;
		}
		Usart1_Control_Data.tx_index = 0;	
		USART_SendData(USART1,Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_index++]);
		Usart1_Control_Data.rx_aframe = 0;	//��պ�������ͨѶ������ͨѶ����
		Usart1_Control_Data.rx_count = 0;	
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
  u8 res = 0;
	if(slave_rec_state == 1){//ִ���������͵�����
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












