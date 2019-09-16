#include "registers.h"

void Delay(unsigned int);
void Setup(void);

extern void SensorADCSetup(void);

void Delay(unsigned int numLoops)
{
	volatile unsigned int lp;
	volatile unsigned int i;

	for(lp=0; lp<numLoops; lp++)
		for (i=0; i<=0xFFFF; i++) ;
}

void Setup(void)
{
	SensorADCSetup();
	
	// 
	
}





