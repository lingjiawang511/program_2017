#include"HeadType.h"


COMM_Rec_Union_Type  MCU_Host_Rec;//MCU��Ϊ����ʱ�Ľṹ�����Ӧ�����
static u8 slave_rec_state;

//=============================================================================
//��������: LED_GPIO_Config
//���ܸ�Ҫ:LED����������
//��������:��
//��������:��
//ע��    :��
//=============================================================================
void RS485_GPIO_Config(void)
{	
	//����һ��GPIO_InitTypeDef ���͵Ľṹ�壬���ֽ�GPIO_InitStructure 
	GPIO_InitTypeDef  GPIO_InitStructure;
	//ʹ��GPIOC������ʱ��
	RCC_APB2PeriphClockCmd(RS485EN_RCC,ENABLE);	
	GPIO_InitStructure.GPIO_Pin = RS485EN_IO;		 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(RS485EN_PORT, &GPIO_InitStructure);
	

}

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
//	  u8 res;
//		static u8 slaveaddr = 3;
		if(Usart1_Control_Data.rx_count < 22){
				return;
		}
//		if((Usart1_Control_Data.rxbuf[2] != slaveaddr)||(Usart1_Control_Data.rxbuf[5] != Usart1_Control_Data.rx_count - 10)){
//				return ;
//		}
//		crc=CRC_GetCCITT(Usart1_Control_Data.rxbuf,Usart1_Control_Data.rx_count-4);
			crc=LRC16(&Usart1_Control_Data.rxbuf[1],Usart1_Control_Data.rx_count-5);	//�ڶ����ֽڶ�У��͵���Ч����
		if((Usart1_Control_Data.rxbuf[Usart1_Control_Data.rx_count-3]+\
			Usart1_Control_Data.rxbuf[Usart1_Control_Data.rx_count-4]*256 == crc)){	 
			for(i = 0;i < Usart1_Control_Data.rx_count;i++){
					MCU_Host_Rec.rec_buf[i] = Usart1_Control_Data.rxbuf[i];
			}//�����ݸ��Ƹ�����ͨѶ�ṹ��,������ȷ���Ȼ�Ӧ��������¼ˢдOLED״̬λ
			slave_rec_state = 1;	//�ӻ�����������ȷ
			Execute_Host_Comm();  //ִ���궯���ٻظ�PC�������Ƚ��������ǿ��Ը�PC��ȷ״̬�Ĵ�
		}
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
  u8 res;
	if(slave_rec_state == 1){//ִ���������͵�����
		if((MCU_Host_Rec.control.smg1H ==0x30)&&(MCU_Host_Rec.control.smg1L ==0x30)&&(MCU_Host_Rec.control.smg2H ==0x30)&&(MCU_Host_Rec.control.smg2L ==0x30)\
			 &&(MCU_Host_Rec.control.smg3H ==0x30)&&(MCU_Host_Rec.control.smg3L ==0x30)&&(MCU_Host_Rec.control.smg4H ==0x30)&&(MCU_Host_Rec.control.smg4L ==0x30)\
			 &&(MCU_Host_Rec.control.smg5H ==0x30)&&(MCU_Host_Rec.control.smg5L ==0x30)&&(MCU_Host_Rec.control.smg6H ==0x30)&&(MCU_Host_Rec.control.smg6L ==0x30)\
		 ){		//����
				RLED_OFF;
			}else{//����
				RLED_ON;
			}
		slave_rec_state =0;
	}
	return res;
}

void Communication_Process(void )
{
		if (1 == Usart1_Control_Data.rx_aframe){ 
			Respond_Host_Comm();
			Usart1_Control_Data.rx_count = 0;
			Usart1_Control_Data.rx_aframe = 0;
		}
}












