// set up the relavent instructions, see handouts 
// 4002500 base address


#define SYSCTL_RCGC2_R (*((volatile unsigned long *) 0x400FE108))
// Base address for port F = 0x40025000
#define GPIO_PORTF_DATA_R (*((volatile unsigned long *) 0x400253FC))
#define GPIO_PORTF_DIR_R (*((volatile unsigned long *) 0x40025400))
#define GPIO_PORTF_AFSEL_R (*((volatile unsigned long *) 0x40025420))
#define GPIO_PORTF_DEN_R (*((volatile unsigned long *) 0x4002551C))
#define GPIO_PORTF_AMSEL_R (*((volatile unsigned long *) 0x40025528))
#define GPIO_PORTF_PCTL_R (*((volatile unsigned long *) 0x4002552C))
#define PIN_F1 (*((volatile unsigned long *) 0x40025008))

// A time variable to use below

void initPortF (){
	// function to set up the port
	unsigned long volatile delay;
	// Non destuctively set the bit to enable the clock for port F
	
	// Step 1: Set the bits to enable the clock for port F
	SYSCTL_RCGC2_R |= 0x20; // this will only set the bits we want, no other bits will be affected.
	delay = SYSCTL_RCGC2_R;
	
	// Enable functions ///
	
	//Step 2: enable pin F1 (Red LED)
	GPIO_PORTF_DEN_R |= 0x02;
	
	//Step 3: enable output 
	GPIO_PORTF_DIR_R |= 0x02;
	
	// Alternate function controls // 
	// Step 4: disable analog
	GPIO_PORTF_AMSEL_R &= ~0x02;
	// Step 5: disable alternate function
	GPIO_PORTF_AFSEL_R &= ~0x02;
	// Step 6: set bits to alternate function control register
	GPIO_PORTF_PCTL_R &= ~0x000000F0;

}


void Delay100ms(unsigned long time){
	unsigned long i;
	
	while (time > 0){
			i = 266667; // means 100ms
		while(i>0) {
			i = i - 1;
		}
		time = time -1;
	}
	}


	int main(void) {
		initPortF();
		/*
		This is a loop, we initially set the bit to 0 and then to 1
		*/
	while(1){
		// toggle the LED
		// delay for a specified time
		//GPIO_PORTF_DATA_R ^= 0x02;
		//
		//GPIO_PORTF_DATA_R |= 0x02;
		//Delay100ms(10);
		PIN_F1 = 0x02;
		Delay100ms(10);
		PIN_F1 = 	0x00;
		Delay100ms(10);
		
		
	}
	return 0;
	}
	