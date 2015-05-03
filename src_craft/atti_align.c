// /*
//  * atti_align.h
//  * 初始姿态计算
//  * 创建者：死猫
//  * 创建时间：2015.02.26
//  * 修改者：(每次修改后, 请将最后修改者置于末尾, 其他人顺序保持不变)
//  *     死猫,
//  * 最终修改时间：2015.02.26
//  */

// #include "system_define.h"
// #include "craft_data.h"

// #include "MahonyAHRS.h"
// #include "MadgwickAHRS.h"

// #include "quat_unit.h"
// #include "my_math.h"

// #define Align_Kp 0.95

// uint8_t Flag_Running_Old;

// uint8_t flag_first_align;  //是否为对准第一步赋值

// void atti_align_init(void)
// {
// 	flag_first_align = 1;
// }

// void atti_align_start(void)
// {
// 	if(flags.run != RUN_ALIGN)
// 	{
// 		Flag_Running_Old = flags.run;
// 		flags.run = RUN_ALIGN;
// 		flag_first_align = 1;
// 	}
// }

// void atti_align_stop(void)
// {
// 	//CALCU_TYPE tem[4];
// 	if(flags.run == RUN_ALIGN)
// 	{
// 		flags.run = Flag_Running_Old;
// 		quat_unit_to_eular3(craft_data.Attitude_Quat_ENU, craft_data.Attitude_Eular_ENU_INIT);
// 		craft_data.Attitude_Eular_ENU_INIT[2] = 0;
// 		//quat_from_eular(Attitude_ENU_Quat, atti_eular);
// // 		Attitude_ENU_INIT_Quat[0] = Attitude_ENU_Quat[0];
// // 		Attitude_ENU_INIT_Quat[1] = Attitude_ENU_Quat[1];
// // 		Attitude_ENU_INIT_Quat[2] = Attitude_ENU_Quat[2];
// // 		Attitude_ENU_INIT_Quat[3] = Attitude_ENU_Quat[3];
// 		/*
// 		//quat_to_eular(Attitude_ENU_Quat, tem);
// 		//tem[3] = atti_eular[3];
// 		//quat_from_eular(Attitude_ENU_Quat, tem);
// 		tem[0] = 0;
// 		tem[1] = 0;
// 		tem[2] = 0;
// 		quat_from_eular(Attitude_ENU_INIT_Quat, tem);
// 		*/
// 		
// 		//quat_unit(Attitude_ENU_INIT_Quat);
// 		
// 		//MahonyAHRSClearInteValue();
// 	}
// }

// void atti_align(void)
// {
// 	if(1 == flag_first_align)  //第一次对准赋值
// 	{
// 		//Attitude_ENU_INIT_Eular[0] = 0;
// 		//Attitude_ENU_INIT_Eular[1] = 0;
// 		//Attitude_ENU_INIT_Eular[2] = 0;
// 		//Attitude_ENU_INIT_Eular[3] = 0;
// 		
// 		
// 		//Attitude_ENU_INIT_Eular[0] = 0;
// 		//Attitude_ENU_INIT_Eular[1] = my_atan2(AY_NORM, my_sqrt(1.0f - AY_NORM * AY_NORM));
// 		//Attitude_ENU_INIT_Eular[2] = my_atan2(AX_NORM, AZ_NORM);
// 		//Attitude_ENU_INIT_Eular[3] = my_atan2(MZ_NORM * AX_NORM - MX_NORM * AZ_NORM, 
// 		//									MY_NORM * (1.0f - AY_NORM * AY_NORM) - (MX_NORM * AX_NORM + MZ_NORM * AZ_NORM) * AY_NORM);
// 		//Attitude_ENU_INIT_Eular[3] = M_PI_2 + my_atan2(MZ_NORM * AX_NORM - MX_NORM * AZ_NORM, AZ_NORM * MY_NORM - AY_NORM * MZ_NORM);

// 		//quat_from_eular(Attitude_ENU_Quat, atti_eular);
// 		flag_first_align = 0;
// 	}else{
// 		/*
// 		atti_eular[1] = (1.0f - Align_Kp) * atti_eular[1] + Align_Kp * my_atan2(AY_NORM, my_sqrt(1.0f - AY_NORM * AY_NORM));
// 		atti_eular[2] = (1.0f - Align_Kp) * atti_eular[2] + Align_Kp * my_atan2(AX_NORM, AZ_NORM);
// 		
// 		//atti_eular[3] = (1.0f - Align_Kp) * atti_eular[3] + Align_Kp * my_atan2(MZ_NORM * AX_NORM - MX_NORM * AZ_NORM, 
// 		//									MY_NORM * (1.0f - AY_NORM * AY_NORM) - (MX_NORM * AX_NORM + MZ_NORM * AZ_NORM) * AY_NORM);
// 		atti_eular[3] = (1.0f - Align_Kp) * atti_eular[3] + Align_Kp * my_atan2(MZ_NORM * AX_NORM - MX_NORM * AZ_NORM, AZ_NORM * MY_NORM - AY_NORM * MZ_NORM);
// 		*/
// 	}
// }
