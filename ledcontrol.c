#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int ledControl(int gpio,  int onOff)
{
 	int fd;
 	char buf[128];

 	fd = open("/sys/class/gpio/export", O_WRONLY);
	sprintf(buf, "%d", gpio); // buf : "21"
 	write(fd, buf, strlen(buf));
 	close(fd);

	sprintf(buf, "/sys/class/gpio/gpio%d/direction", gpio);
 	fd = open(buf, O_WRONLY);
 	write(fd, "out", 4);
 	close(fd);
 
 	sprintf(buf, "/sys/class/gpio/gpio%d/value", gpio);
 	fd = open(buf, O_WRONLY);
 	if( onOff == 1)
  		write(fd,"1",2);
 	else
  		write(fd,"0",2);
   	close(fd);
 	getchar(); // input KBD
 
	fd = open("/sys/class/gpio/unexport", O_WRONLY);
	sprintf(buf, "%d", gpio);
	write(fd, buf,strlen(buf));
	close(fd);
  return 0;
}
// Usage : ./ledgpio 21 1/0
int main(int argc, char **argv) 
{
 	int gno;
	int onOff;
 
 	if(argc != 3){
		printf("Usage: %s GPIO_NO\n", argv[0]);
		return -1;
 	}
 
 	gno = atoi(argv[1]);
 	onOff = atoi(argv[2]);
	ledControl(gno, onOff);
 
 	return 0;
}
