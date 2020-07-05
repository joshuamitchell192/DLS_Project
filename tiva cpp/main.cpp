#include "registers.h"
//#include "setup.h"
#include "DLS.h"
//#include "main.h"

//void delayMS(int n); /*function prototype for delay*/
//extern "C" void UART0_Handler (void);
//using namespace std;
extern "C" void UART0_Handler (void);
extern "C" void ADC0SS3_Handler (void);

DLS dls;

int main(void)
{
    Helpers::WriteChar('G');
    Helpers::WriteChar('O');

    //Turn on adc timer for sampling
	//TIMER0_CTL |= 0x21;
    dls.eventLoop();
    while(1);
}

void UART0_Handler(void) {
    
    char c = Helpers::ReadChar();
    dls.readSerial(c);
    Helpers::WriteChar(c);
}

void ADC0SS3_Handler (void)
{
	ADC0_ISC |= 0x8;

	// Get Data from FIFO
	unsigned int sensorData = ADC0_SSFIFO3;
    
    Helpers::WriteChar(sensorData);
    Helpers::WriteChar(' ');
//	sampleTotal += sensorData;
//	numSamples++;
	
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
