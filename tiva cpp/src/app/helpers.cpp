#include "helpers.h"
#include "stdlib.h"

void Helpers::EnableInterrupts(void)
{ 
     __asm("CPSIE i");
}


void Helpers::DisableInterrupts(void)
{ 
    __asm("CPSID i");
}

unsigned int Helpers::getPrescaler(double seconds){
    volatile double preload = 0xFFFFFFFF;
    volatile double prescaler = (seconds * 16000000)/(preload+1) -1;
    volatile double fractional = prescaler - ((long)prescaler);
    if (fractional > 0.0001){
            prescaler += 1;
    }
    return (unsigned int)prescaler;
}

unsigned int Helpers::getPreload(double seconds, int prescaler){
    volatile double preload = (seconds * 16000000)/(prescaler + 1) -1;
    volatile double fractional = preload - ((long)preload);
    if (fractional > 0.5){
            return (int)preload + 1;
    }

    return (unsigned int)preload;
    
}

// unsigned char * Helpers::appendBytes(unsigned char * byteArray, int endPosition, unsigned char * newBytes, int newBytesLength) {
//     for (int i = 0; i < newBytesLength; i++) {
//         byteArray[endPosition + i] = newBytes[i];
//     }

//     return byteArray;
// }

int Helpers::ToInt(char* string) {
    char *ptr;
    return strtol(string, &ptr, 10);
}

double Helpers::ToDouble(char* string) {
    char *ptr;
    return strtod(string, &ptr);
}

bool Helpers::IsDouble(char* string) {
    for (int i = 0; i < strlen(string); i++) {
        if (string[i] == '.') {
            return true;
        }
    }

    return false;
}