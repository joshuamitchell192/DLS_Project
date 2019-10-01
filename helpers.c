#include "registers.h"

void EnableInterrupts(void);
void DisableInterrupts(void);
void WriteChar(unsigned int);
unsigned int ReadChar();

int getPreload(double seconds, int prescaler);
int getPrescaler(double seconds);

static inline void EnableInterrupts(void)
{ 
	 __asm("CPSIE i");
}


static inline void DisableInterrupts(void)
{ 
	__asm("CPSID i");
}

void WriteChar(unsigned int value)
{
	while ((UART0_FR & 0x20) == 0x20);
	UART0_DR |= value;
	
}

unsigned int ReadChar()
{
	while ((UART0_FR & 0x10) == 0x10);
	unsigned int data = (unsigned int)UART0_DR;
	UART0_FR &= ~0x10;
	return data;
	
}

int getPrescaler(double seconds){
	double preload = 65535;
	double prescaler = (seconds * 16000000)/(preload+1) -1;
	double fractional = prescaler - ((long)prescaler);
	if (fractional > 0.0001){
			prescaler += 1;
	}
	return (int)prescaler;
}

int getPreload(double seconds, int prescaler){
	double preload = (seconds * 16000000)/(prescaler + 1) -1;
	double fractional = preload - ((long)preload);
	if (fractional > 0.5){
			return (int)preload + 1;
	}

	return (int)preload;
	
}
