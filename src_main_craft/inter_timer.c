/*
 * inter_inter.c
 * 定时器中断处理程序
 * 创建者：死猫
 * 创建时间：2014.xx.xx
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.04.09
 */

//测量数据

//波形关系:
//20ms:  ___|-|_______
//1ms:   _|-|____|-|__

//2015.03.24
//TSET_1:
//  T = 1ms
//  闲: Tp = 150us
//  忙: Tp = 150us
//TSET_2:
//  T = 20ms
//  闲: Tp = 5.1us
//  忙: Tp = 172us

#include "system_define.h"
#include "hardware.h"		//io端口宏定义


//STM32
#include "stm32_systick.h"
#include "stm32_gpio.h"


//IO
#include "io_data.h"
#include "package.h"
#include "stm32_pwm.h"


//Craft
#include "craft_data.h"
#include "craft_cmd.h"
#include "imu_sensor.h"
#include "imu_attitude.h"
#include "imu_physics.h"
#include "imu_smooth.h"
#include "imu_normalize.h"
#include "calcu_pwm.h"
#include "atti_align.h"
#include "MahonyAHRS.h"


void TIM2_IRQHandler()
{
	static uint8_t Magn_Read_Count = 0;
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) 		//判断中断标志位是否为1；为1则中断响应啦，0则没有
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);       	//清除中断挂起位
		TSET_1 = 1;

		if(flags.power == POWER_ON)
		{
			if(Magn_Read_Count == 0)
			{
				Magn_Read_Count = 50;  //50 * 1ms = 50ms

				imu_sensor_get_gyro_acc_mag(craft_data.GYRO_raw, craft_data.ACC_raw, craft_data.MAG_raw);
				imu_physics_mag(craft_data.MAG_physics, craft_data.MAG_raw);
				//imu_normalize(craft_data.MAG_norm, craft_data.MAG_physics);
			}
			else
			{
				imu_sensor_get_gyro_acc(craft_data.GYRO_raw, craft_data.ACC_raw);
			}
			Magn_Read_Count--;

			imu_physics_gyro(craft_data.GYRO_physics, craft_data.GYRO_raw);
			imu_smooth(craft_data.GYRO_smooth, craft_data.GYRO_physics, 0.25);

			imu_physics_acc(craft_data.ACC_physics, craft_data.ACC_raw);
			imu_smooth(craft_data.ACC_smooth, craft_data.ACC_physics, 0.0625);
			//imu_normalize(craft_data.ACC_norm, craft_data.ACC_smooth);

			switch(flags.run)
			{
				case RUN_POWER_ON:
				{
					if(craft_data.Calibrate_GYRO == 0)  //未校准
					{
						craft_data.Calibrate_GYRO = CALIBRATE_LEVEL_GYRO;
					}
					else if(craft_data.Calibrate_GYRO == -1)
					{
						flags.run = RUN_IDLE;
					}
				}
				break;
				case RUN_IDLE:
				{
				}
				break;
				case RUN_MOV:
				{
					//姿态解算
					imu_attitude();
					//控制解算
					calcu_pwm();

// 					if(0 == Ctrl_Count)
// 					{
// 						Ctrl_Count = 5;  //5 * 1ms = 5ms
// 						calcu_pid_angle();
// 					}
// 					Ctrl_Count--;
// 					
// 					calcu_pid_rate();
					//控制输出
					pid_to_pwm();
					pwm_set(craft_data.pwm);
				}
				break;
				case RUN_PWM:
				{
				}
				break;
				case RUN_MAG_CALI:
				{
					magn_cali_input(craft_data.MAG_raw);
				}
				break;
				case RUN_ALIGN:
				{
				}
				break;
				default: break;
			}
		}
		TSET_1 = 0;
	}
}

void TIM3_IRQHandler()
{
	static uint8_t TIM3_Count = 0;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) 		//判断中断标志位是否为1；为1则中断响应啦，0则没有
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);       	//清除中断挂起位
		TSET_2 = 1;
		
		if(flags.power == POWER_ON && flags.rc_recved != 0)
		{
			
			//calcu_pwm();

			switch(flags.run)
			{
				case RUN_POWER_ON:
				{
				}
				break;
				case RUN_IDLE:
				{
					pwm_set_zero();
				}
				break;
				case RUN_MOV:
				{
					
					
				}
				break;
				case RUN_PWM:
				{
					pwm_set(craft_data.pwm);
				}
				break;
				default:
				{
					pwm_set_zero();
				}
				break;
			}
		}
		else  //Power off or no rc recved
		{
			calcu_pwm_reset();
			pwm_set_zero();
		}

		TIM3_Count++;
		if(TIM3_Count == 10)  // 20ms * 10 = 200
		{
			TIM3_Count = 0;
			package_craft_encode(package_cc1101_send_buff);
		}

		TSET_2 = 0;
	}
}


void TIM5_IRQHandler()
{
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) 		//判断中断标志位是否为1；为1则中断响应啦，0则没有
	{
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);       	//清除中断挂起位
	}
}
