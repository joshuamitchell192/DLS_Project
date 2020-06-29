#include <string.h>
#include "helpers.h"
#include "Queue.h"
#include "Instruction.h"

class DLS{

    private:
        const int StepsPerRev = 6600;
        char *inputString;
        Queue *queue;
        Instruction *instruction;
    public:
        DLS();
        void eventLoop();
        void readSerial(char inChar);
};