/***********************************************************************
* 
* Authour: Chuby, Stew, Khoa
*/

#ifndef GPIO_H
#define GPIO_H

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>


#define IN  0
#define OUT 1
#define LOW  0
#define HIGH 1

class gpio
{
public:
	static const int VALUE_MAX =  30;
	static const int BUFFER_MAX =  10;
	static const int DIRECTION_MAX = 35;
	gpio(int gpioId, int gpioDir);
	~gpio();
	int gpioId;
	int gpioDir;
	int GPIOExport();
	int GPIOUnexport();
	int GPIODirection();
	int GPIORead();
	int GPIOWrite(int value);	
};


#endif
