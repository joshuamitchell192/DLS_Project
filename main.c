#include "registers.h"

extern void Delay(unsigned int);
extern void Setup(void);
void WriteChar(unsigned int);
	
int main(void)
{	  
	Setup();
	
	WriteChar(0x47);
	WriteChar(0x4F);
	
	
	//Delay(1000);
	while(1)
	{
		
	}
}

