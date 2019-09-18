#include "registers.h"

void ADC0SS3_Handler (void);
extern void WriteChar(unsigned int);
extern void Delay(unsigned int);

void ADC0SS3_Handler (void)
{
	ADC0_ISC |= 0x8;
	
	// Get Data from FIFO
	unsigned int sensorData = ADC0_SSFIFO3 & 0xF;
	unsigned int delayValue = sensorData;
	
	// Write variable to terminal
	
	// Wait for transmission queue to be empty
	//while ((UART0_FR & 0x20) == 0x20);

	WriteChar(0x21);
	Delay(delayValue);
	
}
