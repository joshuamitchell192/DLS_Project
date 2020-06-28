


/**
 * main.c
 */
#define PORTFDAT (*(volatile unsigned int *)0x400253FC)
#define PORTFDIR (*(volatile unsigned int *)0x40025400)
#define PORTFDEN (*(volatile unsigned int *)0x4002551C)
#define RCGCGPIO (*(volatile unsigned int *)0x400FE608)
#define SCB_CPAC (*(volatile unsigned int *)0xE000ED8)

void delayMS(int n); /*function prototype for delay*/


int main(void)
{
    RCGCGPIO |= 0x20;

    PORTFDIR  = 0x0E;

    PORTFDEN = 0x0E;

    while(1) {
        PORTFDAT = 0x02;

        delayMS(500);

        PORTFDAT = 0x04;

        delayMS(500);

        PORTFDAT = 0x08;

        delayMS(500);

        PORTFDAT = 0x0E;

        delayMS(500);

    }

    return 0;
}
void delayMS(int n)
{
    int i, j;

    for (i = 0; i < n; i++)
        for(j = 0; j < 3180; j++)
        {
        }

}

//#include "registers.h"

//extern void Delay(unsigned int);
//extern void Setup(void);
//extern void WriteChar(unsigned int);
//extern unsigned int ReadChar();
//extern int calibrate(void);
//extern void sampleTwoPoints(double sampleDuration, int point1, int point2);
//extern void goTo(int point);
//extern void sampleHere(int duration,int avgInterval);


//void wait(void);

//unsigned int instruction;
//unsigned int point1;
//unsigned int point2;
//float sampleDuration;
//float stepSize;
//unsigned int avgInt;
//unsigned int stepAmount;
//unsigned int timeElapsed;

//unsigned int ALREADY_READ=0;

//int STOP;
//float stepsPerMM;

//extern int currentPosition;
//	
//	
//int main(void)
//{	  
//	STOP = 0;
//	Setup();
//	wait();
//}

//void wait (void)
//{
//	
//	/*
//		Main program loop, waiting for instructions to be sent from the GUI
//	*/
//    while (1)
//    {
//        if (instruction == '*')
//        {
//            STOP = 0;
//            WriteChar(instruction);
//            calibrate();
//						unsigned int sendBackStepSize = stepsPerMM * 100;
//						unsigned int sendBackStepSizeLower = sendBackStepSize & 0xFF;
//						unsigned int sendBackStepSizeUpper = (sendBackStepSize & 0xFF00)>>8;
//						WriteChar(sendBackStepSizeLower);
//						WriteChar(sendBackStepSizeUpper);
//            instruction = 0;
//        }
//        if (instruction == ':')//sample 2 points : opcode
//        {
//            ALREADY_READ = 0;
//            STOP = 0;
//            sampleTwoPoints((double)sampleDuration, point1*stepsPerMM, point2*stepsPerMM);
//            //sampleTwoPoints((double)0.001,100, 300);
//            instruction = 0;

//        }
//        if (instruction == '.') 
//        {
//            ALREADY_READ = 0;
//						STOP = 0;
//            sampleHere(sampleDuration, avgInt);

//            instruction = 0;
//        }
//        if (instruction == '>')
//        {
//            ALREADY_READ = 0;
//            goTo(point1*stepsPerMM);
//            instruction=0;

//        }
//        STOP = 0;

//    }
//}