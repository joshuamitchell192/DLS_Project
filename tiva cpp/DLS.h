#include <string.h>
#include "Queue.h"
#include "Instruction.h"
#include "setup.h"


class DLS{

    private:
        static const int StepsPerRev = 6600;
        char inputString[10];
        Queue *queue;
    public:
        DLS();
        void eventLoop();
        void readSerial(char inChar);
};