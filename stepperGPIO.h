/***********************************************************************
* 
* Authour: Chuby, Stew, Khoa
*/

#ifndef STEPPER_GPIO_H
#define STEPPER_GPIO_H

#include "gpio.h"
#include <time.h>

#define PULSE_DURATION_NS 40000 

class stepperGPIO
{
public:
	stepperGPIO(int gpioIdDir, int gpioIdPulse, int speed_ns);
	~stepperGPIO();
	void setSpeed_ns(int speed_ns);
	void pulseRoutine(int stepsToTake);
	int setup();
	gpio* dirGpio;
	gpio* pulseGpio;
	int stepSpeed_ns;
	int dirMem; 
	timespec pulseSleep = {};
	timespec speedSleep = {};


	
};
#endif