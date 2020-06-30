#include "helpers.h"

void Helpers::EnableInterrupts(void)
{ 
	 __asm("CPSIE i");
}


void Helpers::DisableInterrupts(void)
{ 
	__asm("CPSID i");
}

void Helpers::WriteChar(unsigned int value)
{
	while ((UART0_FR & 0x20) == 0x20);
	UART0_DR = value;
}

unsigned int Helpers::ReadChar()
{
	while ((UART0_FR & 0x10) == 0x10);
	unsigned int data = (unsigned int)UART0_DR;
	UART0_FR &= ~0x10;
	return data;
}



unsigned int Helpers::getPrescaler(double seconds){
	volatile double preload = 0xFFFFFFFF;
	volatile double prescaler = (seconds * 16000000)/(preload+1) -1;
	volatile double fractional = prescaler - ((long)prescaler);
	if (fractional > 0.0001){
			prescaler += 1;
	}
	return (unsigned int)prescaler;
}

unsigned int Helpers::getPreload(double seconds, int prescaler){
	volatile double preload = (seconds * 16000000)/(prescaler + 1) -1;
	volatile double fractional = preload - ((long)preload);
	if (fractional > 0.5){
			return (int)preload + 1;
	}

	return (unsigned int)preload;
	
}
