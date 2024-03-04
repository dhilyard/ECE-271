#include "stm32l476xx.h"
#include "SysClock.h"

// PA.5  <--> Green LED
// PC.13 <--> Blue user button
#define LED_PIN    5
#define BUTTON_PIN 13

void EXTI_Init(void) {
	
	//Enable SYSCFG clock
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	
	// Select PC.13 as the trigger source of EXTI 13
	SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13;
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC; //Sets to 1 to select PC13
	SYSCFG->EXTICR[3] &= ~(0x000F);

	//Disable rising edge trigger
	EXTI->RTSR1 &= ~EXTI_RTSR1_RT13;
	
	//Enable falling edge trigger
	EXTI->FTSR1 |= EXTI_FTSR1_FT13;
	
	//Enable EXTI13 Interrupt
	EXTI->IMR1 |= EXTI_IMR1_IM13;
	
	//Set EXTI 13 priority to 1
	NVIC_SetPriority(EXTI15_10_IRQn, 1);
	
	//Enable EXTI 13 interrupt
	NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void configure_LED_pin(){
  // Enable the clock to GPIO Port A	
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;   
	
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
		
	// GPIO Mode: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOA->MODER &= ~(3UL<<(2*LED_PIN));  
	GPIOA->MODER |=   2UL<<(2*LED_PIN);

	GPIOC->MODER &= ~(3UL<<(2*BUTTON_PIN));  

	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
	GPIOA->OSPEEDR &= ~(3<<(2*LED_PIN));
	GPIOA->OSPEEDR |=   2<<(2*LED_PIN);  // Fast speed
	
	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1) 
	GPIOA->OTYPER &= ~(1<<LED_PIN);      // Push-pull
	
	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOA->PUPDR  &= ~(3<<(2*LED_PIN));  // No pull-up, no pull-down
	
	GPIOC->PUPDR  &= ~(3<<(2*BUTTON_PIN));
	
	GPIOA->AFR[0] &= ~(3UL<<(4*LED_PIN));
	GPIOA->AFR[0] |= 1UL<<(4*LED_PIN);
}

void configure_SRVO_pin(){ 
		
	// GPIO Mode: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOA->MODER &= ~(3UL);  
	GPIOA->MODER |=   2UL;

	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
	GPIOA->OSPEEDR &= ~3;
	GPIOA->OSPEEDR |=  2;  // Fast speed
	
	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1) 
	GPIOA->OTYPER &= ~1;      // Push-pull
	
	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOA->PUPDR  &= ~3;  // No pull-up, no pull-down
	
	GPIOA->AFR[0] &= ~3;
	GPIOA->AFR[0] |= 2;
	
	
}

void TIM2_Init(){
	//Enable Timer 2 Clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
	
	//Counting direction: 0 = Up-counting, 1 = Down-counting
	TIM2->CR1 &= ~TIM_CR1_DIR;
	
	//Prescaler, slows down input clock
	TIM2->PSC = 39; // 4MHz / (1+39) = 100 KHz
	
	//Auto-reload
	TIM2->ARR = 1999;
	
	//Clear output compare mode bits for channel 1
	TIM2->CCMR1 &= ~TIM_CCMR1_OC1M;
	
	//Select PWM Mode 1 output on channel 1 (OC1M = 110)
	TIM2->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
	
	//Output 1 preload enable
	TIM2->CCMR1 |= TIM_CCMR1_OC1PE;
	
	//Select output polarity: 0 = active high, 1 = active low
	TIM2->CCER &= ~TIM_CCER_CC1NP;
	
	//Enable complementary output of channel 1
	TIM2->CCER |= TIM_CCER_CC1E;
	
	//Initial Duty Cycle 
	TIM2->CCR1 = 500;
	
	//Enable Counter
	TIM2->CR1 |= TIM_CR1_CEN;
}

void TIM5_Init(){
	// Enable TIMER 5 clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM5EN;
	
	// Counting direction: 0 = Up-counting, 1 = Down-counting
	TIM5->CR1 &= ~TIM_CR1_DIR; // Select up-counting
	
	// Prescaler, slow down the input clock by a factor of (1 + prescaler)
	TIM5->PSC = 39; // 4 MHz | (1 + 39) = 100 KHz
	
	// Auto-reload
	TIM5->ARR = 1999; // PWM period = (1999 + 1) * 1/100kHz = 0.02s
	
	// Clear output compare mode bits for channel 1
	TIM5->CCMR1 &= ~TIM_CCMR1_OC1M;
	
	// Select PWM Mode 1 output on channel 1 (OC1M = 110)
	TIM5->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
	
	// Output 1 preload enable
	TIM5->CCMR1 |= TIM_CCMR1_OC1PE;
	
	// Select output polarity: 0 = Active high, 1 = Active Low
	TIM5->CCER &= ~TIM_CCER_CC1NP; // OC1N = OCREF + CC1NP
	
	// Enable complementary output of channel 1 (CH1N)
	TIM5->CCER |= TIM_CCER_CC1E;
}


void srvo_Control(){
	int i;
	
	TIM5->CCR1 = 159;
	
	TIM5->CR1 |= TIM_CR1_CEN; //0 Degrees
	
	for(i=0; i<500000; i++);
	
	TIM5->CR1 |= TIM_CR1_CEN; //90 Degrees
	
	TIM5->CCR1 = 246;
	
	// Enable counter
	TIM5->CR1 |= TIM_CR1_CEN; // 90 degrees
	
	for(i = 0; i < 500000; i++);
	
	// Disable counter
	TIM5->CR1 &= ~TIM_CR1_CEN;
	
	// Output Compare Register for channel 1
	TIM5->CCR1 = 62;
	
	// Enable counter
	TIM5->CR1 |= TIM_CR1_CEN; // -90 degrees
	
	for(i = 0; i < 500000; i++);
	
	// Disable counter
	TIM5->CR1 &= ~TIM_CR1_CEN;
	
	// Output Compare Register for channel 1
	TIM5->CCR1 = 159;
	
	// Enable counter
	TIM5->CR1 |= TIM_CR1_CEN; // 0 degrees

}

void EXTI15_10_IRQHandler(void){
	int i;
	//Check for EXTI 13 Interrupt Flag
	if ((EXTI->PR1 & EXTI_PR1_PIF13) == EXTI_PR1_PIF13) {
		if(TIM5->CCR1 == 62){
			TIM5->CCR1 = 159;
			
			TIM5->CR1 |= TIM_CR1_CEN; //0 Degrees
			
			for(i=0; i<500000; i++);
			
			TIM5->CR1 |= TIM_CR1_CEN; //90 Degrees
		}else if(TIM5->CCR1==159){
			TIM5->CCR1 = 246;
			
			// Enable counter
			TIM5->CR1 |= TIM_CR1_CEN; // 90 degrees
			
			for(i = 0; i < 500000; i++);
			
			// Disable counter
			TIM5->CR1 &= ~TIM_CR1_CEN;
		}else if(TIM5->CCR1==246){
			TIM5->CCR1 = 62;
			
			// Enable counter
			TIM5->CR1 |= TIM_CR1_CEN; // -90 degrees
			
			for(i = 0; i < 500000; i++);
			
			// Disable counter
			TIM5->CR1 &= ~TIM_CR1_CEN;			
		}
	}
		EXTI->PR1 |= EXTI_PR1_PIF13; //Write 1 to clear
}

int main(void){

	//System_Clock_Init(); // Switch System Clock = 80 MHz
	EXTI_Init();
	configure_LED_pin();
	configure_SRVO_pin();
	TIM2_Init();
	TIM5_Init();
	
	srvo_Control();
	
	int i; 
	int brightness = 1; 
	int stepSize = 1; 
	
	
	while(1) { 
		if ((brightness >= 99) || (brightness <= 0 )) 
			stepSize = -stepSize; // Reverse direction 
		brightness += stepSize; // Change brightness 
		TIM2->CCR1 = brightness; // Change duty cycle of channel 1 outputs 
		for(i = 0; i < 1000; i++); // Short delay
	}
	
	while(1);
}
