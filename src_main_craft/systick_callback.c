/*
 * systick_callback.h
 * Systick����ص�
 * �����ߣ���è
 * ����ʱ�䣺2015.03.15
 * �޸��ߣ���ÿ���޸ĺ��뽫����޸�������ĩβ��������˳�򱣳ֲ��䣩
 *     ��è
 * �����޸�ʱ�䣺2015.03.15
 */

#include "system_define.h"
#include "error.h"

#include "craft_data.h"

#define RF_RECV_TIMEOUT 400

extern volatile uint16_t rf_recv_timeout;

void systick_callback(void)
{
	if(flags.power == POWER_ON && flags.run == RUN_MOV && flags.rc_recved != 0)
	{
		rf_recv_timeout++;
		if(rf_recv_timeout > RF_RECV_TIMEOUT)
		{
			error_alarm(ERROR_CODE_EX_RF | ERROR_CODE_LOST_SIGNAL);
		}
	}
}
