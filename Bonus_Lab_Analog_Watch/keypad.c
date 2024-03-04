#include "stm32l476xx.h"
#include "SysClock.h"
#include "keypad.h"

//Keypad pin defines

void configure_KEYPAD_PINS(){
	//Enable GPIO C Clock
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	
	//Set R1-R4 as digital output
	GPIOC->MODER &= ~(3UL<<(2*KEYPAD_R1));
	GPIOC->MODER |=   1UL<<(2*KEYPAD_R1);
	
	GPIOC->MODER &= ~(3UL<<(2*KEYPAD_R2));
	GPIOC->MODER |=   1UL<<(2*KEYPAD_R2);
	
	GPIOC->MODER &= ~(3UL<<(2*KEYPAD_R3));
	GPIOC->MODER |=   1UL<<(2*KEYPAD_R3);
	
	GPIOC->MODER &= ~(3UL<<(2*KEYPAD_R4));
	GPIOC->MODER |=   1UL<<(2*KEYPAD_R4);
	
	//Set C1-C4 as digital inputs, only need to mask since input is 00.
	GPIOC->MODER &= ~(3UL<<(2*KEYPAD_C1));
	
	GPIOC->MODER &= ~(3UL<<(2*KEYPAD_C2));
	
	GPIOC->MODER &= ~(3UL<<(2*KEYPAD_C3));
	
	GPIOC->MODER &= ~(3UL<<(2*KEYPAD_C4));
}