#include "registers.h"


extern void Delay(unsigned int);

extern int getPreload(double seconds, int prescaler);
extern int getPrescaler(double seconds);

int calibrate(void);
void setDriverTimer(double seconds);
void stepMotor(void);

extern int currentPosition;
extern float stepsPerMM;

int sampleTotal;
int numSamples;

void sampleTwoPoints(double sampleDuration, int point1, int point2){
	setDriverTimer(sampleDuration);
	//get first
	volatile int  v = currentPosition;
	if (point2 < point1){
			int temp = point1;
			point1 = point2;
			point2 = temp;
	}
	int dir;
	if (currentPosition > point1-1){
			GPIOA_DATA &= ~0x8;
			dir = -1;
	}else{
		GPIOA_DATA |= 0x8;
			dir = 1;
	}
	
	while (currentPosition != point1-1){
			currentPosition += dir;
			stepMotor();
	}
	dir = 1;
	GPIOA_DATA |= 0x8;
	
	//Turn on adc timer for sampling
	TIMER0_CTL |= 0x21;
	while (currentPosition <= point2){
			sampleTotal = 0;
			numSamples = 0;
			currentPosition += dir;
		
			stepMotor();
			if (numSamples>0){
				int avgSample = sampleTotal / numSamples;
				
			}
			
	}
	TIMER0_CTL &= ~0x21;
}

void setDriverTimer(double seconds){
	int prescale = getPrescaler(seconds);
	int preload = getPreload(seconds, prescale);
	
	TIMER1_TAILR = preload;
	TIMER1_TAPR = prescale;

	TIMER1_ICR |= 1;
	TIMER1_CTL |= 1;
}


void stepMotor(void){
		while((TIMER1_RIS & 0x1) != 0x1);	
		TIMER1_ICR |= 1;
		GPIOA_DATA |= 0x4;
		while((TIMER1_RIS & 0x1) != 0x1);	
		TIMER1_ICR |= 1;
		GPIOA_DATA &=~ 0x4;
}

int calibrate(void){
	GPIOA_DATA |= 0x8;

	setDriverTimer(0.0007);
	
	while(1){
		stepMotor();
		
		if ((GPIOB_DATA & 0x2) == 0x2){
				break;
		}
	}
	
	GPIOA_DATA &= ~0x8;
	int numSteps = 1;
	while(1){
		stepMotor();
		numSteps ++;
		if ((GPIOB_DATA & 0x1) ==0x1){
				break;
		}
	}
	currentPosition = 0;
	float mm = 60.0;
	stepsPerMM = mm/numSteps;
	return numSteps;
}