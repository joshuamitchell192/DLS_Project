#include "registers.h"

void SensorADCSetup(void);
void SensorGPIOSetup(void);
void SensorTimerSetup(void);
void Uart0TerminalSetup(void);

extern void EnableInterrupts(void);
extern void DisableInterrupts(void);
extern void ADC0SS3_Handler(void);

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
	// Clear sampling rate
	ADC0_PC &= ~0xF;
	
	// Set Max Sampling Rate
	ADC0_PC |= 0x7;
	
	// configure trigger event
	ADC0_EMUX |= 0x5000;
	
	// Set input source to be AIN0
	ADC0_SSMUX3 &= ~0xF;


	
	ADC0_SSCTL3 &= ~0xF;
	ADC0_SSCTL3 |= 0x6;
	
	DisableInterrupts();
	
	// Enable interrupts for sequencer 3
	ADC0_IM |= 0x8;
	
	// NVIC for IRQ 17 (ADC0 Sequencer 3)
	NVIC_EN0 |= (1<<17);
	
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
	
	// enable sensor port E3
	GPIOE_DEN |= 0x8;
}

void SensorTimerSetup(void)
{
	/*
	*	Sets up the timer which will pulse the sensor for data.
	*	
	*	We're using Timer0 A with GPIO on port F. When the timer reaches 0
	*	It'll set off an interrupt to pulse the sensor and receive the data
	* 	In sequencer 3's FIFO.
	*
	*/

	// GPIO Setup for Timer0A
//	SYSCTL_RCGCGPIO |= 0x2;
//	while ((SYSCTL_PRGPIO & 0x2) != 0x2);
//	
//	GPIOB_DEN &= ~0x40;
//	
//	GPIOB_DIR |= 0x40;
//	
//	GPIOB_AFSEL |= 0x40;
//	
//	GPIOB_PCTL &= ~0xF000000;
//	
//	GPIOB_PCTL |= 0x7000000;
//	
//	GPIOB_DEN |= 0x40;
	
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
	TIMER0_TAILR = 10000;
	
	// Clear interrupts
	TIMER0_ICR |= 0x1;
	
	// Enable Timer output trigger for ADC and enable timer
	TIMER0_CTL |= 0x21;
}

void Uart0TerminalSetup(void)
{
	// Setup GPIO for uart
	SYSCTL_RCGCGPIO |= 0x1;
	while((SYSCTL_PRGPIO & 0x1) != 0x1);
	
	GPIOA_AFSEL |= 0x3;
	
	GPIOA_PCTL &= ~0xFF;
	GPIOA_PCTL |= 0x11;
	
	GPIOA_DEN |= 0x3;
	
	// Setup UART0
	
	// Enable clock for UART 0
	SYSCTL_RCGUART |= 0x1;
	while((SYSCTL_PRUART & 0x1) != 0x1);
	
	// Disable UART 0 for setup
	UART0_CTL &= ~0x1;
	
	UART0_IBRD = 34;
	UART0_FBRD = 46;
	
	UART0_LCRH |= 0x4A;
	
	UART0_CTL |= 0x321;
}