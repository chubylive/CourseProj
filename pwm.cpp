/***********************************************************************
* 
* Authour: Chuby, Stew, Khoa
*/
#include "pwm.h"


pwm::pwm(int pwmId, int period, int duty_cycle){
	this->pwmId = pwmId;
	this->period = period;
	this->duty_cycle = duty_cycle;
	this->enabled = false;
}
pwm::~pwm(){
	PWMUnexport();
}
int pwm::PWMExport(){
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;

	fd = open("/sys/class/pwm/pwmchip0/export", O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open pwm export for writing!\n");
		return(-1);
	}

	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", this->pwmId);
	if(write(fd, buffer, bytes_written) == -1){
		fprintf(stderr, "Failed to write export for pwm!\n");
		return(-1);
	}
	close(fd);
	return (0);
}

int pwm::PWMUnexport(){
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;

	fd = open("/sys/class/pwm/pwmchip0/unexport", O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open pwm unexport for writing!\n");
		return(-1);
	}

	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", this->pwmId);
	if(write(fd, buffer, bytes_written) == -1){
		fprintf(stderr, "Failed to write unexport for pwm!\n");
		return(-1);
	}
	close(fd);
	return(0);
}

int pwm::PWMSetPeriod(int period_ns){
	if(period_ns > MAX_PERIOD_ns){
		fprintf(stderr, "pwm period invalid! %d\n",period_ns);

		return -1;
	}
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;

	char path[VALUE_MAX];
	int fd;

	snprintf(path, VALUE_MAX, "/sys/class/pwm/pwmchip0/pwm%d/period", this->pwmId);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open pwm value for setting period!\n");
		return(-1);
	}

	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", period_ns);
	if(write(fd, buffer, bytes_written) == -1){
		fprintf(stderr, "Failed to write period for pwm!\n");
		return(-1);
	}
	close(fd);
	return(0);
}

int pwm::PWMSetDutyCycle(int duty_cycle_ns){
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	char path[VALUE_MAX];
	int fd;

	snprintf(path, VALUE_MAX, "/sys/class/pwm/pwmchip0/pwm%d/duty_cycle", this->pwmId);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open pwm value for setting duty_cycle!\n");
		return(-1);
	}

	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", duty_cycle_ns);
	if(write(fd, buffer, bytes_written) == -1){
		fprintf(stderr, "Failed to write duty cycle for pwm!\n");
		return(-1);
	}
	close(fd);
	return(0);
}

int pwm::PWMSetFreqPulse(float freqHz){
	if(freqHz < 23){ // if frequency in hertz is less than 23 return error
		return -1;
	}
	int periodCov = (1000000000/freqHz);
	PWMSetDutyCycle(periodCov/2);
	PWMSetPeriod(periodCov);
	if (!enabled)
	{
		PWMEnable();
	}
	return	0;
}

int pwm::PWMSetFreqNanoSec(int nano_sec){

	if(nano_sec > 434782608){ // if nano sec greater that 434,782,608 return error
		return -1;
	}
	
	PWMSetDutyCycle(nano_sec/2);
	PWMSetPeriod(nano_sec);
	if (!enabled)
	{
		PWMEnable();
	}
	return	0;
}

int pwm::PWMEnable(){
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	char path[VALUE_MAX];
	int fd;

	snprintf(path, VALUE_MAX, "/sys/class/pwm/pwmchip0/pwm%d/enable", this->pwmId);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open pwm value for setting enable!\n");
		return(-1);
	}

	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", 1);
	if(write(fd, buffer, bytes_written) == -1){
		fprintf(stderr, "Failed to write enable for pwm!\n");
		return(-1);
	}
	close(fd);
	enabled = true;
	return(0);
}


int pwm::PWMDisable(){
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	char path[VALUE_MAX];
	int fd;

	snprintf(path, VALUE_MAX, "/sys/class/pwm/pwmchip0/pwm%d/enable", this->pwmId);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open pwm value for setting disable!\n");
		return(-1);
	}

	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", 0);
	if(write(fd, buffer, bytes_written) == -1){
		fprintf(stderr, "Failed to write disable for pwm!\n");
		return(-1);
	}
	close(fd);
	enabled = false;
	return(0);
}