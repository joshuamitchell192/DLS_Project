#include "DLS.h"
#include "serial.h"
#include "helpers.h"
#include <string.h>
#include "Instruction.h"
#include "setup.h"
#include <stdio.h>
#include <stdlib.h>
 
DLS::DLS () {
    Setup::SensorADCSetup();
    Setup::Uart0TerminalSetup();
    Setup::DriverGPIOSetup();
    Setup::DriverTimerSetup();
    //Setup::TimeElapsedTimer();
    Setup::LimitSwitchesGPIOSetup();

    stop = false;
    wait = false;
}

void DLS::ReadSerial(char inChar){

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
        else if (strcmp(inputString, "M01\n") == 0) {
            wait = true;
        }
        else if (strcmp(inputString, "M03\n") == 0) {
            wait = false;
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

void DLS::EventLoop(){

    for (;;) {
        if (!queue.isEmpty() && !wait) {
            char *queuePeekedStr = queue.peek();
            char currentInstruction[10];

            strcpy(currentInstruction, queuePeekedStr);

            instruction parsedInstruction = Instruction::splitInstruction(currentInstruction, " \n");

            for (int i = 0; i < parsedInstruction.parameterCount; i++){
                Serial::WriteString(parsedInstruction.parameters[i]);
            }
            
            if (strcmp(parsedInstruction.instruction, Instruction::M04) == 0){
                driver.Calibrate(stop);
            }

            if (strcmp(parsedInstruction.instruction, Instruction::T1) == 0)  {
                MotorDriver::TurnAdcOn();
            }

            if (strcmp(parsedInstruction.instruction, Instruction::T2) == 0)  {
                MotorDriver::TurnAdcOff();
            }

            if (strcmp(parsedInstruction.instruction, Instruction::G01) == 0){
                driver.Move(stop, Helpers::ToDouble(parsedInstruction.parameters[0]), false);
            }

            if (strcmp(parsedInstruction.instruction, Instruction::G00) == 0){
                driver.Move(stop, Helpers::ToDouble(parsedInstruction.parameters[0]), true);
            }
            if (strcmp(parsedInstruction.instruction, Instruction::S1) == 0){
                driver.SetSampleDuration(Helpers::ToDouble(parsedInstruction.parameters[0]));
            }
            if (strcmp(parsedInstruction.instruction, Instruction::S2) == 0){
                driver.SetStepMode(Helpers::ToInt(parsedInstruction.parameters[0]));
            }

            if (strcmp(parsedInstruction.instruction, Instruction::S3) == 0) {
                driver.SetStepsBetweenSamples(Helpers::ToDouble(parsedInstruction.parameters[0]));
            }

            if (strcmp(parsedInstruction.instruction, Instruction::S4) == 0) {
                Serial::WriteString("Not Implemented");
            }

            Serial::WriteString(currentInstruction);
            queue.dequeue();
            memset(queuePeekedStr, 0, strlen(queuePeekedStr));
        }
    }
}

void DLS::PrintState() {
    Serial::WriteString("\n\rADC : ");
    if (MotorDriver::IsAdcOn()){
        Serial::WriteString("On");
    }
    else {
        Serial::WriteString("Off");
    }
    
    char stateString[20];

    Serial::WriteString("\n\rSample Duration: ");
    snprintf(stateString, 0, "%f", driver.sampleDuration_);
    Serial::WriteString(stateString);
    
    Serial::WriteString("\n\rStepMode: ");

    char *stepMode;
    switch (driver.stepAmount){
        case 1:
            stepMode = "Quarter";
            break;
        case 2:
            stepMode = "Half";
            break;
        case 4:
            stepMode = "Full";
            break;
    }
    Serial::WriteString(stepMode);
    
    Serial::WriteString("\n\rStepsBetweenSamples: ");
    snprintf(stateString, 10, "%d", driver.stepsBetweenSamples);
    Serial::WriteString(stateString);

}
