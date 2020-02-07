#include "usart.h"
#include "stm32f10x_usart.h"
/* 串口相关定义 */
u8 USART1_RX_FLAG=0;
u8 USART1_RX_BUFF[USART1_RX_SIZE];
u32 USART1_RX_COUNT=0;

u8 USART2_RX_FLAG=0;
u8 USART2_RX_BUFF[USART2_RX_SIZE];
u32 USART2_RX_COUNT=0;

u8 USART3_RX_FLAG=0;
u8 USART3_RX_BUFF[USART3_RX_SIZE];
u32 USART3_RX_COUNT=0;
/*
函数功能: 串口1、2、3的初始化
函数参数: 
	USART_TypeDef *USARTx  :串口指针。 USART1\USART2\USART3
	u32 baud  : 波特率
串口1硬件连接:
	TX-PA9   复用推挽输出 0xB
	RX-PA10  上下拉输入   0x8
串口2硬件连接:
	TX-PA2   复用推挽输出 0xB
	RX-PA3   上下拉输入   0x8
串口3硬件连接:
	TX-PB10   复用推挽输出 0xB
	RX-PB11   上下拉输入   0x8		
*/
void USART1_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 使能 USART1 时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE); 

	/* USART1 使用IO端口配置 */    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);   //初始化GPIOA
}
void USART2_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 使能 USART1 时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB2Periph_GPIOA, ENABLE); 

	/* USART1 使用IO端口配置 */    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);   //初始化GPIOA
}
void USART3_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 使能 USART1 时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART3 | RCC_APB2Periph_GPIOB, ENABLE); 

	/* USART1 使用IO端口配置 */    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);   //初始化GPIOA
}
/*USARTx配置函数*/
void USARTx_Config(USART_TypeDef *USARTx,int USART_BaudRate)
{	
	USART_InitTypeDef USART_InitStructure;
	if(USARTx==USART1)
		USART1_GPIO_Config();
	if(USARTx==USART2)
		USART2_GPIO_Config();
	if(USARTx==USART3)
		USART3_GPIO_Config();
	if(IS_USART_123_PERIPH(USARTx))
	{
		/* USARTx 工作模式配置 */
		USART_InitStructure.USART_BaudRate = USART_BaudRate;	//波特率设置
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//数据位数设置：8位
		USART_InitStructure.USART_StopBits = USART_StopBits_1; 	//停止位设置：1位
		USART_InitStructure.USART_Parity = USART_Parity_No ;  //是否奇偶校验：无
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制模式设置：没有使能
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//接收与发送都使能
		USART_Init(USARTx, &USART_InitStructure);  //初始化USART1
		USART_Cmd(USARTx, ENABLE);// USART1使能
	}
}
/*USARTx中断配置函数*/
void USARTx_NVIC_Config(USART_TypeDef *USARTx,uint32_t PreemptPriority, uint32_t SubPriority)
{
	if(USARTx==USART1)
		STM32_NVIC_SetPriority(USART1_IRQn,PreemptPriority,SubPriority);
	if(USARTx==USART2)
		STM32_NVIC_SetPriority(USART1_IRQn,PreemptPriority,SubPriority);
	if(USARTx==USART3)
		STM32_NVIC_SetPriority(USART1_IRQn,PreemptPriority,SubPriority);
	if(IS_USART_123_PERIPH(USARTx))
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}


 /*发送一个字节数据*/
void USARTx_SendByte(USART_TypeDef *USARTx,unsigned char SendData)
{	   
	USART_SendData(USARTx,SendData);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);	    
}	

 /*发送一个字符串数据*/
u32 USARTx_SendString(USART_TypeDef *USARTx,char *SendString)
{
	u32 count=0;
	while(*SendString != '\0')
	{
		USARTx_SendByte(USARTx,(u8)*SendString);
		SendString++;
		count++;
	}
	return count;
}
/*printf底层调用代码*/
int _write(int fd, char *ptr, int len)
{
    int i = 0;

    /*
     * write "len" of char from "ptr" to file id "fd"
     * Return number of char written.
     *
    * Only work for STDOUT, STDIN, and STDERR
     */
    if (fd > 2)
    {
        return -1;
    }

    while (*ptr && (i < len))
    {
        USARTx_SendByte(USART1, *ptr);

        if (*ptr == '\n')
        {
            USARTx_SendByte(USART1, '\r');
        }

        i++;
        ptr++;
    }

    return i;
}

/* USART1_IRQHandler中断服务函数 */
void USART1_IRQHandler(void)
{
    if (USART_GetFlagStatus(USART1,USART_IT_RXNE) && !USART1_RX_FLAG)
    {
		START_TIME(TIM2);
		USART1_RX_BUFF[USART1_RX_COUNT]=USART_ReceiveData(USART1);
		USART1_RX_COUNT++;
    }
}

