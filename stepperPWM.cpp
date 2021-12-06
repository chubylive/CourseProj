/***********************************************************************
* 
* Authour: Chuby, Stew, Khoa
*/

#include "stepperPWM.h"

stepperPWM::stepperPWM(int gpioIdDir, int pwmIdPulse, int speed_ns, int duty_cycle_ns){
	this->dirGpio = new gpio(gpioIdDir, OUT);
	this->pulsePwm = new pwm(pwmIdPulse, speed_ns, duty_cycle_ns);	
	this->stepSpeed_ns = speed_ns;
}

stepperPWM::~stepperPWM(){
	this->dirGpio->~gpio();
	this->pulsePwm->~pwm();
}

int  stepperPWM::setup(){
	if(this->dirGpio->GPIOExport() != 0){
		return -1;
	}
	return this->pulsePwm->PWMExport();
}
void stepperPWM::setSpeed_ns(int speed_ns){
	this->stepSpeed_ns = speed_ns;
}

void stepperPWM::pulseRoutine(int stepsToTake){

	this->stepDuration.tv_nsec = stepsToTake *  this->stepSpeed_ns;
	if (stepsToTake < 0)
	{
		this->dirGpio->GPIOWrite(HIGH);
		this->stepDuration.tv_nsec = -stepsToTake *  this->stepSpeed_ns;
	}else{
		this->dirGpio->GPIOWrite(LOW);
		this->stepDuration.tv_nsec = stepsToTake *  this->stepSpeed_ns;
	}
	this->pulsePwm->PWMEnable();
	nanosleep(&this->stepDuration, NULL);
	this->pulsePwm->PWMDisable();

}