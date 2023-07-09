#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int gpioExport(int gpio) {
 	int fd;
	char buf[128];

 	fd = open("/sys/class/gpio/export", O_WRONLY);
	if(fd < 0) {
		perror("GPIO ERR : export\n");
		return(fd);
	}
	sprintf(buf, "%d", gpio); // buf : "number"

	printf("gpio#:%s\n", buf);
 	write(fd, buf, strlen(buf));
 	close(fd);
}

int gpioDirection(int gpio, int dir) {
 	int fd;
	char buf[128];

	printf("INput gpio_Direction !!!\n");

	sprintf(buf, "/sys/class/gpio/gpio%d/direction", gpio);
 	fd = open(buf, O_WRONLY);
	if(fd < 0) {
		perror("GPIO ERR : direction\n");
		return(fd);
	}
	if(dir == 0)
 		write(fd, "in", 3);
	else
 		write(fd, "out", 4);
 	close(fd);
}
 
int gpioRead(int gpio) {
 	int fd;
	char buf[128], inCh;

 	sprintf(buf, "/sys/class/gpio/gpio%d/value", gpio);
 	fd = open(buf, O_RDONLY);
	if(fd < 0) {
		perror("GPIO ERR : value\n");
		return(fd);
	}

	read(fd,&inCh,1);
	printf("INput gpio%d[%x] !!!\n", gpio, inCh);

	inCh = inCh - '0';

   	close(fd);

	return(inCh);
}

int gpioWrite(int gpio, int val) {
 	int fd;
	char buf[128];

 	sprintf(buf, "/sys/class/gpio/gpio%d/value", gpio);
 	fd = open(buf, O_WRONLY);
	if(fd < 0) {
		perror("GPIO ERR : value\n");
		return(fd);
	}

	if(val == 0)
		write(fd, "0", 2);
	else
		write(fd, "1", 2);

   	close(fd);

	return(0);
}

int gpioUnexport(int gpio) {
 	int fd;
	char buf[128];
	fd = open("/sys/class/gpio/unexport", O_WRONLY);
	if(fd < 0) {
		perror("GPIO ERR : unexport\n");
		return(fd);
	}
	sprintf(buf, "%d", gpio);
	write(fd, buf,strlen(buf));
	close(fd);
}

int ledControl(int gpio,  int onOff)
{
 	int fd;
 	char buf[128];

	gpioExport(gpio);

	gpioDirection(gpio, 1) ; // "out"

	gpioWrite(gpio, onOff);

	gpioUnexport(gpio);

	return 0;
}

int ledSW_Control(int gpio,  int onOff)
{
 	int fd;
 	char buf[128];
	char inCh;

	gpioExport(gpio);

	gpioDirection(gpio, 0) ; // "in"

	while(1) {
  		ledControl(21, 0);
		while(gpioRead(gpio) == 0) 
  			ledControl(21, 1);
	}

	gpioUnexport(gpio);

	return 0;
}

// Usage : ./ledgpio 21 1/0
int main(int argc, char **argv) 
{
 	int gno;
	int onOff;
/* 
 	if(argc != 3){
		printf("Usage: %s GPIO_NO\n", argv[0]);
		return -1;
 	}
 
 	gno = atoi(argv[1]);
 	onOff = atoi(argv[2]);
*/
	printf("Start Led SW !!!\n");
	ledSW_Control(20, 0);
 
 	return 0;
}
