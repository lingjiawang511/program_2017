//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//�о�԰����
//���̵�ַ��http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  �� �� ��   : LCD_ZK.H
//  �� �� ��   : v2.0
//  ��    ��   : HuangKai
//  ��������   : 2014-0101
//  ����޸�   : 
//  ��������   : �ֿ�� OLED SPI�ӿ���ʾ����(STM32ϵ��)
//              ˵��: 
//              ------------------------����ΪOLED��ʾ���õ��Ľӿ----------------------------------------
//              GND    ��Դ��
//              VCC   ��5V��3.3v��Դ
//              CLK   PA5��CLK��
//              MOSI   PA7��DIN��
//              DC   PB0
//              CS1   PB1
//              FSO   PA4                
//              CS2   PB10
//
// �޸���ʷ   :
// ��    ��   : 
// ��    ��   : HuangKai
// �޸�����   : �����ļ�
//��Ȩ���У�����ؾ���
//Copyright(C) �о�԰����2014/3/16
//All rights reserved
//******************************************************************************/
#define _LCD_ZK_H_
#ifdef _LCD_ZK_H_



/* the macro definition to trigger the led on or off 
 * 1 - off
 - 0 - on
 */
#define ON  0
#define OFF 1


#define		LCD_SCLK_IO					GPIO_Pin_13					 //SCLK  ʱ�� D0��SCLK��
#define		LCD_SCLK_PORT				GPIOB
#define   LCD_SCLK_RCC				RCC_APB2Periph_GPIOB
#define		LCD_SDA_IO					GPIO_Pin_15					 //SDA   D1��MOSI�� ����
#define		LCD_SDA_PORT				GPIOB
#define   LCD_SDA_RCC					RCC_APB2Periph_GPIOB
#define		LCD_RST_IO					GPIO_Pin_10					//_RES  hardware reset   ��λ 
#define		LCD_RST_PORT				GPIOB
#define   LCD_RST_RCC					RCC_APB2Periph_GPIOB
#define		LCD_DC_IO						GPIO_Pin_11				 //A0  H/L ��������ѡͨ�ˣ�H�����ݣ�L:����
#define		LCD_DC_PORT					GPIOB
#define   LCD_DC_RCC					RCC_APB2Periph_GPIOB
#define		LCD_CS_IO						GPIO_Pin_12				 //OLEDƬѡ�ź�
#define		LCD_CS_PORT					GPIOB
#define   LCD_CS_RCC					RCC_APB2Periph_GPIOB

#define		ROM_CS_IO						GPIO_Pin_5				 //romƬѡ�ź�
#define		ROM_CS_PORT					GPIOA
#define   ROM_CS_RCC					RCC_APB2Periph_GPIOA
#define		ROM_OUT_IO					GPIO_Pin_4				 //
#define		ROM_OUT_PORT				GPIOA
#define   ROM_OUT_RCC					RCC_APB2Periph_GPIOA

#define   LCD_SCLK(N) 	 			{if(N==1){GPIO_SetBits(LCD_SCLK_PORT, LCD_SCLK_IO);}else{GPIO_ResetBits(LCD_SCLK_PORT, LCD_SCLK_IO);}}
#define   LCD_SDA(N) 	 				{if(N==1){GPIO_SetBits(LCD_SDA_PORT, LCD_SDA_IO);}else{GPIO_ResetBits(LCD_SDA_PORT, LCD_SDA_IO);}}
#define   LCD_RST(N) 	 				{if(N==1){GPIO_SetBits(LCD_RST_PORT, LCD_RST_IO);}else{GPIO_ResetBits(LCD_RST_PORT, LCD_RST_IO);}}
#define   LCD_DC(N) 	 				{if(N==1){GPIO_SetBits(LCD_DC_PORT, LCD_DC_IO);}else{GPIO_ResetBits(LCD_DC_PORT, LCD_DC_IO);}}
#define   ROM_CS(N) 	 				{if(N==1){GPIO_SetBits(ROM_CS_PORT, ROM_CS_IO);}else{GPIO_ResetBits(ROM_CS_PORT, ROM_CS_IO);}}
#define   ROM_OUTPUT(N) 	 		{if(N==1){GPIO_SetBits(ROM_OUT_PORT, ROM_OUT_IO);}else{GPIO_ResetBits(ROM_OUT_PORT, ROM_OUT_IO);}}

#if 1

//���κ꣬��������������һ��ʹ��
#define lcd_cs1(a)	if (a)	\
					GPIO_SetBits(LCD_CS_PORT,LCD_CS_IO);\
					else		\
					GPIO_ResetBits(LCD_CS_PORT,LCD_CS_IO)

//#define lcd_rs(a)	if (a)	\
//					GPIO_SetBits(LCD_DC_PORT,LCD_DC_IO);\
//					else		\
//					GPIO_ResetBits(LCD_DC_PORT,LCD_DC_IO)
#define lcd_rs(a)  LCD_DC(a)

//#define lcd_sid(a)	if (a)	\
//					GPIO_SetBits(LCD_SDA_PORT,LCD_SDA_IO);\
//					else		\
//					GPIO_ResetBits(LCD_SDA_PORT,LCD_SDA_IO)
#define lcd_sid(a) LCD_SDA(a) 
					
//#define lcd_sclk(a)	if (a)	\
//					GPIO_SetBits(LCD_SCLK_PORT,LCD_SCLK_IO);\
//					else		\
//					GPIO_ResetBits(LCD_SCLK_PORT,LCD_SCLK_IO)
#define lcd_sclk(a) LCD_SCLK(a)		
					
//#define Rom_CS(a)	if (a)	\
//					GPIO_SetBits(ROM_CS_PORT,ROM_CS_IO);\
//					else		\
//					GPIO_ResetBits(ROM_CS_PORT,ROM_CS_IO)
#define Rom_CS(a) ROM_CS(a)

//#define Rom_OUT(a)	if (a)	\
//					GPIO_SetBits(ROM_OUT_PORT,ROM_OUT_IO);\
//					else		\
//					GPIO_ResetBits(ROM_OUT_PORT,ROM_OUT_IO)				
#define Rom_OUT(a)  ROM_OUTPUT(a)

#define ROM_OUT    GPIO_ReadInputDataBit(ROM_OUT_PORT,ROM_OUT_IO)
					
#else
//���κ꣬��������������һ��ʹ��
#define lcd_cs1(a)	if (a)	\
					GPIO_SetBits(GPIOB,GPIO_Pin_1);\
					else		\
					GPIO_ResetBits(GPIOB,GPIO_Pin_1)

#define lcd_rs(a)	if (a)	\
					GPIO_SetBits(GPIOB,GPIO_Pin_0);\
					else		\
					GPIO_ResetBits(GPIOB,GPIO_Pin_0)


#define lcd_sid(a)	if (a)	\
					GPIO_SetBits(GPIOA,GPIO_Pin_7);\
					else		\
					GPIO_ResetBits(GPIOA,GPIO_Pin_7)

#define lcd_sclk(a)	if (a)	\
					GPIO_SetBits(GPIOA,GPIO_Pin_5);\
					else		\
					GPIO_ResetBits(GPIOA,GPIO_Pin_5)
#define Rom_CS(a)	if (a)	\
					GPIO_SetBits(GPIOB,GPIO_Pin_10);\
					else		\
					GPIO_ResetBits(GPIOB,GPIO_Pin_10)

#define Rom_OUT(a)	if (a)	\
					GPIO_SetBits(GPIOA,GPIO_Pin_4);\
					else		\
					GPIO_ResetBits(GPIOA,GPIO_Pin_4)				

#define ROM_OUT    GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)
#endif						
					
extern unsigned char const bmp1[];
extern unsigned char const bmp2[];
extern unsigned char const bmp3[];
extern unsigned char const jiong1[];
extern unsigned char const lei1[];


/*дָ�LCDģ��*/
void transfer_command_lcd(int data1);   
/*д���ݵ�LCDģ��*/
void transfer_data_lcd(int data1);
/*��ʱ*/
void delay(int n_ms);               
/*LCDģ���ʼ��*/
void initial_lcd(void);
void lcd_address(unsigned char page,unsigned char column);
/*ȫ������*/
//void clear_screen(void);
void clear_screen(u8 dat);
/*��ʾ128x64����ͼ��*/
void display_128x64(const unsigned char *dp);
/*��ʾ132x64����ͼ��*/
//void display_132x64(unsigned char *dp);
/*��ʾ16x16����ͼ�񡢺��֡���Ƨ�ֻ�16x16���������ͼ��*/
void display_graphic_16x16(unsigned int page,unsigned int column,const unsigned char *dp);
/*��ʾ8x16����ͼ��ASCII, ��8x16����������ַ�������ͼ��*/
void display_graphic_8x16(unsigned int page,unsigned char column,unsigned char *dp);
/*��ʾ5*7����ͼ��ASCII, ��5x7����������ַ�������ͼ��*/
void display_graphic_5x7(unsigned int page,unsigned char column,unsigned char *dp);
/****��ָ�����Ѷ�ֿ�IC***/
void send_command_to_ROM( unsigned char datu );
/****�Ӿ���Ѷ�ֿ�IC��ȡ���ֻ��ַ����ݣ�1���ֽڣ�***/
static unsigned char get_data_from_ROM(void);
/*����ص�ַ��addrHigh����ַ���ֽ�,addrMid����ַ���ֽ�,addrLow����ַ���ֽڣ�����������DataLen���ֽڵ����ݵ� pBuff�ĵ�ַ*/
/*������ȡ*/
void get_n_bytes_data_from_ROM(unsigned char addrHigh,unsigned char addrMid,unsigned char addrLow,unsigned char *pBuff,unsigned char DataLen );
/******************************************************************/
void display_GB2312_string(unsigned char y,unsigned char x,unsigned char *text);
void display_string_5x7(unsigned char y,unsigned char x,unsigned char *text);
void LCD_P8x16Showchar(unsigned char x, unsigned char y,unsigned char dat);
void LCD_P8x16Str(unsigned char x, unsigned char y,unsigned char ch[]);
void display_bmp(u8 x,u8 y,u8 size_x,u8 size_y,const unsigned char *dp);
void Gui_Battery(u8 x,u8 y,u8 bat);


#endif
