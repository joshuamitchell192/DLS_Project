#include "registers.h"

extern void Delay(unsigned int);
extern void Setup(void);
extern void WriteChar(unsigned int);
extern unsigned int ReadChar();
extern void calibrate(void);
	
int main(void)
{	  
	Setup();

	WriteChar(0x47);
	WriteChar(0x4F);
	
	while(1)
	{
		unsigned int input = ReadChar();
		
		if (input == (unsigned int)0x2A)
		{
			calibrate();
		}
		WriteChar(input);

//		// Enable Timer output trigger for ADC and enable timer
//		//TIMER0_CTL |= 0x21;
	}
}
