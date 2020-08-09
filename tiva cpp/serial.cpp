#include "serial.h"
#include "registers.h"
#include <string.h>

void Serial::WriteChar(unsigned int value)
{
    while ((UART0_FR & 0x20) == 0x20);
    UART0_DR = value;
}

unsigned int Serial::ReadChar()
{
    while ((UART0_FR & 0x10) == 0x10);
    unsigned int data = (unsigned int)UART0_DR;
    UART0_FR &= ~0x10;
    return data;
}

void Serial::WriteString(const char *string){
    for (int i = 0; i < strlen(string); i++){
        WriteChar(string[i]);
    }
}

void Serial::SendSampleAverage(int &sampleTotal, int &numSamples){
    int avg_sample = (double)sampleTotal / numSamples;
    Serial::SendInt(avg_sample);
}

void Serial::SendInt(int input){
    unsigned int avgSample_lowerHalf = (0xFF & input); 
    unsigned int avgSample_upperHalf = (0xF00 & input) >> 8;

    Serial::WriteChar(avgSample_lowerHalf);
    Serial::WriteChar(avgSample_upperHalf);
}
void Serial::SendTime(float time) {
    //SEND TIME
//    volatile float totalSecondsF = (float)timeElapsed;
//    volatile float leftOverTimeF = (float)(NVIC_ST_RELOAD - NVIC_ST_CURRENT) / 16000000.0;
//    volatile unsigned int totalTime = ((float)(totalSecondsF) + leftOverTimeF) * 1000;
    //d SendFloat(totalTime);
}

void Serial::SendFloat(float input){
    unsigned int inputLower = (0xFF && input);
    unsigned int inputMid = (0xFF00 && input) >> 8;
    unsigned int inputUpper = (0xFF0000 && input) >> 16;
    WriteChar(inputLower);
    WriteChar(inputMid);
    WriteChar(inputUpper);
}