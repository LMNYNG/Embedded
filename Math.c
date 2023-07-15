#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
/*
 A=26 B=16 C=20 DP=21 D=19 E=13 F=6 G=5 
 */
#define LED_NUM 8
int gpios[]=			{26,16,20,19,13,06,05,21};
					   //A  B  C  D  E  F  G  DP
int digitNum[10][8]={
						{1, 1, 1, 1, 1, 1, 0, 0},//0
						{0, 1, 1, 0, 0, 0, 0, 0},//1
						{1, 1, 0, 1, 1, 0, 1, 0},//2
						{1, 1, 1, 1, 0, 0, 1, 0},//3
						{0, 1, 1, 0, 0, 1, 1, 0},//4
						{1, 0, 1, 1, 0, 1, 1, 0},//5
						{1, 0, 1, 1, 1, 1, 1, 0},//6
						{1, 1, 1, 0, 0, 1, 0, 0},//7
						{1, 1, 1, 1, 1, 1, 1, 0},//8
						{1, 1, 1, 1, 0, 1, 1, 0} //9
						
};

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
/*
 A=26 B=16 C=20 DP=21 D=19 E=13 F=6 G=5 
 */
void displayNUM2(int num){
	for(int i=0;i<LED_NUM;i++) gpioExport(gpios[i]);
	for(int i=0;i<LED_NUM;i++) gpioDirection(gpios[i],1);//out
	for(int i=0;i<8;i++) gpioWrite(gpios[i],digitNum[num][i]);
	usleep(500000);
	for(int i=0;i<LED_NUM;i++) gpioWrite(gpios[i],0);
	for(int i=0;i<LED_NUM;i++) gpioUnexport(gpios[i]);
	
}

int displayNUM(int num)
{
 	 
	for(int i=0;i<LED_NUM;i++) gpioExport(gpios[i]);
 	for(int i=0;i<LED_NUM;i++) gpioDirection(gpios[i],1);//out


 	switch(num){
	case 0: for(int i=0;i<LED_NUM;i++) gpioWrite(gpios[i],0);
			gpioWrite(26,1);gpioWrite(16,1);gpioWrite(20,1);gpioWrite(19,1);gpioWrite(13,1);gpioWrite(6,1);
			usleep(1500000);
			break;
	case 1: for(int i=0;i<LED_NUM;i++) gpioWrite(gpios[i],0);
			gpioWrite(16,1);gpioWrite(20,1);
			usleep(1500000);
			break;
	case 2: for(int i=0;i<LED_NUM;i++) gpioWrite(gpios[i],0);
			gpioWrite(26,1);gpioWrite(16,1);gpioWrite(19,1);gpioWrite(13,1);gpioWrite(5,1);
			usleep(1500000);
			break;
	case 3: for(int i=0;i<LED_NUM;i++) gpioWrite(gpios[i],0);
			gpioWrite(26,1);gpioWrite(16,1);gpioWrite(20,1);gpioWrite(19,1);gpioWrite(5,1);
			usleep(1500000);
			break;
	case 4: for(int i=0;i<LED_NUM;i++) gpioWrite(gpios[i],0);
			gpioWrite(16,1);gpioWrite(20,1);gpioWrite(5,1);gpioWrite(6,1);
			usleep(1500000);
			break;	
	case 5: for(int i=0;i<LED_NUM;i++) gpioWrite(gpios[i],0);
			gpioWrite(26,1);gpioWrite(5,1);gpioWrite(20,1);gpioWrite(19,1);gpioWrite(6,1);
			usleep(1500000);
			break;	
	case 6: for(int i=0;i<LED_NUM;i++) gpioWrite(gpios[i],0);
			gpioWrite(26,1);gpioWrite(5,1);gpioWrite(20,1);gpioWrite(19,1);gpioWrite(13,1);gpioWrite(6,1);
			usleep(1500000);
			break;
	case 7: for(int i=0;i<LED_NUM;i++) gpioWrite(gpios[i],0);
			gpioWrite(26,1);gpioWrite(16,1);gpioWrite(20,1);gpioWrite(6,1);
			usleep(1500000);
			break;
	case 8: for(int i=0;i<LED_NUM;i++) gpioWrite(gpios[i],0);
			gpioWrite(26,1);gpioWrite(16,1);gpioWrite(20,1);gpioWrite(19,1);gpioWrite(13,1);gpioWrite(6,1);gpioWrite(5,1);
			usleep(1500000);
			break;
	case 9: for(int i=0;i<LED_NUM;i++) gpioWrite(gpios[i],0);
			gpioWrite(26,1);gpioWrite(16,1);gpioWrite(20,1);gpioWrite(19,1);gpioWrite(5,1);gpioWrite(6,1);
			usleep(1500000);
			break;
	
	}
	for(int i=0;i<LED_NUM;i++) gpioWrite(gpios[i],0);
	for(int i=0;i<LED_NUM;i++) gpioUnexport(gpios[i]);
 	
 	

  return 0;
}
void segCheck(){
	for(int i=0;i<LED_NUM;i++) gpioExport(gpios[i]);
 	for(int i=0;i<LED_NUM;i++) gpioDirection(gpios[i],1);//out
 	for(int i=0;i<3;i++){
		for(int i=0;i<LED_NUM;i++) gpioWrite(gpios[i],1);
		usleep(200000);
		for(int i=0;i<LED_NUM;i++) gpioWrite(gpios[i],0);
		usleep(200000);
	}
}
// Usage : ./ledgpio 21 1/0
int main(int argc, char **argv) 
{
 	int gno;
	int onOff;
	int n1 = (int)(getchar()-48);
	getchar();
	int n2 = (int)(getchar()-48);
	getchar();
	if(n1+n2<10){
		displayNUM(n1+n2);
	}
	else {
		segCheck();
	}
 	return 0;
}
