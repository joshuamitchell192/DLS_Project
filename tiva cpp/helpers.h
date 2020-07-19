#include "registers.h"
#include <string.h>

class Helpers{
    public:
        static void EnableInterrupts();
        static void DisableInterrupts();
        static void WriteChar(unsigned int value);
        static unsigned int ReadChar();
        static void WriteString(const char* string);
        static int ToInt(char* string);
        static double ToDouble(char* string);
        static bool IsDouble(char* string);
        static unsigned int getPreload(double seconds, int prescaler);
        static unsigned int getPrescaler(double seconds);
};
