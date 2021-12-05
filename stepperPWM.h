/***********************************************************************
* 
* Authour: Chuby, Stew, Khoa
*/

#ifndef STEPPER_PWM_H
#define STEPPER_PWM_H

#include "gpio.h"
#include "pwm.h"
#include <time.h>

#define PULSE_DURATION_NS 1000 

class stepper_PWM
{
public:
	stepper_PWM(int gpioIdDir,  int pwmIdPulse, int speed_ns, int duty_cycle_ns);
	~stepper_PWM();
	void setSpeed_ns(int speed_ns);
	void pulseRoutine(int stepsToTake);

	gpio* dirGpio;
	pwm* pulsePwm;
	int stepSpeed_ns;
	timespec stepDuration = {};


	
};
#endif