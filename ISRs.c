#include "registers.h"

void ADC0SS3_Handler (void);
extern void WriteChar(unsigned int);
extern unsigned int ReadChar();
extern void Delay(unsigned int);
extern int calibrate(void);
extern void wait(void);
extern void sampleTwoPoints(double sampleDuration, int point1, int point2);

int numSteps;
float stepsPerMM;
int currentPosition;

void ADC0SS3_Handler (void)
{
	ADC0_ISC |= 0x8;
	
	// Get Data from FIFO
	unsigned int sensorData = ADC0_SSFIFO3;
	unsigned int delayValue = sensorData & 0xF;

	// Write variable to terminal
	
	// Wait for transmission queue to be empty
	while ((UART0_FR & 0x20) == 0x20);

	WriteChar(0x21);
	Delay(delayValue);
	
}

void UART0_Handler (void)
{
	unsigned int input = ReadChar();
	
	if (input == (unsigned int)0x2A)
	{
		WriteChar(input);
		numSteps = calibrate();
		stepsPerMM = numSteps/55;

	}
	if (input == (unsigned int)0x21)
	{
		WriteChar(input);
		TIMER1_ICR &= ~1;
		TIMER1_CTL &= ~0x1;
		wait();
	}

	if (input == (unsigned int)0x3A)//sample 2 points : opcode
	{
		WriteChar(input);
			volatile unsigned int point1 = ReadChar();
		WriteChar(point1);
			volatile unsigned int point2 = ReadChar();
		WriteChar(point2);
			volatile unsigned int sampleDuration = ReadChar();
		WriteChar(sampleDuration);
			unsigned int stepSize = ReadChar();
		WriteChar(stepSize);
		
		sampleTwoPoints((double)sampleDuration * 0.001, point1*stepsPerMM, point2*stepsPerMM);
	}

	if (input == (unsigned int)0x2E) 
	{
		WriteChar(input);
//		ReadChar();
//		TIMER0_CTL &= ~0x21;
//		TIMER1_ICR &= ~1;

	}
	
}

