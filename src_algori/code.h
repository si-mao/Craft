/*
 * code.h
 * �������
 * �����ߣ���è
 * ����ʱ�䣺2015.02.13
 * �޸��ߣ���ÿ���޸ĺ��뽫����޸�������ĩβ��������˳�򱣳ֲ��䣩
 *     ��è
 * �����޸�ʱ�䣺2015.02.13
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
