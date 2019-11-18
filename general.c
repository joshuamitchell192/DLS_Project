#include "registers.h"

void Delay(unsigned int);
void Setup(void);

extern void SensorADCSetup(void);
extern void Uart0TerminalSetup(void);

extern void DriverGPIOSetup(void);
extern void LimitSwitchesGPIOSetup(void);
extern void DriverTimerSetup(void);
extern void StepSizeGPIOSetup(void);


void Delay(unsigned int numLoops)
{
	/*
		Simple delay function used to pause when reaching first position in scan between etc.
	*/
	volatile unsigned int lp;
	volatile unsigned int i;

	for(lp=0; lp<numLoops; lp++)
		for (i=0; i<=0xA00; i++) ;
}

void Setup(void)
{
	/*
		Calls all setup functions and returns to main to wait for instructions
	*/
	SensorADCSetup();
	
	Uart0TerminalSetup(); 
	
	DriverGPIOSetup();
	
	LimitSwitchesGPIOSetup();
	
	StepSizeGPIOSetup();

	DriverTimerSetup();
}
