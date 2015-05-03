/*
 * usart_mgr.h
 * usart�ڴ����
 * �����ߣ���è
 * ����ʱ�䣺2015.04.24
 * �޸��ߣ�(ÿ���޸ĺ�, �뽫����޸�������ĩβ, ������˳�򱣳ֲ���)
 *     ��è,
 * �����޸�ʱ�䣺2015.04.25
 */
 
#ifndef USART_MGR_H_
#define USART_MGR_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "system_define.h"
#include "queue.h"


extern void usart_mgr_init(void);
extern void usart_mgr_loop(void);

extern void usart_mgr_read(uint8_t *pack, uint32_t *error);
extern void usart_mgr_fifo_write(uint8_t ch);
extern void usart_mgr_dma_write(const uint8_t *pack, uint8_t len, uint32_t *error);

#ifdef __cplusplus
};
#endif

#endif
