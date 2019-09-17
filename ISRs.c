#include "registers.h"

void ISR_Seq3ADC0(void);

void ISR_Seq3ADC0(void)
{
	ADC0_ISC |= 0x8;
	
	// Get Data from FIFO
	int sensorData = ADC0_SSFIFO3 & 0xF;
	
	
}
