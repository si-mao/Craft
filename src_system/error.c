/*
 * error.c
 * ������
 * �����ߣ���è
 * ����ʱ�䣺2015.03.14
 * �޸��ߣ���ÿ���޸ĺ��뽫����޸�������ĩβ��������˳�򱣳ֲ��䣩
 *     ��è
 * �����޸�ʱ�䣺2015.04.06
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
