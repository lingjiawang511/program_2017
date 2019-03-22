#include"HeadType.h"


COMM_Rec_Union_Type  MCU_Host_Rec;//MCU��Ϊ����ʱ�Ľṹ�����Ӧ�����
static u8 slave_rec_state;
u8 slaveaddr = 3;
static u8 openlockflag = 0x41;

u16 Read_Sensor_Time = READ_SENSOR_TIME;
u16 Check_Lock_State_Time = CHECK_LOCK_STATE_TIME;
u16 Temp_Data = 0;
u16 Humi_Data = 0;
#define  	SENSOR_ADDR				0X01
//=============================================================================
//��������:Respond_Host_Comm
//���ܸ�Ҫ:��Ӧ��λ���ķ�����������������Ѿ��Ӵ���һ��������
//����˵��:��
//��������:��
//ע��    :��
//=============================================================================
void Communication_GPIO_Config(void)
{	
	//����һ��GPIO_InitTypeDef ���͵Ľṹ�壬���ֽ�GPIO_InitStructure 
	GPIO_InitTypeDef  GPIO_InitStructure;
	//ʹ��GPIOC������ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	//ѡ��Ҫ�õ�GPIO����		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	///��������ģʽΪ�������ģʽ			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	//���������ٶ�Ϊ50MHZ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	//���ÿ⺯������ʼ��GPIO
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	SENSOR_RE485_REC;

}
static void Respond_Host_Comm(void)
{
		u8 i;
		u16 lrc;
	  u8 res,detection_lock,lock_state;
		if(Usart1_Control_Data.rx_count < 8){
				return;
		}
// 		if(Usart1_Control_Data.rxbuf[1] != openlockflag){
// 				return ;
// 		}
		lrc=LRC_GetLRC16(&Usart1_Control_Data.rxbuf[1],Usart1_Control_Data.rx_count-5);
		if((Usart1_Control_Data.rxbuf[Usart1_Control_Data.rx_count-3]+\
			Usart1_Control_Data.rxbuf[Usart1_Control_Data.rx_count-4]*256 == lrc)){	 
			if(Usart1_Control_Data.rx_count >9){
				Usart1_Control_Data.rx_count = 9;
			}
			for(i = 0;i < Usart1_Control_Data.rx_count;i++){
					MCU_Host_Rec.rec_buf[i] = Usart1_Control_Data.rxbuf[i];
			}//�����ݸ��Ƹ�����ͨѶ�ṹ��,������ȷ���Ȼ�Ӧ��������¼ˢдOLED״̬λ
			slave_rec_state = 1;	//�ӻ�����������ȷ
			
		 while(Usart1_Control_Data.tx_count!=0);
			
		 if(MCU_Host_Rec.control.comm == 0x41){//'A'
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
			}else if(MCU_Host_Rec.control.comm == 0x4C){ //'L'һ���Զ�ȡ32������״̬
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
			}else if(MCU_Host_Rec.control.comm == 0x4D){//��M��һ��ֻ��ȡһ������״̬
					
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
			}else if(MCU_Host_Rec.control.comm == 0x48){//��H����ȡ��ʪ��
				RE485_SEND;
				Usart1_Control_Data.tx_count = 0;	
				Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = MCU_Host_Rec.control.frame_start;
				Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = MCU_Host_Rec.control.comm;
				Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = MCU_Host_Rec.control.addr;
				Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = Temp_Data >>8;
				Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = Temp_Data;
				Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = Humi_Data >>8;
				Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = Humi_Data;
				lrc=LRC_GetLRC16(&Usart1_Control_Data.txbuf[1],Usart1_Control_Data.tx_count - 1);
				Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = (lrc>>8)&0xFF; 
				Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = lrc&0xFF;
				Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0X0D;
				Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0X09;
			}else if(MCU_Host_Rec.control.comm == 0x53){//��λ���ظ�����������״̬��Ĭ�ϲ�����
				
				Usart1_Control_Data.rx_aframe = 0;	//��պ�������ͨѶ������ͨѶ����
				Usart1_Control_Data.rx_count = 0;	
				return;
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
static void send_data_to_read_sensor(void)
{
		u16 crc16;
		SENSOR_RE485_SEND;
		Usart2_Control_Data.tx_count = 0;	
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = SENSOR_ADDR;
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0x03;
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0x00;
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0x00;
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0x00;
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0x02;
		crc16=CRC_GetModbus16(&Usart2_Control_Data.txbuf[0],Usart2_Control_Data.tx_count);
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = crc16&0xFF;
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = (crc16>>8)&0xFF; 
		Usart2_Control_Data.tx_index = 0;	
		USART_SendData(USART2,Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_index++]);
		Usart2_Control_Data.rx_aframe = 0;	//��պ�������ͨѶ������ͨѶ����
		Usart2_Control_Data.rx_count = 0;	
}
void Dispose_Humiture_Sensor_Data(void)
{
		u16 crc16;
		if(Usart2_Control_Data.rxbuf[0] != SENSOR_ADDR){
				return ;
		}
		if((Usart2_Control_Data.rxbuf[1] != 0X03)||(Usart2_Control_Data.rxbuf[2] != 0X04)){
				return ;
		}
		crc16=CRC_GetModbus16(&Usart2_Control_Data.rxbuf[0],Usart2_Control_Data.rx_count-2);
		if((Usart2_Control_Data.rxbuf[Usart2_Control_Data.rx_count-2]+\
			Usart2_Control_Data.rxbuf[Usart2_Control_Data.rx_count-1]*256 == crc16)){	 
				Temp_Data =(Usart2_Control_Data.rxbuf[3]<<8) + Usart2_Control_Data.rxbuf[4];
				Humi_Data =(Usart2_Control_Data.rxbuf[5]<<8) + Usart2_Control_Data.rxbuf[6];	
		}
		return;
}
void Communication_Process(void )
{
		if ((1 == Usart1_Control_Data.rx_aframe)&&(open_lock_count == 0)){ 
			Respond_Host_Comm();
			Usart1_Control_Data.rx_count = 0;
			Usart1_Control_Data.rx_aframe = 0;
		}
		if (1 == Usart2_Control_Data.rx_aframe){ 
			Dispose_Humiture_Sensor_Data();
			Usart2_Control_Data.rx_count = 0;
			Usart2_Control_Data.rx_aframe = 0;
		}
		if(Read_Sensor_Time == 0){
				send_data_to_read_sensor();
				Read_Sensor_Time = READ_SENSOR_TIME;
		}
		
}












