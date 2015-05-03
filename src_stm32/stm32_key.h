#ifndef STM32_KEY_H_
#define STM32_KEY_H_

//
#define KEY1 PEin(0)			 //∞Â…œKEY±‡∫≈ S4, KEY1
#define KEY2 PCin(13)			 //∞Â…œKEY±‡∫≈ S3, KEY2

#ifdef __cplusplus
extern "C"{
#endif

extern void key_init(void);

#ifdef __cplusplus
};
#endif

#endif
