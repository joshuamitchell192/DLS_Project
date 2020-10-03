#include "setup.h"
#include "helpers.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>

void Setup::DriverGPIOSetup(void){
	SYSCTL_RCGCGPIO_R |= 0x1;
	while((SYSCTL_PRGPIO_R & 0x1) != 0x1);
	
	// set GPIO A Direction to output on pins 2, 3, 4, 5
	GPIO_PORTA_DIR_R |= 0x3C;
	
	// Turn off alternative function select
	GPIO_PORTA_AFSEL_R &= ~0x3C;
	
	// Turn on the port if not already
	GPIO_PORTA_DEN_R |= 0x3C;
}

void Setup::sysTickSamplingTimerSetup(void){
	NVIC_ST_CTRL_R &= ~0x1;
	NVIC_ST_RELOAD_R |= 0xFFFFFF;
	NVIC_ST_CURRENT_R |= 0x1;
	NVIC_ST_CTRL_R |= 6;
	NVIC_ST_CTRL_R |= 1;
}

// PA6,7 OUTPUT FOR SETTING MOTOR STEP SIZE 
// void Setup::StepSizeGPIOSetup(void){
// 	GPIOA_DIR |= 0xC0;
// 	GPIOA_DEN &= ~0xC0;
// 	GPIOA_DEN |= 0xC0;
// 	MotorDriver::setStepAmount(2);
// }

void Setup::DriverTimerSetup(void){
	
	SYSCTL_RCGCTIMER_R |= 0x2;
	while((SYSCTL_PRTIMER_R & 0x2) != 0x2);
	
	// Disable Timer 1
	TIMER1_CTL_R &= ~0x1;
	
	// Select 32 bit timer configuration
	TIMER1_CFG_R &= ~0x5;
	
	
	TIMER1_TAMR_R &= 3;
	TIMER1_TAMR_R |= 2;

}

//PB0,1
void Setup::LimitSwitchesGPIOSetup(void){
	SYSCTL_RCGCGPIO_R |= 0x2;
	while((SYSCTL_PRGPIO_R & 0x2) != 0x2);
	
	GPIO_PORTB_DIR_R &= ~0x3;
	GPIO_PORTB_AFSEL_R &= ~0x3;
	GPIO_PORTB_DEN_R |= 0x3;
}

void Setup::SensorADCSetup(void)
{
	// ADC Sequencer 0 on port E3 (AIN0 P6)
	
	// Enable Clock
	SYSCTL_RCGCADC_R |= 0x1;
	while ((SYSCTL_PRADC_R) != 0x1);
	
	SensorGPIOSetup();
	
	SensorTimerSetup();
	
	// Turn off sequencer 3
	ADC0_ACTSS_R &= ~0x8;
	// Clear sampling rate
	ADC0_PC_R &= ~0xF;
	
	// Set Max Sampling Rate
	ADC0_PC_R |= 0x7;
	
	// configure trigger event
	ADC0_EMUX_R |= 0x5000;
	
	// Set input source to be AIN0
	ADC0_SSMUX3_R &= ~0xF;


	/* 
	* Set control config for sequencer 3 to set END0 for last 
	* sample on single input and enable sample interrupts (IE0)
	*/
	ADC0_SSCTL3_R &= ~0xF;
	ADC0_SSCTL3_R |= 0x6;
	
	Helpers::DisableInterrupts();
	
	// Enable interrupts for sequencer 3
	ADC0_IM_R |= 0x8;
	
	// NVIC for IRQ 17 (ADC0 Sequencer 3)
	NVIC_EN0_R |= (1<<17);
	
	
	Helpers::EnableInterrupts();
	
	// Clear interrupts for sequencer 3
	ADC0_ISC_R |= 0x8;
	
	// Turn on sequencer 3
	ADC0_ACTSS_R |= 0x8; 
}

void Setup::SensorGPIOSetup(void)
{
	// Enable GPIO Clock for port E
	SYSCTL_RCGCGPIO_R |= 0x10;
	while ((SYSCTL_PRGPIO_R & 0x10) != 0x10);
	
	// Disable and wait until ADC is setup
	GPIO_PORTE_DEN_R &= ~0x8;
	
	// Set Direction to be input
	GPIO_PORTE_DIR_R &= ~0x8;
	
	// Set AFSEL to AIN0 on Port E3
	GPIO_PORTE_AFSEL_R |= 0x8;
	
	// Enable analog on pin 3
	GPIO_PORTE_AMSEL_R |= 0x8;
	
	// enable sensor port E3
	GPIO_PORTE_DEN_R |= 0x8;
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
	SYSCTL_RCGCTIMER_R |= 0x1;
	while ((SYSCTL_PRTIMER_R & 0x1) != 0x1);
	
	// disable timer
	TIMER0_CTL_R &= ~0x1;
	// set to 16 bit config
	TIMER0_CFG_R |= 0x4;
	
	// ensure capture mode doesn't enable with 0x3;
	TIMER0_TAMR_R &= ~0x1;
	// set to periodic timer
	TIMER0_TAMR_R |= 0x2;
	
	// set delay of 1000 clock cycles (6.25x10^-5 seconds)
	TIMER0_TAILR_R = 1000;
	
	// Clear interrupts
	TIMER0_ICR_R |= 0x1;
    TIMER0_CTL_R |= 0x20;
    // SYSCTL_RCGCTIMER |= 0x1;
	// while ((SYSCTL_PRTIMER & 0x1) != 0x1);
	
	// // Disable timer
	// TIMER0_CTL &= ~0x1;
	// // Set to 32 bit config
	// TIMER0_CFG &= ~0x5;
	
	// // Ensure capture mode doesn't enable with 0x3;
	// TIMER0_TAMR &= ~0x1;
	// // Set to periodic timer
	// TIMER0_TAMR |= 0x2;
	
    // //unsigned int preload = Helpers::getPreload(10000.0, Helpers::getPrescaler(10000.0));
    
    // int prescale = Helpers::getPrescaler(10.0);
	// int preload = Helpers::getPreload(10.0, prescale);
    
	// // Set delay of 1000 clock cycles (6.25x10^-5 seconds)
	// //TIMER0_TAILR = pr;
    // TIMER0_TAPR = prescale;
	// TIMER0_TAILR = preload;
	
	// // Clear interrupts
	// TIMER0_ICR |= 0x1;
	
	
}

void Setup::TimeElapsedTimer(void) {
    // Setup for timer 
	SYSCTL_RCGCTIMER_R |= 0x4;
	while ((SYSCTL_PRTIMER_R & 0x4) != 0x4);
	
	// Disable timer
	TIMER2_CTL_R &= ~0x1;
	// Set to 32 bit config
	TIMER2_CFG_R &= ~0x5;
	
	// Ensure capture mode doesn't enable with 0x3;
	TIMER2_TAMR_R &= ~0x1;
	// Set to periodic timer
	TIMER2_TAMR_R |= 0x2;
	
	// Set delay of 1000 clock cycles (6.25x10^-5 seconds)
	TIMER2_TAILR_R = Helpers::getPreload(2.0, Helpers::getPrescaler(2.0));
	
	// Clear interrupts
	TIMER2_ICR_R |= 0x1;
}

// void Setup::SampleDurationTimer(void) {
//     // Setup for timer 
// 	SYSCTL_RCGCTIMER |= 0x4;
// 	while ((SYSCTL_PRTIMER & 0x4) != 0x4);
	
// 	// Disable timer
// 	TIMER2_CTL &= ~0x1;
// 	// Set to 32 bit config
// 	TIMER2_CFG &= ~0x5;
	
// 	// Ensure capture mode doesn't enable with 0x3;
// 	TIMER2_TAMR &= ~0x1;
// 	// Set to periodic timer
// 	TIMER2_TAMR |= 0x2;
	
// 	// Set delay of 1000 clock cycles (6.25x10^-5 seconds)
// 	TIMER2_TAILR = Helpers::getPreload(2.0, Helpers::getPrescaler(2.0));
	
// 	// Clear interrupts
// 	TIMER2_ICR |= 0x1;
// }

void Setup::Uart0TerminalSetup(void)
{
	/*
		Sets up UART to communicate over usb to the python GUI application.
	
		Uses UART0 (only UART that can communicate over USB). Enable alternative function and select UART on PCTL for GPIOA 0, 1. 
		Then set baud rate for UART communication, line control values to odd parity, 2 stop bits and word length of 8 bits. 
		Finally setup interrutps on UART receive.
	*/

	// Setup GPIO for uart
	SYSCTL_RCGCGPIO_R |= 0x1;
	while((SYSCTL_PRGPIO_R & 0x1) != 0x1);
	
	GPIO_PORTA_AFSEL_R |= 0x3;
	
	GPIO_PORTA_PCTL_R &= ~0xFF;
	GPIO_PORTA_PCTL_R |= 0x11;
	
	GPIO_PORTA_DEN_R |= 0x3;
	
	// Setup UART0
	
	// Enable clock for UART 0
	SYSCTL_RCGCUART_R |= 0x1;
	while((SYSCTL_PRUART_R & 0x1) != 0x1);
	
	// Disable UART 0 for setup
	UART0_CTL_R &= ~0x1;
	
	// Set baud rate for 115200
	UART0_IBRD_R = 34;
	UART0_FBRD_R = 45;
	
	// Set line control register to odd parity, 2 stop bits, and word length of 8 bits
	UART0_LCRH_R |= 0x6A;

    __asm("CPSID i");
	//DisableInterrupts();

	// Set interrupt to trigger when FIFO is 1/8 full
	UART0_IFLS_R &= ~0x1;
	
	// Enable receive interrupts on UART0
	UART0_IM_R |= 0x10;

	// Enable Interrupts for UART0
	NVIC_EN0_R |= (1<<5);
	NVIC_PRI1_R |= (7<<13);
	
    __asm("CPSIE i");
	//EnableInterrupts();
	
	// Clear interrupts on uart
	UART0_ICR_R |= 0x10;
	
	UART0_CTL_R |= 0x321;
}
