#include "Queue.h"
#include "motorDriver.h"

class DLS{

    private:
        static const int StepsPerRev = 6600;
        char inputString[10];
        Queue queue;
        void PrintState();

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
        void EventLoop();
        void ReadSerial(char inChar);
        MotorDriver driver;
};