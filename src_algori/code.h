/*
 * code.h
 * 编码解码
 * 创建者：死猫
 * 创建时间：2015.02.13
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.02.13
 */
 
#ifndef CODE_H_
#define CODE_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "system_define.h"

extern void code_encode_16bits(const uint16_t *src, uint8_t *dst, uint8_t len);
extern void code_encode_32bits(const uint32_t *src, uint8_t *dst, uint8_t len);
extern void code_decode_16bits(const uint8_t *src, uint16_t *dst, uint8_t len);
extern void code_decode_32bits(const uint8_t *src, uint32_t *dst, uint8_t len);


#ifdef __cplusplus
};
#endif

#endif
