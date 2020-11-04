#include "serial.h"
#include "tm4c123gh6pm.h"
#include <string.h>
#include <stdio.h>
#include "crc.h"

/**
 * 
 */
void Serial::WriteChar(unsigned int value)
{
    while ((UART0_FR_R & 0x20) == 0x20);
    UART0_DR_R = value;
}

/**
 * 
 */
void Serial::WriteCrc(unsigned char * value, int length)
{
    crc_t crc = crc_init();

    crc = crc_update(crc, value, length);
    crc = crc_finalize(crc);
    
    unsigned char crcBytes[2];
    shortToBytes(crcBytes, crc);

    for (int i = 0; i < 2; i++) {
        WriteChar(crcBytes[i]);
    }
}

/**
 * If the Uart Receive Flag register is set then the value from the
 * data register is read and returned after clearing the receive flag.
 */
unsigned int Serial::ReadChar()
{
    while ((UART0_FR_R & 0x10) == 0x10);
    unsigned int data = (unsigned int)UART0_DR_R;
    UART0_FR_R &= ~0x10;
    return data;
}

/**
 * Writes an array of characters to the serial connection until reaching the null terminating character
 */
void Serial::WriteString(const char *string){
    for (int i = 0; i < strlen(string); i++){
        WriteChar(string[i]);
    }
}

/**
 * Writes an array of raw bytes to the serial connection until the specified length is reached.
 */
void Serial::WriteBytes(unsigned char *bytes, int length) {
    for (int i = 0; i < length; i++) {
        WriteChar(bytes[i]);
    }
}

/**
 * Calculates the sample average from the given sample total and number of samples
 * 
 * @param sampleTotal : the total sum of the samples collected
 * @param numSamples : number of samples to average the sample total by
 */
int Serial::CalculateSampleAverage(int &sampleTotal, int &numSamples){
    return (double)sampleTotal / numSamples;
}

/**
 * Converts a 16 bit int into an array of chars of length 2
 */
unsigned char * Serial::shortToBytes(unsigned char bytes[2], short input){
    
    union{
        short a;
        unsigned char bytes[2];
    } shortStore;
    
    shortStore.a = input;
    memcpy(bytes, shortStore.bytes, sizeof(short));
}

/**
 * Converts a 32 bit float into an array of chars of length 4
 */
unsigned char * Serial::floatToBytes(unsigned char bytes[4], float input){

    union{
        float a;
        unsigned char bytes[4];
    } floatStore;

    floatStore.a = input;
    memcpy(bytes, floatStore.bytes, sizeof(float));
}

/** 
 * Sends a 16 bit int (short) over the serial connection after converting to an array of chars
 */
unsigned char * Serial::SendShort(int input){

    unsigned char bytes[2];
    Serial::shortToBytes(bytes, input);

    Serial::WriteChar(bytes[0]);
    Serial::WriteChar(bytes[1]);

    // Serial::WriteBytes(bytes, sizeof(short));
    Serial::WriteCrc(bytes, sizeof(short));

    return bytes;
}

/**
 * Sends a 32 bit float over the serial connection by first converting to an array of chars
 */
unsigned char * Serial::SendFloat(float input){

    unsigned char bytes[sizeof(float)];
    Serial::floatToBytes(bytes, input);

    for (int i = 0; i < 4; i++){
        WriteChar(bytes[i]);
    }
    // Serial::WriteBytes(bytes, sizeof(float));
    
    Serial::WriteCrc(bytes, sizeof(float));

    return bytes;
}

/**
 * Sends a 16 bit flag value over the serial connection
 */
void Serial::WriteFlag(int flag){
    WriteChar(0xFF);
    WriteChar(flag);
}