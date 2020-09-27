#include "serial.h"
#include "registers.h"
#include <string.h>
#include <stdio.h>
#include "crc.h"

const char Serial::sample_type = 's';
const char Serial::time_type = 't';
const char Serial::position_type = 'p';

void Serial::WriteChar(unsigned int value)
{
    while ((UART0_FR & 0x20) == 0x20);
    UART0_DR = value;
}

void Serial::WriteCrc(unsigned char * value)
{
    crc_t crc = crc_init();
    int valueLength = strlen((char *) value);
    crc = crc_update(crc, value, 4);
    crc = crc_finalize(crc);
    
    unsigned char crcBytes[2];
    intToBytes(crcBytes, crc);

    for (int i = 0; i < 2; i++) {
        WriteChar(crcBytes[i]);
    }
}
    


// void Serial::WriteChar(unsigned int value, char type, bool sendCrc){

//     WriteChar(type);
//     WriteChar(value);
    
//     if (sendCrc){
//         crc_t crc = crc_init();
//         crc = crc_update(crc, (unsigned char *)value , strlen((char *)value));
//         crc = crc_finalize(crc);
        
//         while ((UART0_FR & 0x20) == 0x20);
//         UART0_DR = crc;
//     }
// }

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

void Serial::SendFloat(float input){
    unsigned char bytes[4];
    floatToBytes(bytes, input);
    
    for (int i = 0; i < 4; i++){
        WriteChar(bytes[i]);
    }

    WriteCrc(bytes);
}

void Serial::WriteFlag(int flag){
    WriteChar(0xFF);
    WriteChar(flag);
}