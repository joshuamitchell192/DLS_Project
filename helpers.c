#include "registers.h"

void EnableInterrupts(void);
void DisableInterrupts(void);
void WriteChar(unsigned int);

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
	while ((UART0_FR & 0x20) != 0);
	UART0_DR |= value;
	UART0_DR &= ~0xFF;
}
