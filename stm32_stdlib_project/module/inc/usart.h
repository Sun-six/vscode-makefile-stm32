#ifndef _USART_H_
#define _USART_H_

#include "stm32f10x.h"
#include <stdio.h>
#include "delay.h"
#include "sys.h"
#include "timer.h"

/*串口接收缓冲区大小*/
#define USART3_RX_SIZE 64
#define USART2_RX_SIZE 64
#define USART1_RX_SIZE 64
/*串口接收缓冲区有数据标志*/
extern u8 USART1_RX_FLAG;
extern u8 USART2_RX_FLAG;
extern u8 USART3_RX_FLAG;
/*串口接收缓冲区*/
extern u8 USART1_RX_BUFF[USART1_RX_SIZE];
extern u8 USART2_RX_BUFF[USART2_RX_SIZE];
extern u8 USART3_RX_BUFF[USART3_RX_SIZE];
/*串口接收缓冲区有效数据个数*/
extern u32 USART3_RX_COUNT;
extern u32 USART2_RX_COUNT;
extern u32 USART1_RX_COUNT;

//void USART1_Config(void);
void USARTx_Config(USART_TypeDef *USARTx,int USART_BaudRate);
void USARTx_NVIC_Config(USART_TypeDef *USARTx,uint32_t PreemptPriority, uint32_t SubPriority);
u32 USARTx_SendString(USART_TypeDef *USARTx,char *SendString);
void USARTx_SendByte(USART_TypeDef *USARTx,unsigned char SendData);
#endif
