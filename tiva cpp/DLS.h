#include <string.h>
#include "Queue.h"
#include "Instruction.h"
#include "setup.h"
#include "motorDriver.h"


class DLS{

    private:
        static const int StepsPerRev = 6600;
        char inputString[10];
        Queue queue;
        static void PrintState();

        //double sampleDuration;
        //double stepLength;
        //int currentPosition;
        //int averageInterval;
        bool stop;
        //float stepsPerMM;
        //int sampleTotal;
        //int numSamples;

    public:
        DLS();
        void eventLoop();
        void readSerial(char inChar);
        MotorDriver driver;
    
        
};