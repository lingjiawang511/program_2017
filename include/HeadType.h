#ifndef __HEADTYPE_H_
#define __HEADTYPE_H_

#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>	
#include "led.h"
#include "delay.h"
//#include "exti.h"
#include "usart.h"
#include "TIM.h"
#include "communication.h"
#include "LCD_ZK.H"
#include "24cxx.h" 

/*************Typedef datatype start*******************/
typedef char int8;
typedef volatile char vint8;
typedef unsigned char uint8;
typedef volatile unsigned char vuint8;
typedef int int16;
typedef unsigned short uint16;
typedef long int32;
typedef unsigned long uint32;
typedef uint8	uchar,u8,BYTE;		/**< 8-bit value */
typedef uint8 	SOCKET;
typedef uint16	ushort,u16,WORD;	/**< 16-bit value */
typedef uint16	uint;		/**< 16-bit value */
typedef uint32	ulong;		/**< 32-bit value */
/*************Typedef datatype end*******************/

/*************define type start*******************/
#define false      (0)
#define true       (!false)

#ifndef NULL
#define NULL  ((void *)(0))
#endif


#define RxBufMax 512
#define TxBufMax 512

/*�Զ���֡��ʱ��ʱʱ��*/
#define AUTO_FRAME_TIMEOUT1  10  //10*2ms
#define AUTO_FRAME_TIMEOUT2  10  //10*2ms
#define AUTO_FRAME_TIMEOUT3  10  //10*2ms
/*�Զ���֡��ʱ��ʱʱ��*/
//��Ӧ��ʱʱ�䣬����λ��ȷ�϶�ά����ȷ��ɨ��ǹһֱɨ��ֱ���õ���һ�����ݵ��ʱ��
#define ANSWER_SCANTIME	 	 4000	  //4000*5ms
//����Ӧ��ʱʱ�䣬������λ���������ݸ���λ��������λ��û����Ӧ���ʱ��
#define NANSWER_TIME	 1000	  //1000*5ms
#define NANSWER_NUMOUT	 	 3	  //1000*5ms

#define SEND_MEDICINE_TIMEOUT		200    //��ҩ�����м�⵽û��ҩ����ʱ200*5MS��ֹͣ
#define IRQ_TIMEOUT							4			//�ж������ʱʱ��
#define	MOTOR_START_DELAYTIME		40		//ÿ�����������ʱ���

#define selectboard		     	1
#define MENU_EXIT_TIME 			4000
#define MENU_VALID_TIME			12000
/*************define type end*******************/

/*************union type start*******************/
typedef union{
	long l;
	unsigned long ul;
	int  i;
	unsigned int ui;
	short si[2];
	char  c[4];
	unsigned char uc[4];	
}UL;

typedef union{
	short i;
	unsigned short ui;
	char c[2];
	unsigned char uc[2];
}UI;

/*************union type end*******************/

/*************enum type start*******************/
enum
{
	False=0,True
};
enum{
	READLOW,READHIGH
};
typedef enum{
	SLAVE,
	HOST
}MCU_State_Type;
typedef enum{
	NO_USART_WORK,
	USART2_WORK,
	USART3_WORK,
}USART_WORK_Type;
typedef enum{
	RESERVE,
	READY,
	WORKING,
	WORKEND,
	END
}CH_Work_Enum_Type;
typedef enum{
	MENU_RESERVE,
	MENU_READPARAM,
	MENU_SETPARAM,
	MENU_SAVEPARAM,
	MENU_END
}Menu_Option;
/*************enum type end*******************/

/*************struct type start*******************/

typedef struct{
    u16 tx_index;        //���͵�ǰ���ݵ�����
    u16 rx_index;        //���յ���ǰ���ݵ�����
    u16 tx_count;        //������������
    u16 rx_count;        //������������
    u8 rx_start;
    u8 rx_aframe;       //�Ѿ����յ�һ֡����
    u8 txbuf[RxBufMax]; //�������ݻ�����
    u8 rxbuf[RxBufMax]; //�������ݻ�����
}Usart_Type;
//������������Э���ֽ�
typedef struct{
	u8  frame_soh;
	u8  frame_x;
	u16 datasize;
	u8  ch1_state;
	u8  ch1_num;
	u8  ch2_state;
	u8  ch2_num;
	u8  ch3_state;
	u8  ch3_num;
	u8  ch4_state;
	u8  ch4_num;
	u8  belt_state;
	u8  belt_time;
	u16 crc16_ccitt; 
	u8  frame_end1;
	u8  frame_end2;
}Communation_Send_Type;
//����������ӦЭ���ֽ�
typedef struct{
	u8  frame_soh;
	u8  frame_x;
	u8  addr;
	u8  funcode;
	u8 datasizeH;
	u8 datasizeL;
	u8  x;
	u8  y;
	u8  recbuf[20];
}Communation_Rec_Type;

typedef union{
	Communation_Send_Type control;
	u8	send_buf[28];	
}COMM_Send_Union_Type;

typedef union{
	Communation_Rec_Type control;
	u8	rec_buf[28];	
}COMM_Rec_Union_Type;

typedef struct{
	u8  answer_state;
	u8 	answer_numout;
	u16 answer_timeout;
	u16 Nanswer_timeout;
}Answer_Type;

typedef struct{
	CH_Work_Enum_Type  state;
	u8 	send_num;			//��Ҫ��ҩ������PC�����͹���
	u8  motor_pulse;	//���ת����Ȧ����ʹ�ô�����
	u8  send_actual;	//ʵ�ʷ�ҩ����ʹ�ô���������
	u8  motor_state;	//����Ĺ���״̬�����ڹ���ʱ������ִ��״̬ת��
	u8 	timeoutstart; //���ʵ�ʷ�ҩ������PC��Ҫ��ҩ��ʱ������ʱ����
	u16  timeout;			//���ʵ�ʷ�ҩ����ʱʱ��
	u8  motor_irqstate;//����ж�����˲�״̬
	u8 motor_irqtime;//����ж�����˲�ʱ��
	u8 send_actual_irqstate;	//ʵʱ�����ж�����˲�״̬
	u8 send_actual_irqtime;		//ʵʱ�����ж�����˲�ʱ��
	u8 motor_start_state;			//���������ʱ״̬
	u16 motor_start_time;			//�����ʱ����ʱ��
}CH_Work_Type;

typedef struct{
	CH_Work_Enum_Type  state;
	u8 	 send_time;
	u16  actual_time;
	u8	 speed_step;
	u8   start_signal;
	u8   stop_signal;
	u8   start_ok;
	u8   stop_ok;
	u8   stop_delay_state;
}Belt_Work_Type;
typedef struct{
	CH_Work_Type ch1;
	CH_Work_Type ch2;
	CH_Work_Type ch3;
	CH_Work_Type ch4;
}CH_Work;

typedef struct{
	u8 comm_head;
	u8 comm_word;
	u8 data_len;
	u8 data[4];
	u8 xorsum;
}RFID_REC_Type; 


/*************struct type end*******************/

/*************extern variable start*******************/
extern char Auto_Frame_Time1;
extern char Auto_Frame_Time2;
extern char Auto_Frame_Time3;

extern  Usart_Type Usart1_Control_Data;
extern Usart_Type Usart2_Control_Data;
extern Usart_Type Usart3_Control_Data;
extern  COMM_Send_Union_Type PC_Host_Rec;
extern  COMM_Rec_Union_Type  PC_Hosr_Send;

extern  COMM_Send_Union_Type MCU_Host_Send;
extern  COMM_Rec_Union_Type  MCU_Host_Rec;

extern  MCU_State_Type MCU_State;
extern  Answer_Type 	 PC_Answer;

extern  CH_Work Channel;
extern	Belt_Work_Type belt11;
extern	Belt_Work_Type belt12;
extern u8 Key_ScanNum;
extern RFID_REC_Type Usart2_RFIDRec;
extern RFID_REC_Type Usart3_RFIDRec;
extern u8 Key_SetParamFlag;
extern  u16  timeflag;
extern u8 AdrrOK_Flag;
extern u16  Menu_Exit_Time;
extern u32 Menu_Valid_Time;
/*************extern variable end*******************/

/*************function start*******************/
u16 CRC_GetModbus16(u8 *pdata, int len);
u16 CRC_GetCCITT(u8 *pdata, int len);
u8 Key_Scan(void);
void KEY_GPIO_Config(void);
u16 switch_init_time(void);
void dispose_key(void );
void dispose_menu(void);
/*************function end*******************/
#endif


