#include "registers.h"

void EnableInterrupts(void);
void DisableInterrupts(void);
void WriteChar(unsigned int);
unsigned int ReadChar();

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
