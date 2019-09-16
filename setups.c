#include "registers.h"

void SensorADCSetup(void);
void SensorGPIOSetup(void);
void SensorTimerSetup(void);

extern void EnableInterrupts(void);
extern void DisableInterrupts(void);

void SensorADCSetup(void)
{
	// ADC Sequencer 0 on port E3 (AIN0 P6)
	
	// Enable Clock
	SYSCTL_RCGCADC |= 0x1;
	while ((SYSCTL_PRADC) != 0x1);
	
	SensorGPIOSetup();
	
	SensorTimerSetup();
	
	// Turn off sequencer 3
	ADC0_ACTSS &= ~0x8;
	ADC0_PC &= ~0xF;
	
	// Set Max Sampling Rate
	ADC0_PC |= 0x7;
	
	ADC0_SSMUX3 &= ~0xF;
	//ADC0_SSMUX3 |= 0x3;

	ADC0_EMUX &= ~0xF000;
	ADC0_EMUX |= 0x5000;
	
	ADC0_SSCTL3 &= ~0xF;
	ADC0_SSCTL3 |= 0x6;
	
	DisableInterrupts();
	
	// Enable interrupts for sequencer 3
	ADC0_IM |= 0x8;
	
	// NVIC for IRQ 17
	NVIC_EN0 |= (1<<17);
	
	// enable sensor port
	GPIOE_DEN |= 0x8;
	
	EnableInterrupts();
	
	// Clear interrupts for sequencer 3
	ADC0_ISC |= 0x8;
	
	// Turn on sequencer 3
	ADC0_ACTSS |= 0x8; 
}

void SensorGPIOSetup(void)
{
	// Enable GPIO Clock for port E
	SYSCTL_RCGCGPIO |= 0x10;
	while ((SYSCTL_PRGPIO & 0x10) != 0x10);
	
	// Disable and wait until ADC is setup
	GPIOE_DEN &= ~0x8;
	
	// Set Direction to be input
	GPIOE_DIR &= ~0x8;
	
	// Set AFSEL to AIN0 on Port E3
	GPIOE_AFSEL |= 0x8;
	
	// Enable analog on pin 3
	GPIOE_AMSEL |= 0x8;
}

void SensorTimerSetup(void)
{
	// GPIO Setup for Timer0A
	SYSCTL_RCGCGPIO |= 0x2;
	while ((SYSCTL_PRGPIO & 0x2) != 0x2);
	
	GPIOB_DEN &= ~0x40;
	
	GPIOB_DIR |= 0x40;
	
	GPIOB_AFSEL |= 0x40;
	
	GPIOB_PCTL &= ~0xF000000;
	
	GPIOB_PCTL |= 0x7000000;
	
	GPIOB_DEN |= 0x40;
	
	// Setup for timer 
	SYSCTL_RCGCTIMER |= 0x1;
	while ((SYSCTL_PRTIMER & 0x1) != 0x1);
	
	// Disable timer
	TIMER0_CTL &= ~0x1;
	// Set to 16 bit config
	TIMER0_CFG |= 0x4;
	
	// Ensure capture mode doesn't enable with 0x3;
	TIMER0_TAMR &= ~0x1;
	// Set to periodic timer
	TIMER0_TAMR |= 0x2;
	
	// Set delay of 1000
	TIMER0_TAILR = 1000;
	
	// Clear interrupts
	TIMER0_ICR |= 1;
	
	// Enable Timer output trigger for ADC and enable timer
	TIMER0_CTL |= 0x21;
}