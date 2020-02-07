#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include "stm32f10x.h"

//位带操作相关宏定义
/*
位带操作参考“M3权威指南”第五章 92页
*/
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr&0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr) *((volatile unsigned long *) (addr))
//MEM_ADDR(BITBAND(DEVICE_REG0,1)) = 0x1; //使用
//寄存器的地址
#define GPIOA_IDR_ADDR (0x40010800+0x08) 
#define GPIOA_ODR_ADDR (0x40010800+0x0C)
#define GPIOB_IDR_ADDR (0X40010C00+0x08) 
#define GPIOB_ODR_ADDR (0X40010C00+0x0C)
#define GPIOC_IDR_ADDR (0x40011000+0x08) 
#define GPIOC_ODR_ADDR (0x40011000+0x0C)
#define GPIOD_IDR_ADDR (0x40011400+0x08) 
#define GPIOD_ODR_ADDR (0x40011400+0x0C)
#define GPIOE_IDR_ADDR (0x40011800+0x08) 
#define GPIOE_ODR_ADDR (0x40011800+0x0C)
#define GPIOF_IDR_ADDR (0x40011C00+0x08) 
#define GPIOF_ODR_ADDR (0x40011C00+0x0C)
#define GPIOG_IDR_ADDR (0x40012000+0x08) 
#define GPIOG_ODR_ADDR (0x40012000+0x0C)

#define PAin(x)  MEM_ADDR(BITBAND(GPIOA_IDR_ADDR,x))
#define PAout(x) MEM_ADDR(BITBAND(GPIOA_ODR_ADDR,x))
#define PBin(x)  MEM_ADDR(BITBAND(GPIOB_IDR_ADDR,x))
#define PBout(x) MEM_ADDR(BITBAND(GPIOB_ODR_ADDR,x))
#define PCin(x)  MEM_ADDR(BITBAND(GPIOC_IDR_ADDR,x))
#define PCout(x) MEM_ADDR(BITBAND(GPIOC_ODR_ADDR,x))
#define PDin(x)  MEM_ADDR(BITBAND(GPIOD_IDR_ADDR,x))
#define PDout(x) MEM_ADDR(BITBAND(GPIOD_ODR_ADDR,x))
#define PEin(x)  MEM_ADDR(BITBAND(GPIOE_IDR_ADDR,x))
#define PEout(x) MEM_ADDR(BITBAND(GPIOE_ODR_ADDR,x))
#define PFin(x)  MEM_ADDR(BITBAND(GPIOF_IDR_ADDR,x))
#define PFout(x) MEM_ADDR(BITBAND(GPIOF_ODR_ADDR,x))
#define PGin(x)  MEM_ADDR(BITBAND(GPIOG_IDR_ADDR,x))
#define PGout(x) MEM_ADDR(BITBAND(GPIOG_ODR_ADDR,x))

void STM32_NVIC_SetPriority(IRQn_Type IRQn,uint32_t PreemptPriority, uint32_t SubPriority);
void INTX_DISABLE(void);
void INTX_ENABLE(void);
#endif
