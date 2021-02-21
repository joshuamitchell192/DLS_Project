#include "serial.h"
#include "DLS.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>

extern "C" void UART0_Handler (void);
extern "C" void ADC0SS3_Handler (void);
extern "C" void SysTick_Handler(void);

DLS dls;
bool terminalMode = false;

int main(void)
{
    if (terminalMode) Serial::WriteString("Enter an instruction: ");
    dls.SetTerminalMode(terminalMode);
    dls.EventLoop();
}

/**
* Interrupt Handler for the UART receive buffer to read out the data.
*/
void UART0_Handler(void) {
    char c = Serial::ReadChar();
    dls.ReadSerial(c);

    if (terminalMode) Serial::WriteChar(c);
}

/**
* Interrupt handler to read data out of the ADC data registers and add it to the sample total
* while incrementing the sample count.
*/
void ADC0SS3_Handler (void)
{
    ADC0_ISC_R |= 0x8;

    // Get Data from FIFO
    unsigned int sensorData = ADC0_SSFIFO3_R;
    dls.driver.sampleTotal += sensorData;
    dls.driver.numSamples++;
}

/**
* Interupt handler to increment the sampling time value when the 
* sysTick timer reaches its max value.
*/
void SysTick_Handler(void){
	dls.driver.totalTimeElapsed += 1.048575;
}

