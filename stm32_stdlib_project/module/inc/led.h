#if !defined(LED_H)
#define LED_H

#include "stm32f10x.h"
#include "sys.h"

#define LED0 PBout(5)
#define LED1 PEout(5)

void LED_GPIO_Config(void);

#endif // LED_H

