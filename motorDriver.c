#include "registers.h"


extern void Delay(unsigned int);

extern int getPreload(double seconds, int prescaler);
extern int getPrescaler(double seconds);

int calibrate(void);
void setDriverTimer(double seconds);
void stepMotor(void);
void sampleHere(int duration,int avgInterval);
void goTo(int point);
void setStepAmount(int stepAmount);

extern int currentPosition;
extern float stepsPerMM;
extern int STOP;
extern float stepSize;

int sampleTotal;
int numSamples;

void setStepAmount(int stepAmount){
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

void sampleTwoPoints(double sampleDuration, int point1, int point2){
	//stepAmount - 0=Quarter, 1=Half, 2=Full
	int stepAmount = 2;
	setStepAmount(stepAmount);
	volatile double v_sampleDuration = sampleDuration * 0.0001;
	setDriverTimer(0.003);
	//get first
	
	if (point2 < point1){
			int temp = point1;
			point1 = point2;
			point2 = temp;
	}
	
	int dir;
	if (currentPosition > point1-1){
			GPIOA_DATA &= ~0x8;
			dir = -1;
	}else{
		GPIOA_DATA |= 0x8;
			dir = 1;
	}
	
	while (currentPosition != point1-1 && !STOP){
			currentPosition += dir;
			stepMotor();
	}
	
	setDriverTimer(sampleDuration);
	dir = 1;
	GPIOA_DATA |= 0x8;
	
	Delay(100);
	
	//Turn on adc timer for sampling
	TIMER0_CTL |= 0x21;
	while (currentPosition <= point2 && !STOP){
			sampleTotal = 0;
			numSamples = 0;
			currentPosition += dir;
			setDriverTimer(sampleDuration);
			stepMotor();
			if (numSamples>0){
				unsigned int avgSample = sampleTotal / numSamples;
				//SEND BACK SAMPLE
				unsigned int avgSample_lowerHalf = (0xFF & avgSample); 
				unsigned int avgSample_upperHalf = (0xF00 & avgSample) >> 8;
				
				
				WriteChar(avgSample_lowerHalf);
				WriteChar(avgSample_upperHalf);
				//unsigned int sampleAckt = ReadChar();
			}
			
			setDriverTimer(0.003);
			int movingSteps = stepSize/(float)(1/stepsPerMM);
			for (int i = 0; i < movingSteps-1; i++){
				currentPosition += dir;
				stepMotor();
			}
			
			
	}
	
	TIMER0_CTL &= ~0x21;
	
	WriteChar(0xFF);
	WriteChar(0xFF);
}

void setDriverTimer(double seconds){
	int prescale = getPrescaler(seconds);
	int preload = getPreload(seconds, prescale);
	
	TIMER1_TAILR = preload;
	TIMER1_TAPR = prescale;

	TIMER1_ICR |= 1;
	TIMER1_CTL |= 1;
}


void stepMotor(void){
		//while((TIMER1_RIS & 0x1) != 0x1);	
		TIMER1_ICR |= 1;
		GPIOA_DATA |= 0x4;
		while((TIMER1_RIS & 0x1) != 0x1);	
		//TIMER1_ICR |= 1;
		GPIOA_DATA &=~ 0x4;
}

int calibrate(void){
	GPIOA_DATA |= 0x8;

	setDriverTimer(0.003);
	//setDriverTimer(0.0007);
	
	while(!STOP){
		stepMotor();
		
		if ((GPIOB_DATA & 0x2) == 0x2){
				break;
		}
	}
	
	GPIOA_DATA &= ~0x8;
	int numSteps = 1;
	while(!STOP){
		stepMotor();
		numSteps ++;
		if ((GPIOB_DATA & 0x1) ==0x1){
				break;
		}
	}
	currentPosition = 0;
	float mm = 60.0;
	stepsPerMM = numSteps/mm;
	return numSteps;
}

void sampleHere(int duration,int avgInterval){
    sampleTotal = 0;
    numSamples = 0;
    TIMER0_CTL |= 0x21;
//setDriverTimer(duration);
  //  TIMER1_ICR |= 1;
	//TIMER1_CTL |= 1;
		avgInterval = 10;
    while(!STOP){
        if (numSamples >= avgInterval){
            unsigned int avgSample = sampleTotal / numSamples;

			unsigned int avgSample_lowerHalf = (0xFF & avgSample); 
			unsigned int avgSample_upperHalf = (0xF00 & avgSample) >> 8;	
			
			WriteChar(avgSample_lowerHalf);
			
			WriteChar(avgSample_upperHalf);

			sampleTotal = 0;
            numSamples = 0;
        }
    }
    TIMER0_CTL &= ~0x21;
	WriteChar(0xFF);
	WriteChar(0xFF);
}

void goTo(int point){
    setDriverTimer(0.003);
    int dir;
    if (currentPosition > point-1){
            GPIOA_DATA &= ~0x8;
            dir = -1;
    }else{
        GPIOA_DATA |= 0x8;
            dir = 1;
    }
    while (currentPosition != point-1 && !STOP){
            currentPosition += dir;
            stepMotor();
    }
    GPIOA_DATA |= 0x8;
    stepMotor();
    currentPosition ++;
}
