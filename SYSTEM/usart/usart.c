#include "sys.h"
#include "usart.h"
#include"HeadType.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif
 
 /*串口1管脚重映射*/
#define USART1REMAP 0


Usart_Type Usart1_Control_Data;
Usart_Type Usart2_Control_Data;
Usart_Type Usart3_Control_Data;
char Auto_Frame_Time1;
char Auto_Frame_Time2;
char Auto_Frame_Time3;

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/*使用microLib的方法*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/
 
#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	  
  
void uart_init(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART1, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART1, ENABLE);                    //使能串口1 

}


//=============================================================================
//函数名称:Init_USART1
//功能概要:USART1 初始化串口相关参数
//参数说明:无
//函数返回:无
//=============================================================================
static void Init_USART1(void)
{
    Usart1_Control_Data.tx_index = 0;
    Usart1_Control_Data.rx_index = 0;
    Usart1_Control_Data.tx_count = 0;
    Usart1_Control_Data.rx_count = 0;
    Usart1_Control_Data.rx_start = 0;
    Usart1_Control_Data.rx_aframe = 0;
    
}
//=============================================================================
//函数名称:Init_USART2
//功能概要:USART2 初始化串口相关参数
//参数说明:无
//函数返回:无
//=============================================================================
static void Init_USART2(void)
{
    Usart2_Control_Data.tx_index = 0;
    Usart2_Control_Data.rx_index = 0;
    Usart2_Control_Data.tx_count = 0;
    Usart2_Control_Data.rx_count = 0;
    Usart2_Control_Data.rx_start = 0;
    Usart2_Control_Data.rx_aframe = 0;
    
}
//=============================================================================
//函数名称:Init_USART3
//功能概要:USART3 初始化串口相关参数
//参数说明:无
//函数返回:无
//=============================================================================
static void Init_USART3(void)
{
    Usart3_Control_Data.tx_index = 0;
    Usart3_Control_Data.rx_index = 0;
    Usart3_Control_Data.tx_count = 0;
    Usart3_Control_Data.rx_count = 0;
    Usart3_Control_Data.rx_start = 0;
    Usart3_Control_Data.rx_aframe = 0;
    
}
//=============================================================================
//函数名称:USART1_Interrupts_Config
//功能概要:USART1 中断优先级配置
//参数说明:无
//函数返回:无
//=============================================================================
static void USART1_Interrupts_Config(void)
{
     NVIC_InitTypeDef NVIC_InitStructure; 
    
     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
    
     NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
     NVIC_Init(&NVIC_InitStructure);
}
//=============================================================================
//函数名称:USART2_Interrupts_Config
//功能概要:USART2 中断优先级配置
//参数说明:无
//函数返回:无
//=============================================================================
static void USART2_Interrupts_Config(void)
{
     NVIC_InitTypeDef NVIC_InitStructure; 
    
     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
    
     NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
     NVIC_Init(&NVIC_InitStructure);
}
//=============================================================================
//函数名称:USART3_Interrupts_Config
//功能概要:USART3 中断优先级配置
//参数说明:无
//函数返回:无
//=============================================================================
static void USART3_Interrupts_Config(void)
{
     NVIC_InitTypeDef NVIC_InitStructure; 
    
     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
    
     NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
     NVIC_Init(&NVIC_InitStructure);
}
//=============================================================================
//函数名称:USART1_Config
//功能概要:USART1 初始化配置,工作模式配置。57600 8-N-1
//参数说明:无
//函数返回:无
//=============================================================================
#if USART1REMAP  //如果USART1使用了映射

void USART1_Config(void )
{	
    GPIO_InitTypeDef GPIO_InitStructure;	
    USART_InitTypeDef USART_InitStructure;  //定义串口初始化结构体
    
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB | RCC_APB2Periph_USART1,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);

  /*  USART1_TX -> PB6 */
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	       //选中串口默认输出管脚         
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //定义输出最大速率 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//定义管脚9的模式  
  GPIO_Init(GPIOB, &GPIO_InitStructure);           //调用函数，把结构体参数输入进行初始化	
  
  /*  USART1_RX ->	PB7*/

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	        
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  USART_InitStructure.USART_BaudRate = 19200; //波特率
  USART_InitStructure.USART_WordLength = USART_WordLength_8b; //数据位8位
  USART_InitStructure.USART_StopBits = USART_StopBits_1;	//停止位1位
  USART_InitStructure.USART_Parity = USART_Parity_No;		//校验位 无
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无流控制
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//使能接收和发送引脚

  USART_Init(USART1, &USART_InitStructure); //将以上赋完值的结构体带入库函数USART_Init进行初始化
  USART_ClearFlag(USART1,USART_FLAG_TC);
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  USART_ITConfig(USART1, USART_IT_TC, ENABLE);
    /* Enable the EXTI Interrupt */
  USART_Cmd(USART1, ENABLE);//开启USART1，注意与上面RCC_APB2PeriphClockCmd()设置的区别
  USART1_Interrupts_Config();
  Init_USART1();
}
#else
void USART1_Config(void)
{
	//gpio，usart初始化结构体定义
	//当gpio复用时（开启外设时），需要初始化同时gpio和ppp外设，并对他们使能和开启时钟
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	//开启gpio和外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

	//gpio初始化结构体成员赋值
	//初始化PA。9，复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//执行上面的gpio初始化
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// PA.10浮空输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//USART初始化
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_ClearFlag(USART1,USART_FLAG_TC);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1, USART_IT_TC, ENABLE);
	/* Enable the EXTI Interrupt */
	USART_Cmd(USART1, ENABLE);//开启USART1，注意与上面RCC_APB2PeriphClockCmd()设置的区别
	
	USART1_Interrupts_Config();
	Init_USART1();
}

#endif
 //=============================================================================
//函数名称:USART2_Config
//功能概要:USART2 初始化配置,工作模式配置。57600 8-N-1
//参数说明:无
//函数返回:无
//=============================================================================
void USART2_Config(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* Enable the USART2 Pins Software Remapping */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); 

	/* Configure USART2 Tx (PA.02) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* Configure USART2 Rx (PA.03) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    

	USART_InitStructure.USART_BaudRate = 19200; 			 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	   
	USART_InitStructure.USART_Parity = USART_Parity_No; 	  
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  

	USART_Init(USART2, &USART_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART2, USART_IT_TC, ENABLE);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	/* Enable USART2 */
	USART_Cmd(USART2, ENABLE);
	USART2_Interrupts_Config();
	Init_USART2();

}
//=============================================================================
//函数名称:USART1_Config
//功能概要:USART1 初始化配置,工作模式配置。57600 8-N-1
//参数说明:无
//函数返回:无
//=============================================================================


void USART3_Config(void )
{	
  
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* Enable the USART3 Pins Software Remapping */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); 

	/* Configure USART3 Tx (PB .10) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* Configure USART3 Rx (PB.11) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
      
	USART_InitStructure.USART_BaudRate = 19200; 			 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	   
	USART_InitStructure.USART_Parity = USART_Parity_No; 	  
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  

	USART_Init(USART3, &USART_InitStructure);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART3, USART_IT_TC, ENABLE);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	/* Enable USART2 */
	USART_Cmd(USART3, ENABLE);
	USART3_Interrupts_Config();
	Init_USART3();
}
//=============================================================================
//函数名称:fputc
//功能概要:重定向c库函数printf到USART
//参数说明:无
//函数返回:无
//注意   :由printf调用,需要勾选Use Microlib
//=============================================================================

// int fputc(int ch, FILE *f)
// {
// //将Printf内容发往串口 
//   USART_SendData(USART1, (unsigned char) ch);
//   while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); 
//   return (ch);
// }
//=============================================================================
//函数名称:USART1_Putc
//功能概要:将USART1_Putc（）内容打印到串口
//参数说明:无
//函数返回:无
//注意   :无
//=============================================================================
void USART1_Putc(unsigned char c)
{
    USART_SendData(USART1, c);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET );
}

//=============================================================================
//函数名称:USART1_Putc
//功能概要:将USART1_Putc（）内容打印到串口
//参数说明:无
//函数返回:无
//注意    :无
//=============================================================================
void USART1_Puts(char * str)
{
    while(*str)
    {
        USART_SendData(USART1, *str++);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    }
}
//=============================================================================
//函数名称:USART2_Putc
//功能概要:将USART2_Putc（）内容打印到串口
//参数说明:无
//函数返回:无
//注意   :无
//=============================================================================
void USART2_Putc(unsigned char c)
{
    USART_SendData(USART2, c);
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET );
}

//=============================================================================
//函数名称:USART2_Putc
//功能概要:将USART2_Putc（）内容打印到串口
//参数说明:无
//函数返回:无
//注意    :无
//=============================================================================
void USART2_Puts(char * str)
{
    while(*str){
        USART_SendData(USART2, *str++);
        while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    }
}
//=============================================================================
//函数名称:USART3_Putc
//功能概要:将USART3_Putc（）内容打印到串口
//参数说明:无
//函数返回:无
//注意   :无
//=============================================================================
void USART3_Putc(unsigned char c)
{
    USART_SendData(USART3, c);
    while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET );
}

//=============================================================================
//函数名称:USART3_Putc
//功能概要:将USART3_Putc（）内容打印到串口
//参数说明:无
//函数返回:无
//注意    :无
//=============================================================================
void USART3_Puts(char * str)
{
    while(*str){
        USART_SendData(USART3, *str++);
        while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
    }
}
//=============================================================================
//函数名称:USART1_Do_Tx
//功能概要:将串口一的数据发送出去，通过中断发送
//参数说明:无
//函数返回:无
//注意    :无
//=============================================================================
void USART1_Do_Tx(void )
{
    if (Usart1_Control_Data.tx_index < Usart1_Control_Data.tx_count) {
		USART_SendData(USART1, Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_index]);
		Usart1_Control_Data.tx_index++;
	}else{
       Usart1_Control_Data.tx_count = 0; 
       Usart1_Control_Data.tx_index = 0;
    }
}
//=============================================================================
//函数名称:USART2_Do_Tx
//功能概要:将串口二的数据发送出去，通过中断发送
//参数说明:无
//函数返回:无
//注意    :无
//=============================================================================
void USART2_Do_Tx(void )
{
    if (Usart2_Control_Data.tx_index < Usart2_Control_Data.tx_count) {
		USART_SendData(USART2, Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_index]);
		Usart2_Control_Data.tx_index++;
	}else{
       Usart2_Control_Data.tx_count = 0; 
       Usart2_Control_Data.tx_index = 0;	   
    }
}
//=============================================================================
//函数名称:USART3_Do_Tx
//功能概要:将串口二的数据发送出去，通过中断发送
//参数说明:无
//函数返回:无
//注意    :无
//=============================================================================
void USART3_Do_Tx(void )
{
    if (Usart3_Control_Data.tx_index < Usart3_Control_Data.tx_count) {
		USART_SendData(USART3, Usart3_Control_Data.txbuf[Usart3_Control_Data.tx_index]);
		Usart3_Control_Data.tx_index++;
	}else{
       Usart3_Control_Data.tx_count = 0; 
       Usart3_Control_Data.tx_index = 0;	   
    }
}
//=============================================================================
//函数名称:USART1_Do_Rx
//功能概要:处理串口一接收的数据
//参数说明:无
//函数返回:无
//注意    :无
//=============================================================================
void USART1_Do_Rx(u8 rxdata)
{       
		static u8 old_frame_end1,old_frame_end2;
    if (0 == Usart1_Control_Data.rx_aframe){
       if (0 == Usart1_Control_Data.rx_index){  //接收第一帧的第一个数据开启定时器3做时间自动成帧处理
           Usart1_Control_Data.rx_start = 1;
           Auto_Frame_Time1 = AUTO_FRAME_TIMEOUT1; 
       }else {
            if (Auto_Frame_Time1 <=0){   //时间超时自动成帧
               Usart1_Control_Data.rx_aframe = 1; 
               Usart1_Control_Data.rx_start = 0;
               Usart1_Control_Data.rx_count = Usart1_Control_Data.rx_index;
               Usart1_Control_Data.rx_index = 0;   //得到一帧数据后及时把索引清零
               return ;   //直接返回不接受数据              
            }                    
        }
				Auto_Frame_Time1 = AUTO_FRAME_TIMEOUT1;
       	Usart1_Control_Data.rxbuf[Usart1_Control_Data.rx_index] = rxdata;
				Usart1_Control_Data.rx_index++;	
				if(Usart1_Control_Data.rx_index >= 2){		//接受数据帧尾成帧
					old_frame_end1 = Usart1_Control_Data.rxbuf[Usart1_Control_Data.rx_index - 2];
					old_frame_end2 = Usart1_Control_Data.rxbuf[Usart1_Control_Data.rx_index - 1];
					if((old_frame_end1 == 0x0D)&&(old_frame_end2 == 0X09)){
							Usart1_Control_Data.rx_aframe = 1;  //接收数据长度自动成帧
							Usart1_Control_Data.rx_count = Usart1_Control_Data.rx_index;
						  Usart1_Control_Data.rx_index = 0;   //得到一帧数据后及时把索引清零
							Usart1_Control_Data.rx_start = 0;
							Auto_Frame_Time1 = AUTO_FRAME_TIMEOUT1; 
					}
				}
        if (Usart1_Control_Data.rx_index > (RxBufMax - 1)){
            Usart1_Control_Data.rx_index = (RxBufMax - 1);
            Usart1_Control_Data.rx_aframe = 1;  //接收数据长度自动成帧
            Usart1_Control_Data.rx_count = Usart1_Control_Data.rx_index+1;
            Usart1_Control_Data.rx_index = 0;   //得到一帧数据后及时把索引清零
            Usart1_Control_Data.rx_start = 0;
            Auto_Frame_Time1 = AUTO_FRAME_TIMEOUT1; 
        }   
    }else{  //接收到一帧数据后必须处理完才可以继续接受下一帧数据
       Usart1_Control_Data.rx_start = 0;
       Auto_Frame_Time1 = AUTO_FRAME_TIMEOUT1; 
        return;
    }           
}
//=============================================================================
//函数名称:USART2_Do_Rx
//功能概要:处理串口二接收的数据
//参数说明:无
//函数返回:无
//注意    :无
//=============================================================================
void USART2_Do_Rx(u8 rxdata)
{       
    if (0 == Usart2_Control_Data.rx_aframe){
       if (0 == Usart2_Control_Data.rx_index){  //接收第一帧的第一个数据开启定时器3做时间自动成帧处理
           Usart2_Control_Data.rx_start = 1;
           Auto_Frame_Time2 = AUTO_FRAME_TIMEOUT2; 
       }else {
            if (Auto_Frame_Time2 <=0){   //时间超时自动成帧
               Usart2_Control_Data.rx_aframe = 1; 
               Usart2_Control_Data.rx_start = 0;
               Usart2_Control_Data.rx_count = Usart2_Control_Data.rx_index;
               Usart2_Control_Data.rx_index = 0;   //得到一帧数据后及时把索引清零
               return ;   //直接返回不接受数据              
            }                    
        }
				Auto_Frame_Time2 = AUTO_FRAME_TIMEOUT2;
       	Usart2_Control_Data.rxbuf[Usart2_Control_Data.rx_index] = rxdata;
        Usart2_Control_Data.rx_index++;
        if (Usart2_Control_Data.rx_index > (RxBufMax - 1)){
            Usart2_Control_Data.rx_index = (RxBufMax - 1);
            Usart2_Control_Data.rx_aframe = 1;  //接收数据长度自动成帧
            Usart2_Control_Data.rx_count = Usart2_Control_Data.rx_index+1;
            Usart2_Control_Data.rx_index = 0;   //得到一帧数据后及时把索引清零
            Usart2_Control_Data.rx_start = 0;
            Auto_Frame_Time2 = AUTO_FRAME_TIMEOUT2; 
        }   
    }else{  //接收到一帧数据后必须处理完才可以继续接受下一帧数据
       Usart2_Control_Data.rx_start = 0;
       Auto_Frame_Time2 = AUTO_FRAME_TIMEOUT2; 
        return;
    }           
}
//=============================================================================
//函数名称:USART3_Do_Rx
//功能概要:处理串口二接收的数据
//参数说明:无
//函数返回:无
//注意    :无
//=============================================================================
void USART3_Do_Rx(u8 rxdata)
{       
    if (0 == Usart3_Control_Data.rx_aframe){
       if (0 == Usart3_Control_Data.rx_index){  //接收第一帧的第一个数据开启定时器3做时间自动成帧处理
           Usart3_Control_Data.rx_start = 1;
           Auto_Frame_Time3 = AUTO_FRAME_TIMEOUT3; 
       }else {
            if (Auto_Frame_Time3 <=0){   //时间超时自动成帧
               Usart3_Control_Data.rx_aframe = 1; 
               Usart3_Control_Data.rx_start = 0;
               Usart3_Control_Data.rx_count = Usart3_Control_Data.rx_index;
               Usart3_Control_Data.rx_index = 0;   //得到一帧数据后及时把索引清零
               return ;   //直接返回不接受数据              
            }                    
        }
				Auto_Frame_Time3 = AUTO_FRAME_TIMEOUT3;
       	Usart3_Control_Data.rxbuf[Usart3_Control_Data.rx_index] = rxdata;
        Usart3_Control_Data.rx_index++;
        if (Usart3_Control_Data.rx_index > (RxBufMax - 1)){
            Usart3_Control_Data.rx_index = (RxBufMax - 1);
            Usart3_Control_Data.rx_aframe = 1;  //接收数据长度自动成帧
            Usart3_Control_Data.rx_count = Usart3_Control_Data.rx_index+1;
            Usart3_Control_Data.rx_index = 0;   //得到一帧数据后及时把索引清零
            Usart3_Control_Data.rx_start = 0;
            Auto_Frame_Time3 = AUTO_FRAME_TIMEOUT3; 
        }   
    }else{  //接收到一帧数据后必须处理完才可以继续接受下一帧数据
       Usart3_Control_Data.rx_start = 0;
       Auto_Frame_Time3 = AUTO_FRAME_TIMEOUT3; 
        return;
    }           
}


void USART1_IRQHandler(void)                	//串口1中断服务程序
	{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res =USART_ReceiveData(USART1);	//读取接收到的数据
		
		if((USART_RX_STA&0x8000)==0)//接收未完成
			{
			if(USART_RX_STA&0x4000)//接收到了0x0d
				{
				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
				}
			else //还没收到0X0D
				{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
					{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
					}		 
				}
			}   		 
     } 
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
} 
#endif	

