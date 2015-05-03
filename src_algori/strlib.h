/*
 * strlib.h
 * �ַ�������
 * �����ߣ���è
 * ����ʱ�䣺2015.03.15
 * �޸��ߣ���ÿ���޸ĺ��뽫����޸�������ĩβ��������˳�򱣳ֲ��䣩
 *     ��è
 * �����޸�ʱ�䣺2015.03.15
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
