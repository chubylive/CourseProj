/***********************************************************************
* 
* Authour: Chuby Okafor
* build: g++ -O1 -g -o t1 robot.cpp mpu6050.cpp mpu6050.h -Wall 
* run: ./t1
*/


#include "mpu6050.h"

mpu6050 mpu;
int loopDelay = 3700;

int main(int argc, char const *argv[])
{

	
	if(mpu.setup() !=0){
		return -1;
	}
	if(mpu.calibrate(3700) !=0){
		return -1;
	}
	for(int idx = 0; ; idx++){
		if(mpu.calculateAngle() != 0){
			return -1;
		}
		usleep(37000);
	}
	
	return 0;
}
