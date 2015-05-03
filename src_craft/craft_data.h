/*
 * craft_data.h
 * ���ݶ���
 * �����ߣ���è
 * ����ʱ�䣺2015.01.31
 * �޸��ߣ�(ÿ���޸ĺ�, �뽫����޸�������ĩβ, ������˳�򱣳ֲ���)
 *     ��è,
 * �����޸�ʱ�䣺2015.02.26
 */

#ifndef CRAFT_DATA_H_
#define CRAFT_DATA_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "system_define.h"

#include "craft_type.h"
#include "craft_conf.h"



extern conf_t conf;
extern flags_t flags;
extern craft_data_t craft_data;
extern int16_t rcData[RC_CHANS];
extern int16_t rcCommand[4];

extern void craft_data_init(void);

#ifdef __cplusplus
};
#endif

#endif
