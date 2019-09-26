#include "registers.h"

extern void Delay(unsigned int);

void calibrate(void);

void calibrate(void){
	GPIOA_DATA &= ~0x8;

	while(1){
		GPIOA_DATA |= 0x4;
		Delay(1);
		GPIOA_DATA &= ~0x4;
		Delay(1);
		
		if ((GPIOB_DATA & 0x1) == 0x1){
				break;
		}
	}
	
	GPIOA_DATA |= 0x8;
	int numSteps = 0;
	while(1){
		GPIOA_DATA |= 0x4;
		Delay(1);
		GPIOA_DATA &= ~0x4;
		Delay(1);
		numSteps ++;
		if ((GPIOB_DATA & 0x2) ==0x2){
				break;
		}
	}
	double numStepsPerMM = 25.0f/numSteps;
	
}
