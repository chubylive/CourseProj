/***********************************************************************
* 
* Authour: Chuby, Stew, Khoa
*/
#ifndef PWM_H
#define PWM_H

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>

class pwm
{
public:
	static const int BUFFER_MAX = 10;
	static const int VALUE_MAX = 60;
	static const int MAX_PERIOD_ns = 43600000;
	pwm(int pwmId, int period, int duty_cycle);
	~pwm();
	int pwmId;
	int period;
	bool enabled;
	int duty_cycle;
	int PWMEnable();
	int PWMDisable();
	int PWMExport();
	int PWMUnexport();
	int PWMSetPeriod(int period);
	int PWMSetDutyCycle(int duty_cycle);
	int PWMSetFreqPulse(float freqHz);
	int PWMSetFreqNanoSec(int nano_sec);
};
#endif