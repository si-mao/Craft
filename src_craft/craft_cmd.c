/*
 * craft_cmd.h
 * 命令
 * 创建者：死猫
 * 创建时间：2015.03.15
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.03.15
 */

#include "system_define.h"

#include "stm32_pwm.h"

#include "craft_data.h"
#include "calcu_pwm.h"

void craft_cmd_halt(void)
{
	int8_t i;
	int16_t pwm[4];
	for(i = 0; i < 4; i++) pwm[i] = 0;
	
	pwm_set(pwm);
	calcu_pwm_init();
	craft_data_init();
}
