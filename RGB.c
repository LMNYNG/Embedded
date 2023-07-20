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

#define BH1750_ADDR 0x23 // Replace with the actual I2C address of the sensor



int main()

{
	gpioExport(21);
	gpioDirection(21,1);
	
    int fd;

        int     luxRaw=1000;

        float   lux;

    unsigned char buf[3] = {0};



    if ((fd = open("/dev/i2c-1", O_RDWR)) < 0) { // Open the I2C device file

        perror("open");

        exit(1);

    }



    if (ioctl(fd, I2C_SLAVE, BH1750_ADDR) < 0) { // Set the I2C slave address

        perror("ioctl");

        exit(1);

    }



	while(luxRaw>12){

    // Send measurement request to the sensor

    //buf[0] = 0x01; // Power On

    //buf[0] = 0x07; // Reset

    buf[0] = 0x23; // One time L-Resolution measurement

    if (write(fd, buf, 1) != 1) {

        perror("write");

        exit(1);

    }



    // Wait for measurement to be ready (typically takes 120ms)

    usleep(120000);



    // Read the measurement value from the sensor

    if (read(fd, buf, 2) != 2) {

        perror("read");

        exit(1);

    }



    // Convert the measurement value to lux

    luxRaw = (buf[0] << 8) | buf[1];

    lux = (float)luxRaw / 1.2; // Divide by 1.2 to get the actual lux value



    printf("Lux: %d(%7.3f) :: %x, %x\n", luxRaw, lux, buf[0], buf[1]);
	gpioExport(14);
	gpioExport(15);
	gpioExport(18);
	gpioDirection(14,1);
	gpioDirection(15,1);
	gpioDirection(18,1);
	for(int i=0;i<LED_NUM;i++) gpioExport(gpios[i]);
 	for(int i=0;i<LED_NUM;i++) gpioDirection(gpios[i],1);//out
 	/*
	if(luxRaw<=100) {
		gpioWrite(14,1);
		gpioWrite(15,0);
		gpioWrite(18,0);
	}
	if(luxRaw>100&&luxRaw<=200){
		gpioWrite(14,0);
		gpioWrite(15,1);
		gpioWrite(18,0);
	}
	if(luxRaw>200){
		gpioWrite(14,0);
		gpioWrite(15,0);
		gpioWrite(18,1);
	}
	*/
	gpioWrite(14,0);
	gpioWrite(15,1);
	gpioWrite(18,1);
	if(luxRaw<=40){
		for(int i=0;i<8;i++) gpioWrite(gpios[i],digitNum[0][i]);
	}
	else if(luxRaw<=80){
		for(int i=0;i<8;i++) gpioWrite(gpios[i],digitNum[1][i]);
	}
	else if(luxRaw<=120){
		for(int i=0;i<8;i++) gpioWrite(gpios[i],digitNum[2][i]);
	}
	else if(luxRaw<=160){
		for(int i=0;i<8;i++) gpioWrite(gpios[i],digitNum[3][i]);
	}
	else if(luxRaw<=200){
		for(int i=0;i<8;i++) gpioWrite(gpios[i],digitNum[4][i]);
	}
	else if(luxRaw<=240){
		for(int i=0;i<8;i++) gpioWrite(gpios[i],digitNum[5][i]);
	}
	else if(luxRaw<=280){
		for(int i=0;i<8;i++) gpioWrite(gpios[i],digitNum[6][i]);
	}
	else if(luxRaw<=320){
		for(int i=0;i<8;i++) gpioWrite(gpios[i],digitNum[7][i]);
	}
	else if(luxRaw<=360){
		for(int i=0;i<8;i++) gpioWrite(gpios[i],digitNum[8][i]);
	}
	else {
		for(int i=0;i<8;i++) gpioWrite(gpios[i],digitNum[9][i]);
	}

}
	gpioWrite(14,0);
	gpioWrite(15,0);
	gpioWrite(18,0);
		
	gpioUnexport(14);
	gpioUnexport(15);
	gpioUnexport(18);
	for(int i=0; i<8;i++) {
		gpioWrite(gpios[i],0);
		gpioUnexport(gpios[i]);
	}	
    close(fd);



    return 0;

}

