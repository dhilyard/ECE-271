#include "SysClock.h"

// ******************************************************************************************
// Switch the PLL source from MSI to HSI, and select the PLL as SYSCLK source.
// ******************************************************************************************
void System_Clock_Init(void){
	
	//This clears MSION which is the 1st bit of the RCC_CR Register
	RCC->CR &= ~1UL; 
	
	//MSIRGSEL Bit Set
	RCC->CR &= ~(1UL<<3);
	RCC->CR |= 1UL<<3;
	
	//MSIRange set to 0111
	RCC->CR &= ~(15UL<<4);
	RCC->CR |= 7UL<<4;
	
	
	//Reenable MSION
	RCC->CR |= (1);
	
	//Wait for MSIRDY
	while((RCC->CR & RCC_CR_MSIRDY)== 0);
}
