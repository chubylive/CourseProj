/***********************************************************************
* 
* Authour: Chuby, Stew, Khoa
*/

#include "stepperPWM.h"

stepper_PWM::stepper_PWM(int gpioIdDir, int pwmIdPulse, int speed_ns, int duty_cycle_ns){
	this->dirGpio = new gpio(gpioIdDir, OUT);
	this->pulsePwm = new pwm(pwmIdPulse, speed_ns, duty_cycle_ns);	
}

stepper_PWM::~stepper_PWM(){
	this->dirGpio->~gpio();
	this->pulsePwm->~pwm();
}

void stepper_PWM::setSpeed_ns(int speed_ns){
	this->stepSpeed_ns = speed_ns;
}

void stepper_PWM::pulseRoutine(int stepsToTake){

	this->stepDuration.tv_nsec = stepsToTake *  this->stepSpeed_ns;
	if (stepsToTake < 0)
	{
		this->dirGpio->GPIOWrite(HIGH);
	}else{
		this->dirGpio->GPIOWrite(LOW);
	}
	this->pulsePwm->PWMEnable();
	nanosleep(&this->stepDuration, NULL);
	this->pulsePwm->PWMDisable();

}