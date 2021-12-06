/***********************************************************************
* 
* Authour: Chuby Okafor
* build: g++ -O1 -g -o t1 robot.cpp mpu6050.cpp mpu6050.h -Wall 
* run: ./t1
*/


#include "mpu6050.h"
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

#define pwnPinId 0 
#define pwmEnablePinId 79
#define pwmDirPinId 76
#define gpioPulsePin 168
#define stepSpeedNs 	400000
#define stepDutyCycleNs 4000

void sigint(int);
void *myfunc(void *);
bool volatile running = true;
mpu6050 mpu;
gpio pwmEnable(pwmEnablePinId, OUT);
// stepperPWM motors(pwmDirPinId,pwnPinId, stepSpeedNs, stepDutyCycleNs); 
stepperGPIO motors(pwmDirPinId, gpioPulsePin, stepSpeedNs);

int loopDelay = 3700;

int main(int argc, char const *argv[])
{
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


	// if(mpu.setup() !=0){
	// 	return -1;
	// }
	// if(mpu.calibrate(3700) !=0){
	// 	return -1;
	// }
	// for(int idx = 0; ; idx++){
	// 	if(mpu.calculateAngle() != 0){
	// 		return -1;
	// 	}
	// 	usleep(37000);
	// }

	// setup gpio

	return 0;
}

void sigint(int) {

    running = false;
    pwmEnable.~gpio();
    // motors.~stepperPWM();
    motors.~stepperGPIO();
}

void *myfunc(void *) {
	if(pwmEnable.GPIOExport() != 0){
		printf("pwmEnable pin GPIOExport failed\n");
	}else{

		if(pwmEnable.GPIOWrite(LOW) != 0){
			printf("pwmEnable pin write low failed\n");
		}
	}

	//setup stepper
	if(motors.setup() != 0){
		printf("stepper motors setup failed\n");
	}

	motors.pulseRoutine(800);
	
	motors.pulseRoutine(-800);
	motors.pulseRoutine(800);
	// motors.pulsePwm->PWMEnable();
	// usleep(5000000);
	// motors.pulsePwm->PWMDisable();
	






	if(pwmEnable.GPIOWrite(HIGH) != 0){
			printf("pwmEnable pin write low failed\n");
	}
	return NULL;
}