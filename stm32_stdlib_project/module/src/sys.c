#include "sys.h"

void STM32_NVIC_SetPriority(IRQn_Type IRQn,uint32_t PreemptPriority, uint32_t SubPriority)
{
	NVIC_InitTypeDef NVIC_InitStruct;

	/* 设置中断分组 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStruct.NVIC_IRQChannel = IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = PreemptPriority;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = SubPriority;
    NVIC_Init(&NVIC_InitStruct);
}

//关闭所有中断
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//开启所有中断
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
} 


