#include "motorDriver.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "helpers.h"
#include "serial.h"

#define MIN_SAMPLE_DURATION 0.001
#define STAGE_LENGTH_MM 65.0
#define TIVA_CLOCK_SPEED 16000000.0
#define SCANBETWEEN_PACKAGE_LENGTH 10

MotorDriver::MotorDriver(){
    stepsPerMM = 0.0;
    currentPosition = 0;
    sampleDuration_ = 0.0017;
    stepsBetweenSamples = 48;
    stepAmount = 4;
    totalTimeElapsed = 0;
    averageInterval = 20;
}

void MotorDriver::SetStepMode(int stepMode){
    //M0 = A6, M1 = A7
    if (stepMode == Quarter){
        GPIO_PORTA_DATA_R &= ~0x40;
        GPIO_PORTA_DATA_R |= 0x80;

        stepAmount = 1;
    }
    else if(stepMode == Half){
        GPIO_PORTA_DATA_R |= 0x40;
        GPIO_PORTA_DATA_R &= ~0x80;
        stepAmount = 2;
    }
    else if(stepMode == Full){
        GPIO_PORTA_DATA_R  &= ~0x40;
        GPIO_PORTA_DATA_R  &= ~0x80;
        stepAmount = 4;
    }
}

void MotorDriver::SetAverageInterval(int averageInterval_) {
    averageInterval = averageInterval_;
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
    
    TIMER1_TAPR_R = prescale;
    TIMER1_TAILR_R = preload;

    TIMER1_ICR_R |= 1;
    TIMER1_CTL_R |= 1;
}


void MotorDriver::StepMotor(void){
        while((TIMER1_RIS_R & 0x1) != 0x1);
        TIMER1_ICR_R |= 1;
        GPIO_PORTA_DATA_R |= 0x4;
        while((TIMER1_RIS_R & 0x1) != 0x1);
        TIMER1_ICR_R |= 1;
        GPIO_PORTA_DATA_R  &=~ 0x4;
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
    GPIO_PORTA_DATA_R  |= 0x8;

    SetDriverTimer(MIN_SAMPLE_DURATION);

    while(!stop && !IsSwitchB2On()){
        StepMotor();
    }

    GPIO_PORTA_DATA_R &= ~0x8;
    int numSteps = 1;
    while(!stop){
        StepMotor();
        numSteps++;
        if (IsSwitchB1On()){
            stepsPerMM = numSteps / STAGE_LENGTH_MM;
            currentPosition = 0;
            Serial::WriteFlag(0xFD);
            //Serial::SendShort(stepsPerMM, Serial::Calibration);
            break;
        }
    }
   
}

void MotorDriver::StartSamplingHere(bool &stop){
    
    while (!stop){
        sampleTotal = 0;
        numSamples = 1;
        
        Serial::WriteFlag(0xFF);
        while(numSamples < averageInterval){
            Serial::CalculateSampleAverage(sampleTotal, numSamples);
            float currentTime = CalculateCurrentTime();
            //Serial::SendFloat(currentTime, Serial::Time);
        }
        
        Serial::WriteFlag(0xFE);
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
    if (dir == 1) {
        while (currentPosition < dest && !stop && !IsSwitchB2On()){
            currentPosition += stepAmount * dir;
            StepMotor();
        }
    }
    else if (dir == -1) {
        while (currentPosition > dest && !stop && !IsSwitchB1On()){
            currentPosition += stepAmount * dir;
            StepMotor();
        }
    }
    
}
 

/**
 * Determines what direction to step the motor base on the current position in
 * relation to the destination position.
 * 
 * @param dest : The desired position to move to
 * @return integer describing if the destination is greater, less or equal to the current position
*/
 int MotorDriver::SetDirection(int dest) {
    int dir;
    if (currentPosition > dest) {
        GPIO_PORTA_DATA_R  &= ~0x8;
        dir = -1;
    }
    else if (currentPosition < dest ) {
        GPIO_PORTA_DATA_R  |= 0x8;
        dir = 1;
    }
    else {
        dir = 0;
    }

    return dir;
}

/**
 * Moves to destination position while taking samples
 * 
 * This will run if given a G01 instruction after a T1 instruction (ADC is on).
 * 
 * @param stop : Flag to stop and return to event loop if true
 * @param dest : Destination position
*/
void MotorDriver::ScanBetween(bool &stop, int dest) {
    numSamples = 0;
    sampleTotal = 0;
    totalTimeElapsed = 0;
    
    int direction = SetDirection(dest);

    if (direction == 1) {
        while (currentPosition < dest && !stop && !IsSwitchB2On()){
            Serial::WriteFlag(0xFF);
            WaitForSamples();
            currentPosition += stepAmount;
            GoToPosition(stop, currentPosition + stepsBetweenSamples, MIN_SAMPLE_DURATION);
        }
    }
    else if (direction == -1) {
        while (currentPosition > dest && !stop && !IsSwitchB1On()){
            Serial::WriteFlag(0xFF);
            WaitForSamples();
            currentPosition -= stepAmount;
            GoToPosition(stop, currentPosition + (stepsBetweenSamples * direction), MIN_SAMPLE_DURATION);
        }
    }

    Serial::WriteFlag(0xFE);
}

/**
 * 
 */
void MotorDriver::WaitForSamples() {

    sampleTotal = 0;
    numSamples = 1;

    SetDriverTimer(sampleDuration_);
    StepMotor();

    int sampleAvg = Serial::CalculateSampleAverage(sampleTotal, numSamples);
    //unsigned char sampleBytes[2];
    unsigned char * sampleBytes = Serial::SendShort(sampleAvg, Serial::Sample);
    sampleBytes = (unsigned char*)"123";

    volatile float currentTime = CalculateCurrentTime();
    //currentTime = 4.0;

    //unsigned char timeBytes[sizeof(float)];
    // Serial::floatToBytes(timeBytes, currentTime);

    unsigned char * timeBytes = Serial::SendFloat(currentTime, Serial::Time);

    float currentPosMM = (float)(currentPosition / stepsPerMM / 4);
    //unsigned char positionBytes[sizeof(float)];
    // Serial::floatToBytes(positionBytes, currentPosition);

    unsigned char * positionBytes = Serial::SendFloat(currentPosMM, Serial::Position);
    unsigned char crcData[30];
    
//    Helpers::appendBytes(crcData, 0, sampleBytes, sizeof(short));
//    Helpers::appendBytes(crcData, sizeof(short), timeBytes, sizeof(float));
//    Helpers::appendBytes(crcData, sizeof(short) + sizeof(float), positionBytes, sizeof(float));
    strcpy((char *)crcData, (char *)sampleBytes);
    strcat((char *)crcData, (char *)timeBytes);
    strcat((char *)crcData, (char *)positionBytes); // Hard Fault here!

    Serial::WriteCrc(crcData, SCANBETWEEN_PACKAGE_LENGTH);
}

/**
 * 
 */
float MotorDriver::CalculateCurrentTime() {
    volatile int totalSecondsF = totalTimeElapsed;
    volatile float leftOverTimeF = (float)(NVIC_ST_RELOAD_R - NVIC_ST_CURRENT_R) / TIVA_CLOCK_SPEED;
    return ((float)(totalSecondsF) + leftOverTimeF);
}

bool MotorDriver::IsSwitchB2On(){
    return (GPIO_PORTB_DATA_R & 0x2) == 0x2;
}

bool MotorDriver::IsSwitchB1On() {
    return (GPIO_PORTB_DATA_R & 0x1) == 0x1;
}

bool MotorDriver::IsAdcOn(){
    return (TIMER0_CTL_R & 0x1);
}

void MotorDriver::TurnAdcOn(){
    TIMER0_CTL_R |= 0x1;
}

void MotorDriver::TurnAdcOff(){
    TIMER0_CTL_R &= ~1;
}
