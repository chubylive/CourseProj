/***********************************************************************
* 
* Authour: Chuby, Stew, Khoa
*/
#include "stepperGPIO.h"


stepperGPIO::stepperGPIO(int gpioIdDir, int gpioIdPulse, int speed_ns){
	this->dirGpio = new gpio(gpioIdDir, OUT);
	this->pulseGpio = new gpio(gpioIdPulse, OUT);	
	this->pulseSleep.tv_nsec = PULSE_DURATION_NS;
	this->stepSpeed_ns = speed_ns;
	this->speedSleep.tv_nsec = (long)speed_ns;
	this->dirMem =  0;

}
stepperGPIO::~stepperGPIO(){
	this->dirGpio->~gpio();
	this->pulseGpio->~gpio();
}
int stepperGPIO::setup(){
	if(this->dirGpio->GPIOExport() != 0){
		return -1;
	}
	return this->pulseGpio->GPIOExport();
}
void stepperGPIO::setSpeed_ns(int speed_ns){
	this->stepSpeed_ns = speed_ns;
	this->speedSleep.tv_nsec = (long)speed_ns;
}
void stepperGPIO::pulseRoutine(int stepsToTake){
	if (stepsToTake < 0)
	{
		if (this->dirMem == 0)
		{
			usleep(1);
		}
		this->dirMem =  1;
		this->dirGpio->GPIOWrite(HIGH);
		stepsToTake = -stepsToTake;
	}else{
		if (this->dirMem == 1)
		{
			usleep(1);
		}
		this->dirMem =  0;
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