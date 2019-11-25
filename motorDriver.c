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
extern unsigned int stepAmount;
extern unsigned int timeElapsed;

int sampleTotal;
int numSamples;

int numMultiples;
double remainder;

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
	setStepAmount(2);
	
	volatile double v_sampleDuration = sampleDuration * 0.001;
	setDriverTimer(0.0014);
	//get first
	
	if (point2 < point1){
			int temp = point1;
			point1 = point2;
			point2 = temp;
	}
	
	int dir;
	if (currentPosition > (point1-1)*4){
			GPIOA_DATA &= ~0x8;
			dir = -4;
	}else{
		GPIOA_DATA |= 0x8;
			dir = 4;
	}
	
	while (currentPosition != (point1-1)*4 && !STOP){
			currentPosition += dir;
			stepMotor();
	}
	
	setDriverTimer(v_sampleDuration);
	
	GPIOA_DATA |= 0x8;
	
	if (stepAmount == 0) dir = 1;
	if (stepAmount == 1) dir = 2;
	if (stepAmount == 2) dir = 4;
	
	Delay(100);
	
	//stepAmount - 0=Quarter, 1=Half, 2=Full
	setStepAmount(stepAmount);
	//Turn on adc timer for sampling
	TIMER0_CTL |= 0x21;
	NVIC_ST_CURRENT |= 1;
	timeElapsed = 0;
	volatile unsigned int totalTime;
	while (currentPosition <= (point2)*4 && !STOP){
			sampleTotal = 0;
			numSamples = 0;
			currentPosition += dir;
			setDriverTimer(v_sampleDuration);
			stepMotor();
			if (numSamples>0){
				unsigned int avgSample = sampleTotal / numSamples;
				//SEND BACK SAMPLE
				unsigned int avgSample_lowerHalf = (0xFF & avgSample); 
				unsigned int avgSample_upperHalf = (0xF00 & avgSample) >> 8;
				WriteChar(avgSample_lowerHalf);
				WriteChar(avgSample_upperHalf);
				//SEND TIME
				volatile float totalSecondsF = (float)timeElapsed;
				volatile float leftOverTimeF = (float)(NVIC_ST_RELOAD - NVIC_ST_CURRENT) / 16000000.0;
				totalTime = ((float)(totalSecondsF/1000000) + leftOverTimeF) * 1000;
				unsigned int timeLower = (0xFF & totalTime);
				unsigned int timeMid = (0xFF00 & totalTime) >> 8;
				unsigned int timeUpper = (0xFF0000 & totalTime) >> 16;
				WriteChar(timeLower);
				WriteChar(timeMid);
				WriteChar(timeUpper);
			}
			
			setDriverTimer(0.0014);
			int movingSteps = stepSize*stepsPerMM;//stepSize/step
			if (stepAmount == 0){
				movingSteps *= 4;
			}else if (stepAmount == 1){
				movingSteps *= 2;
			}
			for (int i = 0; i < movingSteps-1; i++){
				currentPosition += dir;
				stepMotor();
			}	
	}
	
	
	TIMER0_CTL &= ~0x21;
	
	WriteChar(0xFF);
	WriteChar(0xFF);
	
	setStepAmount(2);
}

void setDriverTimer(volatile double seconds){
	if (seconds > 1){
		numMultiples = (int)seconds;
		remainder = seconds - (int)seconds;
		seconds = 1;
	}else{
		numMultiples = -1;
	}
	
	
	
	int prescale = getPrescaler(seconds);
	int preload = getPreload(seconds, prescale);
	
	TIMER1_TAPR = prescale;
	TIMER1_TAILR = preload;
	

	TIMER1_ICR |= 1;
	TIMER1_CTL |= 1;
}


void stepMotor(void){
		if (numMultiples == -1){
			while((TIMER1_RIS & 0x1) != 0x1);
			TIMER1_ICR |= 1;
		}else{
			for (int i = 0; i < numMultiples; i++){
				while((TIMER1_RIS & 0x1) != 0x1);
				TIMER1_ICR |= 1;
			}
			setDriverTimer(remainder);
			while((TIMER1_RIS & 0x1) != 0x1);
		}
		
		//TIMER1_ICR |= 1;
		GPIOA_DATA |= 0x4;
		//while((TIMER1_RIS & 0x1) != 0x1);		
		TIMER1_ICR |= 1;
		GPIOA_DATA &=~ 0x4;
		
	
		
}

int calibrate(void){
	setStepAmount(2);
	GPIOA_DATA |= 0x8;

	setDriverTimer(0.0014);
	
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
	float mm = 45.0;
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
		setStepAmount(2);
    setDriverTimer(0.0014);
    int dir;
    if (currentPosition > (point-1)*4){
            GPIOA_DATA &= ~0x8;
            dir = -4;
    }else{
        GPIOA_DATA |= 0x8;
            dir = 4;
    }
    while (currentPosition != (point-1)*4 && !STOP){
            currentPosition += 4;
            stepMotor();
    }
    GPIOA_DATA |= 0x8;
    stepMotor();
    currentPosition += 4;
}
