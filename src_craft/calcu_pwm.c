/*
 * calcu_pwm.c
 * 控制解算算法
 * 创建者：死猫
 * 创建时间：2015.01.15
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.04.06
 */
 
#include "system_define.h"

#include "quat_unit.h"
#include "my_math.h"

#include "pid.h"

#include "craft_data.h"

#include "calcu_pwm.h"


CALCU_TYPE eular_temp[4];  //无需初始化, 存储计算欧拉角的临时变量

void calcu_pwm_init(void)
{
	calcu_pwm_reset();
}

void calcu_pwm_reset(void)
{
	craft_data.ratePID[0] = 0;
	craft_data.ratePID[1] = 0;
	craft_data.ratePID[2] = 0;
	
	craft_data.axisPID[0] = 0;
	craft_data.axisPID[1] = 0;
	craft_data.axisPID[2] = 0;
	
	craft_data.pwm[0] = 0;
	craft_data.pwm[1] = 0;
	craft_data.pwm[2] = 0;
	craft_data.pwm[3] = 0;
	
	pid_reset_data(PID_PITCH     );
	pid_reset_data(PID_ROLL      );
	pid_reset_data(PID_YAW       );
	pid_reset_data(PID_PITCH_RATE);
	pid_reset_data(PID_ROLL_RATE );
	pid_reset_data(PID_YAW_RATE  );
}

#define DT (0.001f)
void calcu_pwm_eular(int16_t* dst_pwm, const CALCU_TYPE* attitude_eular)
{
	//PID计算
// 	craft_data.axisPID[0] = pid_calculate(PID_PITCH, craft_data.GYRO_smooth[0]);
// 	craft_data.axisPID[1] = pid_calculate(PID_ROLL,  craft_data.GYRO_smooth[1]);
// 	craft_data.axisPID[2] = pid_calculate(PID_YAW,   craft_data.GYRO_smooth[2]);

// 	craft_data.axisPID[0] = pid_calculate(PID_PITCH, attitude_eular[1]);
// 	craft_data.axisPID[1] = pid_calculate(PID_ROLL,  attitude_eular[2]);
// 	craft_data.axisPID[2] = pid_calculate(PID_YAW,   attitude_eular[3]);
	
// 	static CALCU_TYPE tmp = 0;
// 	craft_data.axisPID[0] = craft_data.GYRO_smooth[1];
// 	craft_data.axisPID[1] = (attitude_eular[2] - tmp) / 0.02;
// 	tmp = attitude_eular[2];
	

// 	CALCU_TYPE prop;
// 	uint8_t axis;
// 	CALCU_TYPE rc;
// 	
// 	CALCU_TYPE error, PTerm, ITerm, DTerm;
// 	static CALCU_TYPE errorGyroI[3];
// 	
// 	CALCU_TYPE errorAngle, PTermACC, ITermACC;
// 	static CALCU_TYPE errorAngleI[3];
// 	
// 	CALCU_TYPE delta;
// 	static CALCU_TYPE lastGyro[3], delta1[3], delta2[3];
// 	
// 	CALCU_TYPE dynP[2];
// 	CALCU_TYPE dynD[2];
// 	CALCU_TYPE prop1, prop2;
// 	CALCU_TYPE dynThrPID = 0;


// //annexCode()
// 	for(axis = 0; axis < 4; axis++)
// 	{
// 		rcCommand[axis] = rcData[axis];
// 	}
// 	
// 	prop2 = 100;
// 	if(rcData[THROTTLE] > 500)
// 	{
// 		if(rcData[THROTTLE] < 1000)
// 		{
// 			prop2 = 100.0f - dynThrPID * (rcData[THROTTLE] - 500.0f);
// 		}
// 		else
// 		{
// 			prop2 = 100.0f - dynThrPID;
// 		}
// 	}
// 	prop1 = prop2;  //无外部控制时相等
// 	
// 	for(axis = 0; axis < 2; axis++)
// 	{
// 		dynP[axis] = prop1 / 100.0f * conf.pid[axis].kp;
// 		dynD[axis] = prop1 / 100.0f * conf.pid[axis].kd;
// 	}

// //if ( f.HORIZON_MODE ) prop = min(max(abs(rcCommand[PITCH]),abs(rcCommand[ROLL])),512);
// 	prop = 0;    //无外部控制时为0, [0, +1]

// //参考MultiWii
// //MultiWii Gyro: Full scale set to 2000 deg/sec
// //So, 2000 / 32767 = 0.06103701895199438459425641651662deg/div
// //  and 2000 * pi / 180 = 34.8889 rad/s, 34.8889 / 32767 = 0.00106475688616256870903313971035 rad/s/div
// 	//PITCH & ROLL
// 	for(axis = 0; axis < 2; axis++)
// 	{
// 		rc = 0;

// 		//GYRO PID
// 		error = rc - craft_data.GYRO_smooth[axis];
// 		errorGyroI[axis] += error;
// 		//MY_CONSTRAIN(errorGyroI[axis], -16000, +16000);  //这个地方是多少?
// 		MY_CONSTRAIN(errorGyroI[axis], -17.444f, +17.444f);  //+-1000deg
// 		//if(MY_ABS(craft_data.GYRO_smooth[axis]) > 640) errorGyroI[axis] = 0;  //这个地方是多少?
// 		if(MY_ABS(craft_data.GYRO_smooth[axis]) > 0.6814444f) errorGyroI[axis] = 0;  //39deg/s

// 		PTerm = rc * conf.pid[axis].kp;                             //0
// 		ITerm = craft_data.GYRO_physics[axis] * conf.pid[axis].ki;

// 		//PID[level]中kd并非为PID的kd
// 		//Acc PID
// 		errorAngle = rc - craft_data.Attitude_Eular[axis + 1];
// 		errorAngleI[axis] += errorAngle;
// 		//MY_CONSTRAIN(errorAngleI[axis], -10000, +10000);  //+-1000deg
// 		MY_CONSTRAIN(errorAngleI[axis], -17.444f, +17.444f);  //+-1000deg
// 		PTermACC = errorAngle * conf.pid[PID_LEVEL].kp;
// 		MY_CONSTRAIN(PTermACC, -conf.pid[PID_LEVEL].kd, +conf.pid[PID_LEVEL].kd);
// 		ITermACC = errorAngleI[axis] * conf.pid[PID_LEVEL].ki;

// 		PTerm = PTermACC + (PTerm - PTermACC) * prop;      //PTerm = PTermACC
// 		ITerm = ITermACC + (ITerm - ITermACC) * prop;
// 		//Acc PID End

// 		PTerm -= craft_data.GYRO_smooth[axis] * dynP[axis];      //PTerm = PTermACC - P_GYRO

// 		delta = craft_data.GYRO_smooth[axis] - lastGyro[axis];
// 		lastGyro[axis] = craft_data.GYRO_smooth[axis];
// 		
// 		DTerm = delta1[axis] + delta2[axis] + delta;
// 		delta2[axis] = delta1[axis];
// 		delta1[axis] = delta;
// 		
// 		DTerm = DTerm * dynD[axis];  //这个系数是求出来的
// 		craft_data.axisPID[axis] = PTerm + ITerm - DTerm;
// 	}

 	uint8_t axis;
 	CALCU_TYPE rc;
 	CALCU_TYPE prop;

//PID
	CALCU_TYPE PTerm, ITerm, DTerm;
//PID gyro
	CALCU_TYPE errorG, PTermG, ITermG;
	static CALCU_TYPE errorIG[3];
	CALCU_TYPE delta;
	static CALCU_TYPE lastGyro[3], delta1[3], delta2[3];

//PID acc
	CALCU_TYPE errorA, PTermA, ITermA;
	static CALCU_TYPE errorIA[3];
//other
	float32_t tmpf32;
	int16_t tmpi16;

	for(axis = 0; axis < 2; axis++)
	{
		rcCommand[axis] = rcData[axis];
	}

	//油门曲线
	//  约束
	//      (1)范围X, Y~[0, 1000]
	//      (2)在Xmid处斜率k可调
	//      (3)Xmid位置可调
	//      (4)最大值最小值可调
	//  归一化方程y=ax^3+cx, a=(Ymid/Xmid-k)/Xmid^2, c=k
	//      mid左侧Xmid'==Ymid'==Xmid==Ymid, 右侧Xmid'==Ymid'==1000/2-Ymid
	tmpf32 = (float32_t)rcData[THROTTLE] / 100.0f;
	tmpi16 = (int16_t)tmpf32;
	rcCommand[THROTTLE] = conf.lookupThrottleRC[tmpi16] + (tmpf32 - (float32_t)tmpi16) * (conf.lookupThrottleRC[tmpi16 + 1] - conf.lookupThrottleRC[tmpi16]);

	//积分清零
	if(rcData[THROTTLE] <= MINCHECK)
	{
		errorIG[0] = 0;
		errorIG[1] = 0;
		errorIA[0] = 0;
		errorIA[1] = 0;
	}

	prop = 0;
	for(axis = 0; axis < 2; axis++)
	{
		rc = rcCommand[axis] * (3.14f / 180.0f);
		errorG = rc - craft_data.GYRO_smooth[axis];
		errorIG[axis] += errorG * DT;
		MY_CONSTRAIN(errorIG[axis], -1.7f, +1.7f);
		if(MY_ABS(craft_data.GYRO_smooth[axis]) > (40.0f * 3.14f / 180.0f)) errorIG[axis] = 0;
		
		PTermG = rc * conf.PID[axis].kp;
		ITermG = errorIG[axis] * conf.PID[axis].ki;
		
		if(1)
		{
			errorA = rc -  craft_data.Attitude_Eular[axis] + conf.AngleTrim[axis];
			errorIA[axis] += errorA * DT;
			MY_CONSTRAIN(errorIA[axis], -0.17f, +0.17f);
			
			PTermA = errorA * conf.PID[PID_LEVEL_ID].kp;
			MY_CONSTRAIN(PTermA, -conf.PID[PID_LEVEL_ID].kd, conf.PID[PID_LEVEL_ID].kd);
			ITermA = errorIA[axis] * conf.PID[PID_LEVEL_ID].ki;
			
			PTerm = PTermA + (PTermG - PTermA) * prop;  //控制角度越大，加速度项比重越小
			ITerm = ITermA + (ITermG - ITermA) * prop;
			PTerm -= craft_data.GYRO_smooth[axis] * conf.PID[axis].kp;
		}
		else
		{
			PTerm = PTermG - craft_data.GYRO_smooth[axis] * conf.PID[axis].kp;
			ITerm = ITermG;
		}
		
		delta = (-craft_data.GYRO_smooth[axis] - lastGyro[axis]) / DT;
		DTerm = (delta + delta1[axis] + delta2[axis]) * conf.PID[axis].kd;
		
		lastGyro[axis] = -craft_data.GYRO_smooth[axis];
		delta2[axis] = delta1[axis];
		delta1[axis] = delta;
		
		craft_data.axisPID[axis] = PTerm + ITerm + DTerm;
	}

	//YAW
	craft_data.axisPID[2] = 0;
	
	//关闭x轴控制
	//craft_data.axisPID[0] = 0;

	//限幅
	MY_CONSTRAIN(craft_data.axisPID[0], -200.0f, +200.0f);
	MY_CONSTRAIN(craft_data.axisPID[1], -200.0f, +200.0f);
	MY_CONSTRAIN(craft_data.axisPID[2], -200.0f, +200.0f);
}

//输入Attitude_ENU_Quat
//输出Attitude_Eular, Attitude_Quat, Attitude_Hold_Eular, pwm
void calcu_pwm(void)
{
	//quat_to_eular(craft_data.Attitude_Quat_ENU, craft_data.Attitude_Eular);

// 	craft_data.Attitude_Eular_Hold[0] = 0;
// 	craft_data.Attitude_Eular_Hold[1] = 0;
// 	craft_data.Attitude_Eular_Hold[2] = craft_data.Attitude_Eular[2];

// 	craft_data.Attitude_Eular[0] -= craft_data.Attitude_Eular_ENU_INIT[0] + craft_data.Attitude_Eular_Hold[0];
// 	craft_data.Attitude_Eular[1] -= craft_data.Attitude_Eular_ENU_INIT[1] + craft_data.Attitude_Eular_Hold[1];
// 	craft_data.Attitude_Eular[2] -= craft_data.Attitude_Eular_ENU_INIT[2] + craft_data.Attitude_Eular_Hold[2];

	calcu_pwm_eular(craft_data.pwm, craft_data.Attitude_Eular);

}

#define K (200.0f)
// void calcu_pid_rate(void)
// {
// 	//速度
//  	craft_data.ratePID[0] = -3.0f * 3.14f / 180.0f;
//  	craft_data.ratePID[1] = 0;
// 	pid_set_expect(PID_ROLL_RATE, craft_data.ratePID[0]);
// 	pid_set_expect(PID_PITCH_RATE, craft_data.ratePID[1]);

// 	craft_data.axisPID[0] = pid_update(PID_ROLL_RATE, craft_data.GYRO_smooth[0]);
// 	craft_data.axisPID[1] = pid_update(PID_PITCH_RATE, craft_data.GYRO_smooth[1]);

// // 	craft_data.axisPID[0] = pid_update(PID_ROLL_RATE, craft_data.GYRO_physics[0]);
// // 	craft_data.axisPID[1] = pid_update(PID_PITCH_RATE, craft_data.GYRO_physics[1]);

// 	craft_data.axisPID[0] -= PID_ROLL -> kp * my_sin(craft_data.Attitude_Eular[0]);
// 	craft_data.axisPID[1] -= PID_ROLL -> kp * my_sin(craft_data.Attitude_Eular[1]);

// 	//YAW
// 	craft_data.axisPID[2] = 0;

// 	//关闭x轴控制
// 	craft_data.axisPID[1] = 0;
// }

// void calcu_pid_angle(void)
// {
// 	//位置
// 	pid_set_expect(PID_ROLL, 0);
// 	pid_set_expect(PID_PITCH, 0);

// 	craft_data.ratePID[0] = pid_update(PID_ROLL, craft_data.Attitude_Eular[0]);
// 	craft_data.ratePID[1] = pid_update(PID_PITCH, craft_data.Attitude_Eular[1]);
// }

void pid_to_pwm(void)
{
	if(rcCommand[THROTTLE] < 50)  //油门关闭
	{
		craft_data.pwm[0] = 0;
		craft_data.pwm[1] = 0;
		craft_data.pwm[2] = 0;
		craft_data.pwm[3] = 0;
	}
	else if(rcCommand[THROTTLE] < 250)
	{
		craft_data.pwm[0] = rcCommand[THROTTLE];
		craft_data.pwm[1] = rcCommand[THROTTLE];
		craft_data.pwm[2] = rcCommand[THROTTLE];
		craft_data.pwm[3] = rcCommand[THROTTLE];
	}
	else
	{
		//油门限幅
		MY_CONSTRAIN(rcCommand[THROTTLE], 0, 800);

		//PWM姿态分量
		craft_data.pwm[0] = rcCommand[THROTTLE] - craft_data.axisPID[0] - craft_data.axisPID[1] - craft_data.axisPID[2];
		craft_data.pwm[1] = rcCommand[THROTTLE] + craft_data.axisPID[0] - craft_data.axisPID[1] + craft_data.axisPID[2];
		craft_data.pwm[2] = rcCommand[THROTTLE] + craft_data.axisPID[0] + craft_data.axisPID[1] - craft_data.axisPID[2];
		craft_data.pwm[3] = rcCommand[THROTTLE] - craft_data.axisPID[0] + craft_data.axisPID[1] + craft_data.axisPID[2];

		//输出限幅, 最低+最高
		MY_CONSTRAIN(craft_data.pwm[0], 100, 900);
		MY_CONSTRAIN(craft_data.pwm[1], 100, 900);
		MY_CONSTRAIN(craft_data.pwm[2], 100, 900);
		MY_CONSTRAIN(craft_data.pwm[3], 100, 900);
	}
}
