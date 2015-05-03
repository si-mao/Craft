/*
 * stm32_spi.h
 * SPI通信模块
 * 创建者：死猫
 * 创建时间：2015.01.16
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.01.19
 */
 
#ifndef STM32_SPI_H_
#define STM32_SPI_H_

#include "system_define.h"

#ifdef __cplusplus
extern "C"{
#endif

extern void spi_init(void);
extern void spi_set_baud(SPI_TypeDef* SPIx, uint16_t SPI_BaudRatePrescaler);

extern uint8_t spi1_rw(uint8_t dat);
extern uint8_t spi2_rw(uint8_t dat);


#ifdef __cplusplus
};
#endif

#endif
