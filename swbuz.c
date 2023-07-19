#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/mman.h>

#include "gpiolib.c"

#define BUZZER_PIN 21 /* GPIO 21 */

#define NOTE_C4  100 /* frequency of C4 note in Hz */
#define NOTE_D4  200 
#define NOTE_E4  300
#define NOTE_F4  400
#define NOTE_G4  500
#define NOTE_A4  600
#define NOTE_B4  700
/*
#define NOTE_C4  262 // frequency of C4 note in Hz
#define NOTE_D4  294 
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
*/
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

int pwmControl(int gpio,  int dutyRate, int dutyT)
{
 	int fd, cnt, dutyOn, dutyOff;
 	char buf[128];
	char inCh;

	printf("PWM duty(%d, %d) \n", dutyOn, dutyOff);

	gpioExport(gpio);

	gpioDirection(gpio, 1) ; // "out"

	cnt = 10;
	dutyOn = (dutyT*dutyRate/100)*1000 ; //nano unit
	dutyOff = (dutyT*(100-dutyRate)/100)*1000 ; //nano unit
	while(cnt--) {
  		ledControl(21, 1);
		usleep(dutyOn);
  		ledControl(21, 0);
		usleep(dutyOff);
	}

	gpioUnexport(gpio);

	//printf("PWM END !!!");

	return 0;
}


/* Function to play a note on the buzzer */
// note : freq(=W), T = 1/W
// duration : time in msec
// period = 10**6 / note = T (in usec)

void playNote(int note, int duration) {
  int period = 1000000 / note;
  int halfPeriod = period / 2;
  int cycles = duration * note / 1000;
  for (int i = 0; i < cycles; i++) { // == cycles * T == duration * 1000 (in usec)
	gpioWrite(BUZZER_PIN, 1);
	usleep(halfPeriod); // T/2
	gpioWrite(BUZZER_PIN, 0);
	usleep(halfPeriod); // T/2
  }
}

/* Function to play a melody */
void playMelody(int notes[], int durations[], int numNotes) {
  for (int i = 0; i < numNotes; i++) {
    playNote(notes[i], durations[i]);
    usleep(500000); /* pause between notes to avoid overlapping */
  }
}

int main(void) {
	// gpio_set_value(BUZZER_PIN, 0); /* set the pin mode to output and turn off the buzzer */
	// usleep(1000000); /* pause for 1 second before playing the melody */

	int melody[] = { NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4 };
	int durations[] = { 500, 500, 500, 500, 500, 500, 500 };
	int numNotes = sizeof(melody) / sizeof(melody[0]);

	int gpio = 21;

	gpioExport(gpio);
	gpioDirection(gpio, 1) ; // "out"
	gpioExport(20);
	gpioDirection(20, 0) ; // "in"
	
	while(1)
{	
	while(gpioRead(20)==0)playNote(NOTE_A4,200);
}
	gpioUnexport(gpio);
	gpioUnexport(20);

	return 0;
}

//////////////////////////////////////////////////////
/* 
// Usage : ./ledgpio 21 1/0
int main(int argc, char **argv) 
{
 	int gno;
	int onOff, vol;
	char ch;

	printf("Start PWM !!!\n");

	vol = 100; // Duty Rate in Percent %
	while(vol) {
		pwmControl(21, vol, 33);
		vol = vol - 5;
	}

 	return 0;
}

 	if(argc != 3){
		printf("Usage: %s GPIO_NO\n", argv[0]);
		return -1;
 	}
 
 	gno = atoi(argv[1]);
 	onOff = atoi(argv[2]);

	ledSW_Control(20, 0);
*/
