/*********************************************************************
* 
* Authour: Chuby Okafor
* build: g++ -O1 -g -o t1 robot.cpp mpu6050.cpp mpu6050.h -Wall 
* run: ./t1
*/


#include "BNO055_support.h"		//Contains the bridge code between the API and Arduino

#include "stepperPWM.h"
#include "stepperGPIO.h"

#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <sched.h>
#include <stdint.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <chrono>

#define pwnPinId 0 
#define stepperMotorEnablePinId 79
#define pwmDirPinId 76
#define gpioPulsePin 168
#define stepSpeedNs 	400000
#define stepDutyCycleNs 4000

//pid parameters
double input, output, setPoint;
double ITerm, lastInput;
double kp, ki, kd;
int sampleTime_us = 10050 ; //10000 microsecond 100hz sample period to match the sensor data generation period

void sigint(int);
void *myfunc(void *);

struct bno055_t myBNO;
struct bno055_euler myEulerData; //Structure to hold the Euler data
volatile bool running;
BNO055_S16 roll;

std::chrono::steady_clock::time_point lastTime;

void computePid();
void setTunings(double Kp, double Ki, double Kd);
gpio stepperMotorEnable(stepperMotorEnablePinId, OUT);
// stepperPWM motors(pwmDirPinId,pwnPinId, stepSpeedNs, stepDutyCycleNs); 
stepperGPIO motors(pwmDirPinId, gpioPulsePin, stepSpeedNs);

int main(int argc, char const *argv[])
{

  	BNO_Init(&myBNO); //Assigning the structure to hold information about the device

  	bno055_set_operation_mode(OPERATION_MODE_NDOF);
  	usleep(1000);
	
	if(motors.setup() != 0){
		printf("stepper motors setup failed\n");
	}

	stepperMotorEnable.GPIOExport();
	stepperMotorEnable.GPIOWrite(LOW);

 	//    //  make sure ctrl-C stops the program under controlled circumstances
    signal(SIGINT, &sigint);
	

	//  create attributes for an isolated real-time thread
	pthread_attr_t attr = {};
	pthread_attr_init(&attr);
	//  lift the thread off core 0, which takes system interrupts
	cpu_set_t cpuset = {};
	CPU_ZERO(&cpuset);
	CPU_SET(1, &cpuset);
	pthread_attr_setaffinity_np(&attr, 1, &cpuset);
	//  make it use FIFO policy for real-time scheduling
	pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
	pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
	//  set the priority
	sched_param param = {};
	param.sched_priority = 30;
	pthread_attr_setschedparam(&attr, &param);

	//  create the real-time thread
	pthread_t mythread;
	int err;
	if ((err = pthread_create(&mythread, &attr, &myfunc, NULL)) != 0) {
	    char const *emsg = ((err == EAGAIN) ? "EAGAIN" : ((err == EINVAL) ? "EINVAL" : ((err == EPERM) ? "EPERM" : "unknown")));
	    fprintf(stderr, "pthread_create() failed (%d %s); are you sure you're root?\n", err, emsg);
	    fprintf(stderr, "You may also need to do:\n");
	    fprintf(stderr, "echo -1 > /proc/sys/kernel/sched_rt_runtime_us\n");
	    exit(1);
	}

	//  wait for the program to be done
	void *ignore = NULL;
	pthread_join(mythread, &ignore);

	return 0;
}

void sigint(int) {
	if(stepperMotorEnable.GPIOWrite(HIGH) != 0){
			printf("stepperMotorEnable pin write low failed\n");
	}
    stepperMotorEnable.~gpio();
    // motors.~stepperPWM();
    motors.~stepperGPIO();
    running = false;
}

void *myfunc(void *) {
	setPoint = 0;
	setTunings(15,1.5,30);
	running = true;

	while(running){
			 computePid();
			 
			 //generate stepper motor control signals

		}
	
	






	return NULL;
}

void computePid(){

	int timeDiff = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - lastTime).count();
	if(timeDiff >= sampleTime_us){
		//sample sensor
		bno055_read_euler_r(&roll);

		input = ((double)roll/16.00); // converts input to degrees not sure if need
		double error = setPoint - input;
		ITerm += (ki * error);
		double dInput = (input - lastInput);

		//comput output
		output = kp * error + ITerm - kd * dInput;

		lastInput = input;
		lastTime = std::chrono::steady_clock::now();
		printf("roll: %d, input: %.6f, setPoint: %.6f,  output: %.6f\n",roll, input,setPoint,output);
	}

}

void setTunings(double Kp, double Ki, double Kd)
{
  double sampleTime_S = ((double)sampleTime_us)/1000000 ;
   kp = Kp;
   ki = Ki * sampleTime_S;
   kd = Kd / sampleTime_S;
}