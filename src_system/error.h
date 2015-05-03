/*
 * error.h
 * 错误处理
 * 创建者：死猫
 * 创建时间：2015.03.14
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.04.21
 */

#ifndef ERROR_H_
#define ERROR_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "system_define.h"

#define ERROR_CODE_NO_ERROR           0x00000000
#define ERROR_CODE_HARDFAULT          0x00000001
#define ERROR_CODE_SOFTFAULT          0x00000002
#define ERROR_CODE_TIME_OUT           0x00000003
#define ERROR_CODE_BUSY               0x00000004
	
#define ERROR_CODE_LOST_SIGNAL        0x00000101

#define ERROR_CODE_EX_NO_INF          0x00000000
#define ERROR_CODE_EX_HARDWARE        0x00010000
#define ERROR_CODE_EX_BUS             0x00020000
#define ERROR_CODE_EX_USAGE           0x00030000
#define ERROR_CODE_EX_UART            0x00040000
#define ERROR_CODE_EX_SPI             0x00050000
	
#define ERROR_CODE_EX_RF              0x01010000
#define ERROR_CODE_EX_CC1101          0x01020000

extern volatile uint32_t last_error;

#define error_alarm(error_code) ((error_code) ? error_report(error_code) : (void)0)
extern void error_report(uint32_t error_code);
__INLINE uint32_t GetLastError(void) { return last_error; }

#ifdef __cplusplus
};
#endif

#endif
