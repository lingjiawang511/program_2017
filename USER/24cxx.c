#include "24cxx.h" 
#include"HeadType.h"	
// #include "delay.h"
// #include "sys.h"

void delay_nus(u32 nus)
{		
    int i;
	while(nus--){
		for(i =0 ;i<2;i++);
	} 
}
void delay_nms(u32 nms)
{
   while(nms--){
    delay_nus(1000);
    } 
}
/****************************************************************************
* Function Name  : IIC_Config
* Description    : ��ʼ��GPIO.
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/
void I2C_INIT()
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	
//     RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//     PWR_WakeUpPinCmd(DISABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��
   // GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    
	GPIO_InitStructure.GPIO_Pin=I2C_SCL|I2C_SDA;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIO_I2C,&GPIO_InitStructure);

	I2C_SCL_H;
	I2C_SDA_H;
}

/*******************************************************************************
* �� �� ��         : I2C_SDA_OUT
* ��������		   : SDA�������	   
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void I2C_SDA_OUT()
{
    GPIO_InitTypeDef GPIO_InitStructure;	
	
	GPIO_InitStructure.GPIO_Pin=I2C_SDA;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIO_I2C,&GPIO_InitStructure);
}

/*******************************************************************************
* �� �� ��         : I2C_SDA_IN
* ��������		   : SDA��������	   
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void I2C_SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	
	GPIO_InitStructure.GPIO_Pin=I2C_SDA;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(GPIO_I2C,&GPIO_InitStructure);
}

//������ʼ�ź�
void I2C_Start(void)
{
    I2C_SDA_OUT();
	
	I2C_SDA_H;
	I2C_SCL_H;
	delay_nus(5);
	I2C_SDA_L;
	delay_nus(6);
	I2C_SCL_L;
}

//����ֹͣ�ź�
void I2C_Stop(void)
{
   I2C_SDA_OUT();

   I2C_SCL_L;
   I2C_SDA_L;
   I2C_SCL_H;
   delay_nus(6);
   I2C_SDA_H;
   delay_nus(6);
}

//��������Ӧ���ź�ACK
void I2C_Ack(void)
{
   I2C_SCL_L;
   I2C_SDA_OUT();
   I2C_SDA_L;
   delay_nus(2);
   I2C_SCL_H;
   delay_nus(5);
   I2C_SCL_L;
}

//����������Ӧ���ź�NACK
void I2C_NAck(void)
{
   I2C_SCL_L;
   I2C_SDA_OUT();
   I2C_SDA_H;
   delay_nus(2);
   I2C_SCL_H;
   delay_nus(5);
   I2C_SCL_L;
}
//�ȴ��ӻ�Ӧ���ź�
//����ֵ��1 ����Ӧ��ʧ��
//		  0 ����Ӧ��ɹ�
u8 I2C_Wait_Ack(void)
{
	u8 tempTime=0;

	I2C_SDA_IN();

	I2C_SDA_H;
	delay_nus(1);
	I2C_SCL_H;
	delay_nus(1);

	while(GPIO_ReadInputDataBit(GPIO_I2C,I2C_SDA))
	{
		tempTime++;
		if(tempTime>250)
		{
			I2C_Stop();
			return 1;
		}	 
	}

	I2C_SCL_L;
	return 0;
}
//I2C ����һ���ֽ�
void I2C_Send_Byte(u8 txd)
{
	u8 i=0;

	I2C_SDA_OUT();
	I2C_SCL_L;//����ʱ�ӿ�ʼ���ݴ���

	for(i=0;i<8;i++)
	{
		if((txd&0x80)>0) //0x80  1000 0000
			I2C_SDA_H;
		else
			I2C_SDA_L;

		txd<<=1;
		I2C_SCL_H;
		delay_nus(2); //��������
		I2C_SCL_L;
		delay_nus(2);
	}
}

//I2C ��ȡһ���ֽ�

u8 I2C_Read_Byte(u8 ack)
{
   u8 i=0,receive=0;

   I2C_SDA_IN();
   for(i=0;i<8;i++)
   {
   		I2C_SCL_L;
		delay_nus(2);
		I2C_SCL_H;
		receive<<=1;
		if(GPIO_ReadInputDataBit(GPIO_I2C,I2C_SDA))
		   receive++;
		delay_nus(1);	
   }

   	if(ack==0)
	   	I2C_NAck();
	else
		I2C_Ack();

	return receive;
}

//��ʼ��IIC�ӿ�
void AT24CXX_Init(void)
{
	  u8 addrtemp1,addrtemp2;
    I2C_INIT();		 //IIC��ʼ��
    delay_nms(4);
		if(0 == AT24CXX_Check()){
				addrtemp1 = AT24CXX_ReadOneByte(0x00);
			  addrtemp2 = AT24CXX_ReadOneByte(0x01);
			  if((addrtemp1 == addrtemp2)&&(addrtemp1 < 128)){
					slaveaddr = addrtemp1;
					clear_screen(0xFF);
					AdrrOK_Flag = 1;
				}else{
					display_GB2312_string(0,8,"�������豸��ַ");
					display_GB2312_string(2,16,"��ǰ��ַ:0");
					display_GB2312_string(4,8,"�õ�ַ�豸�쳣");
					display_GB2312_string(6,16,"1<=��ַ<=128");
					slaveaddr = 0;
					delay_ms(1500);
					delay_ms(1500);
					
					delay_ms(1500);
				}
		}else{
					display_GB2312_string(0,8,"�������豸��ַ");
					display_GB2312_string(2,16,"��ǰ��ַ:0");
					display_GB2312_string(4,8,"�õ�ַ�豸�쳣");
					display_GB2312_string(6,16,"1<=��ַ<=128");
					slaveaddr = 0;
					delay_ms(1500);
					delay_ms(1500);
					delay_ms(1500);
		}
}

/*******************************************************************************
* �� �� ��         : AT24Cxx_ReadOneByte
* ��������		   : 24c02��һ���ֽڵ�ַ  ����	   
* ��    ��         : addr
* ��    ��         : ����ֵtemp
*******************************************************************************/
u8 AT24CXX_ReadOneByte(u16 addr)
{
	u8 temp=0;

	I2C_Start();
	
	if(EE_TYPE>AT24C16)
	{
		I2C_Send_Byte(0xA0);
		I2C_Wait_Ack();
		I2C_Send_Byte(addr>>8);	//�������ݵ�ַ��λ
	}
	else
	{
	   I2C_Send_Byte(0xA0+((addr/256)<<1));//������ַ+���ݵ�ַ
	}

	I2C_Wait_Ack();
	I2C_Send_Byte(addr%256);//˫�ֽ������ݵ�ַ��λ		
							//���ֽ������ݵ�ַ��λ
	I2C_Wait_Ack();

	I2C_Start();
	I2C_Send_Byte(0xA1);
	I2C_Wait_Ack();

	temp=I2C_Read_Byte(0); //  0   ���� NACK
	I2C_NAck();
	I2C_Stop();
	
	return temp;	
}



/*******************************************************************************
* �� �� ��         : AT24Cxx_WriteOneByte
* ��������		   : 24c02дһ���ֽڵ�ַ  ����	   
* ��    ��         : addr  dt
* ��    ��         : ��
*******************************************************************************/
void AT24CXX_WriteOneByte(u16 addr,u8 dt)
{
	I2C_Start();

	if(EE_TYPE>AT24C16)
	{
		I2C_Send_Byte(0xA0);
		I2C_Wait_Ack();
		I2C_Send_Byte(addr>>8);	//�������ݵ�ַ��λ
	}
	else
	{
	   I2C_Send_Byte(0xA0+((addr/256)<<1));//������ַ+���ݵ�ַ
	}

	I2C_Wait_Ack();
	I2C_Send_Byte(addr%256);//˫�ֽ������ݵ�ַ��λ		
							//���ֽ������ݵ�ַ��λ
	I2C_Wait_Ack();

	I2C_Send_Byte(dt);
	I2C_Wait_Ack();
	I2C_Stop();

	delay_nms(10);
}

/*******************************************************************************
* �� �� ��         : AT24CXX_WriteLenByte
* ��������		   : 24c02�������ݳ���д����	   
* ��    ��         : u16 WriteAddr :��ʼд��ĵ�ַ,u32 DataToWrite:д������ݣ�u8 Len:Ҫд�����ݵĳ���2,4
* ��    ��         : ��
*******************************************************************************/
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len)
{  	
	u8 t;
	for(t=0;t<Len;t++)
	{
		AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}												    
}

/*******************************************************************************
* �� �� ��         : AT24CXX_ReadLenByte
* ��������		   : ��AT24CXX�����ָ����ַ��ʼ��������ΪLen������	   
* ��    ��         : u16 ReadAddr :��ʼ�����ĵ�ַ,u8 Len:Ҫ�������ݵĳ���2,4
* ��    ��         : ����
*******************************************************************************/
u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len)
{  	
	u8 t;
	u32 temp=0;
	for(t=0;t<Len;t++)
	{
		temp<<=8;
		temp+=AT24CXX_ReadOneByte(ReadAddr+Len-t-1); 	 				   
	}
	return temp;												    
}
//���AT24CXX�Ƿ�����
//��������24XX�����һ����ַ(255)���洢��־��.
//���������24Cϵ��,�����ַҪ�޸�
//����1:���ʧ��
//����0:���ɹ�
/*******************************************************************************
* �� �� ��         : AT24CXX_Check
* ��������		   : ���AT24CXX�Ƿ�����	   
* ��    ��         : ��
* ��    ��         : ����1:���ʧ��,����0:���ɹ�
*******************************************************************************/
u8 AT24CXX_Check(void)
{
	u8 temp;
    
	temp=AT24CXX_ReadOneByte(255);//����ÿ�ο�����дAT24CXX			   
	if(temp==0X55){
		return 0;
	}else//�ų���һ�γ�ʼ�������
	{
		AT24CXX_WriteOneByte(255,0X55);
	  temp=AT24CXX_ReadOneByte(255);	  
		if(temp==0X55)return 0;
	}
	return 1;											  
}


/*******************************************************************************
* �� �� ��         : AT24CXX_Read
* ��������		   : ��AT24CXX�����ָ����ַ��ʼ����ָ������������	   
* ��    ��         : u16 ReadAddr :��ʼ�����ĵ�ַ,u8 *pBuffer:���������׵�ַ��u8 NumToRead:Ҫ�������ݵĸ���
* ��    ��         : ����
*******************************************************************************/
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
}  

/*******************************************************************************
* �� �� ��         : AT24CXX_Write
* ��������		   : ��AT24CXX�����ָ����ַ��ʼ��������ΪLen������	   
* ��    ��         : u16 WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255,u8 *pBuffer:���������׵�ַ��u8 NumToWrite:Ҫд�����ݵĸ���
* ��    ��         : ����
*******************************************************************************/
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}
