/***********************************************************************
* 
* Authour: Chuby Okafor
*
*/
#ifndef STEPPER_H
#define STEPPER_H

class stepper
{
public:
	stepper(int pwmId);
	~stepper();
	char filename[20];
	int file;
	void setSpeed(int speed);
	void step(int number_steps);
	
};


#endif
