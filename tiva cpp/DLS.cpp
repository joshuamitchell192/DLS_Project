#include "DLS.h"
 
DLS::DLS () {
    queue = new Queue();
}

void DLS::readSerial(char inChar){

    strcat(DLS::inputString, &inChar);

    if (inChar == '\n') {
        queue->enqueue(inputString);
        strcpy(inputString, "");
        return;
    }
}

void DLS::eventLoop(){

    for (;;) {
        char *currentInstruction = queue->peek();
        if (currentInstruction == "G0") {
            // spinMotor();
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