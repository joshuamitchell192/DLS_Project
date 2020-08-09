#include "motorDriver.h"
#include "registers.h"
#include "helpers.h"
#include "serial.h"

#define MIN_SAMPLE_DURATION 0.0014

//int sampleTotal;
//int numSamples;

static const float mm = 60.0;

MotorDriver::MotorDriver(){
    stepMode = StepModes::Full;
    stepsPerMM = 0.0;
    currentPosition = 0;
    sampleDuration = 0.1;
    stepsBetweenSamples = 1;
    stepAmount = 4;
}

// int numMultiples;
// double remainder;

// Sets StepMode  on GPIOA
void MotorDriver::setStepMode(int stepMode){
    //M0 = A6, M1 = A7
    if (stepMode == 0){
        GPIOA_DATA &= ~0x40;
        GPIOA_DATA |= 0x80;
        stepAmount = 1;
    }else if(stepMode == 1){
        GPIOA_DATA |= 0x40;
        GPIOA_DATA &= ~0x80;
        stepAmount = 2;
    }else if(stepMode == 2){
        GPIOA_DATA &= ~0x40;
        GPIOA_DATA &= ~0x80;
        stepAmount = 4;
    }
}

void MotorDriver::setStepsBetweenSamples(double stepLength){
    stepSize = stepLength * stepsPerMM;
}

void MotorDriver::setDriverTimer(volatile double seconds){
    int prescale = Helpers::getPrescaler(seconds);
    int preload = Helpers::getPreload(seconds, prescale);
    
    TIMER1_TAPR = prescale;
    TIMER1_TAILR = preload;

    TIMER1_ICR |= 1;
    TIMER1_CTL |= 1;
}


void MotorDriver::stepMotor(void){
        
        
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
void MotorDriver::calibrate(bool &stop){

    setStepsBetweenSamples(2);
    GPIOA_DATA |= 0x8;

    setDriverTimer(0.0014);

    while(!stop){
        stepMotor();
    
        if ((GPIOB_DATA & 0x2) == 0x2){
        break;
        }
    }

    GPIOA_DATA &= ~0x8;
    int numSteps = 1;
    while(!stop){
        stepMotor();
        numSteps++;
        if ((GPIOB_DATA & 0x1) ==0x1){
                break;
        }
    }
    stepsPerMM = numSteps/mm;
}

//goto rapid positioning to point 1
//set sample duration, step size, step amount
//scan between to point 2
//  set driver timer to sample duration
//  step the motor (which will take sample duration time)
//  average and send samples
//  goto rapid positioning currentPos + stepSize

void MotorDriver::move(bool &stop, int dest, bool setMaxSpeed) {
    
    dest = dest * stepsPerMM;
    
    if (setMaxSpeed){
        sampleDuration = MIN_SAMPLE_DURATION;
    }

    if (MotorDriver::IsAdcOn()){
        ScanBetween( stop, dest);
    }
    else {
        goTo(stop, dest);
    }
}

void MotorDriver::goTo(bool &stop, int dest){
    setStepsBetweenSamples(2);
    setDriverTimer(sampleDuration);
    
    int dir;
    if (currentPosition > (dest-1)*4){
        GPIOA_DATA &= ~0x8;
        dir = -stepAmount;
        while (currentPosition >= (dest)*dir && !stop){
            currentPosition += dir;
            stepMotor();
        }
    }else{
        GPIOA_DATA |= 0x8;
        dir = stepAmount;
        while (currentPosition <= (dest)*dir && !stop){
            currentPosition += dir;
            stepMotor();
        }
    }
    
    //GPIOA_DATA |= 0x8;
    //stepMotor();
    currentPosition += dir;
    
}

void MotorDriver::stepSizeMove(bool &stop, int dest, double sampleDuration){
    setStepsBetweenSamples(2);
    setDriverTimer(sampleDuration);
    
    int dir;
    if (currentPosition > (dest-1)*4){
        GPIOA_DATA &= ~0x8;
        dir = -stepAmount;
    }else{
        GPIOA_DATA |= 0x8;
        dir = stepAmount;
    }
    while (currentPosition <= (dest)*dir && !stop){
        currentPosition += dir;
        stepMotor();
    }
    
}
 
 void sendSamples(int &sampleTotal, int &numSamples){
    int avg_sample = (double)sampleTotal / numSamples;
    MotorDriver::sendInt(avg_sample);
 }

 int MotorDriver::SetDirection(int dest) {
    int dir;
    if (currentPosition > (dest-1)*4){
        GPIOA_DATA &= ~0x8;
        dir = -1;
    }
    else{
        GPIOA_DATA |= 0x8;
        dir = 1;
    }

    return dir;
}

//void MotorDriver::scanBetween(bool &stop, int dest){
//    numSamples = 0;
//    sampleTotal = 0;
//    double tempSampleDuration = sampleDuration;

//    int dir = SetDirection(dest);
//    
//    int dirDirection = dest * dir;

//    while (currentPosition <= dirDirection && !stop){
//        setDriverTimer(sampleDuration);
//        sampleTotal = 0;
//        numSamples = 1;
//        stepMotor();
//        currentPosition += stepAmount;
//        sendSamples(sampleTotal, numSamples);
//        sampleDuration = MIN_SAMPLE_DURATION;
//        stepSizeMove(stop, currentPosition + stepSize);
//        sampleDuration = tempSampleDuration;
//    }
//}

void MotorDriver::ScanBetween(bool &stop, int dest) {
    numSamples = 0;
    sampleTotal = 0;
    
    int dir = SetDirection(dest);
    int dirDirection = dest * dir;
    
    while (currentPosition <= dirDirection && !stop){
        setDriverTimer(sampleDuration);
        sampleTotal = 0;
        numSamples = 1;
        stepMotor();
        currentPosition += stepAmount;
        sendSamples(sampleTotal, numSamples);
        stepSizeMove(stop, currentPosition + stepsBetweenSamples, MIN_SAMPLE_DURATION);
    }
}

void MotorDriver::sendInt(int input){
    unsigned int avgSample_lowerHalf = (0xFF & input); 
    unsigned int avgSample_upperHalf = (0xF00 & input) >> 8;

    Serial::WriteChar(avgSample_lowerHalf);
    Serial::WriteChar(avgSample_upperHalf);
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
