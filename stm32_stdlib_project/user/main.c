#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "stdio.h"
#include "led.h"
#include "beep.h"
int main(void)
{
	LED_GPIO_Config();
	BEEP_GPIO_Config();
	Delay_Init();
	USARTx_Config(USART1,115200);
	USARTx_NVIC_Config(USART1,1,1);
	TIMx_Current_Config(TIM2,1000,72);
	TIMx_Current_NVIC_Config(TIM2,1,0);
	while(1)
	{
		if(USART1_RX_FLAG)
		{
			USART1_RX_BUFF[USART1_RX_COUNT]='\0';
			USARTx_SendString(USART1,(char *)USART1_RX_BUFF);
			USART1_RX_COUNT=0;
			USART1_RX_FLAG=0;
		}
		// BEEP=1;
		LED1=0;
		LED0=1;
		DelayMs(500);
		// BEEP=0;
		LED1=1;
		LED0=0;
		DelayMs(500);
	}
}
