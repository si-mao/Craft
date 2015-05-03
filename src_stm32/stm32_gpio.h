
#ifndef STM32_GPIO_H_
#define STM32_GPIO_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "hardware.h"		//io¶Ë¿Úºê¶¨Òå

#define TSET_1 PGout(2)
#define TSET_2 PGout(3)

extern void gpio_init(void);

#ifdef __cplusplus
};
#endif

#endif
