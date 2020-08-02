#include "motorDriver.h"
#include "registers.h"
#include "helpers.h"

#define MIN_SAMPLE_DURATION 0.0014

//int sampleTotal;
//int numSamples;

static const float mm = 45.0;

MotorDriver::MotorDriver(){
    stepMode = StepModes::Full;
    stepsPerMM = 0.0;
    currentPosition = 0;
    sampleDuration = 0.1;
    stepSize = 100;
    stepAmount = 4;
}

// int numMultiples;
// double remainder;

// Sets StepMode  on GPIOA
void MotorDriver::setStepAmount(int stepAmount){
    //M0 = A6, M1 = A7
    if (stepAmount == 0){
        GPIOA_DATA &= ~0x40;
        GPIOA_DATA |= 0x80;
    }else if(stepAmount == 1){
        GPIOA_DATA |= 0x40;
        GPIOA_DATA &= ~0x80;
    }else if(stepAmount == 2){
        GPIOA_DATA &= ~0x40;
        GPIOA_DATA &= ~0x80;
    }
}

// void MotorDriver::sampleTwoPoints(double sampleDuration, int point1, int point2){
// 	setStepAmount(2);
    
// 	volatile double v_sampleDuration = sampleDuration * 0.001;
// 	setDriverTimer(0.0014);
// 	//get first
    
// 	if (point2 < point1){
// 			int temp = point1;
// 			point1 = point2;
// 			point2 = temp;
// 	}
    
// 	int dir;
// 	if (currentPosition > (point1-1)*4){
// 			GPIOA_DATA &= ~0x8;
// 			dir = -4;
// 	}else{
// 		GPIOA_DATA |= 0x8;
// 			dir = 4;
// 	}
    
// 	while (currentPosition != (point1-1)*4 && !STOP){
// 			currentPosition += dir;
// 			stepMotor();
// 	}
    
// 	setDriverTimer(v_sampleDuration);
    
// 	GPIOA_DATA |= 0x8;
    
// 	if (stepAmount == 0) dir = 1;
// 	if (stepAmount == 1) dir = 2;
// 	if (stepAmount == 2) dir = 4;
    
// 	Delay(100);
    
// 	//stepAmount - 0=Quarter, 1=Half, 2=Full
// 	setStepAmount(stepAmount);
// 	//Turn on adc timer for sampling
// 	TIMER0_CTL |= 0x21;
// 	NVIC_ST_CURRENT |= 1;
// 	timeElapsed = 0;
// 	volatile unsigned int totalTime;
// 	while (currentPosition <= (point2)*4 && !STOP){
// 			sampleTotal = 0;
// 			numSamples = 0;
// 			currentPosition += dir;
// 			setDriverTimer(v_sampleDuration);
// 			stepMotor();
// 			if (numSamples>0){
// 				unsigned int avgSample = sampleTotal / numSamples;
// 				//SEND BACK SAMPLE
// 				unsigned int avgSample_lowerHalf = (0xFF & avgSample); 
// 				unsigned int avgSample_upperHalf = (0xF00 & avgSample) >> 8;
// 				WriteChar(avgSample_lowerHalf);
// 				WriteChar(avgSample_upperHalf);
// 				//SEND TIME
// 				volatile float totalSecondsF = (float)timeElapsed;
// 				volatile float leftOverTimeF = (float)(NVIC_ST_RELOAD - NVIC_ST_CURRENT) / 16000000.0;
// 				totalTime = ((float)(totalSecondsF/1000000) + leftOverTimeF) * 1000;
// 				unsigned int timeLower = (0xFF & totalTime);
// 				unsigned int timeMid = (0xFF00 & totalTime) >> 8;
// 				unsigned int timeUpper = (0xFF0000 & totalTime) >> 16;
// 				WriteChar(timeLower);
// 				WriteChar(timeMid);
// 				WriteChar(timeUpper);
// 			}
            
// 			setDriverTimer(0.0014);
// 			int movingSteps = stepSize*stepsPerMM;//stepSize/step
// 			if (stepAmount == 0){
// 				movingSteps *= 4;
// 			}else if (stepAmount == 1){
// 				movingSteps *= 2;
// 			}
// 			for (int i = 0; i < movingSteps-1; i++){
// 				currentPosition += dir;
// 				stepMotor();
// 			}	
// 	}
    
    
// 	TIMER0_CTL &= ~0x21;
    
// 	Helpers::WriteChar(0xFF);
// 	Helpers::WriteChar(0xFF);
    
// 	setStepAmount(2);
// }

void MotorDriver::setDriverTimer(volatile double seconds){
    // if (seconds > 1){
    //     numMultiples = (int)seconds;
    //     remainder = seconds - (int)seconds;
    //     seconds = 1;
    // }else{
    //     numMultiples = -1;
    // }
    
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

    setStepAmount(2);
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

// void MotorDriver::sampleHere(int duration,int avgInterval){
//     sampleTotal = 0;
//     numSamples = 0;
//     TIMER0_CTL |= 0x21;
// //setDriverTimer(duration);
//   //  TIMER1_ICR |= 1;
// 	//TIMER1_CTL |= 1;
// 		avgInterval = 10;
//     while(!STOP){
//         if (numSamples >= avgInterval){
//             unsigned int avgSample = sampleTotal / numSamples;

// 			unsigned int avgSample_lowerHalf = (0xFF & avgSample); 
// 			unsigned int avgSample_upperHalf = (0xF00 & avgSample) >> 8;	
            
// 			WriteChar(avgSample_lowerHalf);
            
// 			WriteChar(avgSample_upperHalf);

// 			sampleTotal = 0;
//             numSamples = 0;
//         }
//     }
//     TIMER0_CTL &= ~0x21;
// 	WriteChar(0xFF);
// 	WriteChar(0xFF);
// }


//goto rapid positioning to point 1
//set sample duration, step size, step amount
//scan between to point 2
//  set driver timer to sample duration
//  step the motor (which will take sample duration time)
//  average and send samples
//  goto rapid positioning currentPos + stepSize

void MotorDriver::move(bool &stop, int dest, bool setMaxSpeed) {
    if (setMaxSpeed){
        sampleDuration = MIN_SAMPLE_DURATION;
    }

    if (MotorDriver::IsAdcOn()){
        scanBetween( stop, dest);
    }
    else {
        goTo(stop, dest);
    }
}

void MotorDriver::goTo(bool &stop, int dest){
    setStepAmount(2);
    setDriverTimer(sampleDuration);
    
    int destStepPos = dest * stepsPerMM;
    int dir;
    if (currentPosition > (dest-1)*4){
        GPIOA_DATA &= ~0x8;
        dir = -stepAmount;
    }else{
        GPIOA_DATA |= 0x8;
        dir = stepAmount;
    }
    while (currentPosition != (dest-1)*stepAmount && !stop){
        currentPosition += dir;
        stepMotor();
    }
    GPIOA_DATA |= 0x8;
    stepMotor();
    currentPosition += dir;
}
 
 void sendSamples(int &sampleTotal, int &numSamples){
    int avg_sample = (double)sampleTotal / numSamples;
    MotorDriver::sendInt(avg_sample);
 }
 
 void MotorDriver::scanBetween(bool &stop, int dest){
    int destStepPos = dest * stepsPerMM;
    int numSamples = 0;
    int sampleTotal = 0;
    int tempSampleDuration = sampleDuration;
    int dir;
    if (currentPosition > (dest-1)*4){
        GPIOA_DATA &= ~0x8;
        dir = -stepAmount;
    }else{
        GPIOA_DATA |= 0x8;
        dir = stepAmount;
    }
    while (currentPosition <= (dest)*stepAmount && !stop){
        setDriverTimer(sampleDuration);
        sampleTotal = 0;
        numSamples = 1;
        stepMotor();
        currentPosition += stepAmount;
        sendSamples(sampleTotal, numSamples);
        sampleDuration = MIN_SAMPLE_DURATION;
        goTo(stop, currentPosition + stepSize);
        sampleDuration = tempSampleDuration;
    }
 }

void MotorDriver::sendInt(int input){
    unsigned int avgSample_lowerHalf = (0xFF & input); 
    unsigned int avgSample_upperHalf = (0xF00 & input) >> 8;

    Helpers::WriteChar(avgSample_lowerHalf);
    Helpers::WriteChar(avgSample_upperHalf);
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