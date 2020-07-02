#include "DLS.h"
 
DLS::DLS () {
    //Setup::SensorADCSetup();
    Setup::Uart0TerminalSetup();

    //queue = new Queue();
}

void DLS::readSerial(char inChar){

    strcat(DLS::inputString, &inChar);

    if (inChar == '\n') {
        DLS::queue.enqueue(inputString);
        //char * lastInstruction = DLS::queue.peek();
        Helpers::WriteChar('\r');
        
//        for (int j = 0; j < queue.size(); j++){
//            char * lastInstruction = DLS::queue.peek();
//            DLS::queue.dequeue();
//            for (int i = 0; i < strlen(lastInstruction); i++){
//                Helpers::WriteChar(lastInstruction[i]);
//            }
//            Helpers::WriteChar('\n');
//        }
        
        //Helpers::WriteChar('\n');
        //Helpers::WriteChar('\r');
        
        strcpy(inputString, "");
        return;
    }
    if (inChar == 'p'){
        int queueSize = queue.size();
        for (int j = 0; j <= queue.size(); j++){
            char * lastInstruction = DLS::queue.get(j);
            //DLS::queue.dequeue();
            for (int i = 0; i < strlen(lastInstruction); i++){
                Helpers::WriteChar(lastInstruction[i]);
            }
            Helpers::WriteChar('\n');
        }
    }

}

void DLS::eventLoop(){

    for (;;) {
        //char *currentInstruction = queue->peek();

//        if (strcmp(currentInstruction, Instruction::G00)) {
//            // spinMotor();
//            Helpers::WriteChar(currentInstruction[0]);
//        }
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
