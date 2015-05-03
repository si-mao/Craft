/*
 * package.h
 * 打包解包
 * 创建者：死猫
 * 创建时间：2015.01.24
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.03.29
 */

#include "io_data.h"
#include "rf_mgr.h"

#include "package.h"
#include "code.h"

#include "craft_data.h"
#include "craft_cmd.h"

#include "imu_attitude.h"
#include "imu_physics.h"
#include "imu_normalize.h"

#include "calcu_pwm.h"

#include "MahonyAHRS.h"

//#include "atti_align.h"

#include "pid.h"
#include "quat_unit.h"

volatile uint16_t rf_recv_timeout = 0;

void package_cc1101_recv(const uint8_t *pack)
{
	rf_recv_timeout = 0;
	package_craft_decode(pack);
}

void package_craft_decode(const uint8_t *pack)
{
	CALCU_TYPE tem[4];
	switch(pack[2])
	{
		case MODE_NULL:
		{
		}
		break;
		case MODE_CMD:
		{
			flags.power = pack[3];
			flags.run = pack[4];
			flags.echo = pack[5];
			flags.algori = pack[6];
		}
		break;
		case MODE_CTRL:
		{
			switch(pack[3])
			{
				case CTRL_NULL:
				{
				}
				break;
				case CTRL_SET_FIELD:
				{
//					atti_align_start();
				}
				break;
				case CTRL_SET_PID:
				{
					code_decode_32bits(pack + 5, (uint32_t *)tem, 3);
					if((pack[4] & (1 << 0)) != 0)  //PID_ATT_X
					{
						pid_reset_data(PID_PITCH);
						pid_set_pid(PID_PITCH, tem[0], tem[1], tem[2]);
					}
					if((pack[4] & (1 << 1)) != 0)  //PID_ATT_Y
					{
// 						pid_reset_data(PID_ROLL_RATE);
// 						pid_set_pid(PID_ROLL_RATE, tem[0], tem[1], tem[2]);
						pid_reset_data(PID_LEVEL);
						pid_set_pid(PID_LEVEL, tem[0], tem[1], tem[2]);
					}
					if((pack[4] & (1 << PID_YAW_ID)) != 0)  //PID_ATT_Z
					{
						pid_reset_data(PID_PITCH);
						pid_set_pid(PID_PITCH, tem[0], tem[1], tem[2]);
					}
				}
				break;
				case CTRL_CALI_GYRO:
				{
					craft_data.Calibrate_GYRO = CALIBRATE_LEVEL_GYRO;
				}
				break;
				case CTRL_CALI_ACC:
				{
					craft_data.Calibrate_ACC = CALIBRATE_LEVEL_ACC;
				}
				break;
				case CTRL_CALI_TRIM:
				{
					craft_data.Calibrate_TRIM = CALIBRATE_LEVEL_TRIM;
				}
				break;
				case CTRL_MAG_CALI_BEGIN:
				{
					magn_cali_begin();
					flags.run = RUN_MAG_CALI;
				}
				break;
				case CTRL_MAG_CALI_END:
				{
					magn_cali_end();
					flags.run = RUN_IDLE;
				}
				break;
				default: break;
			}
		}
		break;
		case MODE_RC:
		{
			if(flags.rc_recved == 0)
			{
				flags.rc_recved = 1;  //接收到RC数据
				calcu_pwm_reset();
			}
			code_decode_16bits(pack + 3, (uint16_t *)rcData, RC_CHANS);
		}
		break;
		case MODE_PWM:
		{
			if(flags.run == RUN_PWM)
			{
				code_decode_16bits(pack + 3, (uint16_t *)craft_data.pwm, 4);
			}
		}
		break;
		default: break;
	}
	if(flags.power == POWER_OFF)
	{
		craft_cmd_halt();
	}
}

void package_craft_encode(uint8_t *pack)
{
	uint32_t err;
	switch(flags.echo)
	{
		case ECHO_NULL:
		{
		}
		break;
		case ECHO_CMD:
		{
		}
		break;
		case ECHO_RAW:
		{
		}
		break;
		case ECHO_CALI:
		{
		}
		break;
		case ECHO_MERG:
		{
			pack[0] = SIZE_ECHO_ATT_PID;
			pack[2] = ECHO_ATT_PID;
			code_encode_32bits((const uint32_t *)craft_data.GYRO_physics, pack + 3, 3);
			code_encode_32bits((const uint32_t *)craft_data.ACC_physics,  pack + 19, 3);
			package_en_check(pack);
			rf_mgr_write(pack, pack[0], &err);
		}
		break;
		case ECHO_ATT_PWM:
		{
		}
		break;
		case ECHO_ATT_PID:
		{
			pack[0] = SIZE_ECHO_ATT_PID;
			pack[2] = ECHO_ATT_PID;
			quat_unit_from_eular3(craft_data.Attitude_Quat, craft_data.Attitude_Eular);
			code_encode_32bits((const uint32_t *)craft_data.Attitude_Quat, pack + 3, 4);
			code_encode_32bits((const uint32_t *)craft_data.axisPID,  pack + 19, 3);
			package_en_check(pack);
			rf_mgr_write(pack, pack[0], &err);
		}
		break;
		default:
		break;
	}
}
