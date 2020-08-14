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
        else if (strcmp(inputString, "M00\n") == 0) {
            wait = true;
        }
        else if (strcmp(inputString, "M01\n") == 0) {
            stop = true;
        }
        else if (strcmp(inputString, "M02\n") == 0) {
            stop = false;
        }
        else if (strcmp(inputString, "M03\n") == 0) {
            wait = false;
        }
        else if (strcmp(inputString, "M05\n") == 0) {
            queue.clear();
        }
        else {
            DLS::queue.enqueue(inputString);
        }

        Serial::WriteChar('\r');
        
        strcpy(inputString, "");
        return;
    }
    
    

}

void DLS::EventLoop(){

    for (;;) {
        if (!queue.isEmpty() && !wait && !stop) {
            char *queuePeekedStr = queue.peek();
            char currentInstruction[10];

            strcpy(currentInstruction, queuePeekedStr);

            instruction parsedInstruction = Instruction::splitInstruction(currentInstruction, " \n");
            
            if (strcmp(parsedInstruction.instruction, Instruction::M04) == 0){
                driver.Calibrate(stop);
            }

            if (strcmp(parsedInstruction.instruction, Instruction::T1) == 0)  {
                MotorDriver::TurnAdcOn();
            }

            if (strcmp(parsedInstruction.instruction, Instruction::T2) == 0)  {
                MotorDriver::TurnAdcOff();
            }
            
            if (strcmp(parsedInstruction.instruction, Instruction::T3) == 0)  {
                driver.StartSamplingHere(stop);
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
                if (strcmp(parsedInstruction.parameters[0], "full") == 0){
                    driver.SetStepMode(Full);
                }
                else if (strcmp(parsedInstruction.parameters[0], "half") == 0){
                    driver.SetStepMode(Half);
                }
                else if (strcmp(parsedInstruction.parameters[0], "quarter") == 0){
                    driver.SetStepMode(Quarter);
                }
                else {
                    driver.SetStepMode(Helpers::ToInt(parsedInstruction.parameters[0]));
                }

            }

            if (strcmp(parsedInstruction.instruction, Instruction::S3) == 0) {
                driver.SetStepsBetweenSamples(Helpers::ToDouble(parsedInstruction.parameters[0]));
            }

            if (strcmp(parsedInstruction.instruction, Instruction::S4) == 0) {
                Serial::WriteString("Not Implemented");
            }
            
            if (!stop){
                Serial::WriteString(currentInstruction);
                queue.dequeue();
            }
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
    snprintf(stateString, 0, "%d", driver.stepsBetweenSamples);
    Serial::WriteString(stateString);
    
    Serial::WriteString("\n\rCurrent Position: ");
    
    snprintf(stateString, 0, "%d", driver.currentPosition);
    Serial::WriteString(stateString);

}
