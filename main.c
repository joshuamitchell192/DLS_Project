#include "registers.h"

extern void Delay(unsigned int);
extern void Setup(void);
extern void WriteChar(unsigned int);
extern unsigned int ReadChar();
	
int main(void)
{	  
	Setup();
	
	WriteChar(0x47);
	WriteChar(0x4F);
	
	while(1)
	{
		unsigned int input = ReadChar();
		
		WriteChar(input);

		// Enable Timer output trigger for ADC and enable timer
		//TIMER0_CTL |= 0x21;
	}
}

