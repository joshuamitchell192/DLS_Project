#include "motorDriver.h"
#include "registers.h"
#include "helpers.h"
#include "serial.h"

#define MIN_SAMPLE_DURATION 0.0014
#define STAGE_LENGTH_MM 65.0

MotorDriver::MotorDriver(){
    stepsPerMM = 0.0;
    currentPosition = 0;
    sampleDuration_ = 0.0017;
    stepsBetweenSamples = 48;
    stepAmount = 4;
    totalTimeElapsed = 0;
    averageInterval = 100;
}

void MotorDriver::SetStepMode(int stepMode){
    //M0 = A6, M1 = A7
    if (stepMode == Quarter){
        GPIOA_DATA &= ~0x40;
        GPIOA_DATA |= 0x80;

        stepAmount = 1;
    }
    else if(stepMode == Half){
        GPIOA_DATA |= 0x40;
        GPIOA_DATA &= ~0x80;
        stepAmount = 2;
    }
    else if(stepMode == Full){
        GPIOA_DATA &= ~0x40;
        GPIOA_DATA &= ~0x80;
        stepAmount = 4;
    }
}


void MotorDriver::SetSampleDuration(double sampleDuration){
    sampleDuration_ = sampleDuration;
}

void MotorDriver::SetStepsBetweenSamples(double stepLength){
    stepsBetweenSamples = stepLength * stepsPerMM * 4;
}

void MotorDriver::SetDriverTimer(volatile double seconds){
    int prescale = Helpers::getPrescaler(seconds);
    int preload = Helpers::getPreload(seconds, prescale);
    
    TIMER1_TAPR = prescale;
    TIMER1_TAILR = preload;

    TIMER1_ICR |= 1;
    TIMER1_CTL |= 1;
}


void MotorDriver::StepMotor(void){
        while((TIMER1_RIS & 0x1) != 0x1);
        TIMER1_ICR |= 1;
        GPIOA_DATA |= 0x4;
        while((TIMER1_RIS & 0x1) != 0x1);
        TIMER1_ICR |= 1;
        GPIOA_DATA &=~ 0x4;
}

/*
    Returns the number of steps per millimeter.
    The function will move the stage to the far left side
    until it hits the limit switch. Once it's hit the limit switch
    it will stop and return to the opposite end while counting
    the number of steps.
*/
void MotorDriver::Calibrate(bool &stop){

    SetStepMode(0);
    GPIOA_DATA |= 0x8;

    SetDriverTimer(MIN_SAMPLE_DURATION);

    while(!stop){
        StepMotor();
    
        if ((GPIOB_DATA & 0x2) == 0x2){
        break;
        }
    }

    GPIOA_DATA &= ~0x8;
    int numSteps = 1;
    while(!stop){
        StepMotor();
        numSteps++;
        if ((GPIOB_DATA & 0x1) ==0x1){
                break;
        }
    }
    stepsPerMM = numSteps / STAGE_LENGTH_MM;
    currentPosition = 0;
    
    Serial::SendFloat(stepsPerMM);
    
    // Send back the stepsPerMM to the GUI
}

void MotorDriver::StartSamplingHere(bool &stop){
    
    while (!stop){
        sampleTotal = 0;
        numSamples = 1;
        while(numSamples < averageInterval){
            Serial::SendSampleAverage(sampleTotal, numSamples);
        }
    }
    
}

//goto rapid positioning to point 1
//set sample duration, step size, step amount
//scan between to point 2
//  set driver timer to sample duration
//  step the motor (which will take sample duration time)
//  average and send samples
//  goto rapid positioning currentPos + stepSize

void MotorDriver::Move(bool &stop, double dest, bool setMaxSpeed) {
    
    int destination = (int)(dest * stepsPerMM * 4);
    
    if (setMaxSpeed){
        sampleDuration_ = MIN_SAMPLE_DURATION;
    }

    if (MotorDriver::IsAdcOn()){
        ScanBetween( stop, destination);
    }
    else {
        GoToPosition(stop, destination, sampleDuration_);
    }
}


void MotorDriver::GoToPosition(bool &stop, int dest, double sampleDuration){
    SetStepMode(2);
    SetDriverTimer(sampleDuration);

    int dir = SetDirection(dest);
    if (dir == 1){
        while (currentPosition <= dest && !stop){
            currentPosition += stepAmount * dir;
            StepMotor();
        }
    }
    else {
        while (currentPosition >= dest && !stop){
            currentPosition += stepAmount * dir;
            StepMotor();
        }
    }
    
}
 


 int MotorDriver::SetDirection(int dest) {
    int dir;
    if (currentPosition > (dest-4)){
        GPIOA_DATA &= ~0x8;
        dir = -1;
    }
    else{
        GPIOA_DATA |= 0x8;
        dir = 1;
    }

    return dir;
}


void MotorDriver::ScanBetween(bool &stop, int dest) {
    numSamples = 0;
    sampleTotal = 0;
    
    int dir = SetDirection(dest);
    int dirDestination = dest * dir;
    
    while (currentPosition <= dirDestination && !stop){
        SetDriverTimer(sampleDuration_);
        sampleTotal = 0;
        numSamples = 1;
        StepMotor();
        //Serial::SendTime(timeElapsed);
        Serial::SendSampleAverage(sampleTotal, numSamples);
        currentPosition += stepAmount;
        GoToPosition(stop, currentPosition + stepsBetweenSamples, MIN_SAMPLE_DURATION);
    }
}


bool MotorDriver::IsAdcOn(){
    return (TIMER0_CTL & 0x1);
}

void MotorDriver::TurnAdcOn(){
    TIMER0_CTL |= 0x1;
}

void MotorDriver::TurnAdcOff(){
    TIMER0_CTL &= ~1;
}
