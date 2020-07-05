#include <string.h>
#include "Queue.h"
#include "Instruction.h"
#include "setup.h"

enum StepModes {
    Full = 2,
    Half = 1,
    Quarter = 0
};

class DLS{

    private:
        static const int StepsPerRev = 6600;
        char inputString[10];
        Queue queue;
        
        StepModes stepMode;
        double sampleDuration;
        double stepLength;
        int currentPosition;
        int averageInterval;

    public:
        DLS();
        void eventLoop();
        void readSerial(char inChar);
    
        
};