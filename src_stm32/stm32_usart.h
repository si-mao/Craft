/*
 * stm32_usart.h
 * 串口
 * 创建者：死猫
 * 创建时间：2015.02.13
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.02.13
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
