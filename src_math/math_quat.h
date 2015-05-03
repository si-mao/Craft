/*
 * math_quat.h
 * ��Ԫ��
 * �����ߣ���è
 * ����ʱ�䣺2015.01.18
 * �޸��ߣ�(ÿ���޸ĺ�, �뽫����޸�������ĩβ, ������˳�򱣳ֲ���)
 *     ��è,
 * �����޸�ʱ�䣺2015.02.24
 */

#ifndef QUAT_H_
#define QUAT_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "system_define.h"

#define QUAT_TYPE CALCU_TYPE*

#define QUAT_DECLARE(x) CALCU_TYPE x[4];

#define QUAT_SET_VALUE(x, d0, d1, d2, d3) { x[0] = d0; x[1] = d1; x[2] = d2; x[3] = d3; }

extern void quat_copy(QUAT_TYPE q1, const QUAT_TYPE q2);
extern void quat_add(QUAT_TYPE q1, const QUAT_TYPE q2);
extern void quat_dec(QUAT_TYPE q1, const QUAT_TYPE q2);
extern void quat_mul(QUAT_TYPE q1, const QUAT_TYPE q2);
extern void quat_mul_num(QUAT_TYPE q, const CALCU_TYPE num);
extern void quat_to_eular(const QUAT_TYPE q, QUAT_TYPE eular);
extern void quat_from_eular(QUAT_TYPE qout, const QUAT_TYPE eular);
extern void quat_unit(QUAT_TYPE q);
extern void quat_inverse(QUAT_TYPE q);
extern void quat_conjugation(QUAT_TYPE q);

#ifdef __cplusplus
};
#endif

#endif
