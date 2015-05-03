/*
 * craft_data.h
 * 数据定义
 * 创建者：死猫
 * 创建时间：2015.01.31
 * 修改者：(每次修改后, 请将最后修改者置于末尾, 其他人顺序保持不变)
 *     死猫,
 * 最终修改时间：2015.02.26
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
