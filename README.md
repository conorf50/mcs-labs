## Microcomputer Software - Labs + Exercises

Please refer to the TM4C123 datasheet available at [TI's website](http://www.ti.com/lit/ds/symlink/tm4c123gh6pm.pdf).
This details the registers referenced throughout the labs.

These code examples were the labs for the module 'Microcomputer Software' which focused on low-level programming of a Texas Instruments Launchpad with the Keil uVision IDE. The code examples are provided as uVision projects so if you are not using this IDE, just use the ```main.c``` file in each folder.
Each lab is a continuation of the previous lab.

More info on the board:
 https://eu.mouser.com/new/Texas-Instruments/ti-tm4c123gxl-eval-board/ 
 
The labs were as follows:

### Lab 1: Intro to using Git 
This was a basic intro to Git. The aim of the lab was to partner up with a classmate, create folders called ```git_mine``` and ```git_partner``` and commit/push them to Gitlab.

Brief outline:
1. Download, install and configure Git for Windows. 
2. Set up an account on Gitlab and create a repo.
3. Add this repo as a remote to a local repo.
4. Make a series of commits, noting what happens along the way.
5. Clone partner's repo, edit their files and push to their repo.

The files were lost, hence they are not provided.

### Lab 2: Keil and GPIO
This lab focused on setting up the drivers and hardware to get started with Cortex M4 and Keil uVision.

Background Theory Covered:
- IDE Basics
- GPIO Operation
- GPIO Registers
- Register Addresses


### Lab 3: GPIO Interrupts
This lab focused on handling interrupts using the NVIC (Nested Vectored Interrupt Controller)

Background Theory:
- ARM NVIC
- Global Interrupt Registers
- GPIO Interrupt Registers
- Register Addresses

### Lab 4: SysTick and Nested Interrupts
**Note** the ```lab4v2``` folder was a correction exercise during the labs to fix an intentional error left by the lecturer. 

Theory:
* Nested Interrupts
* SysTick
* SysTick Interrupt Registers



### Lab 5: Phase Locked Loops

This lab focused on controlling the clock speed using the PLL registers. Calculations were done to determine clock speec and setting bits in the required registers was also performed.

Theory:
* PLL
* Calculating delays and register values
