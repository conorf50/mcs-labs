// set up the relavent instructions, see handouts 
// 4002500 base address


// Registers for NVIC
#define NVIC_EN0_R (*((volatile unsigned long *) 0xE000E100))
#define NVIC_PRI7_R (*((volatile unsigned long *) 0xE000E41C))

// connect port F to clock register
#define SYSCTL_RCGC2_R (*((volatile unsigned long *) 0x400FE108))
	
// Base address for port F = 0x40025000
#define GPIO_PORTF_DATA_R (*((volatile unsigned long *) 0x400253FC))
#define GPIO_PORTF_DIR_R (*((volatile unsigned long *) 0x40025400))
#define GPIO_PORTF_AFSEL_R (*((volatile unsigned long *) 0x40025420))
#define GPIO_PORTF_DEN_R (*((volatile unsigned long *) 0x4002551C))
#define GPIO_PORTF_AMSEL_R (*((volatile unsigned long *) 0x40025528))
#define GPIO_PORTF_PCTL_R (*((volatile unsigned long *) 0x4002552C))
#define GPIO_PORTF_PUR_R (*((volatile unsigned long *) 0x40025510))
#define GPIO_PORTF_PDR_R (*((volatile unsigned long *) 0x40025514))

// question 9 fix -  see notes
#define GPIO_PORTF_ICR_R (*((volatile unsigned long *) 0x4002541C)) // 

#define PIN_F1 (*((volatile unsigned long *) 0x40025008)) // bit specific addressing for pin 1

// new defines for GPIO interrupts
// all values are set just to be sure
#define GPIO_PORTF_IS_R (*((volatile unsigned long *) 0x40025404)) // edge trigger interrupt sense
#define GPIO_PORTF_IBE_R (*((volatile unsigned long *) 0x40025408)) // interrupt both edges 0
#define GPIO_PORTF_IEV_R (*((volatile unsigned long *) 0x4002540C)) // interrupt event
#define GPIO_PORTF_IM_R (*((volatile unsigned long *) 0x40025410)) // 


// Global variable to use for detecting falling edges
volatile unsigned long FallingEdges = 0;


// init only port F

void initPortF (){
	// function to set up the port
	// step 1 - activate clock for port F
	unsigned long volatile delay; 
	SYSCTL_RCGC2_R |= 0x20;
	delay = SYSCTL_RCGC2_R; // create a small delay so we can let the clock settle
	FallingEdges = 0; //init our counter
	// step 2 = set up pin F4
	GPIO_PORTF_DIR_R  &= ~0x10; // set the direction to input
	GPIO_PORTF_AFSEL_R &= ~0x10; // disable alternate function for pin F
	GPIO_PORTF_DEN_R |= 0x12;  // digital enable 
	GPIO_PORTF_PCTL_R&= ~0x000F0000; // config pin as GPIO
	GPIO_PORTF_AMSEL_R = 0x12; // analog mode select - does NOT need a '~'
	GPIO_PORTF_PUR_R |= 0x10; // set the pullup resistor for this pin
	
	GPIO_PORTF_IS_R &= ~0x10; // define pF1 interrupt as edge sensitive
	GPIO_PORTF_IBE_R &= ~0x10; // disable interrupt trigger on both edges
	GPIO_PORTF_IEV_R &= ~0x10; // 
	GPIO_PORTF_IM_R |=0x10;	
	// ICR register here - see notes

	NVIC_EN0_R |= 1<<30; // enable interrupt 30 in NVIC
	NVIC_PRI7_R |= 0x00A00000; // priority 5
	
	__enable_irq();

}

void GPIOF_Handler(void){
	FallingEdges = FallingEdges +1;
	// clear the ICR register - see notes
	GPIO_PORTF_ICR_R = 0x10;
}


int main (void) {
	initPortF();
	while(1){
	}
}
