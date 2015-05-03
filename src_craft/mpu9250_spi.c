/*
 * mpu9250_spi.c
 * MPU9250传感器数据获取
 * 创建者：死猫
 * 创建时间：2015.01.16
 * 修改者：(每次修改后, 请将最后修改者置于末尾, 其他人顺序保持不变)
 *     死猫,
 * 最终修改时间：2015.04.21
 */
 
 /*
 * MPU9250(MPU6500) I2C Timing Characteristics
 * I2C SCL Frequency : 400 kHz max
 * START Condition Hold Time : 0.6us
 * STOP Condition Hold Time : 0.6us
 * SCL High Period : 0.6us
 * SCL Low Period : 1.3us
 * Bus Free Time Between STOP and Start Condition : 1.3us
 * SDA Setup Time : 100ns
 */
 
 /*
 * MPU9250(AK8963, Fast mode) I2C Timing Characteristics
 * I2C SCL Frequency : 400 kHz max
 * START Condition Hold Time : 0.6us
 * STOP Condition Hold Time : 0.6us
 * SCL High Period : 0.6us
 * SCL Low Period : 1.3us
 * Bus Free Time Between STOP and Start Condition : 1.3us
 * SDA Setup Time : 100ns
 */

#include "system_define.h"
#include "hardware.h"		//io端口宏定义

#include "stm32_spi.h"
#include "stm32_delay.h"

#include "mpu9250.h"

#define MPU9250_CS   { PBout(12) = 0; }
#define MPU9250_CS_DIS { PBout(12) = 1; }

#define MPU9250_SPI 2  // 1 -> SPI1, 2-> SPI2

//X, Y, Z输入顺序为MPU6500坐标系顺序
#define SENSOR_ORIENTATION(dst, X, Y, Z) {dst[0] = (Y); dst[1] = SENSOR_NEG(X); dst[2] = (Z);}

#define SENSOR_ORIENTATION_GYRO(dst, buf) {SENSOR_ORIENTATION(dst,  \
											(((uint16_t)buf[0]) << 8) | buf[1],           \
											(((uint16_t)buf[2]) << 8) | buf[3],           \
											(((uint16_t)buf[4]) << 8) | buf[5]);}

#define SENSOR_ORIENTATION_ACC(dst, buf) {SENSOR_ORIENTATION(dst,   \
											(((uint16_t)buf[0]) << 8) | buf[1],           \
											(((uint16_t)buf[2]) << 8) | buf[3],           \
											(((uint16_t)buf[4]) << 8) | buf[5]);}

#define SENSOR_ORIENTATION_MAG(dst, buf) {SENSOR_ORIENTATION(dst,   \
											(((uint16_t)buf[3]) << 8) | buf[2],           \
											(((uint16_t)buf[1]) << 8) | buf[0],           \
											SENSOR_NEG((((uint16_t)buf[5]) << 8) | buf[4]));}  //取负数

#define SENSOR_NEG(X) ((~(X)) + 1)

#if (MPU9250_SPI == 1)

#define SPI_RW spi1_rw

#elif (MPU9250_SPI == 2)

#define SPI_RW spi2_rw

#endif

uint8_t mpu9250_read_buf[21];  //无需初始化

void mpu9250_write_reg(uint8_t regAddr, uint8_t dat)
{
	MPU9250_CS
	SPI_RW(regAddr);
	SPI_RW(dat);
	MPU9250_CS_DIS
}

void mpu9250_read_reg(uint8_t regAddr, uint8_t *buf, uint8_t len)
{
	uint8_t i;
	MPU9250_CS
	SPI_RW(0x80 | regAddr);
	for(i = 0; i < len; i++)
	{
		buf[i] = SPI_RW(0xFF);
	}
	MPU9250_CS_DIS
}

void mpu9250_init(void)
{
	uint8_t i;
	const uint8_t MPU9250_Init_Data[][2] = {
			{MPU9250_PWR_MGMT_1,    0x80},  // Reset Device
			{MPU9250_PWR_MGMT_1,    0x03},  // Clock Source
			{MPU9250_PWR_MGMT_2,    0x00},  // Enable Acc & Gyro
			{MPU9250_CONFIG,        0x00},  // dlpf_cfg = 0
			{MPU9250_CONFIG,        0x00},  // dlpf_cfg = 0
			{MPU9250_GYRO_CONFIG,   0x00},  // (gyro)fc = 0x11, (dlpf_cfg = 0) : fs = 8kHz, bandwidth = 250 delay = 0.97ms
			{MPU9250_ACCEL_CONFIG,  0x18},  // +-16g
			{MPU9250_ACCEL_CONFIG2, 0x00},  // acc_fc = 0x1, a_dlpf = 0 : rate : 1kHz, delay = 1.94ms
			{MPU9250_INT_PIN_CFG,   0x00},  // ByPass disable
			{MPU9250_I2C_MST_CTRL,  0x00},  // I2C Speed 348 kHz
			{MPU9250_USER_CTRL,     0x20},  // I2C Master Enable

			//Samlpe Rate
			{MPU9250_SMPLRT_DIV,    0x00},  // fs/1 = 1kHz/1 = 1kHz

			// Initial AK8963
			{MPU9250_I2C_SLV4_ADDR,  MPU9250_ADDRESS_MAG | 0x00},
			// Write MPU9250_MAG_CNTL2, 0x01
			{MPU9250_I2C_SLV4_REG,   MPU9250_MAG_CNTL2},
			{MPU9250_I2C_SLV4_DO,    0x01},
			{MPU9250_I2C_SLV4_CTRL,  0x80},  // Start Transfer
			// Write MPU9250_MAG_CNTL1 0x00 then 0x16
			{MPU9250_I2C_SLV4_REG,   MPU9250_MAG_CNTL1},
			{MPU9250_I2C_SLV4_DO,    0x00},
			{MPU9250_I2C_SLV4_CTRL,  0x81},  // Start Transfer
			{MPU9250_I2C_SLV4_DO,    0x16},  // Continuous measurement mode 2, 100Hz
			{MPU9250_I2C_SLV4_CTRL,  0x80},  // Start Transfer

			//Samlpe Rate
			{MPU9250_SMPLRT_DIV,         (10 - 1)},  // fs/10 = 1kHz/10 = 100Hz

			// Set Slave0 to auto read AK8963
			{MPU9250_I2C_SLV0_ADDR,      MPU9250_ADDRESS_MAG | 0x80},
			{MPU9250_I2C_SLV0_REG,       MPU9250_MAG_HXL},
			{MPU9250_I2C_SLV0_CTRL,      0x87},  // Start Transfer
			{0x00, 0x00}  //Initial End
		};
	i = 0;
	while((0 != MPU9250_Init_Data[i][0]) || (0 != MPU9250_Init_Data[i][1]))
	{
		//注意: 写入配置时时钟必须<1MHz
    mpu9250_write_reg(MPU9250_Init_Data[i][0], MPU9250_Init_Data[i][1]);
		i++;
		delayms(1);  //延迟
  }
}

void mpu9250_get_gyro_acc(uint16_t *dst_gyro, uint16_t *dst_acc)
{
	uint8_t* buf = mpu9250_read_buf;
	mpu9250_read_reg(MPU9250_ACCEL_XOUT_H, mpu9250_read_buf, 14);
	
	SENSOR_ORIENTATION_ACC(dst_acc, buf);
	buf += 8; SENSOR_ORIENTATION_GYRO(dst_gyro, buf);
}

void mpu9250_get_gyro_acc_mag(uint16_t *dst_gyro, uint16_t *dst_acc, uint16_t *dst_mag)
{
	uint8_t* buf = mpu9250_read_buf;
	mpu9250_read_reg(MPU9250_ACCEL_XOUT_H, mpu9250_read_buf, 20);

	SENSOR_ORIENTATION_ACC(dst_acc, buf);
	buf += 8; SENSOR_ORIENTATION_GYRO(dst_gyro, buf);
	buf += 6; SENSOR_ORIENTATION_MAG(dst_mag,buf);
}

void mpu9250_get_gyro(uint16_t *dst)
{
	mpu9250_read_reg(MPU9250_GYRO_XOUT_H, mpu9250_read_buf, 6);
	SENSOR_ORIENTATION_GYRO(dst, mpu9250_read_buf);
}

void mpu9250_get_acc(uint16_t *dst)
{
	mpu9250_read_reg(MPU9250_ACCEL_XOUT_H, mpu9250_read_buf, 6);
	SENSOR_ORIENTATION_ACC(dst, mpu9250_read_buf);
}

void mpu9250_pre_get_magn(void)
{
	mpu9250_write_reg(MPU9250_I2C_SLV2_DO,   0x11);
	mpu9250_write_reg(MPU9250_I2C_SLV2_CTRL, 0x81);
}

void mpu9250_get_magn(uint16_t *dst)
{
	mpu9250_read_reg(MPU9250_EXT_SENS_DATA_00, mpu9250_read_buf, 6);
	SENSOR_ORIENTATION_MAG(dst, mpu9250_read_buf);
}

//My data : 0xAD, 0xAE, 0xA4
void mpu9250_get_magn_adjust(uint8_t *dst)
{
	mpu9250_write_reg(MPU9250_I2C_SLV4_ADDR, MPU9250_ADDRESS_MAG | 0x80);
	
	mpu9250_write_reg(MPU9250_I2C_SLV4_REG,  MPU9250_MAG_ASAX);
	mpu9250_write_reg(MPU9250_I2C_SLV4_CTRL, 0x80);
	delayms(50);  //50ms
	mpu9250_read_reg(MPU9250_I2C_SLV4_DI, dst, 6);
	
	mpu9250_write_reg(MPU9250_I2C_SLV4_REG,  MPU9250_MAG_ASAY);
	mpu9250_write_reg(MPU9250_I2C_SLV4_CTRL, 0x80);
	delayms(50);  //50ms
	mpu9250_read_reg(MPU9250_I2C_SLV4_DI, dst + 1, 6);
	
	mpu9250_write_reg(MPU9250_I2C_SLV4_REG,  MPU9250_MAG_ASAZ);
	mpu9250_write_reg(MPU9250_I2C_SLV4_CTRL, 0x80);
	delayms(50);  //50ms
	mpu9250_read_reg(MPU9250_I2C_SLV4_DI, dst + 2, 6);
}

uint8_t mpu9250_get_mag_wia(void)
{
	uint8_t ret;
	mpu9250_write_reg(MPU9250_I2C_SLV4_ADDR, MPU9250_ADDRESS_MAG | 0x80);
	mpu9250_write_reg(MPU9250_I2C_SLV4_REG, MPU9250_MAG_WIA);
	mpu9250_write_reg(MPU9250_I2C_SLV4_CTRL, 0x81);
	delayus(50000);
	mpu9250_read_reg(MPU9250_I2C_SLV4_DI, &ret, 1);
	return ret;
}
