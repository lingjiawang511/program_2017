#include"HeadType.h"
#include"stdio.h"
//CRC16 Modbus RTC查询表
const u16 g_McRctable_16[256] =
{
0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};

////逆序CRC计算
//static u16 GetRevCrc_16(u8 * pData, int nLength, u16 init, const u16 *ptable)
//{
//  u16 cRc_16 = init;
//  u8 temp;
//
//  while(nLength-- > 0){
//    temp = cRc_16 >> 8; 
//    cRc_16 = (cRc_16 << 8) ^ ptable[(temp ^ *pData++) & 0xFF];
//  }
//
//  return cRc_16;    
//}


//=============================================================================
//函数名称:GetCrc_16
//功能概要:正序CRC16计算
//参数名称:*pData：16位CRC校验数据；nLength：数据流长度；init：初始化值；*ptable：CRC查询表
//函数返回:CRC16结果
//注意    :无
//=============================================================================
static u16 GetCrc_16(u8 * pData, int nLength, u16 init, const u16 *ptable)
{
  u16 cRc_16 = init;
  u8 temp;

  while(nLength-- > 0){
    temp = cRc_16 & 0xFF; 
    cRc_16 = (cRc_16 >> 8) ^ ptable[(temp ^ *pData++) & 0xFF];
  }
  return cRc_16;
}
//=============================================================================
//函数名称:crc16_ccitt
//功能概要:crc16_ccitt计算，直接计算法
//参数名称:*buf：16位CRC校验数据；len：数据流长度；
//函数返回:CRC16结果
//注意    :此CRC直接计算法需要耗时比较多，占用较多CPU资源
//=============================================================================
static u16 crc16_ccitt(u8 *buf, int len) /*产生16位CRC码的程序*/
{
   u8 i;
   u16 crc=0;
   u16 ccitt=0x1021;
   while(len--!=0){
	  for(i=0x80; i!=0; i/=2){
	      if((crc&0x8000)!=0){
			crc*=2;
			crc^=ccitt;
	      } else{
		  		crc*=2;
		  }/* 余式CRC乘以2再求CRC */
	      if((*buf&i)!=0){
		  	crc^=ccitt;
		  } /* 再加上本位的CRC */
	  }
	  buf++;
   }
   return(crc);
}
/**function: CharToHex() 
*** ACSII change to 16 hex 
*** input:ACSII 
***Return :Hex 
**/  
unsigned char CharToHex(unsigned char bHex)  
{  
    unsigned char res;
    if(bHex<=9){  
        res =bHex + 0x30;  
    }else if((bHex>=10)&&(bHex<=15)){ //Capital 
        res =bHex + 0x37;  
    }else{  
        res = 0xff;  
    }  
    return res;  
}  

//函 数 名：HexToChar() 
//功能描述：把16进制转换为ASCII字符 
//函数说明： //调用函数： 
//全局变量： 
//输 入：16进制 
//返 回：ASCII字符

unsigned char HexToChar(unsigned char bChar)  
{  
    if((bChar>=0x30)&&(bChar<=0x39))  {  
        bChar -= 0x30;  
    }else if((bChar>=0x41)&&(bChar<=0x46)){  // Capital 
        bChar -= 0x37;  
    }else if((bChar>=0x61)&&(bChar<=0x66)){ //littlecase  
        bChar -= 0x57;  
    }else{  
        bChar = 0xff;  
    }  
    return bChar;  
}
//=============================================================================
//函数名称:LRC16
//功能概要:LRC校验方式= BUF所有字节累加，然后取反加一
//参数名称:*buf：16位LRC校验数据；len：数据流长度；
//函数返回:LRC16结果
//=============================================================================
static u16 LRC16(u8 *buf, int len) /*产生16位CRC码的程序*/
{
   u8 lrc = 0;
   u16 lrc_res=0;
   while(len--!=0){
    lrc += *buf;
	  buf++;
   }
   lrc = ~lrc;
   lrc = lrc + 1;
   lrc_res = (CharToHex(lrc/16))*256 + CharToHex(lrc%16);
   return(lrc_res);
}
//=============================================================================
//函数名称:CRC_GetModbus16
//功能概要:Modbus RTC 校验计算
//参数名称:*pdata：16位CRC校验数据；len：数据流长度；
//函数返回:CRC16结果
//注意    :
//=============================================================================
u16 CRC_GetModbus16(u8 *pdata, int len)
{
//MODBUS CRC-16表 8005 逆序
  return GetCrc_16(pdata, len, 0xFFFF, g_McRctable_16);
}
//=============================================================================
//函数名称:CRC_GetCCITT
//功能概要:CRC16 CCITT 校验计算
//参数名称:*pdata：16位CRC校验数据；len：数据流长度；
//函数返回:CRC16结果
//注意    :此CRC直接计算法需要耗时比较多，占用较多CPU资源
//=============================================================================
u16 CRC_GetCCITT(u8 *pdata, int len)
{
	return crc16_ccitt(pdata,len);
}

//=============================================================================
//函数名称:LRC_GetLRC16
//功能概要:LRC_GetLRC16 校验计算
//参数名称:*pdata：16位LRC校验数据；len：数据流长度；
//函数返回:CRC16结果
//=============================================================================
u16 LRC_GetLRC16(u8 *pdata, int len)
{
	return LRC16(pdata,len);
}
