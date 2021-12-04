/***********************************************************************
* 
* Authour: Chuby, Stew, Khoa
*/
#include <stepper_GPIO.h>


stepper::stepper_GPIO(int gpioIdDir, int gpioIdPulse){
	this->dirGpio = new gpio(gpioIdDir, OUT);
	this->pulseGpio = new gpio(gpioIdPulse, OUT);	
}
stepper::~stepper_GPIO(){
	this->dirGpio->~gpio();
	this->pulseGpio->~gpio();
}

int stepper::setSpeed_ns(int speed_ns){
	this->stepSpeed_ns = speed;
}
void stepper::pulseRoutine(){

}