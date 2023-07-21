#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <fcntl.h>

#include <unistd.h>

#include <sys/mman.h>

#include <linux/i2c-dev.h>

#include <sys/ioctl.h>

#include <unistd.h>


#include"gpiolib.c"

//R=14, G=15, B=18
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
float colors[7][6]={
					{1,0,0,1,0,0},//R
					{1,1,0,0.8,0.2,0},//O
					{1,1,0,0.5,0.5,0},//Y
					{0,1,0,0,1,0},//G
					{0,0,1,0,0,1},//B
					{0,0,1,0.4,0.4,0.3},//BB
					{1,0,1,0.5,0,0.5}//P
				};

#define BH1750_ADDR 0x23 // Replace with the actual I2C address of the sensor

int pwmControl(int color, int dutyRate, int dutyT)
{
 	int cnt, dutyOn, dutyOff;


	printf("PWM duty(%d, %d) \n", dutyOn, dutyOff);


	cnt = 50;
	dutyOn = (dutyT*dutyRate/100)*1000 ; //micro unit
	dutyOff = (dutyT*(100-dutyRate)/100)*1000 ; //micro unit
	while(cnt--) {
  		gpioWrite(14,colors[color][0]);
  		usleep(dutyOn*colors[color][3]);
  		gpioWrite(14,0);
  		usleep(dutyOff*colors[color][3]);
  		
  		gpioWrite(15,colors[color][1]);
  		usleep(dutyOn*colors[color][4]);
  		gpioWrite(15,0);
  		usleep(dutyOff*colors[color][4]);
  		
  		gpioWrite(18,colors[color][2]);
  		usleep(dutyOn*colors[color][5]);
  		gpioWrite(18,0);
  		usleep(dutyOff*colors[color][5]);
	}

	printf("PWM END !!!");

	return 0;
}

int main()

{	
	gpioExport(14);
	gpioExport(15);
	gpioExport(18);
	gpioDirection(14,1);
	gpioDirection(15,1);
	gpioDirection(18,1);
	
	for(int c=0;c<7;c++) {
		pwmControl(c,100, 11);
		gpioWrite(14,0);
		gpioWrite(15,0);
		gpioWrite(18,0);
		printf("%d\n",c);
	}

	gpioWrite(14,0);
	gpioWrite(15,0);
	gpioWrite(18,0);
		
	gpioUnexport(14);
	gpioUnexport(15);
	gpioUnexport(18);
	/*
	for(int i=0; i<8;i++) {
		gpioWrite(gpios[i],0);
		gpioUnexport(gpios[i]);
	}	
	*/


    return 0;

}

