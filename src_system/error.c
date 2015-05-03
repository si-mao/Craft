/*
 * error.c
 * 错误处理
 * 创建者：死猫
 * 创建时间：2015.03.14
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.04.06
 */

#include "system_define.h"
#include "error.h"

#ifdef ERROR_LED_ALARM
#include "stm32_led.h"
#endif

__weak void error_report_callback(uint32_t error_code)
{
}

volatile uint32_t last_error = ERROR_CODE_NO_ERROR;

void error_report(uint32_t error_code)
{
	last_error = error_code;
#ifdef ERROR_LED_ALARM
	ERROR_LED_ALARM
#endif
	error_report_callback(error_code);
}

void HardFault_Handler(void)
{
  error_alarm(ERROR_CODE_EX_HARDWARE | ERROR_CODE_HARDFAULT);
  while (1)
  {
  }
}

void BusFault_Handler(void)
{
  error_alarm(ERROR_CODE_EX_BUS | ERROR_CODE_HARDFAULT);
  while (1)
  {
  }
}

void UsageFault_Handler(void)
{
  error_alarm(ERROR_CODE_EX_USAGE | ERROR_CODE_HARDFAULT);
  while (1)
  {
  }
}
