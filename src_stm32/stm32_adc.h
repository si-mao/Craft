/*
 * stm32_adc.h
 * adc
 * 创建者：死猫
 * 创建时间：2015.03.15
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.03.15
 */
 
#ifndef STM32_ADC_H_
#define STM32_ADC_H_

#include "system_define.h"

#ifdef __cplusplus
extern "C"{
#endif

extern void adc_init(void);
extern void adc_start_convert(uint8_t ch);
extern uint8_t adc_is_convert_finish(void);
extern uint16_t adc_get_value(void);

#ifdef __cplusplus
};
#endif

#endif
