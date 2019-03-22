#include"HeadType.h"


COMM_Rec_Union_Type  MCU_Host_Rec;//MCU作为主机时的结构体接收应答变量
static u8 slave_rec_state;
u8 slaveaddr = 3;
static u8 openlockflag = 0x41;

u16 Read_Sensor_Time = READ_SENSOR_TIME;
u16 Check_Lock_State_Time = CHECK_LOCK_STATE_TIME;
u16 Temp_Data = 0;
u16 Humi_Data = 0;
#define  	SENSOR_ADDR				0X01
//=============================================================================
//函数名称:Respond_Host_Comm
//功能概要:响应上位机的发出的数据命令，数据已经从串口一接收完整
//参数说明:无
//函数返回:无
//注意    :无
//=============================================================================
void Communication_GPIO_Config(void)
{	
	//定义一个GPIO_InitTypeDef 类型的结构体，名字叫GPIO_InitStructure 
	GPIO_InitTypeDef  GPIO_InitStructure;
	//使能GPIOC的外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	//选择要用的GPIO引脚		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	///设置引脚模式为推免输出模式			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	//设置引脚速度为50MHZ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	//调用库函数，初始化GPIO
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
			}//把数据复制给主机通讯结构体,数据正确，先回应主机，记录刷写OLED状态位
			slave_rec_state = 1;	//从机接收数据正确
			
		 while(Usart1_Control_Data.tx_count!=0);
			
		 if(MCU_Host_Rec.control.comm == 0x41){//'A'
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
			}else if(MCU_Host_Rec.control.comm == 0x4C){ //'L'一次性读取32个锁的状态
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
			}else if(MCU_Host_Rec.control.comm == 0x4D){//‘M’一次只读取一个锁的状态
					
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
			}else if(MCU_Host_Rec.control.comm == 0x48){//‘H’读取温湿度
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
			}else if(MCU_Host_Rec.control.comm == 0x53){//下位机回复主动发送锁状态，默认不处理
				
				Usart1_Control_Data.rx_aframe = 0;	//清空和主机的通讯，避免通讯错误
				Usart1_Control_Data.rx_count = 0;	
				return;
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
		Usart2_Control_Data.rx_aframe = 0;	//清空和主机的通讯，避免通讯错误
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












