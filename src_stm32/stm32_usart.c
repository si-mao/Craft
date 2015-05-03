/*
 * stm32_usart.c
 * 串口
 * 创建者：死猫
 * 创建时间：2015.02.13
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.02.13
 */

#include "system_define.h"
#include "stm32_config.h"
#include "error.h"

//115200波特率，一字符1个字节，约9us
void usart_config(void)
{
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate            = 115200;               //波特率115200
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;  //8位数据
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;     //停止位1位
	USART_InitStructure.USART_Parity              = USART_Parity_No ;     //无校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, DISABLE);                                           //禁止使能
}

void usart_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);  //使能各个端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                                      //配置串口接收端口挂接到9端口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                                //复用功能输出开漏
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              //配置端口速度为50M
	GPIO_Init(GPIOA, &GPIO_InitStructure);                                         //根据参数初始化GPIOA寄存器	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;                          //浮空输入(复位状态);	   				
	GPIO_Init(GPIOA, &GPIO_InitStructure);                                         //根据参数初始化GPIOA寄存器

	usart_config();

#if (UART_DMA_EN == 1)
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
#endif
}
