#include "stm32l476xx.h"
#include "SysClock.h"

// PA.5  <--> Green LED
// PC.13 <--> Blue user button
#define LED_PIN    5
#define BUTTON_PIN 13

void configure_LED_pin(){
  // Enable the clock to GPIO Port A	
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

	// Enable the clock to GPIO Port C
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
		
	// GPIO Mode: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOA->MODER &= ~(3UL<<(2*LED_PIN));  
	GPIOA->MODER |=   1UL<<(2*LED_PIN);      // Output(01)
	
	//GPIO Mode: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOC->MODER &= ~(3UL<<(2*BUTTON_PIN));  
	
	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
	GPIOA->OSPEEDR &= ~(3<<(2*LED_PIN));
	GPIOA->OSPEEDR |=   2<<(2*LED_PIN);  // Fast speed
	
	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1) 
	GPIOA->OTYPER &= ~(1<<LED_PIN);      // Push-pull
	
	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOA->PUPDR  &= ~(3<<(2*LED_PIN));  // No pull-up, no pull-down
	
	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOC->PUPDR  &= ~(3<<(2*BUTTON_PIN));  // No pull-up, no pull-down
}

int main(void){

	System_Clock_Init(); // Switch System Clock = 80 MHz
	configure_LED_pin();
	
	// Turns on the LED.
	GPIOA->ODR |= 1 << LED_PIN;
	
	//Blinker is the counter for LED toggle timing
	uint32_t Blinker = 0;

  // Checks for Button Press
		while(GPIOC->IDR & GPIO_IDR_IDR_13);
	// Checks for when the Button isn't pushed
		while(!(GPIOC->IDR & GPIO_IDR_IDR_13));
	
	while(1){
		Blinker++;
		//Modulo to control the blinking speed of the LED_PIN. I settled on 2 Million as it seemed a good speed.
		if(Blinker%2000000==0){
			GPIOA->ODR ^= 1 << LED_PIN;
		}
		/*
		This if statement and the 3 while statements inside serve to catch the blue button being pressed and toggle the blinking of the LED.
		The if keeps the while statements from catching too much and the trio of whiles make it so nothing can slip past the detection, using less resulted in occasional slip ups. 
		*/
		if((GPIOC->IDR & GPIO_IDR_IDR_13)==0){
			while(!(GPIOC->IDR & GPIO_IDR_IDR_13));
			while(GPIOC->IDR & GPIO_IDR_IDR_13);
			while(!(GPIOC->IDR & GPIO_IDR_IDR_13));
			
		}
	}
}
