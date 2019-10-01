#include "registers.h"

void Delay(unsigned int);
void Setup(void);

extern void SensorADCSetup(void);
extern void Uart0TerminalSetup(void);

extern void DriverGPIOSetup(void);
extern void LimitSwitchesGPIOSetup(void);
extern void DriverTimerSetup(void);

void Delay(unsigned int numLoops)
{
	volatile unsigned int lp;
	volatile unsigned int i;

	for(lp=0; lp<numLoops; lp++)
		for (i=0; i<=0xA00; i++) ;
}

void Setup(void)
{
	SensorADCSetup();
	
	Uart0TerminalSetup(); 
	
	DriverGPIOSetup();
	
	LimitSwitchesGPIOSetup();

	DriverTimerSetup();
}
