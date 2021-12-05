/***********************************************************************
* 
* Authour: Chuby, Stew, Khoa
*/
#include "stepperGPIO.h"


stepper_GPIO::stepper_GPIO(int gpioIdDir, int gpioIdPulse){
	this->dirGpio = new gpio(gpioIdDir, OUT);
	this->pulseGpio = new gpio(gpioIdPulse, OUT);	
	this->pulseSleep.tv_nsec = PULSE_DURATION_NS;
}
stepper_GPIO::~stepper_GPIO(){
	this->dirGpio->~gpio();
	this->pulseGpio->~gpio();
}

void stepper_GPIO::setSpeed_ns(int speed_ns){
	this->stepSpeed_ns = speed_ns;
	this->speedSleep.tv_nsec = (long)speed_ns;
}
void stepper_GPIO::pulseRoutine(int stepsToTake){
	if (stepsToTake < 0)
	{
		this->dirGpio->GPIOWrite(HIGH);
	}else{
		this->dirGpio->GPIOWrite(LOW);
	}
	for (int step = 0; step < stepsToTake; ++step)
	{
		this->pulseGpio->GPIOWrite(HIGH);
		//sleep pulse duration which is 1us 
		nanosleep(&this->pulseSleep, NULL);
		this->pulseGpio->GPIOWrite(LOW);
		//sleep for speed duration which is around 20 us
		nanosleep(&this->speedSleep, NULL);

	}
}