/***********************************************************************
* 
* Authour: Chuby, Stew, Khoa
*/

#ifndef STEPPER_PWM_H
#define STEPPER_PWM_H

#include "gpio.h"
#include "pwm.h"
#include <time.h>


class stepperPWM
{
public:
	stepperPWM(int gpioIdDir,  int pwmIdPulse, int speed_ns, int duty_cycle_ns);
	~stepperPWM();
	void setSpeed_ns(int speed_ns);
	void pulseRoutine(int stepsToTake);
	int setup();
	gpio* dirGpio;
	pwm* pulsePwm;
	int stepSpeed_ns;
	timespec stepDuration = {};


	
};
#endif