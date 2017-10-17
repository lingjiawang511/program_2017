/*****************************************************/
//������LCD12864������
//���ߣ�С��Ҷ��
//ʱ�䣺2015/07/16
//��������ʾ�밴��
//s2 ---------GPIO52 ;s3-----------GPIO54 ; s4 -------GPIO72 ; s5 ----GPIO71;
//����д����1.������2.״̬��
//������������ʱ����20ms
/*****************************************************/
#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

#define LCD_DATA		(*((volatile Uint16 *)0x45EF))
#define E				GpioDataRegs.GPBDAT.bit.GPIO61
#define RW				GpioDataRegs.GPADAT.bit.GPIO27
#define RS 				GpioDataRegs.GPBDAT.bit.GPIO60
#define LOW 			0
#define HIGH			1
#define CLEAR_SCREEN	0x01		//����ָ�������ACֵΪ00H
#define AC_INIT			0x02		//��AC����Ϊ00H�����α��Ƶ�ԭ��λ��
#define CURSE_ADD		0x06		//�趨�α��Ƶ�����ͼ�������ƶ�����Ĭ���α����ƣ�ͼ�����岻����
#define FUN_MODE		0x30		//����ģʽ��8λ����ָ�
#define DISPLAY_ON		0x0c		//��ʾ��,��ʾ�α꣬���α�λ�÷���
#define DISPLAY_OFF		0x08		//��ʾ��
#define CURSE_DIR		0x14		//�α������ƶ�:AC=AC+1
#define SET_CG_AC		0x40		//����AC����ΧΪ��00H~3FH
#define SET_DD_AC		0x80 
#define s2 GpioDataRegs.GPBDAT.bit.GPIO54
#define s3 GpioDataRegs.GPBDAT.bit.GPIO55
#define s4 GpioDataRegs.GPBDAT.bit.GPIO56
#define s5 GpioDataRegs.GPBDAT.bit.GPIO57

#define	DELAY_MS(A)		DELAY_US((long double)A * 1000.0L)
#define DELAY_100US(B)	DELAY_US((long double)B * 100.0L)

void InitXintf(void);
void ConfigGpio(void);
void InitLcd(void);
void Read_Busy(void);
void WriteCmd12864(Uint16 cmd);
void WriteData12864(Uint16 dat);
void DisplayDdram(uchar *hz);
void DispBMP(uchar *adress);
void CLR_GDRAM(void);
void Disp(void);
void fanbai(uchar y,uchar x1,uchar x2);
void  key(void);
void xuanze();
void xunhuan(uchar hang);
void jiantou(uchar hang);


uchar tab1[]="���ò���ֵ";
uchar tab2[]="��ѯ����ֵ";

uchar tab3[]="��ѯ��ʷ����";
uchar tab4[]="�����ʷ����";
uchar tab5[]="�ڶ����˵���ʾ";

uchar key2 = 0;
uchar key3 = 0;
uchar key4 = 0;
uchar key5 = 0;

uchar keyindex = 0; //��ʼ����ʱ���ǵ�һ�У�ѡ���е�����
uchar keycur = 0; //0,1,2,3 ��ʾ���ĵ�ǰ��

void main(void)
{
	InitSysCtrl();
	InitXintf();
	InitXintf16Gpio();
	DINT;
	InitPieCtrl();
	
	IER = 0x0000;
	IFR = 0x0000;
	
	ConfigGpio();
	InitLcd();
	WriteCmd12864(CLEAR_SCREEN);
	DELAY_US(50);
	
	Disp();
	
	WriteCmd12864(0x34); //������չָ��
	
	fanbai(0,2,11);
	

	WriteCmd12864(0x36);
	WriteCmd12864(0x30); //���ص�����ָ��
	
	while(1)
	{
		xuanze();
	}

}

void ConfigGpio(void)
{
	EALLOW;
	//===12864====================================
	GpioCtrlRegs.GPBMUX2.bit.GPIO60 = 0;
	GpioCtrlRegs.GPBDIR.bit.GPIO60 = 1;
	GpioCtrlRegs.GPBMUX2.bit.GPIO61 = 0;
	GpioCtrlRegs.GPBDIR.bit.GPIO61 = 1;
	GpioCtrlRegs.GPAMUX2.bit.GPIO27 = 0;
	GpioCtrlRegs.GPADIR.bit.GPIO27 = 1;
	//===12864====================================
	
	//===key====================================
	GpioCtrlRegs.GPBMUX2.bit.GPIO54 = 0; //s2 GPIO 
	GpioCtrlRegs.GPBDIR.bit.GPIO54= 0;  //input
	GpioCtrlRegs.GPBMUX2.bit.GPIO55 = 0; //s2 GPIO 
	GpioCtrlRegs.GPBDIR.bit.GPIO55= 0;  //input
	GpioCtrlRegs.GPBMUX2.bit.GPIO56 = 0; //s2 GPIO 
	GpioCtrlRegs.GPBDIR.bit.GPIO56= 0;  //input
	GpioCtrlRegs.GPBMUX2.bit.GPIO57 = 0; //s2 GPIO 
	GpioCtrlRegs.GPBDIR.bit.GPIO57= 0;  //input
	
	//disable pull-up
	
	GpioCtrlRegs.GPAPUD.bit.GPIO0 = 1;
	GpioCtrlRegs.GPAPUD.bit.GPIO1 = 1;
	GpioCtrlRegs.GPAPUD.bit.GPIO2 = 1;
	GpioCtrlRegs.GPAPUD.bit.GPIO3 = 1;
	//===key====================================
	
	EDIS;
}

void InitLcd(void)
{
	DELAY_100US(5);
	WriteCmd12864(FUN_MODE);
	DELAY_US(5);
	
	WriteCmd12864(FUN_MODE);
	DELAY_US(5);
	
	WriteCmd12864(0x0C);
	DELAY_US(5);
	
	WriteCmd12864(CLEAR_SCREEN);
	DELAY_US(30);
	
	WriteCmd12864(AC_INIT);
	DELAY_US(4);

}
void Read_Busy() //��æ��־���λΪ1����busy,���ܽ��ж�д����
{
 	RS = LOW;
	RW = HIGH;
	E = HIGH;
	while(LCD_DATA & 0X80);
	E = LOW;
}
void WriteCmd12864(Uint16 cmd)
{
	Read_Busy();
	RS = LOW;
	RW = LOW;//д����
	E = HIGH;
	LCD_DATA  = cmd;
	E = LOW;
}

void WriteData12864(Uint16 dat)
{
	Read_Busy();
	RS = HIGH;
	RW = LOW;//д����
	E = HIGH;
	LCD_DATA  = dat;
	E = LOW;
}

void DisplayDdram(uchar *hz)
{
	while(*hz != '\0')
	{
		WriteData12864(*hz);//��ַ�����ֵ
		hz++;
	}
}
//������ʾ�Ļ��棬ѡ���һ�з�����ʾ
void Disp()
{
	WriteCmd12864(0x81);
	DisplayDdram(tab1);
	WriteCmd12864(0x91);
	DisplayDdram(tab2);
	
	WriteCmd12864(0x89);
	DisplayDdram(tab3);
	WriteCmd12864(0x99);
	DisplayDdram(tab4);
}
//ѡ��ĳһ�з�����ʾ����ĳһ�еļ���������ʾ��������
//y = {0,1,2,3}
//x = {0,...15} //�޸����� һ����16������
void fanbai(uchar y,uchar x1,uchar x2)
{
	uchar x_adress;
	uchar i;
	uchar j;
	switch(y)
	{
		case 0: x_adress = 0x80;break;
		case 1: x_adress = 0x90;break;
		case 2: x_adress = 0x88;break;
		case 3: x_adress = 0x98;break;
		default: x_adress = 0x80;break;
	}
	
	if(0x80 == x_adress) //��ʾ���ĵ�һ��
	{
		for(i = 0; i < 32; i++)
		{
			WriteCmd12864(0x80 + i); //y
			WriteCmd12864(0x80);     //x
			if(i < 16)
			{
				for(j =0;j < 16;j++)
				{
					if((j >= x1) & (j <= x2))
					{
						WriteData12864(0xff); 
					}
					else
					{
						WriteData12864(0x00);
					}
				}
				
			}
			else //��ʾ���ĵڶ���
			{
				for(j =0;j < 16;j++)
				{
					
					WriteData12864(0x00);
				
				}
			}
		}
		for(i = 0;i < 32; i++)//��ʾ���ĵ�������
		{
			WriteCmd12864(0x80 + i);
			WriteCmd12864(0x88);
			for(j = 0;j <16; j++)
			{
				WriteData12864(0x00);
			}
		}
			
	}
	if(0x90 == x_adress) //�ڶ���
	{
		for(i = 0; i < 32; i++)
		{
			WriteCmd12864(0x80 + i); //y
			WriteCmd12864(0x80);     //x
			if(i < 16)//��ʾ����һ��
			{
				for(j =0;j < 16;j++) 
				{
					WriteData12864(0x00);
				}
				
			}
			else
			{
				for(j =0;j < 16;j++)
				{
					
					if((j >= x1) & (j <= x2))
					{
						WriteData12864(0xff); 
					}
					else
					{
						WriteData12864(0x00);
					}
				
				}
			}
		}
		for(i = 0;i < 32; i++) //��������
		{
			WriteCmd12864(0x80 + i);
			WriteCmd12864(0x88);
			for(j = 0;j <16; j++)
			{
				WriteData12864(0x00);
			}
		}
			
	}
	if(0x88 == x_adress) //��ʾ���ĵ�����
	{
		for(i = 0;i < 32; i++) //��ʾ���ĵ�һ����
		{
			WriteCmd12864(0x80 + i);
			WriteCmd12864(0x80);
			for(j = 0;j <16; j++)
			{
				WriteData12864(0x00); 
			}
		}
		for(i = 0; i < 32; i++)
		{
			WriteCmd12864(0x80 + i); //y
			WriteCmd12864(0x88);     //x
			if(i < 16)
			{
				for(j =0;j < 16;j++) //��ʾ��������
				{
					if((j >= x1) & (j <= x2))
					{
						WriteData12864(0xff); 
					}
					else
					{
						WriteData12864(0x00);
					}
				}
				
			}
			else //��ʾ��������
			{
				for(j =0;j < 16;j++)
				{
					WriteData12864(0x00);
				}
			}
		}
		
			
	}
	
	if(0x98 == x_adress) //��ʾ���ĵ�����
	{
		for(i = 0;i < 32; i++) //��ʾ���ĵ�һ����
		{
			WriteCmd12864(0x80 + i);
			WriteCmd12864(0x80);
			for(j = 0;j <16; j++)
			{
				WriteData12864(0x00); 
			}
		}
		for(i = 0; i < 32; i++)
		{
			WriteCmd12864(0x80 + i); //y
			WriteCmd12864(0x88);     //x
			if(i < 16)
			{
				for(j =0;j < 16;j++) //��ʾ��������
				{
					WriteData12864(0x00);
				}
			}
			else //��ʾ��������
			{
				for(j =0;j < 16;j++)
				{
					if((j >= x1) & (j <= x2))
					{
						WriteData12864(0xff); 
					}
					else
					{
						WriteData12864(0x00);
					}
				}
			}
		}
		
			
	}
		
}

//���������Ƽ���������ʾ��һ��
//s4 --���ƣ�s5 --����
void key()
{
	//1.���ö�ӦIOΪ����,��ConfigGpio�������棬�Ѿ����á�
	if(0 == s4) //��⵽������
	{
		DELAY_US(1000); //��ʱ10MS,ǰ����
		if(0 == s4)
		{
			while(!s4); //ȷ�ϱ����£������ͷ�
			key5 ++;
		}			
		
	}
	
	if(0 == s5) //��⵽������
	{
		DELAY_US(1000); //��ʱ10MS,ǰ����
		if(0 == s5)
		{
			while(!s5); //ȷ�ϱ����£������ͷ�
			key5 ++;
		}			
		
	}
}

void xuanze()
{
	static int16 key_yidong = 0;
	static k = 0;
	if(0 == s4) //���ƣ���⵽������
	{
		DELAY_US(1000); //��ʱ10MS,ǰ����
		if(0 == s4)
		{
			while(!s4); //ȷ�ϱ����£������ͷ�
			key_yidong --;
			if(-1 == key_yidong)
			{
				key_yidong = 3;
				k++;
			}
			xunhuan(key_yidong);
			
		}			
		
	}
	if(0 == s5) //��⵽������
	{
		DELAY_US(1000); //��ʱ10MS,ǰ����
		if(0 == s5)
		{
			while(!s5); //ȷ�ϱ����£������ͷ�
			key_yidong ++;
			if(4 == key_yidong)
			{
				key_yidong = 0;
			}
			xunhuan(key_yidong);
		}			
		
	}
	
	
	
}

void xunhuan(uchar hang)
{
	switch(hang)
	{
		case 0: 
			WriteCmd12864(0x34); //������չָ��
			fanbai(0,2,11);
			WriteCmd12864(0x36);
			WriteCmd12864(0x30); //���ص�����ָ��
			jiantou(0);
			break;
		case 1: 
			WriteCmd12864(0x34); //������չָ��
			fanbai(1,2,11);
			WriteCmd12864(0x36);
			WriteCmd12864(0x30); //���ص�����ָ��
			WriteCmd12864(0x90);
			WriteData12864(0x10);
			jiantou(1);
			break;
		case 2: 
			WriteCmd12864(0x34); //������չָ��
			fanbai(2,2,13);
			WriteCmd12864(0x36);
			WriteCmd12864(0x30); //���ص�����ָ��
			jiantou(2);
			break;
		case 3: 
			WriteCmd12864(0x34); //������չָ��
			fanbai(3,2,13);
			WriteCmd12864(0x36);
			WriteCmd12864(0x30); //���ص�����ָ��
			jiantou(3);
			break;
		default:
			WriteCmd12864(0x34); //������չָ��
			fanbai(0,2,13);
			WriteCmd12864(0x36);
			WriteCmd12864(0x30); //���ص�����ָ��
			jiantou(0);
			break;
	}
	
}
void jiantou(uchar hang)
{
	switch(hang)
	{
		case 0: 
			WriteCmd12864(0x80);
			WriteData12864(0x10);
			WriteCmd12864(0x90);
			WriteData12864(0x20);
			WriteCmd12864(0x88);
			WriteData12864(0x20);
			WriteCmd12864(0x98);
			WriteData12864(0x20);
			break;
		case 1:
			WriteCmd12864(0x80);
			WriteData12864(0x20);
			WriteCmd12864(0x90);
			WriteData12864(0x10);
			WriteCmd12864(0x88);
			WriteData12864(0x20);
			WriteCmd12864(0x98);
			WriteData12864(0x20);
			break;
		case 2:
			WriteCmd12864(0x80);
			WriteData12864(0x20);
			WriteCmd12864(0x90);
			WriteData12864(0x20);
			WriteCmd12864(0x88);
			WriteData12864(0x10);
			WriteCmd12864(0x98);
			WriteData12864(0x20);
			break;
		case 3:
			WriteCmd12864(0x80);
			WriteData12864(0x20);
			WriteCmd12864(0x90);
			WriteData12864(0x20);
			WriteCmd12864(0x88);
			WriteData12864(0x20);
			WriteCmd12864(0x98);
			WriteData12864(0x10);
			break;
		default:
			WriteCmd12864(0x80);
			WriteData12864(0x10);
			WriteCmd12864(0x90);
			WriteData12864(0x20);
			WriteCmd12864(0x88);
			WriteData12864(0x20);
			WriteCmd12864(0x98);
			WriteData12864(0x20);
			break;
	}
}
