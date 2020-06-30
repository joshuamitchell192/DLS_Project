#include "registers.h"


class Helpers{
    public:
        static void EnableInterrupts();
        static void DisableInterrupts();
        static void WriteChar(unsigned int);
        static unsigned int ReadChar();

        static unsigned int getPreload(double seconds, int prescaler);
        static unsigned int getPrescaler(double seconds);
};
