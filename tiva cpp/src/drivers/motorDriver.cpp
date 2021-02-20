#include "motorDriver.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "helpers.h"
#include "serial.h"

#define MIN_SAMPLE_DURATION 0.001
#define STAGE_LENGTH_MM 65.0
#define TIVA_CLOCK_SPEED 16000000.0

MotorDriver::MotorDriver(){
    stepsPerMM = 0.0;
    currentPosition = 0;
    sampleDuration = 0.0017;
    motorStepDelay = MIN_SAMPLE_DURATION;
    stepsBetweenSamples = 48;
    stepAmount = 4;
    totalTimeElapsed = 0;
    averageInterval = 20;
    calibrationStepCount = 1;
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

void MotorDriver::SetAverageInterval(float averageInterval_) {
    averageInterval = averageInterval_;
}

void MotorDriver::SetSampleDuration(double sampleDuration_){
    sampleDuration = sampleDuration_;
}

void MotorDriver::SetStepsBetweenSamples(double stepLength){
    stepsBetweenSamples = stepLength * stepsPerMM * 4;
}

void MotorDriver::SetMotorStepDelayTimer(volatile double seconds){
    int prescale = Helpers::getPrescaler(seconds);
    int preload = Helpers::getPreload(seconds, prescale);
    
    TIMER1_TAPR_R = prescale;
    TIMER1_TAILR_R = preload;

    TIMER1_ICR_R |= 1;
    TIMER1_CTL_R |= 1;
}

void MotorDriver::SetSampleDurationTimer(double seconds) {
    int prescale = Helpers::getPrescaler(seconds);
    int preload = Helpers::getPreload(seconds, prescale);

    TIMER2_TAPR_R = prescale;
    TIMER2_TAILR_R = preload;

    TIMER2_CTL_R |= 1;
}


void MotorDriver::StepMotor(void){
        while((TIMER1_RIS_R & 0x1) != 0x1);
        TIMER1_ICR_R |= 1;
        GPIO_PORTA_DATA_R |= 0x4;
        while((TIMER1_RIS_R & 0x1) != 0x1);
        TIMER1_ICR_R |= 1;
        GPIO_PORTA_DATA_R  &=~ 0x4;
}

void MotorDriver::RunSampleDurationTimer() {

    TIMER2_ICR_R |= 1;
    while((TIMER2_RIS_R & 0x1) != 0x1);
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

    SetMotorStepDelayTimer(MIN_SAMPLE_DURATION);

    while(!stop && !IsSwitchB2On() && calibrationStepCount == 1){
        StepMotor();
    }
    // change direction to negative
    GPIO_PORTA_DATA_R &= ~0x8;
    while(!stop){
        StepMotor();
        calibrationStepCount++;
        if (IsSwitchB1On()){
            stepsPerMM = calibrationStepCount / STAGE_LENGTH_MM;
            currentPosition = 0;
            
            Serial::WriteFlag(Serial::Calibration);
            Serial::SendShort(stepsPerMM);
            calibrationStepCount = 1;
            break;
        }
    }

    
   
}

/**
 * Takes ADC samples at the current position without stepping the motor.
 * 
 * @param stop : System state flag to stop and return to event loop if true.
 */
void MotorDriver::StartSamplingHere(bool &stop){

        sampleTotal = 0;
        numSamples = 1;
        
        Serial::WriteFlag(Serial::Sample);
        // Change to average interval
        SetSampleDurationTimer(averageInterval);
        RunSampleDurationTimer();

        WaitForSamples();
}

//goto rapid positioning to point 1
//set sample duration, step size, step amount
//scan between to point 2
//  set driver timer to sample duration
//  step the motor (which will take sample duration time)
//  average and send samples
//  goto rapid positioning currentPos + stepSize

/**
 * Moves to a given destination with or without taking samples.
 * Destination is converted to the number of quarter steps required.
 * 
 * @param stop: Flag to stop and return to event loop if true
 * @param dest: Destination value in millimetres
 */
void MotorDriver::Move(bool &stop, double dest) {
    
    int destination = (int)(dest * stepsPerMM * 4);

    if (MotorDriver::IsAdcOn()){
        ScanBetween( stop, destination);
    }
    else {
        GoToPosition(stop, destination);
    }
}

/**
 * Steps the motor to the given position, unless a limit switch is turned on in the oncoming direction.
 * 
 * @param stop : System state flag to stop and return to event loop if true.
 * @param dest : Position destination that we wish to step toward.
 */
void MotorDriver::GoToPosition(bool &stop, int dest){
    SetStepMode(2);
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
 * Moves to destination position while taking samples. 
 * 
 * This will run if given a G01 instruction after a T1 instruction (ADC is on).
 * 
 * @param stop : System state flag to stop and return to event loop if true.
 * @param dest : Destination position
*/
void MotorDriver::ScanBetween(bool &stop, int dest) {
    numSamples = 0;
    sampleTotal = 0;
    totalTimeElapsed = 0;
    
    int direction = SetDirection(dest);

    OffSetStage(stop, direction);

    if (direction == 1) {
        while (currentPosition < dest && !stop && !IsSwitchB2On()){
            Serial::WriteFlag(Serial::Sample);
            WaitForSamples();
            GoToPosition(stop, currentPosition + stepsBetweenSamples);
        }
    }
    else if (direction == -1) {
        while (currentPosition > dest && !stop && !IsSwitchB1On()){
            Serial::WriteFlag(Serial::Sample);
            WaitForSamples();
            GoToPosition(stop, currentPosition + (stepsBetweenSamples * direction));
        }
    }

}

/**
 * Offsets the stage a few steps back before it begins sampling to avoid stepper motor inaccuracy upon direction changes.
 * 
 * @param stop : System state flag to stop and return to event loop if true.
 * @param direction : The direction that we wish to travel when sampling. Used to calculate the direction of offset.
 */ 
void MotorDriver::OffSetStage(bool &stop, int direction)
{
        while (!IsSwitchB2On());
        int returnPosition = currentPosition;
        int dest = currentPosition - stepAmount * direction * 100;
        SetDirection(dest);
        GoToPosition(stop, dest);
        SetDirection(returnPosition);
        GoToPosition(stop, returnPosition);
        while (!IsSwitchB1On());
}

/**
 * Retrieves and calculates each part of the ADC sample and writes it back to the serial connection.
 * (Sample average, time when sample was averaged, and the current position where the sample was taken).
 */
void MotorDriver::WaitForSamples() {

    sampleTotal = 0;
    numSamples = 1;

    SetSampleDurationTimer(sampleDuration);
    RunSampleDurationTimer();

    int sampleAvg = Serial::CalculateSampleAverage(sampleTotal, numSamples);
    unsigned char * sampleBytes = Serial::SendShort(sampleAvg);

    volatile float currentTime = CalculateCurrentTime();
    unsigned char * timeBytes = Serial::SendFloat(currentTime);

    float currentPosMM = (float)(currentPosition / stepsPerMM / 4);
    unsigned char * positionBytes = Serial::SendFloat(currentPosMM);
}

/**
 * Calculates the current elapsed time from the sysTick timer since the start of sampling.
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
    TIMER0_CTL_R &= ~0x1;
}
