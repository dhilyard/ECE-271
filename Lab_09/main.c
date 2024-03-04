#include "stm32l476xx.h"
#include "SysClock.h"

// PA.5  <--> Green LED
// PC.13 <--> Blue user button
#define LED_PIN    5
#define BUTTON_PIN 13
#define PA_1 1

#define STEPPER_PIN_IN1 5
#define STEPPER_PIN_IN2 8
#define STEPPER_PIN_IN3 6
#define STEPPER_PIN_IN4 9

#define TURNDEGREES 360

#define VLow 1181
#define VHigh 2430


volatile int counter = 0;
volatile uint16_t ADC_results[4];
volatile int result=0;

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


void configure_LED_pin(){
  // Enable the clock to GPIO Port A	
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;   
	
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
		
	// GPIO Mode: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOA->MODER &= ~(3UL<<(2*LED_PIN));  
	GPIOA->MODER |=   1UL<<(2*LED_PIN);

	GPIOC->MODER &= ~(3UL<<(2*BUTTON_PIN));  
	
	GPIOC->MODER &= ~(3UL<<(2*STEPPER_PIN_IN1));  
	GPIOC->MODER |=   1UL<<(2*STEPPER_PIN_IN1);      // Output(01)

	GPIOC->MODER &= ~(3UL<<(2*STEPPER_PIN_IN2));  
	GPIOC->MODER |=   1UL<<(2*STEPPER_PIN_IN2);      // Output(01)
	
	GPIOC->MODER &= ~(3UL<<(2*STEPPER_PIN_IN3));  
	GPIOC->MODER |=   1UL<<(2*STEPPER_PIN_IN3);      // Output(01)
	
	GPIOC->MODER &= ~(3UL<<(2*STEPPER_PIN_IN4));  
	GPIOC->MODER |=   1UL<<(2*STEPPER_PIN_IN4);      // Output(01)

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
	
	GPIOC->PUPDR  &= ~(3<<(2*BUTTON_PIN));

	GPIOC->PUPDR  &= ~(3<<(2*STEPPER_PIN_IN1));
	
	GPIOC->PUPDR  &= ~(3<<(2*STEPPER_PIN_IN2));
	
	GPIOC->PUPDR  &= ~(3<<(2*STEPPER_PIN_IN3));
	
	GPIOC->PUPDR  &= ~(3<<(2*STEPPER_PIN_IN4));
	
	GPIOA->AFR[0] &= ~(3UL<<(4*LED_PIN));
	GPIOA->AFR[0] |= 1UL<<(4*LED_PIN);
}



void ADC1_Trigger(void){
	//Software start 1 ADC Conversion
	ADC1->CR |= ADC_CR_ADSTART;
	while((ADC123_COMMON->CSR & ADC_CSR_EOC_MST) == 0);
}


void ADCl_Wakeup (void) { 
	int wait_time; 
	// To start ADC operations, the following sequence should be applied 
	// DEEPPWD = e: ADC not in deep-power down 
	// DEEPPWD = 1: ADC in deep-power-down (default reset state) 
	if ((ADC1->CR & ADC_CR_DEEPPWD) == ADC_CR_DEEPPWD) { 
	} 
	// Exit deep power down mode if still in that state 
	ADC1->CR &= ~ADC_CR_DEEPPWD; 
	// Enable the ADC internal voltage regulator 
	// Before performing any operation such as Launching a calibration or 
	// enabling the ADC, the ADC voltage regulator must first be enabled and 
	// the software must wait for the regulator start-up time. 
	ADC1->CR |= ADC_CR_ADVREGEN; 
	// Wait for ADC voltage requlator start-up time. The software must wait for
	// the startup time of the ADC voltage regulator (T_ADCVREG_STUP, i.e., 20 
	// us) before Launching a calibration or enabling the ADC. 
	wait_time = 20 * (80000000 / 1000000); 
	while (wait_time != 0) { 
		wait_time--; 
	} 
} 

void ADC1_Init (void) {
	//Enable High Speed Clock
	RCC->CR |= RCC_CR_HSION;
	
	while((RCC->CR & RCC_CR_HSIRDY) == 0);
	
	
	//GPIOA clock is enabled in configure_LED_pin
	GPIOA->MODER &= ~(3UL<<(2*PA_1));
	GPIOA->MODER |=   3UL<<(2*PA_1);
	
	//Close Analog Switch
	GPIOA->ASCR &= ~(1<<PA_1);
	GPIOA->ASCR |= 1<<PA_1;
	
	//ADC1 Initalization
	
	//Enable ADC Clock
	RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;
	
	//Disable ADC1
	ADC1->CR &= ~ADC_CR_ADEN;
	
	//Enable I/O analog switches voltage booster, BOOSTEN
	SYSCFG->CFGR1 &= ~SYSCFG_CFGR1_BOOSTEN;
	SYSCFG->CFGR1 |= SYSCFG_CFGR1_BOOSTEN;
	
	//Enable conversion of internal channels, VREFEN
	ADC123_COMMON->CCR |= ADC_CCR_VREFEN;
	
	//Configure Prescaler to select ADC clock frequency, select clock not divided by clearing bits, 
	ADC123_COMMON->CCR &= ~ADC_CCR_PRESC;
	
	//Select Synchronous Clock Mode for HCLK/1
	ADC123_COMMON->CCR &= ~ADC_CCR_CKMODE;
	ADC123_COMMON->CCR |= ADC_CCR_CKMODE;
	
	//Configure all ADCs as independent
	ADC123_COMMON->CCR &= ~ADC_CCR_DUAL;
	
	//ADC is off by default so it must be woken up
	ADCl_Wakeup();
	
	//Configure resolution of ADC
	ADC1->CFGR &= ~ADC_CFGR_RES;
	
	//Select Right Alignment
	ADC1->CFGR &= ~ADC_CFGR_ALIGN;
	
	//Select 1 Conversion in the regular channel conversion sequence
	ADC1->SQR1 &= ~ADC_SQR1_L;
	
	//Specify channel 6 as 1st conversion in regular sequence
	ADC1->SQR1 &= ~ADC_SQR1_SQ1;
	ADC1->SQR1 |= 6<<6;
	
	//Configure Channel 6 as single ended
	ADC1->DIFSEL &= ~ADC_DIFSEL_DIFSEL_6;
	
	//Select ADC sample time, setting it to 1
	ADC1->SMPR1 &= ~ADC_SMPR1_SMP6;
	ADC1->SMPR1 |= ADC_SMPR1_SMP6;
	
	//Set ADC to discontinuous mode.
	ADC1->CFGR &= ~ADC_CFGR_CONT;
	
	//Select Software Trigger
	ADC1->CFGR &= ~ADC_CFGR_EXTEN;
	
	//Enable ADC1
	ADC1->CR |= ADC_CR_ADEN;
	
	while((ADC1->ISR & ADC_ISR_ADRDY) == 0);
}




void ADCValue(void){
		ADC1_Trigger();
		if ((ADC1->ISR & ADC_ISR_EOC) == ADC_ISR_EOC) { 
			if(counter % 4 == 0) { 
			counter = 0; // reset counter 
			}
			result = ADC1->DR;
			ADC_results[counter] = ADC1->DR; 
			counter++;
			ADC1->ISR |= ADC_ISR_EOS;
		} else if ((ADC1->ISR & ADC_ISR_JEOS) == ADC_ISR_JEOS) { 
				// For injected channels, check End of Sequence (JEOS) fLag 
				// Reading injected data registers does not clear the JEOS flag 
				// Each injected channel has a dedicated data register 
				ADC_results[0] = ADC1->JDR1; // Injected channel 1 
				ADC_results[1] = ADC1->JDR2; // Injected channel 2 
				ADC_results[2] = ADC1->JDR3; // Injected channel 3 
				ADC_results[3] = ADC1->JDR4; // Injected channeL 4 
				ADC1->ISR |= ADC_ISR_EOS; // Clear the flab by writing 1 to it
		}
		if(result >= VHigh){
			GPIOA->ODR &= ~(1<<LED_PIN);
			GPIOA->ODR |= 1<<LED_PIN;
		} else if(result <= VLow){
			GPIOA->ODR &= ~(1<<LED_PIN);
		}
}

void Delay(uint32_t nTime) {
	ADCValue();
	//nTime is the delay length
	TimeDelay = sqrt(nTime+(result/64));
	while(TimeDelay != 0);
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
	ADCValue();
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

	System_Clock_Init();

	configure_LED_pin();
	
	SysTick_Initalize(7999);
	
	ADC1_Init();

	while(1){
		Delay(500);
		halfStep();
		Delay(500);
		halfStepCCW();
	}
	while(1);
}