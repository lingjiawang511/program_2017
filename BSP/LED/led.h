#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define LED0 PAout(1)	// PA1
#define LED1 PAout(0)	// PA0	

extern u16 Led_Flash_Time;

void LED_Init(void);//≥ı ºªØ
void LED_Display(void);
		 				    
#endif
