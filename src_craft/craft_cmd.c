/*
 * craft_cmd.h
 * ����
 * �����ߣ���è
 * ����ʱ�䣺2015.03.15
 * �޸��ߣ���ÿ���޸ĺ��뽫����޸�������ĩβ��������˳�򱣳ֲ��䣩
 *     ��è
 * �����޸�ʱ�䣺2015.03.15
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
