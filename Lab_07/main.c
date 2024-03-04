#include "stm32l476xx.h"
#include "SysClock.h"

// PA.5  <--> Green LED
// PC.13 <--> Blue user button
#define LED_PIN    5
#define BUTTON_PIN 13
#define STEPPER_PIN_IN1 5
#define STEPPER_PIN_IN2 8
#define STEPPER_PIN_IN3 6
#define STEPPER_PIN_IN4 9

//Degree*64/45 = repeats
#define TURNDEGREES 360

volatile int TimeDelay = 0;

void SysTick_Initalize(int ticks){
	//Disables SysTick Counter
	SysTick->CTRL = 0;
	
	//Set Reload Register Value
	SysTick->LOAD = ticks-1;
	
	//Set interrupt Priority of SysTick
	NVIC_SetPriority(SysTick_IRQn, (1<<__NVIC_PRIO_BITS)-1);
	
	//Reset Count
	SysTick->VAL = 0;
	
	//Select Internal Processor Clock
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
	
	//Enables SysTick exception request
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	
	//Enable SysTick Timer
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	
}



void SysTick_Handler(void){
	if(TimeDelay>0)
		TimeDelay--;
}

void Delay(uint32_t nTime) {
	//nTime is the delay length
	TimeDelay = nTime;
	while(TimeDelay != 0);
}

void configure_LED_pin(){
  // Enable the clock to GPIO Port A	
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	// Clock for Port C
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
		
	// GPIO Mode: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOA->MODER &= ~(3UL<<(2*LED_PIN));  
	GPIOA->MODER |=   1UL<<(2*LED_PIN);      // Output(01)

	GPIOC->MODER &= ~(3UL<<(2*STEPPER_PIN_IN1));  
	GPIOC->MODER |=   1UL<<(2*STEPPER_PIN_IN1);      // Output(01)

	GPIOC->MODER &= ~(3UL<<(2*STEPPER_PIN_IN2));  
	GPIOC->MODER |=   1UL<<(2*STEPPER_PIN_IN2);      // Output(01)
	
	GPIOC->MODER &= ~(3UL<<(2*STEPPER_PIN_IN3));  
	GPIOC->MODER |=   1UL<<(2*STEPPER_PIN_IN3);      // Output(01)
	
	GPIOC->MODER &= ~(3UL<<(2*STEPPER_PIN_IN4));  
	GPIOC->MODER |=   1UL<<(2*STEPPER_PIN_IN4);      // Output(01)
	
	GPIOC->MODER &= ~(3UL<<(2*BUTTON_PIN));  
	
	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
	GPIOA->OSPEEDR &= ~(3<<(2*LED_PIN));
	GPIOA->OSPEEDR |=   2<<(2*LED_PIN);  // Fast speed

	GPIOC->OSPEEDR &= ~(3<<(2*STEPPER_PIN_IN1));  
	GPIOC->OSPEEDR |=   2<<(2*STEPPER_PIN_IN1);

	GPIOC->OSPEEDR &= ~(3<<(2*STEPPER_PIN_IN2));  
	GPIOC->OSPEEDR |=   2<<(2*STEPPER_PIN_IN2);
	
	GPIOC->OSPEEDR &= ~(3<<(2*STEPPER_PIN_IN3));  
	GPIOC->OSPEEDR |=   2<<(2*STEPPER_PIN_IN3);
	
	GPIOC->OSPEEDR &= ~(3<<(2*STEPPER_PIN_IN4));  
	GPIOC->OSPEEDR |=   2<<(2*STEPPER_PIN_IN4);
	
	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1) 
	GPIOA->OTYPER &= ~(1<<LED_PIN);      // Push-pull
	
	GPIOC->OTYPER &= ~(1<<STEPPER_PIN_IN1);      // Push-pull
	
	GPIOC->OTYPER &= ~(1<<STEPPER_PIN_IN2);
	
	GPIOC->OTYPER &= ~(1<<STEPPER_PIN_IN3);
	
	GPIOC->OTYPER &= ~(1<<STEPPER_PIN_IN4);
	
	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOA->PUPDR  &= ~(3<<(2*LED_PIN));  // No pull-up, no pull-down
	
	GPIOC->PUPDR  &= ~(3<<(2*STEPPER_PIN_IN1));
	
	GPIOC->PUPDR  &= ~(3<<(2*STEPPER_PIN_IN2));
	
	GPIOC->PUPDR  &= ~(3<<(2*STEPPER_PIN_IN3));
	
	GPIOC->PUPDR  &= ~(3<<(2*STEPPER_PIN_IN4));
	
	GPIOC->PUPDR  &= ~(3<<(2*BUTTON_PIN));
}

void fullStep(){
	int repeats = ((TURNDEGREES*64)/45);
	int i;
	for(i = 0; i<repeats; i++){

		//IN1 is A, IN2 is B, IN3 is !A, IN4 is !B
		//Step 1
		GPIOC->ODR |= 1 << STEPPER_PIN_IN1;
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN2);
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN3);
		GPIOC->ODR |= 1 << STEPPER_PIN_IN4;
		
		
		Delay(2);
		//Step 2
		GPIOC->ODR |= 1 << STEPPER_PIN_IN1;
		GPIOC->ODR |= 1 << STEPPER_PIN_IN2;
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN3);
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN4);
		
		Delay(2);
		//Step 3
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN1);
		GPIOC->ODR |= 1 << STEPPER_PIN_IN2;
		GPIOC->ODR |= 1 << STEPPER_PIN_IN3;
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN4);
		
		Delay(2);
		//Step 4
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN1);
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN2);
		GPIOC->ODR |= 1 << STEPPER_PIN_IN3;
		GPIOC->ODR |= 1 << STEPPER_PIN_IN4;
		
		Delay(2);
	}
}

void fullStepCCW(){
	int repeats = ((TURNDEGREES*64)/45);
	int i;
	for(i = 0; i<repeats; i++){

		//IN1 is A, IN2 is B, IN3 is !A, IN4 is !B
		//Step 1
		GPIOC->ODR |= 1 << STEPPER_PIN_IN1;
		GPIOC->ODR |= 1 << STEPPER_PIN_IN2;
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN3);
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN4);
		
		
		Delay(2);
		//Step 2
		GPIOC->ODR |= 1 << STEPPER_PIN_IN1;
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN2);
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN3);
		GPIOC->ODR |= 1 << STEPPER_PIN_IN4;
		
		Delay(2);
		//Step 3
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN1);
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN2);
		GPIOC->ODR |= 1 << STEPPER_PIN_IN3;
		GPIOC->ODR |= 1 << STEPPER_PIN_IN4;
		
		Delay(2);
		//Step 4
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN1);
		GPIOC->ODR |= 1 << STEPPER_PIN_IN4;
		GPIOC->ODR |= 1 << STEPPER_PIN_IN3;
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN4);
		
		Delay(2);
	}
	
	
}

void halfStep(){
	int repeats = ((TURNDEGREES*64)/45);
	int i;
	for(i = 0; i<repeats; i++){

		//IN1 is A, IN2 is B, IN3 is !A, IN4 is !B
		//Step 1
		GPIOC->ODR |= 1 << STEPPER_PIN_IN1;
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN2);
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN3);
		GPIOC->ODR |= 1 << STEPPER_PIN_IN4;
		
		Delay(1);
		//Step 2
		GPIOC->ODR |= 1 << STEPPER_PIN_IN1;
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN2);
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN3);
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN4);
		
		Delay(1);
		//Step 3
		GPIOC->ODR |= 1 << STEPPER_PIN_IN1;
		GPIOC->ODR |= 1 << STEPPER_PIN_IN2;
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN3);
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN4);
		
		Delay(1);
		//Step 4
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN1);
		GPIOC->ODR |= 1 << STEPPER_PIN_IN2;
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN3);
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN4);
		
		Delay(1);
		//Step 5
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN1);
		GPIOC->ODR |= 1 << STEPPER_PIN_IN2;
		GPIOC->ODR |= 1 << STEPPER_PIN_IN3;
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN4);
		
		Delay(1);
		//Step 6
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN1);
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN2);
		GPIOC->ODR |= 1 << STEPPER_PIN_IN3;
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN4);
		
		Delay(1);
		//Step 7
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN1);
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN2);
		GPIOC->ODR |= 1 << STEPPER_PIN_IN3;
		GPIOC->ODR |= 1 << STEPPER_PIN_IN4;
		
		Delay(1);
		//Step 8
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN1);
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN2);
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN3);
		GPIOC->ODR |= 1 << STEPPER_PIN_IN4;
		
		Delay(1);
	}
}

void halfStepCCW(){
	int repeats = ((TURNDEGREES*64)/45);
	int i;
	for(i = 0; i<repeats; i++){

		//IN1 is A, IN2 is B, IN3 is !A, IN4 is !B
		//Step 1
		GPIOC->ODR |= 1 << STEPPER_PIN_IN1;
		GPIOC->ODR |= 1 << STEPPER_PIN_IN2;
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN3);
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN4);
		
		Delay(1);
		//Step 2
		GPIOC->ODR |= 1 << STEPPER_PIN_IN1;
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN2);
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN3);
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN4);
		
		Delay(1);
		//Step 3
		GPIOC->ODR |= 1 << STEPPER_PIN_IN1;
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN2);
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN3);
		GPIOC->ODR |= 1 << STEPPER_PIN_IN4;
		
		Delay(1);
		//Step 4
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN1);
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN2);
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN3);
		GPIOC->ODR |= 1 << STEPPER_PIN_IN4;
		
		Delay(1);
		//Step 5
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN1);
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN2);
		GPIOC->ODR |= 1 << STEPPER_PIN_IN3;
		GPIOC->ODR |= 1 << STEPPER_PIN_IN4;
		
		Delay(1);
		//Step 6
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN1);
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN2);
		GPIOC->ODR |= 1 << STEPPER_PIN_IN3;
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN4);
		
		Delay(1);
		//Step 7
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN1);
		GPIOC->ODR |= 1 << STEPPER_PIN_IN2;
		GPIOC->ODR |= 1 << STEPPER_PIN_IN3;
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN4);
		
		Delay(1);
		//Step 8
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN1);
		GPIOC->ODR |= 1 << STEPPER_PIN_IN2;
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN3);
		GPIOC->ODR &= ~(1<<STEPPER_PIN_IN4);
		
		Delay(1);
	}
	
}

int main(void){
	System_Clock_Init(); // Switch System Clock = 80 MHz
	configure_LED_pin();
	SysTick_Initalize(7999);

	while(1){
		Delay(1000);
		GPIOA->ODR ^= 1<<LED_PIN;
		if((GPIOA->ODR)==0){
			//fullStepCCW();
			halfStepCCW();
		}else{
			//fullStep();
			halfStep();
		}
		
		
	}
	while(1);   // Dead loop & program hangs here
}
