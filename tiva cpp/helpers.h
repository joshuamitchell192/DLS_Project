#include "registers.h"

inline void EnableInterrupts(void)
{ 
	 __asm("CPSIE i");
}


inline void DisableInterrupts(void)
{ 
	__asm("CPSID i");
}


class Helpers{
    public:
        
        static void WriteChar(unsigned int);
        static unsigned int ReadChar();

        static unsigned int getPreload(double seconds, int prescaler);
        static unsigned int getPrescaler(double seconds);
};
