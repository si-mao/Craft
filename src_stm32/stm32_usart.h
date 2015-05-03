/*
 * stm32_usart.h
 * ����
 * �����ߣ���è
 * ����ʱ�䣺2015.02.13
 * �޸��ߣ���ÿ���޸ĺ��뽫����޸�������ĩβ��������˳�򱣳ֲ��䣩
 *     ��è
 * �����޸�ʱ�䣺2015.02.13
 */

#ifndef STM32_USART_H
#define STM32_USART_H

#ifdef __cplusplus
extern "C"{
#endif

#include "system_define.h"

extern void usart_init(void);
extern void usart_dma_send(uint8_t *buf, uint8_t len);

__INLINE void usart_put_char(uint16_t data)
{
	while (!(USART1->SR & USART_FLAG_TXE));
	USART1->DR = (data & (uint16_t)0x01FF);
}

#ifdef __cplusplus
};
#endif

#endif
