/*
 * cc1101_mgr.h
 * cc1101内存管理
 * 创建者：死猫
 * 创建时间：2015.04.24
 * 修改者：(每次修改后, 请将最后修改者置于末尾, 其他人顺序保持不变)
 *     死猫,
 * 最终修改时间：2015.04.24
 */
 
#ifndef CC1101_MGR_H_
#define CC1101_MGR_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "system_define.h"



extern void cc1101_mgr_init(void);
extern void cc1101_mgr_recv_start(void);
extern void cc1101_mgr_send_start(const uint8_t *pack, uint8_t len);
extern void cc1101_mgr_loop(void);

extern void cc1101_mgr_read(uint8_t *pack, uint32_t *error);
extern void cc1101_mgr_write(const uint8_t *pack, uint8_t len, uint32_t *error);




#ifdef __cplusplus
};
#endif

#endif
