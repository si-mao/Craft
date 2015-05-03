/*
 * kalman.h
 * �������˲�
 * �����ߣ���è
 * ����ʱ�䣺2015.01.26
 * �޸��ߣ�(ÿ���޸ĺ�, �뽫����޸�������ĩβ, ������˳�򱣳ֲ���)
 *     ��è,
 * �����޸�ʱ�䣺2015.01.26
 */

#ifndef KALMAN_H_
#define KALMAN_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "system_define.h"

#include "quat.h"

extern void kalman_init(void);
extern void kalman_calcu(QUAT_TYPE atti, const QUAT_TYPE atti_gyro, const QUAT_TYPE atti_field, const CALCU_TYPE* wb);

#ifdef __cplusplus
};
#endif

#endif
