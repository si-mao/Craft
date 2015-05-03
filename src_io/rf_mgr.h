/*
 * rf_mgr.h
 * 无线管理
 * 创建者：死猫
 * 创建时间：2015.04.24
 * 修改者：(每次修改后, 请将最后修改者置于末尾, 其他人顺序保持不变)
 *     死猫,
 * 最终修改时间：2015.04.24
 */
 
#ifndef RF_MGR_H_
#define RF_MGR_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "cc1101_mgr.h"

#define rf_mgr_init       cc1101_mgr_init
#define rf_mgr_recv_start cc1101_mgr_recv_start
#define rf_mgr_loop       cc1101_mgr_loop

#define rf_mgr_read       cc1101_mgr_read
#define rf_mgr_write      cc1101_mgr_write

#ifdef __cplusplus
};
#endif

#endif
