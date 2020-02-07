#include "exti.h"

void XPT2046_EXIT_Config(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	//��������ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	//���ⲿ�ж�
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,GPIO_PinSource10);
	//�����ⲿ�ж�
	EXTI_InitStructure.EXTI_Line = EXTI_Line10;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	//�����ⲿ�ж�NVIC���ȼ�
	STM32_NVIC_SetPriority(EXTI15_10_IRQn,0,1);
}


