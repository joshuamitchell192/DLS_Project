#include "setup.h"

void Setup::DriverGPIOSetup(void){
	SYSCTL_RCGCGPIO |= 0x1;
	while((SYSCTL_PRGPIO & 0x1) != 0x1);
	
	// set GPIO A Direction to output on pins 2, 3, 4, 5
	GPIOA_DIR |= 0x3C;
	
	// Turn off alternative function select
	GPIOA_AFSEL &= ~0x3C;
	
	// Turn on the port if not already
	GPIOA_DEN |= 0x3C;
}

void Setup::sysTickSamplingTimerSetup(void){
	NVIC_ST_CTRL &= ~0x1;
	NVIC_ST_RELOAD |= 0xFFFFFF;
	NVIC_ST_CURRENT |= 0x1;
	NVIC_ST_CTRL |= 6;
	NVIC_ST_CTRL |= 1;
}

// PA6,7 OUTPUT FOR SETTING MOTOR STEP SIZE 
// void Setup::StepSizeGPIOSetup(void){
// 	GPIOA_DIR |= 0xC0;
// 	GPIOA_DEN &= ~0xC0;
// 	GPIOA_DEN |= 0xC0;
// 	MotorDriver::setStepAmount(2);
// }

void Setup::DriverTimerSetup(void){
	
	SYSCTL_RCGCTIMER |= 0x2;
	while((SYSCTL_PRTIMER & 0x2) != 0x2);
	
	// Disable Timer 1
	TIMER1_CTL &= ~0x1;
	
	// Select 16 bit timer configuration
	TIMER1_CFG = 0x4;
	
	
	TIMER1_TAMR &= 3;
	TIMER1_TAMR |= 2;
	
}

//PB0,1
void Setup::LimitSwitchesGPIOSetup(void){
	SYSCTL_RCGCGPIO |= 0x2;
	while((SYSCTL_PRGPIO & 0x2) != 0x2);
	
	GPIOB_DIR &= ~0x3;
	GPIOB_AFSEL &= ~0x3;
	GPIOB_DEN |= 0x3;
}

void Setup::SensorADCSetup(void)
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


	/* 
	* Set control config for sequencer 3 to set END0 for last 
	* sample on single input and enable sample interrupts (IE0)
	*/
	ADC0_SSCTL3 &= ~0xF;
	ADC0_SSCTL3 |= 0x6;
	
	Helpers::DisableInterrupts();
	
	// Enable interrupts for sequencer 3
	ADC0_IM |= 0x8;
	
	// NVIC for IRQ 17 (ADC0 Sequencer 3)
	NVIC_EN0 |= (1<<17);
	
	
	Helpers::EnableInterrupts();
	
	// Clear interrupts for sequencer 3
	ADC0_ISC |= 0x8;
	
	// Turn on sequencer 3
	ADC0_ACTSS |= 0x8; 
}

void Setup::SensorGPIOSetup(void)
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

void Setup::SensorTimerSetup(void)
{
	/*
	*	Sets up the timer which will pulse the sensor for data.
	*	
	*	We're using Timer0 A with GPIO on port F. When the timer reaches 0
	*	It'll set off an interrupt to pulse the sensor and receive the data
	* 	in sequencer 3's FIFO sample queue from the single input on sequencer 3.
	*
	*/
	
	// Setup for timer 
//	SYSCTL_RCGCTIMER |= 0x1;
//	while ((SYSCTL_PRTIMER & 0x1) != 0x1);
//	
//	// Disable timer
//	TIMER0_CTL &= ~0x1;
//	// Set to 16 bit config
//	TIMER0_CFG |= 0x4;
//	
//	// Ensure capture mode doesn't enable with 0x3;
//	TIMER0_TAMR &= ~0x1;
//	// Set to periodic timer
//	TIMER0_TAMR |= 0x2;
//	
//	// Set delay of 1000 clock cycles (6.25x10^-5 seconds)
//	TIMER0_TAILR = 1000;
//	
//	// Clear interrupts
//	TIMER0_ICR |= 0x1;
    SYSCTL_RCGCTIMER |= 0x1;
	while ((SYSCTL_PRTIMER & 0x1) != 0x1);
	
	// Disable timer
	TIMER0_CTL &= ~0x1;
	// Set to 32 bit config
	TIMER0_CFG &= ~0x5;
	
	// Ensure capture mode doesn't enable with 0x3;
	TIMER0_TAMR &= ~0x1;
	// Set to periodic timer
	TIMER0_TAMR |= 0x2;
	
    //unsigned int preload = Helpers::getPreload(10000.0, Helpers::getPrescaler(10000.0));
    
    int prescale = Helpers::getPrescaler(10.0);
	int preload = Helpers::getPreload(10.0, prescale);
    
	// Set delay of 1000 clock cycles (6.25x10^-5 seconds)
	//TIMER0_TAILR = pr;
    TIMER0_TAPR = prescale;
	TIMER0_TAILR = preload;
	
	// Clear interrupts
	TIMER0_ICR |= 0x1;
	
	
}

void Setup::SampleDurationTimer(void) {
    // Setup for timer 
	SYSCTL_RCGCTIMER |= 0x4;
	while ((SYSCTL_PRTIMER & 0x4) != 0x4);
	
	// Disable timer
	TIMER2_CTL &= ~0x1;
	// Set to 32 bit config
	TIMER2_CFG &= ~0x5;
	
	// Ensure capture mode doesn't enable with 0x3;
	TIMER2_TAMR &= ~0x1;
	// Set to periodic timer
	TIMER2_TAMR |= 0x2;
	
	// Set delay of 1000 clock cycles (6.25x10^-5 seconds)
	TIMER2_TAILR = Helpers::getPreload(10.0, Helpers::getPrescaler(10.0));
	
	// Clear interrupts
	TIMER2_ICR |= 0x1;
}


void Setup::Uart0TerminalSetup(void)
{
	/*
		Sets up UART to communicate over usb to the python GUI application.
	
		Uses UART0 (only UART that can communicate over USB). Enable alternative function and select UART on PCTL for GPIOA 0, 1. 
		Then set baud rate for UART communication, line control values to odd parity, 2 stop bits and word length of 8 bits. 
		Finally setup interrutps on UART receive.
	*/

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
	
	// Set baud rate for 56700
	UART0_IBRD = 34;
	UART0_FBRD = 46;
	
	// Set line control register to odd parity, 2 stop bits, and word length of 8 bits
	UART0_LCRH |= 0x6A;

    __asm("CPSID i");
	//DisableInterrupts();

	// Set interrupt to trigger when FIFO is 1/8 full
	UART0_IFLS &= ~0x1;
	
	// Enable receive interrupts on UART0
	UART0_IM |= 0x10;

	// Enable Interrupts for UART0
	NVIC_EN0 |= (1<<5);
	NVIC_PR |= (7<<13);
	
    __asm("CPSIE i");
	//EnableInterrupts();
	
	// Clear interrupts on uart
	UART0_ICR |= 0x10;
	
	UART0_CTL |= 0x321;
}


