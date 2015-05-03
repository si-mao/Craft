/*
 * usart_mgr.h
 * usart内存管理
 * 创建者：死猫
 * 创建时间：2015.04.24
 * 修改者：(每次修改后, 请将最后修改者置于末尾, 其他人顺序保持不变)
 *     死猫,
 * 最终修改时间：2015.04.25
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
