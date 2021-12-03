/***********************************************************************
* 
* Authour: Chuby Okafor
*
*/


#ifndef MPU6050_H
#define MPU6050_H
#include "../i2c-tools/include/i2c/smbus.h"
#include <linux/i2c-dev.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <sys/ioctl.h>
#include <math.h>

/*  0x68 or 0x69 */
#define mpu_addr 0x68 
#define adapter_nr 1
#define PWR_MGMT_1_REG 0x6B
#define GYRO_CONFIG_REG 0x1B
#define ACCEL_CONFIG_REG 0x1C
#define CONFIG_REG 0x1A
#define PWR_MGMT_1_VAL 0x00
#define GYRO_CONFIG_VAL 0x00
#define ACCEL_CONFIG_VAL 0x08
#define CONFIG_VAL 0x03
#define GYRO_DATA_REG 0x43
#define ACCEL_DATA_REG 0x3F
#define ACC_CAL_VAL 480


class mpu6050
{
public:
	mpu6050();
	~mpu6050();
	

	int gyro_pitch_data_raw, gyro_yaw_data_raw, accelerometer_data_raw;
	long gyro_yaw_calibration_value, gyro_pitch_calibration_value;
	float angle_gyro, angle_acc, angle;
	int file;
	int start;
  	char filename[20];
	int setup();
	int calibrate(int loopDelay);
	int mpu_write(int file, char reg, char data);
	int mpu_read(int file, char reg, char *readBuf, int num_bytes);
	int calculateAngle();
		
};

#endif
