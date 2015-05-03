/*
 * craft_conf.h
 * ����
 * �����ߣ���è
 * ����ʱ�䣺2015.03.22
 * �޸��ߣ�(ÿ���޸ĺ�, �뽫����޸�������ĩβ, ������˳�򱣳ֲ���)
 *     ��è,
 * �����޸�ʱ�䣺2015.03.22
 */

#ifndef CRAFT_CONF_H_
#define CRAFT_CONF_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "system_define.h"

#define CALIBRATE_LEVEL_GYRO 512
#define CALIBRATE_LEVEL_ACC 512
#define CALIBRATE_LEVEL_TRIM 512

#define ACC_ADC_1G 2048
#define ACC_1G 9.8f
#define ACC_1G_SQ (ACC_1G * ACC_1G)

#define MINCHECK 150

#ifdef __cplusplus
};
#endif

#endif
