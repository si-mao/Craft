/*
 * stm32_spi.h
 * SPIͨ��ģ��
 * �����ߣ���è
 * ����ʱ�䣺2015.01.16
 * �޸��ߣ���ÿ���޸ĺ��뽫����޸�������ĩβ��������˳�򱣳ֲ��䣩
 *     ��è
 * �����޸�ʱ�䣺2015.01.19
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
