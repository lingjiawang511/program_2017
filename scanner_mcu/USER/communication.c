#include"HeadType.h"

Host_COMM_Union_Type Host_Control;
Host_Answer_Type 	 Host_Answer;

//=============================================================================
//��������: Comm_GPIO_Config
//���ܸ�Ҫ:Ӳ��ͨ����������
//��������:��
//��������:��
//ע��    :��
//=============================================================================
 void Communication_GPIO_Config(void)
{	
	//����һ��GPIO_InitTypeDef ���͵Ľṹ�壬���ֽ�GPIO_InitStructure 
	GPIO_InitTypeDef  GPIO_InitStructure;
	//COMMUNICATION_IO1  IO����,�豸״̬���
	RCC_APB2PeriphClockCmd(COMMUNICATION_IO1_RCC,ENABLE);		
	GPIO_InitStructure.GPIO_Pin = COMMUNICATION_IO1_IO;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(COMMUNICATION_IO1_PORT, &GPIO_InitStructure);
	//COMMUNICATION_IO2  IO����
	RCC_APB2PeriphClockCmd(COMMUNICATION_IO2_RCC,ENABLE);		
	GPIO_InitStructure.GPIO_Pin = COMMUNICATION_IO2_IO;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(COMMUNICATION_IO2_PORT, &GPIO_InitStructure);
	//COMMUNICATION_IO3  IO����
	RCC_APB2PeriphClockCmd(COMMUNICATION_IO3_RCC,ENABLE);		
	GPIO_InitStructure.GPIO_Pin = COMMUNICATION_IO3_IO;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(COMMUNICATION_IO3_PORT, &GPIO_InitStructure);

}
void Comm_With_Device_State(void)
{
	static u8 delaytime1 = 0;
	static u8 delaytime2 = 0;
   if(Device_State == 0){
		if(READ_COMMUNICATION_IO1 == 0){
				delaytime1++;
			if(delaytime1 >= 10){
				if(READ_COMMUNICATION_IO1 == 0){
					Device_State = 1;
				}else{
					Device_State = 0;
				}
				delaytime1 = 0;
			}
		}else{
			 Device_State = 0;
			 delaytime1 = 0;
		}
	}else{
			if(READ_COMMUNICATION_IO1 == 1){
				delaytime1++;
				if(delaytime1 >= 10){
					if(READ_COMMUNICATION_IO1 == 1){
						Device_State = 0;
					}else{
						Device_State = 1;
					}
					delaytime1 = 0;
				}
			}else{
					delaytime1 = 0;
			}
	}
//test program
	if(Control.scanner.state == 0){
		if(READ_COMMUNICATION_IO2 == 0){
				delaytime2++;
			if(delaytime2 >= 10){
				if(READ_COMMUNICATION_IO2 == 0){
					Control.scanner.state = 1;
				}else{
					Control.scanner.state = 0;
				}
				delaytime2 = 0;
			}
		}else{
			 delaytime2 = 0;
		}
	}
}
//=============================================================================
//��������:Execute_Host_Comm
//���ܸ�Ҫ:ִ����λ������������
//����˵��:��
//��������:��
//ע��    :��
//=============================================================================
static u8 Execute_Host_Comm(void)
{
	u8 i;
	u16 crc;
	if(Usart1_Control_Data.rx_count != 16){
		return 0;
	}
	crc=CRC_GetCCITT(Usart1_Control_Data.rxbuf,Usart1_Control_Data.rx_count-2);
	if((Usart1_Control_Data.rxbuf[Usart1_Control_Data.rx_count-1]+\
 	    Usart1_Control_Data.rxbuf[Usart1_Control_Data.rx_count-2]*256 == crc)){	    
		for(i = 0;i < 16;i++){
            Host_Control.host_buf[i] = Usart1_Control_Data.rxbuf[i];
        }//�����ݸ��Ƹ�����ͨѶ�ṹ��
		switch(Host_Control.control.beep_state){
		case 0x00:	 
					 break;
		case 0x01: 	 Host_Control.control.beep_state = 0;
					 break;
		case 0x02: 	
					 break;
		default : break;
		}
		switch(Host_Control.control.gled_state){
		case 0x00: 
					break;
		case 0x01: 	

					break;
		case 0x02: 	
					break;
	    case 0x03:
					break;
		default : break;
		}
		switch(Host_Control.control.rled_state){
		case 0x00: 
					break;
		case 0x01: 	
					break;
		case 0x02: 	
					break;
	    case 0x03: 	
					break;
		default : break;
		}
		switch(Host_Control.control.scanner_state){
		case 0x00: 
					break;
		case 0x01: 	Host_Answer.answer_state = Host_Control.control.scanner_state;
					Host_Control.control.scanner_state = 0;
					break;
		case 0x02: 	Host_Answer.answer_state = Host_Control.control.scanner_state;
					Host_Control.control.scanner_state = 0;
					break;
		default : break;
		}
		return 1;	
	}else{
		 return 0;
	}
}
//=============================================================================
//��������:response_reader
//���ܸ�Ҫ:��Ӧɨ��ͷ���ݰ�����
//����˵��:��
//��������:����1��������У����ȷ������0��������У�����
//ע��    :��ɨ��ͷ�����ݣ�ʹ��CRC14У�鲢��ֻУ������λ����У������ͷ
//=============================================================================
#if SCAN_CR8000
static u8 response_reader(void)
{	   	
	u16 crc;
	if(Usart2_Control_Data.rx_count < 16){
		return 0;
	}
	crc=CRC_GetCCITT(Usart2_Control_Data.rxbuf,Usart2_Control_Data.rx_count-2);
	if((Usart2_Control_Data.rxbuf[Usart2_Control_Data.rx_count-1]+\
 	    Usart2_Control_Data.rxbuf[Usart2_Control_Data.rx_count-2]*256 == crc)){
		/**CRCУ��OK����ӦREADER�ź� 'Y'**/
		Usart2_Control_Data.tx_count = 0;
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0xEE;
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0xEE;
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0xEE;
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0xEE;
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 'Y' ;
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0x01;  //data size 
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = Usart2_Control_Data.rxbuf[8];  
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0x00;  //data end
		crc=CRC_GetCCITT(&Usart2_Control_Data.txbuf[4],Usart2_Control_Data.tx_count-4);
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = (crc>>8)&0x7F; //CRC14
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = crc&0x7F;
		/**��������**/
//			Beep_Num = 5;
//			Gled_Num = 5;
//			Rled_Num = 5;
//			Host_Answer.answer_state = 1;
//			Host_Answer.answer_timeout = ANSWER_SCANTIME;
		Host_Answer.answer_state = 1; //ɨ��ͷ���յ���ȷ���ݺ�Ͱ�״̬ת��
		Host_Answer.start_timeout =2; //���Ϳ�ʼ����λ�����ճ�ʱʱ��
		Host_Answer.Nanswer_timeout = NANSWER_SCANTIME;
		Host_Answer.answer_timeout = ANSWER_SCANTIME;
		Usart2_Control_Data.tx_index = 0;
		USART_SendData(USART2,Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_index++]);
		return 1;
	}else{
		 /**CRCУ��NOK����ӦREADER�ź� 'R'**/
		Usart2_Control_Data.tx_count = 0;
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0xEE;
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0xEE;
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0xEE;
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0xEE;
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 'R' ;
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0x00;  //data size less than 32
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0x05;  //the Reader will use its preferred maximum packet size
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = 0x00;  //data end
		crc=CRC_GetCCITT(&Usart2_Control_Data.txbuf[4],Usart2_Control_Data.tx_count);
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = (crc>>8)&0x7F;	//CRC14
		Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_count++] = crc&0x7F;
		/**��������**/
		Usart2_Control_Data.tx_index = 0;
		USART_SendData(USART2,Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_index++]);
	
		return 0;
	}	
}
#else
static u8 response_reader(void)
{	   	
	if(Usart2_Control_Data.rx_count < 3){
		return 0;
	}else{
		Host_Answer.answer_state = 1; //ɨ��ͷ���յ���ȷ���ݺ�Ͱ�״̬ת��
		Host_Answer.start_timeout =2; //���Ϳ�ʼ����λ�����ճ�ʱʱ��
		Host_Answer.Nanswer_timeout = NANSWER_SCANTIME;
		Host_Answer.answer_timeout = ANSWER_SCANTIME;
		return 1;
   	}
}
#endif
//=============================================================================
//��������:Copy_Scanner_Data
//���ܸ�Ҫ:��ɨ�����õ������ݸ��Ƶ�����һ
//����˵��:��
//��������:��
//ע��    :��
//=============================================================================
#if SCAN_CR8000
static void  Copy_Scanner_Data(void)
{
    u8 i;
    for(i = 0;i < Usart2_Control_Data.rx_count;i++){
        Usart1_Control_Data.txbuf[i] = Usart2_Control_Data.rxbuf[i];
    }
    Usart1_Control_Data.tx_count = Usart2_Control_Data.rx_count;

    Usart2_Control_Data.rx_count = 0;
    Auto_Frame_Time2 = AUTO_FRAME_TIMEOUT2;
    
    Usart1_Control_Data.rx_index = 0;
    Usart1_Control_Data.tx_index = 0;
    USART_SendData(USART1,Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_index++]);
//        return Usart2_Control_Data.rx_count;
}
#else

static void  Copy_Scanner_Data(void)
{
  u8 i,j;
	u16 crc;
	j=0;
	Usart1_Control_Data.txbuf[j++] = 0x01;	
	Usart1_Control_Data.txbuf[j++] = 0x58;
	Usart1_Control_Data.txbuf[j++] = 0x52;	
	Usart1_Control_Data.txbuf[j++] = 0x31;
	Usart1_Control_Data.txbuf[j++] = 0x35;	
	Usart1_Control_Data.txbuf[j++] = 0x36;
	Usart1_Control_Data.txbuf[j++] = 0x37;	
	Usart1_Control_Data.txbuf[j++] = 0x38;
	Usart1_Control_Data.txbuf[j++] = 0x01;	
	Usart1_Control_Data.txbuf[j++] = 0xFF;
	Usart1_Control_Data.txbuf[j++] = 0xFF;	
	Usart1_Control_Data.txbuf[j++] = 0xFF;
	Usart1_Control_Data.txbuf[j++] = 0xFF;
	Usart1_Control_Data.txbuf[j++] = Usart2_Control_Data.rx_count >>8;	
	Usart1_Control_Data.txbuf[j++] = Usart2_Control_Data.rx_count;
    for(i = 0;i < Usart2_Control_Data.rx_count;i++){
        Usart1_Control_Data.txbuf[i+j] = Usart2_Control_Data.rxbuf[i];
    }
  Usart1_Control_Data.tx_count = Usart2_Control_Data.rx_count + j;
	crc=CRC_GetCCITT(Usart1_Control_Data.txbuf,Usart1_Control_Data.tx_count);
	Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = crc >> 8;
  Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = crc;

	Usart2_Control_Data.rx_count = 0;
  Auto_Frame_Time2 = AUTO_FRAME_TIMEOUT2;
    
  Usart1_Control_Data.rx_index = 0;
  Usart1_Control_Data.tx_index = 0;
  USART_SendData(USART1,Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_index++]);
//        return Usart2_Control_Data.rx_count;
}

#endif

//=============================================================================
//��������:Dispose_Data_For_Host
//���ܸ�Ҫ:��������ɨ���������������͵���λ��
//����˵��:��
//��������:��
//ע��    :������ȷ���ϴ�����λ��
//=============================================================================
void Dispose_Data_For_Host(void)
{

	if (1 == Usart2_Control_Data.rx_aframe){    
   		if(response_reader()){ 
				Baffle_Control.bag_input_flag = 0;	 //ɨ��ǹ�յ���ά�룬Һ���źű�־����			
   			Copy_Scanner_Data();
		}else{
		//no use 	
		}
		Usart2_Control_Data.rx_aframe = 0;
   }
}
//=============================================================================
//��������:Respond_Host_Comm
//���ܸ�Ҫ:��Ӧ��λ���ķ�����������������Ѿ��Ӵ���һ��������
//����˵��:��
//��������:��
//ע��    :��
//=============================================================================
void Respond_Host_Comm(void)
{
//    u8 i=0;
    if (1 == Usart1_Control_Data.rx_aframe){    
        if(Execute_Host_Comm()){
//	        /*���Գ��򣬰ѵ��Է����������ݻط���ȥ*/
//	        for(i = 0;i < Usart1_Control_Data.rx_count;i++){
//	            Usart1_Control_Data.txbuf[i] = Usart1_Control_Data.rxbuf[i];
//	        }
//	        Usart1_Control_Data.tx_count =Usart1_Control_Data.rx_count;
//	        Usart1_Control_Data.tx_index = 0;
//	        USART_SendData(USART1,Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_index++]);
//	        /*���Գ��򣬰ѵ��Է����������ݻط���ȥ*/
			Usart1_Control_Data.rx_count = 0;
			Auto_Frame_Time1 = AUTO_FRAME_TIMEOUT1;
			Host_Answer.start_timeout = 1;
			Host_Answer.answer_timeout = ANSWER_SCANTIME;
			Baffle_Control.bag_ok_flag = 1;
		}
	    Usart1_Control_Data.rx_aframe = 0;
    }else{
        return;
    }
}

//=============================================================================
//��������:scanner_scan
//���ܸ�Ҫ:ѭ��ɨ��ʱ�䶨ʱ���ڶ�ʱ������
//��������:��
//��������:��
//ע��    :��ʱ��ɨ��ʱ��Ϊ5ms,����ʵ��ɨ��ͷɨ��ʱ��ΪN*5*100
//=============================================================================
void scanner_scan(void)
{	
	static u16 scan_time = DEFAULT_SCANTIME;					//Ĭ��ɨ������ʱ��
	static u16 scan_time_old = DEFAULT_SCANTIME;

	if(Host_Answer.answer_state == 2){//��һʱ������Ӧ�����޸�,��λ������ɨ��ͷɨ��ʱ��
		if(Host_Control.control.scanner_time <0X02){
			Host_Control.control.scanner_time = 0X02;
		}
		scan_time_old =Host_Control.control.scanner_time * 100;
		scan_time =	scan_time_old;
		Host_Answer.answer_state = 1;//�޸�֮���Ϊ������Ӧ״̬
		Host_Answer.answer_timeout = ANSWER_SCANTIME;
	}
	if(Host_Answer.answer_state == 1){//������Ӧ�ˣ��Ͳ��Թ̶���ɨ������ɨ���ˣ�ɨ�赽��ά�룬һֱɨ�赽����ֵ����
		if(Host_Answer.answer_timeout <=0){
			Host_Answer.answer_state = 0;
			Host_Answer.start_timeout = 0;
		}
		if(Host_Answer.start_timeout ==1){
		    if(Host_Answer.answer_timeout == ANSWER_SCANTIME){

			 }
		 Host_Answer.answer_timeout--;	//�˴��൱�ڲ��������һֱΪ��
		 }else if(Host_Answer.start_timeout ==2){		 	 	
				if(Host_Answer.Nanswer_timeout <=0){//��λ����ʱû�л�Ӧ����������
					Rled_Num = 5;	//������
					Host_Answer.answer_state = 0;

					Host_Control.control.gled_state = 0;
					Gled_Light_Static = 0;
					GLED_OFF;
					Host_Control.control.rled_state = 0;
					Rled_Light_Static = 0;
				}
				Host_Answer.Nanswer_timeout--;
		 }
		return;
	}
	if(scan_time >0){
		scan_time--;
	}
}


void  Update_Err_Scanner_Data(void)
{
  u8 i,j;
	u16 crc;
	j=0;
	Usart1_Control_Data.txbuf[j++] = 0x01;	
	Usart1_Control_Data.txbuf[j++] = 0x58;
	Usart1_Control_Data.txbuf[j++] = 0x52;	
	Usart1_Control_Data.txbuf[j++] = 0x31;
	Usart1_Control_Data.txbuf[j++] = 0x35;	
	Usart1_Control_Data.txbuf[j++] = 0x36;
	Usart1_Control_Data.txbuf[j++] = 0x37;	
	Usart1_Control_Data.txbuf[j++] = 0x38;
	Usart1_Control_Data.txbuf[j++] = 0x01;	
	Usart1_Control_Data.txbuf[j++] = 0xFF;
	Usart1_Control_Data.txbuf[j++] = 0xFF;	
	Usart1_Control_Data.txbuf[j++] = 0xFF;
	Usart1_Control_Data.txbuf[j++] = 0xFF;
	Usart1_Control_Data.txbuf[j++] = 0;	
	Usart1_Control_Data.txbuf[j++] = 8;
    for(i = 0;i < 8;i++){
        Usart1_Control_Data.txbuf[i+j] = 0xFF;
    }
  Usart1_Control_Data.tx_count = Usart2_Control_Data.rx_count + j;
	crc=CRC_GetCCITT(Usart1_Control_Data.txbuf,Usart1_Control_Data.tx_count);
	Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = crc >> 8;
  Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = crc;

  Auto_Frame_Time2 = AUTO_FRAME_TIMEOUT2;
    
  Usart1_Control_Data.rx_index = 0;
  Usart1_Control_Data.tx_index = 0;
  USART_SendData(USART1,Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_index++]);
//        return Usart2_Control_Data.rx_count;
}


