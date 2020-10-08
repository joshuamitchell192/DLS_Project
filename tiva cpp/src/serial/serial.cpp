#include "serial.h"
#include "tm4c123gh6pm.h"
#include <string.h>
#include <stdio.h>
#include "crc.h"

void Serial::WriteChar(unsigned int value)
{
    while ((UART0_FR_R & 0x20) == 0x20);
    UART0_DR_R = value;
}

void Serial::WriteCrc(unsigned char * value, int length)
{
    crc_t crc = crc_init();

    crc = crc_update(crc, value, length);
    crc = crc_finalize(crc);
    
    unsigned char crcBytes[2];
    intToBytes(crcBytes, crc);

    for (int i = 0; i < 2; i++) {
        WriteChar(crcBytes[i]);
    }
}

unsigned int Serial::ReadChar()
{
    while ((UART0_FR_R & 0x10) == 0x10);
    unsigned int data = (unsigned int)UART0_DR_R;
    UART0_FR_R &= ~0x10;
    return data;
}

void Serial::WriteString(const char *string){
    for (int i = 0; i < strlen(string); i++){
        WriteChar(string[i]);
    }
}

int Serial::CalculateSampleAverage(int &sampleTotal, int &numSamples){
    return (double)sampleTotal / numSamples;
}

unsigned char * Serial::SendInt(int input, char type){
    // unsigned int avgSample_lowerHalf = (0xFF & input); 
    // unsigned int avgSample_upperHalf = (0xF00 & input) >> 8;

    // Serial::WriteChar(avgSample_lowerHalf);
    // Serial::WriteChar(avgSample_upperHalf);

    unsigned char sampleBytes[2];
    intToBytes(sampleBytes, input);

    Serial::WriteChar(sampleBytes[0]);
    Serial::WriteChar(sampleBytes[1]);

    return sampleBytes;
}

unsigned char * Serial::intToBytes(unsigned char bytes[2], int input){
    
    union{
        int a;
        unsigned char bytes[2];
    } intStore;
    
    intStore.a = input;
    memcpy(bytes, intStore.bytes, 2);
}

unsigned char * Serial::floatToBytes(unsigned char bytes[4], float input){
    
    union{
        float a;
        unsigned char bytes[4];
    } floatStore;
    
    floatStore.a = input;
    memcpy(bytes, floatStore.bytes, 4);
}

unsigned char * Serial::SendFloat(float input, char type){

    unsigned char bytes[4];
    floatToBytes(bytes, input);
    
    for (int i = 0; i < 4; i++){
        WriteChar(bytes[i]);
    }

    return bytes;
}

void Serial::WriteFlag(int flag){
    WriteChar(0xFF);
    WriteChar(flag);
}