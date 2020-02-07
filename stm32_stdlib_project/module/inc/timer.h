#ifndef _TIMER_H_
#define _TIMER_H_

#include "stm32f10x.h"
#include "sys.h"
#include "led.h"
#include "usart.h"
/* LIST10: TIM 2, 3, 4 */
#define IS_TIM_LIST10_PERIPH(PERIPH)  (((PERIPH) == TIM2) || \
                                      ((PERIPH) == TIM3) || \
                                      ((PERIPH) == TIM4))  

#define START_TIME(TIMx)  TIM_Cmd(TIMx, ENABLE);TIM_SetCounter(TIMx,0)
#define STOP_TIME(TIMx)  TIM_Cmd(TIMx, DISABLE)

void TIMx_Current_Config(TIM_TypeDef* TIMx,u16 TIM_Period,u16 TIM_Prescaler);
void TIMx_Current_NVIC_Config(TIM_TypeDef* TIMx,uint32_t PreemptPriority, uint32_t SubPriority);

#endif
