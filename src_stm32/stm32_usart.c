/*
 * stm32_usart.c
 * ����
 * �����ߣ���è
 * ����ʱ�䣺2015.02.13
 * �޸��ߣ���ÿ���޸ĺ��뽫����޸�������ĩβ��������˳�򱣳ֲ��䣩
 *     ��è
 * �����޸�ʱ�䣺2015.02.13
 */

#include "system_define.h"
#include "stm32_config.h"
#include "error.h"

//115200�����ʣ�һ�ַ�1���ֽڣ�Լ9us
void usart_config(void)
{
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate            = 115200;               //������115200
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;  //8λ����
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;     //ֹͣλ1λ
	USART_InitStructure.USART_Parity              = USART_Parity_No ;     //��У��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, DISABLE);                                           //��ֹʹ��
}

void usart_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);  //ʹ�ܸ����˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                                      //���ô��ڽ��ն˿ڹҽӵ�9�˿�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                                //���ù��������©
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                              //���ö˿��ٶ�Ϊ50M
	GPIO_Init(GPIOA, &GPIO_InitStructure);                                         //���ݲ�����ʼ��GPIOA�Ĵ���	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;                          //��������(��λ״̬);	   				
	GPIO_Init(GPIOA, &GPIO_InitStructure);                                         //���ݲ�����ʼ��GPIOA�Ĵ���

	usart_config();

#if (UART_DMA_EN == 1)
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
#endif
}
