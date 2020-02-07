#ifndef DELAY_H
#define DELAY_H
#include "stm32f10x.h"
void Delay_Init(void);	    								   
void DelayUs(u32 nus);
void DelayMs(u16 nms);
#endif
