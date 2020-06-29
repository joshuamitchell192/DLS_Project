#include "registers.h"

void ADC0SS3_Handler (void);
extern void WriteChar(unsigned int);
extern unsigned int ReadChar();
extern void Delay(unsigned int);
extern int calibrate(void);
extern void wait(void);
extern void sampleTwoPoints(double sampleDuration, int point1, int point2);

extern unsigned int instruction;
extern unsigned int instruction;
extern unsigned int point1;
extern unsigned int point2;
extern float sampleDuration;
extern float stepSize;
extern unsigned int ALREADY_READ;

extern int sampleTotal;
extern int numSamples;
extern unsigned int stepAmount;

int numSteps;
extern int STOP;
extern unsigned int avgInt;

int currentPosition;
int prio = 0;

void ADC0SS3_Handler (void)
{
	ADC0_ISC |= 0x8;
	
	// Get Data from FIFO
	unsigned int sensorData = ADC0_SSFIFO3;
	sampleTotal += sensorData;
	numSamples++;
	
}

void UART0_Handler (void)
{
    if (ALREADY_READ) return;

    instruction = ReadChar();

    if (instruction == (unsigned int)0x3A)//sample 2 points : opcode
    {
        STOP = 0;
        WriteChar(instruction);
        point1 = ReadChar();
        WriteChar(point1);
        point2 = ReadChar();
        WriteChar(point2);
        //sampleDuration = ReadChar();
				unsigned int sampleDurationLower = ReadChar();
        WriteChar(sampleDurationLower);
				unsigned int sampleDurationMid = ReadChar();
        WriteChar(sampleDurationMid);
				unsigned int sampleDurationUpper = ReadChar();
				WriteChar(sampleDurationUpper);
				unsigned int stepSizeLower = ReadChar();
        WriteChar(stepSizeLower);
				unsigned int stepSizeUpper = ReadChar();
        WriteChar(stepSizeUpper);
		
				stepAmount = ReadChar();
				WriteChar(stepAmount);
				
				sampleDuration = (sampleDurationUpper << 16) + (sampleDurationMid << 8) + sampleDurationLower;
				sampleDuration /= 1000;

				stepSize = (stepSizeUpper << 8) + stepSizeLower;
				stepSize /= 1000;

				ALREADY_READ = 1;
    }
    if (instruction == '.'){
				STOP = 0;
				WriteChar(instruction);
				//sampleDuration = ReadChar();
				//WriteChar(sampleDuration);
				//avgInt = ReadChar();
				//WriteChar(avgInt);
				ALREADY_READ = 1;
    }
    if (instruction == '>'){
				STOP = 0;
				WriteChar(instruction);
				point1 = ReadChar();
				WriteChar(point1);
				ALREADY_READ = 1;
    }

    if (instruction == (unsigned int)0x21)
    {
        STOP = 1;
        WriteChar(instruction);
        instruction = 0;
        //TIMER1_ICR &= ~1;
        //TIMER1_CTL &= ~0x1;
        //wait();
        ALREADY_READ = 0;
    }
}
