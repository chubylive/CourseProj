/***********************************************************************
* 
* Authour: Chuby, Stew, Khoa
*/

#include "gpio.h"

gpio::gpio(int gpioId, int gpioDir){
	this->gpioId = gpioId;
	this->gpioDir = gpioDir;

}

gpio::~gpio(){
	GPIOUnexport();
}

int gpio::GPIOExport()
{
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;

	fd = open("/sys/class/gpio/export", O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open export for writing!\n");
		return(-1);
	}

	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", this->gpioId);
	if(write(fd, buffer, bytes_written) == -1){
		fprintf(stderr, "Failed to write export for gpio!\n");
		return(-1);
	}
	close(fd);
	printf("successful gpio %d GPIOExport\n", this->gpioId);
	usleep(1000000);
	return GPIODirection();
}

int gpio::GPIOUnexport()
{
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;

	fd = open("/sys/class/gpio/unexport", O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open unexport for writing!\n");
		return(-1);
	}

	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", this->gpioId);
	if(write(fd, buffer, bytes_written) == -1){
		fprintf(stderr, "Failed to write unexport for gpio %d!\n",this->gpioId);
		return(-1);
	}
	close(fd);
	printf("successful gpio %d GPIOUnexport\n", this->gpioId);

	return(0);
}

int gpio::GPIODirection()
{
	static const char s_directions_str[]  = "in\0out";

	char path[DIRECTION_MAX];
	int fd;

	snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", this->gpioId);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio direction for writing!\n");
		return(-1);
	}

	if (-1 == write(fd, &s_directions_str[IN == this->gpioDir ? 0 : 3], IN == this->gpioDir ? 2 : 3)) {
		fprintf(stderr, "Failed to set direction!\n");
		return(-1);
	}

	close(fd);
	printf("successful gpio %d GPIODirection\n", this->gpioId);

	return(0);
}

int gpio::GPIORead()
{

	char path[VALUE_MAX];
	char value_str[3];
	int fd;

	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", this->gpioId);
	fd = open(path, O_RDONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio value for reading!\n");
		return(-1);
	}

	if (-1 == read(fd, value_str, 3)) {
		fprintf(stderr, "Failed to read value!\n");
		return(-1);
	}

	close(fd);

	return(atoi(value_str));
}

int gpio::GPIOWrite(int value)
{
	static const char s_values_str[] = "01";

	char path[VALUE_MAX];
	int fd;

	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", this->gpioId);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio value for writing!\n");
		return(-1);
	}

	if (1 != write(fd, &s_values_str[LOW == value ? 0 : 1], 1)) {
		fprintf(stderr, "Failed to write value!\n");
		return(-1);
	}

	close(fd);
	return(0);
}