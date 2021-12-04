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

#define BUFFER_MAX 10
#define VALUE_MAX 60
#define MAX_PERIOD_ns 43600000


class pwm
{
public:
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
};
#endif