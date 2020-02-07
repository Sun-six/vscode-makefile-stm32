#ifndef _BEEP_H_
#define _BEEP_H_

#include "stm32f10x.h"
#include "sys.h"
//BEEP PB8;
#define BEEP PBout(8)
					
void BEEP_GPIO_Config(void);

#endif
