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

unsigned char * floatToBytes(unsigned char bytes[4], float input){
    
    union{
        float a;
        unsigned char bytes[4];
    } floatStore;
    
    floatStore.a = input;
    memcpy(bytes, floatStore.bytes, 4);
}

void Serial::SendFloat(float input){
    unsigned char bytes[4];
    floatToBytes(bytes, input);
    
    for (int i = 0; i < 4; i++){
        WriteChar(bytes[i]);
    }
}

void Serial::WriteFlag(int flag){
    WriteChar(0xFF);
    WriteChar(flag);
}