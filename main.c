#include "registers.h"

extern void Delay(unsigned int);
extern void Setup(void);
extern void WriteChar(unsigned int);
extern unsigned int ReadChar();
extern int calibrate(void);
void wait(void);
	
int main(void)
{	  
	Setup();

	WriteChar(0x47);
	WriteChar(0x4F);
	
	// Enable Timer output trigger for ADC and enable timer
	//TIMER0_CTL |= 0x21;
	
	wait();
}

void wait (void)
{
	while (1)
	{
		
	}
}