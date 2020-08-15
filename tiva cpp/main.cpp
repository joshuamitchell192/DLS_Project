#include "registers.h"
#include "serial.h"
#include "DLS.h"

extern "C" void UART0_Handler (void);
extern "C" void ADC0SS3_Handler (void);

DLS dls;

int main(void)
{
    Serial::WriteString("Enter an instruction: ");

    //Turn on adc timer for sampling
    dls.EventLoop();

    while(1);
}

void UART0_Handler(void) {
    char c = Serial::ReadChar();
    dls.ReadSerial(c);
    Serial::WriteChar(c);
}

void ADC0SS3_Handler (void)
{
    ADC0_ISC |= 0x8;

    // Get Data from FIFO
    unsigned int sensorData = ADC0_SSFIFO3;
    dls.driver.sampleTotal += sensorData;
    dls.driver.numSamples++;
}

void delayMS(int n)
{
    int i, j;

    for (i = 0; i < n; i++)
        for(j = 0; j < 3180; j++)
        {
        }

}


// void SystemInit(void)
// {
//     SCB_CPAC |= 0x00F00000;
// }
