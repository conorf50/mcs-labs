// set up the relavent instructions, see handouts 
// 4002500 base address


// Registers for NVIC
#define NVIC_EN0_R (*((volatile unsigned long *) 0xE000E100)) // global enable for NVIC
#define NVIC_PRI7_R (*((volatile unsigned long *) 0xE000E41C))
#define NVIC_SYSPRI3_R (*((volatile unsigned long *) 0xE000ED20))


// new defines for (PLL RCC and RCC2)
// base offset for RCC and RCC2 = 0x400FE000
#define SYSCTL_RCC_R (*((volatile unsigned long *) 0x400FE060))
#define SYSCTL_RCC2_R (*((volatile unsigned long *) 0x400FE070))
#define SYSCTL_RIS_R (*((volatile unsigned long *) 0x400FE050))

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


#define PIN_F1 (*((volatile unsigned long *) 0x40025008)) // bit specific addressing for pin 1
#define PIN_F2 (*((volatile unsigned long *) 0x40025010)) // bit specific addressing for pin 2


// new defines for GPIO interrupts
// all values are set just to be sure
#define GPIO_PORTF_IS_R (*((volatile unsigned long *) 0x40025404)) // edge trigger interrupt sense
#define GPIO_PORTF_IBE_R (*((volatile unsigned long *) 0x40025408)) // interrupt both edges 0
#define GPIO_PORTF_IEV_R (*((volatile unsigned long *) 0x4002540C)) // interrupt event
#define GPIO_PORTF_IM_R (*((volatile unsigned long *) 0x40025410)) // 
#define GPIO_PORTF_ICR_R (*((volatile unsigned long *) 0x4002541C)) // 



// #defines for systick registers
#define ST_CTRL_R (*((volatile unsigned long *) 0xE000E010)) // set up a systick for port F
#define ST_RELOAD_R (*((volatile unsigned long *) 0xE000E014)) // init the reload register
#define ST_CURRENT_R (*((volatile unsigned long *) 0xE000E018)) // address for current value register


// Global variable to use for detecting falling edges
volatile unsigned long FallingEdges = 0;
// Count up the number of times the system timer runs
volatile unsigned long TickCount = 0;



// init only port F

void initPortF (){
	// function to set up the port
	// step 1 - activate clock for port F
	unsigned long volatile delay; 
	SYSCTL_RCGC2_R |= 0x20;
	delay = SYSCTL_RCGC2_R; // create a small delay so we can let the clock settle
	FallingEdges = 0; //init our counter
	// step 2 = set up pin F4
	GPIO_PORTF_DIR_R  &= ~0x10; // set switch as input
	GPIO_PORTF_DIR_R  |= 0x06; // set pin F2 as output

	GPIO_PORTF_AFSEL_R &= ~0x16; // disable alternate function for pin F
	GPIO_PORTF_DEN_R |= 0x16;  // digital enable 
	GPIO_PORTF_PCTL_R&= ~0x000F0FF0; // config pin as GPIO
	GPIO_PORTF_AMSEL_R = 0x16; // analog mode select - does NOT need a '~'
	GPIO_PORTF_PUR_R |= 0x10; // set the pullup resistor for this pin
	
	GPIO_PORTF_IS_R &= ~0x10; // define pF1 interrupt as edge sensitive
	GPIO_PORTF_IBE_R &= ~0x10; // disable interrupt trigger on both edges
	GPIO_PORTF_IEV_R &= ~0x10; // 
	
	GPIO_PORTF_IM_R |=0x10;	

	NVIC_EN0_R |= 1<<30; // enable interrupt 30 in NVIC
	NVIC_PRI7_R |= 0x00A00000; // priority 5
}

void PLL_init(unsigned long divisor){
	// Use RCC2
	SYSCTL_RCC2_R |= 0x80000000;
	// Bypass the PLL by setting BYPASS2 to be 1
	SYSCTL_RCC2_R |= 0x00000800;
	// The board has a 16Mhz crystal so we need to specify a XTAL of 0x15 (10101)
	SYSCTL_RCC_R = (SYSCTL_RCC_R &~ 0x000007C0) 	// clear the XTAL field first (clear bits 10 to 6)
												+ (0x15<<6);   // This line also sets the main osc as the clock source by setting the 16MHz XTAL
	
	// clear PWRDWN to enable the PLL
	SYSCTL_RCC2_R &= ~0x00002000;
	
	// Set the desired clock frequency
	SYSCTL_RCC2_R |= 	0x40000000;
	SYSCTL_RCC2_R  = (SYSCTL_RCC2_R & ~0x1FC00000) + ((divisor-1) << 22);
	
	// wait for PLL ready, when the clock settles, the RIS value will be 1 and this loop will end
	while((SYSCTL_RIS_R & 0x00000040) == 0){ }	
	// set the BYPASS2 to connect the PLL
	SYSCTL_RCC2_R &= ~ 0x00000800;
}

void  sysTickInit(unsigned long period){
	// set up the system timer
	ST_CTRL_R = 0;
	ST_RELOAD_R = period;
	ST_CURRENT_R = 0; // clear the count flag
	
	// clock source internal
	// interrupt enable - send interrupt to NVIC
	// enable
	NVIC_EN0_R |= 1<<15;
	ST_CTRL_R = 0x07;
	NVIC_SYSPRI3_R = (NVIC_SYSPRI3_R&0x00FFFFFF) | 0xC0000000;
}

void GPIOF_Handler(void){
GPIO_PORTF_ICR_R = 0x10;
	unsigned int i;
	unsigned int j;
	for (j=0;j<2;j++){
			PIN_F1 ^=0x02;
			i = 0x6FFFFF;
		while(i>0){
			i = i - 1;
		}
	}
	
FallingEdges = FallingEdges +1;
// clear the ICR register - see notes
}


void interruptInit(void){
__enable_irq();
}

void SysTick_Handler(void){
	PIN_F2 ^= 0x04; // toggle pin F2
	TickCount = TickCount + 1;
}

int main (void) {
	PLL_init(5); // divide the 400MHz PLL by 127 times to get the lowest possible freq 
	initPortF();
	sysTickInit(0xE4E1BF);
	interruptInit();
	while(1){
	}
}
