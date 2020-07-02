#include "DLS.h"
 
LinearStage::LinearStage () {
    Setup::SensorADCSetup();
    Setup::Uart0TerminalSetup();

    queue = new Queue();
}

void LinearStage::readSerial(char inChar){

    strcat(LinearStage::inputString, &inChar);

    if (inChar == '\n') {
        queue->enqueue(inputString);
        strcpy(inputString, "");
        return;
    }
}

void LinearStage::eventLoop(){

    for (;;) {
        char *currentInstruction = queue->peek();

        if (strcmp(currentInstruction, Instruction::G00)) {
            // spinMotor();
            Helpers::WriteChar(currentInstruction[0]);
        }
    }
}

// void spinMotor() {
//     for (int x = 0; x < StepsPerRev; x++) {
//         digitalWrite(stepPin, HIGH);
        
//         delayMicroseconds(800);
//         digitalWrite(stepPin, LOW);
        
//         delayMicroseconds(800);

//     }
// }
