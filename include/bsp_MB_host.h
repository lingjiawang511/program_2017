#ifndef __BSP_MB_HOST_H__
#define __BSP_MB_HOST_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f10x.h"
#include"HeadType.h"
/* 类型定义 ------------------------------------------------------------------*/

/* 宏定义 --------------------------------------------------------------------*/
#define MB_SLAVEADDR        0x0001
#define MB_REG_ADDR         0x0000//寄存器地址
/* 扩展变量 ------------------------------------------------------------------*/

/* 函数声明 ------------------------------------------------------------------*/
uint16_t MB_CRC16(uint8_t *pushMsg,uint8_t usDataLen);
void MB_ReadCoil_01H(Usart_Type *usart,uint8_t _addr, uint16_t _reg, uint16_t _num);
void MB_WriteCoil_05H(Usart_Type *usart,uint8_t _addr, uint16_t _reg, uint16_t _sta);
void MB_ReadInput_02H(Usart_Type *usart,uint8_t _addr, uint16_t _reg, uint16_t _num);
void MB_ReadHoldingReg_03H(Usart_Type *usart,uint8_t _addr, uint16_t _reg, uint16_t _num);
void MB_ReadInputReg_04H(Usart_Type *usart,uint8_t _addr, uint16_t _reg, uint16_t _num);
void MB_WriteHoldingReg_06H(Usart_Type *usart,uint8_t _addr, uint16_t _reg, uint16_t _data);
void MB_WriteNumHoldingReg_10H(Usart_Type *usart,uint8_t _addr, uint16_t _reg, uint16_t _num,uint8_t *_databuf);
#endif /* __BSP_MB_HOST_H__ */


