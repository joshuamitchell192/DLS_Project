#include "DLS.h"
#include "serial.h"
#include "helpers.h"
 
DLS::DLS () {
    Setup::SensorADCSetup();
    Setup::Uart0TerminalSetup();
    Setup::DriverGPIOSetup();
    Setup::DriverTimerSetup();
    Setup::LimitSwitchesGPIOSetup();
    
    //averageInterval = 0;
    stop = false;
}

void DLS::readSerial(char inChar){

    if (inChar == 'p'){
        queue.printToSerial();
    }
    else {
        strcat(DLS::inputString, &inChar);
    }
    
    if (inChar == '\n') {
        if (strcmp(inputString, "state\n") == 0){
            PrintState();
        }
        else {
            DLS::queue.enqueue(inputString);
        }
        //char * lastInstruction = DLS::queue.peek();
        Serial::WriteChar('\r');
        
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
    
    

}

void DLS::eventLoop(){

    for (;;) {
        if (!queue.isEmpty()) {
            char *queuePeekedStr = queue.peek();
            char currentInstruction[10];

            strcpy(currentInstruction, queuePeekedStr);

            instruction parsedInstruction = Instruction::splitInstruction(currentInstruction, " \n");

            for (int i = 0; i < parsedInstruction.parameterCount; i++){
                Serial::WriteString(parsedInstruction.parameters[i]);
            }

            if (strcmp(parsedInstruction.instruction, Instruction::M04) == 0){
                driver.calibrate(stop);
            }

            if (strcmp(parsedInstruction.instruction, Instruction::T1) == 0)  {
                MotorDriver::TurnAdcOn();
            }

            if (strcmp(parsedInstruction.instruction, Instruction::T2) == 0)  {
                MotorDriver::TurnAdcOff();
            }

            if (strcmp(parsedInstruction.instruction, Instruction::S1) == 0){
                driver.sampleDuration = Helpers::ToDouble(parsedInstruction.parameters[0]);
            }

            if (strcmp(parsedInstruction.instruction, Instruction::G01) == 0){
                driver.move(stop, Helpers::ToInt(parsedInstruction.parameters[0]), false);
            }

            if (strcmp(parsedInstruction.instruction, Instruction::G00) == 0){
                driver.move(stop, Helpers::ToInt(parsedInstruction.parameters[0]), true);
            }

            Serial::WriteString(currentInstruction);
            queue.dequeue();
            memset(queuePeekedStr, 0, strlen(queuePeekedStr));

//        else if (strcmp(currentinstruction, Instruction::G00)) {
//            // spinmotor();
//            Helpers::WriteString(currentinstruction);
//        }
//        else if (strcmp(currentinstruction, Instruction::G01)){
//            Helpers::WriteString(currentinstruction);
//        }
//        else if (strcmp(currentinstruction, Instruction::M01)){
//            Helpers::WriteString(currentinstruction);
//        }
//        else if (strcmp(currentinstruction, Instruction::M02)){
//            Helpers::WriteString(currentinstruction);
//        }
//        else if (strcmp(currentinstruction, Instruction::M03)){
//            Helpers::WriteString(currentinstruction);
//        }
//        else if (strcmp(currentinstruction, Instruction::T1)){
//            Helpers::WriteString(currentinstruction);
//        }
//        else if (strcmp(currentinstruction, Instruction::T2)){
//            Helpers::WriteString(currentinstruction);
//        }
        }
    }
}

void DLS::PrintState() {
    Serial::WriteString("ADC : ");
    if (MotorDriver::IsAdcOn()){
        Serial::WriteString("On\n\r");
    }
    else {
        Serial::WriteString("Off\n\r");
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
