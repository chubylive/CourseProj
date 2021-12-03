/***********************************************************************
* 
* Authour: Chuby Okafor
* build: g++ -O1 -g -o lsd ledSwitchDemo.cpp -Wall -std=gnu++
* run: ./lsd
*/



#include "mpu6050.h"

	mpu6050::mpu6050(){
		snprintf(filename, 19, "/dev/i2c-%d", adapter_nr);
		start = 0;

	}
	mpu6050::~mpu6050(){
		close(file);
	}

	int mpu6050::mpu_write(int file, char reg, char data){
		char buf[2];
		buf[0] = reg;
		buf[1] = data; 
		if(write(file,buf,2) !=2 ){
			printf("failed to write reg %d\n", reg);
			return -1;
		}
		return 0;
	}

	int mpu6050::mpu_read(int file, char reg, char * readBuf, int num_bytes){
		char buf[1];
		buf[0] = reg;
		if(write(file,buf,1) !=1 ){
            printf("failed to write Read REG\n");
    	}
	
		if(read(file,readBuf,num_bytes) != num_bytes){
			printf("failed to read sensor\n");
			return -1;
		}else{
			//printf("data: 0x%x\n",(*(uint32_t*)(readBuf)));
		}
		return 0;
	}


	int mpu6050::setup(){
		file = open(filename, O_RDWR);
		if (file < 0) {
    		/* ERROR HANDLING; you can check errno to see what went wrong */
    		printf("failed to open /dev/i2c-%d\n",adapter_nr);
			return -1;

  		}
  		if (ioctl(file, I2C_SLAVE, mpu_addr) < 0) { 
   			/* ERROR HANDLING; you can check errno to see what went wrong */
   			printf("failed to ioctl addr: %d\n", mpu_addr);
			return -1;
  		}

  		if(mpu_write(file,PWR_MGMT_1_REG,PWR_MGMT_1_VAL) !=0 ){
			printf("failed to write PWR_MGMT_1_REG\n");
			return -1;
		}
		if(mpu_write(file,GYRO_CONFIG_REG,GYRO_CONFIG_VAL) !=0 ){
		    printf("failed to write GYRO_CONFIG_REG\n");
		    return -1;
		}
		if(mpu_write(file,ACCEL_CONFIG_REG,ACCEL_CONFIG_VAL) !=0 ){
		    printf("failed to write ACCEL_CONFIG_REG\n");
		    return -1;
		}
		if(mpu_write(file,CONFIG_REG,CONFIG_VAL) !=0 ){
		    printf("failed to write CONFIG_REG\n");
		    return -1;
		}
		return 0;
	}

	int mpu6050::calibrate(int loopDelay){
		char buf[4];
		for(int idx = 0;idx < 500;idx++){
			if(mpu_read(file,GYRO_DATA_REG,buf,4) != 0){
				return -1;
			}
			gyro_yaw_calibration_value += buf[0] << 8 | buf[1]; 
			gyro_pitch_calibration_value += buf[2] << 8 | buf[3];
			//printf("yaw: %ld pitch: %ld\n", gyro_yaw_calibration_value, gyro_pitch_calibration_value);
		
			usleep(loopDelay);
		}
		gyro_yaw_calibration_value /= 500;
		gyro_pitch_calibration_value /= 500;
		return 0;
	}

	int mpu6050::calculateAngle(){
		char buf[2];
		int gyro_pitch_data_raw, gyro_yaw_data_raw, accelerometer_data_raw;
		if(mpu_read(file,ACCEL_DATA_REG,buf,2) != 0){
				return -1;
		}
		accelerometer_data_raw = (int)(buf[0] << 8 | buf[1]);                      //Combine the two bytes to make one integer
		accelerometer_data_raw += (int)ACC_CAL_VAL;                          //Add the accelerometer calibration value

//Todo angle code has error
		if(accelerometer_data_raw > 8200){
			accelerometer_data_raw = 8200;           //Prevent division by zero by limiting the acc data to +/-8200;
		}
		if(accelerometer_data_raw < -8200){
		accelerometer_data_raw = -8200;         //Prevent division by zero by limiting the acc data to +/-8200;
		}

		angle_acc = asin((float)accelerometer_data_raw/8200.0)* 57.296;           //Calculate the current angle according to the accelerometer
		if(start == 0 ){                     //If the accelerometer angle is almost 0
			angle_gyro = angle_acc;                                                 //Load the accelerometer angle in the angle_gyro variable
    			start = 1;                                                              //Set the start variable to start the PID controller
  		}
		char buf1[4];
		if(mpu_read(file,GYRO_DATA_REG,buf1,4) != 0){
				return -1;
		}
		gyro_yaw_data_raw = buf1[0] << 8 | buf1[1];  
		gyro_pitch_data_raw = buf1[2] << 8 | buf1[3]; 
		gyro_pitch_data_raw -= gyro_pitch_calibration_value;	//Add the gyro calibration value
		angle_gyro += gyro_pitch_data_raw * 0.000031;                             //Calculate the traveled during this loop angle and add this to the angle_gyro variable


		//MPU-6050 offset compensation
		//Not every gyro is mounted 100% level with the axis of the robot. This can be cause by misalignments during manufacturing of the breakout board. 
		//As a result the robot will not rotate at the exact same spot and start to make larger and larger circles.
 		//To compensate for this behavior a VERY SMALL angle compensation is needed when the robot is rotating.
 		//Try 0.0000003 or -0.0000003 first to see if there is any improvement.

		gyro_yaw_data_raw -= gyro_yaw_calibration_value;                          //Add the gyro calibration value
  		//Uncomment the following line to make the compensation active
 		//angle_gyro -= gyro_yaw_data_raw * 0.0000003;                            //Compensate the gyro offset when the robot is rotating

		angle_gyro = angle_gyro * 0.9996 + angle_acc * 0.0004;                    //Correct the drift of the gyro angle with the accelerometer angl
		printf("angle: %.6f, raw gyro pitch: %d raw accel pitch: %d \n", angle_gyro, gyro_pitch_data_raw, accelerometer_data_raw);
		return 0;

	}
