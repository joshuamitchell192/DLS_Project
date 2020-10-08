#include "Queue.h"
#include "motorDriver.h"

class DLS{

    private:
        static const int StepsPerRev = 6600;
        char inputString[10];
        Queue queue;
        void PrintState();
        void LoadProgram();
        
        bool stop;
        bool wait;
        Queue program;

    public:
        DLS();
        void EventLoop();
        void ReadSerial(char inChar);
        MotorDriver driver;
};