#ifndef __HEADTYPE_H_
#define __HEADTYPE_H_

#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>	
#include "led.h"
#include "delay.h"
#include "send_medicine_control.h"
#include "exti.h"
#include "usart.h"
#include "TIM.h"
#include "belt.h"
#include "printer.h"
#include "air_cylinder.h"
#include "communication.h"
#include "baffle.h"

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

#define IRQ_TIMEOUT							4			//�ж������ʱʱ��


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
	IN_DOWN,
	IN_UP
}Air_Position_Type;

typedef enum{
	PRINTER_RESERVE,
	PRINTER_READY,
//	PRINTER_START,
	PRINTER_WORKING,
	PRINTER_END
}Pulse_Type;
//typedef enum{
//	RESERVE,
//	READY,
//	WORKING,
//	WORKEND,
//	END
//}CH_Work_Enum_Type;
typedef enum{
	RESERVE,
	READY,
	WORKING,
	WORKEND,
	END
}Printer_Work_Enum_Type;
typedef Printer_Work_Enum_Type CH_Work_Enum_Type;
typedef Printer_Work_Enum_Type Air_Cylinder_Enum_Type;
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
	u8  crc_result;       //��������CRC���
	u8  scanner_result;   //��ά��ɨ����
  u8  device_state;    //�豸״̬
	u8  printer_state;   //��ӡ״̬
	u8  recom_state;    //����״̬
	u8  err_message;
	u16 crc16_ccitt; 
	u8  frame_end1;
	u8  frame_end2;
}Communation_Send_Type;
//����������ӦЭ���ֽ�
typedef struct{
	u8  frame_soh;
	u8  frame_x;
	u16 datasize;
	u8  scanner_result;
	u8  check_state;
	u8  device_control;  //�豸���ƣ������豸����ֹͣ
	u8  baffle_control;  //������ƣ�ֻ���豸ֹͣ״̬����Ч
	u8  printer_fix;
	u8 printer_delay_H;
	u8 printer_delay_L;
	u8  baffle_fix;
	u8 baffle_delay_H;
	u8 baffle_delay_L;
	u16 crc16_ccitt; 
	u8  frame_end1;
	u8  frame_end2;
}Communation_Rec_Type;

typedef union{
	Communation_Send_Type control;
	u8	send_buf[14];	
}COMM_Send_Union_Type;

typedef union{
	Communation_Rec_Type control;
	u8	rec_buf[18];	
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
}Belt_Work_Type;
typedef struct{
	CH_Work_Type ch1;
	CH_Work_Type ch2;
	CH_Work_Type ch3;
	CH_Work_Type ch4;
}CH_Work;
typedef struct {
	u8 state;
	u8 irqstate;
	u8 irqtime;
}Printer_Input_Type;
typedef struct{
	Printer_Work_Enum_Type  state;  
	Printer_Input_Type 	err;					//��ӡ����������
	Printer_Input_Type  end;					//��ӡ����ӡ��������
	Printer_Input_Type  pinline;			//��ӡ����������
	Printer_Input_Type  color_end;	  //��ӡ��ɫ��������
	Printer_Input_Type 	color_less;   //��ӡ��ɫ��������
	Printer_Input_Type tag_end;			  //��ӡ����ǩ��������
	u8 input_state;   //��ӡ������״̬
	Pulse_Type process;         //��ӡ������
	u8 start_delay_time;
	u8 restart;       //��ӡ���ظ�����
	u8 complete;
	u16 fluid_bag_timeout;  //Һ���ź�״̬��ʱ����
	u8 printer_work_timeout;
}Printer_Type;
typedef struct{
	Air_Position_Type 	air_cylinder_position;		//��ѹ����λ��
	u8	air_cylinder_satte;		//��ѹ����״̬
	u8 	air_blow_satte;				//��������״̬
	u8 	vacuum_satte;					//�����ֽ״̬
	Air_Cylinder_Enum_Type process; //��ѹ���׹���
	u16 delay_time;    //��ѹ������ѹ��ʱʱ��
	u16 PC_send_delay_time;
	u8  complete; 
  u16 air_cylinder_dowm_timeout;   //��ѹ������ѹ����������ʱ
	u16 air_cylinder_up_timeout;   //��ѹ������ѹ����������ʱ
}Air_Controlr_Type;
typedef struct{
	Printer_Input_Type fit_reach;
	Printer_Input_Type upper_reach;
	Printer_Input_Type fluid_bag;
	Printer_Input_Type scanner;
}Control_Input_Type;
typedef struct{
	u16 Scanner_Err_Time;
	u16 bag_Err_Time;
	u16 process_time;
	u16 PC_send_process_time;
	u8  bag_input_flag;
	u8  bag_err_flag;
	u8  bag_ok_flag;
	u8  process_flag;
	u8  PC_send_scanner_result;
	u8  baffle_state;
	u8  scanner_result_old;
}Control_Baffle_Type;
/*************struct type end*******************/

/*************extern variable start*******************/
extern char Auto_Frame_Time1;

extern  Usart_Type Usart1_Control_Data;

extern  COMM_Send_Union_Type PC_Host_Rec;
extern  COMM_Rec_Union_Type  PC_Hosr_Send;

extern  COMM_Send_Union_Type MCU_Host_Send;
extern  COMM_Rec_Union_Type  MCU_Host_Rec;

extern  MCU_State_Type MCU_State;
extern  Answer_Type 	 PC_Answer;

extern  CH_Work Channel;
extern	Belt_Work_Type belt;


extern u8 Key_ScanNum;
extern u8 Device_State;
extern Printer_Type Printer;
extern Control_Input_Type Control;
extern Air_Controlr_Type Air_Control;
extern Control_Baffle_Type Baffle_Control;
/*************extern variable end*******************/

/*************function start*******************/
u16 CRC_GetModbus16(u8 *pdata, int len);
u16 CRC_GetCCITT(u8 *pdata, int len);
void Key_Light_Init(void);
void Key_Light_Dispose(void);
void ROLL_PAPER_GPIO_Config(void);
void Roll_Paper_Control(void);
void param_init(void);
/*************function end*******************/
#endif


