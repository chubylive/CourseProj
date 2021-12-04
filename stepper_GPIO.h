/***********************************************************************
* 
* Authour: Chuby, Stew, Khoa
*/

#ifndef STEPPER_GPIO_H
#define STEPPER_GPIO_H

#include <gpio.h>

class stepper_GPIO
{
public:
	stepper_GPIO(int gpioIdDir, int gpioIdPulse);
	~stepper_GPIO();
	int setupStepper();
	int setSpeed_ns(int speed_ns);
	void pulseRoutine();

	gpio* dirGpio;
	gpio* pulseGpio;
	int stepSpeed_ns;
	
};
#endif