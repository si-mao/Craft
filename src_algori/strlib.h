/*
 * strlib.h
 * 字符串处理
 * 创建者：死猫
 * 创建时间：2015.03.15
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.03.15
 */

#ifndef STRLIB_H_
#define STRLIB_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "system_define.h"

extern void strcpy(uint8_t* dst, const uint8_t* src, uint8_t len);
extern uint8_t* strfind(const uint8_t* src, uint8_t src_len, const uint8_t* dst, uint8_t dst_len);

#ifdef __cplusplus
};
#endif

#endif
