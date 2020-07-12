#include "DLS.h"
 
DLS::DLS () {
    Setup::SensorADCSetup();
    Setup::Uart0TerminalSetup();
    
    stepMode = StepModes::Full;
    currentPosition = 0;
    sampleDuration = 0.0;
    averageInterval = 0;
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
        queue.printToSerial();
    }

}

void DLS::eventLoop(){

    for (;;) {
        char *currentinstruction = queue.peek();
        
        const char* i;
        int params[10];
        
        instruction parsedInstruction = Instruction::splitInstruction(currentinstruction, " ");
        
        // Parse instruction to instruction and parameter parts.
        if (currentinstruction == NULL){}

        else if (strcmp(currentinstruction, Instruction::G00)) {
            // spinmotor();
            Helpers::WriteString(currentinstruction);
        }
        else if (strcmp(currentinstruction, Instruction::G01)){
            Helpers::WriteString(currentinstruction);
        }
        else if (strcmp(currentinstruction, Instruction::M01)){
            Helpers::WriteString(currentinstruction);
        }
        else if (strcmp(currentinstruction, Instruction::M02)){
            Helpers::WriteString(currentinstruction);
        }
        else if (strcmp(currentinstruction, Instruction::M03)){
            Helpers::WriteString(currentinstruction);
        }
        else if (strcmp(currentinstruction, Instruction::T1)){
            Helpers::WriteString(currentinstruction);
        }
        else if (strcmp(currentinstruction, Instruction::T2)){
            Helpers::WriteString(currentinstruction);
        }
        Helpers::WriteString(currentinstruction);
        queue.dequeue();
        memset(currentinstruction, 0, strlen(currentinstruction));
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
