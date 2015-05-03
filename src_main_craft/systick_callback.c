/*
 * systick_callback.h
 * Systick处理回调
 * 创建者：死猫
 * 创建时间：2015.03.15
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.03.15
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
