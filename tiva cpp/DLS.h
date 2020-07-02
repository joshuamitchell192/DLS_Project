#include <string.h>
#include "Queue.h"
#include "Instruction.h"
#include "setup.h"


class LinearStage{

    private:
        static const int StepsPerRev = 6600;
        char inputString[10];
        Queue *queue;
    public:
        LinearStage();
        void eventLoop();
        void readSerial(char inChar);
};