#include"HeadType.h"


COMM_Rec_Union_Type  MCU_Host_Rec;//MCU��Ϊ����ʱ�Ľṹ�����Ӧ�����
static u8 slave_rec_state;
u8 slaveaddr = 3;
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
		if(Usart1_Control_Data.rx_count < 8){
				return;
		}
		if((Usart1_Control_Data.rxbuf[2] != slaveaddr)||(Usart1_Control_Data.rxbuf[5] != Usart1_Control_Data.rx_count - 10)){
				return ;
		}
		crc=CRC_GetCCITT(Usart1_Control_Data.rxbuf,Usart1_Control_Data.rx_count-4);
		if((Usart1_Control_Data.rxbuf[Usart1_Control_Data.rx_count-3]+\
			Usart1_Control_Data.rxbuf[Usart1_Control_Data.rx_count-4]*256 == crc)){	 
			for(i = 0;i < Usart1_Control_Data.rx_count;i++){
					MCU_Host_Rec.rec_buf[i] = Usart1_Control_Data.rxbuf[i];
			}//�����ݸ��Ƹ�����ͨѶ�ṹ��,������ȷ���Ȼ�Ӧ��������¼ˢдOLED״̬λ
			if(AdrrOK_Flag == 1){//������ַ�����ȷ���޸ĵ�ַ������ȷ�������Ļ��ʾ�豸OK������һ��ͨѶ��ȷ��Ϩ����Ļ��ʾ��λ����Ϣ
				clear_screen(0x00);
				AdrrOK_Flag = 0;
			}
			slave_rec_state = 1;	//�ӻ�����������ȷ
			res = Execute_Host_Comm();  //ִ���궯���ٻظ�PC�������Ƚ��������ǿ��Ը�PC��ȷ״̬�Ĵ�		
			RE485_SEND;
			Usart1_Control_Data.tx_count = 0;	
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0x01;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0x58;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = MCU_Host_Rec.control.addr;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = MCU_Host_Rec.control.funcode;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0x00;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0x02;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = res;	//���ݽ�����ȷ
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0x00;
			crc=CRC_GetCCITT(Usart1_Control_Data.txbuf,Usart1_Control_Data.tx_count);
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = (crc>>8)&0xFF; 
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = crc&0xFF;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0X0D;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0X0A;
		}else{	//CRC����
			RE485_SEND;
			Usart1_Control_Data.tx_count = 0;	
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0x01;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0x58;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = MCU_Host_Rec.control.addr;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = MCU_Host_Rec.control.funcode;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0x00;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0x02;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0x00;	//���ݽ�����ȷ
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0x00;
			crc=CRC_GetCCITT(Usart1_Control_Data.txbuf,Usart1_Control_Data.tx_count);
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = (crc>>8)&0xFF; 
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = crc&0xFF;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0X0D;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0X0A;
		}
		Usart1_Control_Data.tx_index = 0;	
		USART_SendData(USART1,Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_index++]);
		Usart1_Control_Data.rx_aframe = 0;	//��պ�������ͨѶ������ͨѶ����
		Usart1_Control_Data.rx_count = 0;	
}

//x�Ǻ����꣬��128���㣬��д16���ַ���y�������꣬��64���㣬��д4���ַ�
static  u8 func01(u8 x,u8 y,u8 length,u8 *p)
{//��ַ����һ���ַ���ֻ��ʾ�����ַ�����ʾ�Ĳ��֣��������ֺ���
	u8 i,tempx,tempy,templength;
	u8 res;
	x=x*8;	//�����У�����ֻ����ʾ4��
	y=y*2; //�����У�����ֻ����ʾ8��
	tempy = y;
	templength = 0;
	if(x+length*8 > 128)
	{
		tempx =length*8 + x -128 ;
		templength = tempx/8;
		if(tempx%8 != 0){
			templength++;
		}
		tempy += 2;
		res = 1;
		return res; //������ַ��Χ����ʾ
	}
	for(i=0;i<length - templength;i++)
	{
		if(*p == 0x00){

		}else{
			LCD_P8x16Showchar(x+i*8,y,*p);
		}
		p++;
	}
	res = 0;
	return res;
}


static  u8 func02(u8 x,u8 y,u8 length,u8 *text)
{
	uchar i,tempx,tempy,templength;
	uchar addrHigh,addrMid,addrLow ;
	uchar fontbuf[32];
	ulong  fontaddr=0;
	u8 res;
	x=x*16;	//�����У�����ֻ����ʾ4��
	y=y*2; //�����У�����ֻ����ʾ8��
	tempy = y;
	templength = 0;
	if(x+length*8 > 128)
	{
		tempx =length*8 + x -128 ;
		templength = tempx/8;
		if(tempx%8 != 0){
			templength++;
		}
		if(templength %2 != 0){
			templength++;	//������16���ַ�����ȥ�ĳ���Ҫ��2�ı���
		}
		tempy += 2;
		res = 1;
		return res; //������ַ��Χ����ʾ
	}
	for(i=0;i<(length - templength)/2;i++){
			/*������壨GB2312�������ھ���Ѷ�ֿ�IC�еĵ�ַ�����¹�ʽ�����㣺*/
		/*Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32+ BaseAdd;BaseAdd=0*/
		/*���ڵ���8λ��Ƭ���г˷�������⣬���Է�����ȡ��ַ*/
		if((text[i*2] == 0x00)&&(text[i*2+1] == 0x00)){

		}else{
			fontaddr = (text[i*2]- 0xb0)*94; 
			fontaddr += (text[i*2+1]-0xa1)+846;
			fontaddr = (ulong)(fontaddr*32);
			
			addrHigh = (fontaddr&0xff0000)>>16;  /*��ַ�ĸ�8λ,��24λ*/
			addrMid = (fontaddr&0xff00)>>8;      /*��ַ����8λ,��24λ*/
			addrLow = fontaddr&0xff;	     /*��ַ�ĵ�8λ,��24λ*/
			get_n_bytes_data_from_ROM(addrHigh,addrMid,addrLow,fontbuf,32 );/*ȡ32���ֽڵ����ݣ��浽"fontbuf[32]"*/
			display_graphic_16x16(y,x,fontbuf);/*��ʾ���ֵ�LCD�ϣ�yΪҳ��ַ��xΪ�е�ַ��fontbuf[]Ϊ����*/
		}
		x+=16;
	}
	res = 0;
	return res;
}
//�����������ݱ���ʹ��A3+"�ַ�"����ģʽ
static  u8 func03(u8 x,u8 y,u8 length,u8 *text)
{
		uchar i,tempx,tempy,templength;
		uchar addrHigh,addrMid,addrLow ;
		uchar fontbuf[32];
	  u8 texttemp[16];
		ulong  fontaddr=0;
		u8 res;
		x=x*16;	//�����У�����ֻ����ʾ4��
	  y=y*2; //�����У�����ֻ����ʾ8��
		tempy = y;
		templength = 0;
		length = length*2;
		if(x+length*8 > 128)
		{
			tempx =length*8 + x -128 ;
			templength = tempx/8;
			if(tempx%8 != 0){
				templength++;
			}
			if(templength %2 != 0){
				templength++;	//������16���ַ�����ȥ�ĳ���Ҫ��2�ı���
			}
			tempy += 2;
		res = 1;
		return res; //������ַ��Χ����ʾ
		}
		for(i=0;i<length;i++)
		{
			  if(text[i] == 0x00){
					texttemp[i*2] = 0x00;
					texttemp[i*2+1] = 0x00;
				}else{
					texttemp[i*2] = 0xA3;
					texttemp[i*2+1] = text[i]- 0x20 + 0xA0;
				}
		}
		for(i=0;i<(length - templength);i=i+2){
			/*������壨GB2312��15x16����ַ��ھ���Ѷ�ֿ�IC�еĵ�ַ�����¹�ʽ�����㣺*/
			/*Address = ((MSB - 0xa1) * 94 + (LSB - 0xA1))*32+ BaseAdd;BaseAdd=0*/
			/*���ڵ���8λ��Ƭ���г˷�������⣬���Է�����ȡ��ַ*/
//			text[i] = 0XA3;
//			text[i+1] = text[i+1] - 0x20 + 0xA0;
			if((texttemp[i] == 0x00)&&(texttemp[i+1] == 0x00)){
			
			}else{
				fontaddr = (texttemp[i]- 0xa1)*94; 
				fontaddr += (texttemp[i+1]-0xa1);
				fontaddr = (ulong)(fontaddr*32);
				
				addrHigh = (fontaddr&0xff0000)>>16;  /*��ַ�ĸ�8λ,��24λ*/
				addrMid = (fontaddr&0xff00)>>8;      /*��ַ����8λ,��24λ*/
				addrLow = fontaddr&0xff;	     /*��ַ�ĵ�8λ,��24λ*/
				get_n_bytes_data_from_ROM(addrHigh,addrMid,addrLow,fontbuf,32 );/*ȡ32���ֽڵ����ݣ��浽"fontbuf[32]"*/
				display_graphic_16x16(y,x,fontbuf);/*��ʾ���ֵ�LCD�ϣ�yΪҳ��ַ��xΪ�е�ַ��fontbuf[]Ϊ����*/
			}
			x+=16;
		}
	res = 0;
	return res;
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
		switch(MCU_Host_Rec.control.funcode){
		case 0x01:
			res = func01(MCU_Host_Rec.control.x,MCU_Host_Rec.control.y,(u8)(MCU_Host_Rec.control.datasizeL-2),&MCU_Host_Rec.control.recbuf[0]);
		break;
		case 0x02:
			res = func02(MCU_Host_Rec.control.x,MCU_Host_Rec.control.y,(u8)(MCU_Host_Rec.control.datasizeL-2),&MCU_Host_Rec.control.recbuf[0]);
		break;
		case 0x03:
			res = func03(MCU_Host_Rec.control.x,MCU_Host_Rec.control.y,(u8)(MCU_Host_Rec.control.datasizeL-2),&MCU_Host_Rec.control.recbuf[0]);
		break;
		case 0x04:
		  display_bmp(MCU_Host_Rec.control.x,MCU_Host_Rec.control.y,MCU_Host_Rec.control.recbuf[0],MCU_Host_Rec.control.recbuf[1],bmp2);
		  //display_bmp(MCU_Host_Rec.control.x,MCU_Host_Rec.control.y,54,16,bmp3);
		  res = 1;
		break;
		case 0x05:break;
		case 0x06:
					clear_screen(0XFF);    //clear all dots 
					AdrrOK_Flag = 1; //��λ�������൱�ڵ�ַ��־λ��ȷ��������Ļ��ʾ���´�ͨѶʱ����ʾ��Ӧ��Ϣ
					res = 1;
					break;
		case 0x07:res = 2;break;			
		default: res = 2;break;
		}
		slave_rec_state = 0;
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
//		if(Key_ScanNum == 0x01){
//		func02(0,0,16,"Τ�ֺ���ҽҩ�豸");
//			Key_ScanNum = 0;
//		}else if(Key_ScanNum == 0x11){
//			func02(0,1,16,"Τ�ֺ���ҽҩ�豸");
//			Key_ScanNum = 0;
//		}else if(Key_ScanNum == 0xff){
//			func02(0,2,16,"Τ�ֺ���ҽҩ�豸");
//			func02(0,3,16,"Τ�ֺ���ҽҩ�豸");
//			Key_ScanNum = 0;
//		}
//		delay_ms(1000);
//		clear_screen();
//		delay_ms(1000);
}












