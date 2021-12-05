/***********************************************************************
* 
* Authour: Chuby, Stew, Khoa
*/

#ifndef STEPPER_GPIO_H
#define STEPPER_GPIO_H

#include "gpio.h"
#include <time.h>

#define PULSE_DURATION_NS 1000 

class stepper_GPIO
{
public:
	stepper_GPIO(int gpioIdDir, int gpioIdPulse);
	~stepper_GPIO();
	void setSpeed_ns(int speed_ns);
	void pulseRoutine(int stepsToTake);

	gpio* dirGpio;
	gpio* pulseGpio;
	int stepSpeed_ns;
	timespec pulseSleep = {};
	timespec speedSleep = {};


	
};
#endif