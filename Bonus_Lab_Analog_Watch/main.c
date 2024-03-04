#include <string.h>
#include <stdio.h>
#include <math.h>
#include "stm32l476xx.h"
#include "SysClock.h"
#include "I2C.h"
#include "ssd1306.h"
#include "ssd1306_tests.h"

// Note: The code gets Warning L6989W
// Report: this is an known issue for STM32L4xx.
// https://www.keil.com/support/docs/3853.htm
// We can ignore this warning because we are not using
// external memory.

uint8_t Data_Receive[6];
uint8_t Data_Send[6];

#define Pi 	3.14159265358979323846


void I2C_GPIO_init(void);

void LCD_RTC_Clock_Enable(void)
{
	// Enable write access to the backup domain
	if ((RCC->APB1ENR1 & RCC_APB1ENR1_PWREN) == 0) {
		// Enable power interface clock
		RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN;
		// Short delay after enabling an RCC peripheral clock
		(void) RCC->APB1ENR1;
	}
	
	// Select LSE as RTC clock source
	// RTCILCD Clock: (1) LSE is in the backup domain (2) HSE and LSI are not
	if ((PWR->CR1 & PWR_CR1_DBP) == 0) {
		// Enable write access to RTC and registers in backup domain
		PWR->CR1 |= PWR_CR1_DBP;
	
		// Wait until the backup domain write protection has been disabled
		while((PWR->CR1 & PWR_CR1_DBP) == 0);
	}
	
	// Reset LSEON and LSEBYP bits before configuring LSE
	// BDCR = Backup Domain Control Register
	RCC->BDCR &= ~(RCC_BDCR_LSEON | RCC_BDCR_LSEBYP);
	
	// RTC Clock selection can be changed only if the backup domain is reset
	RCC->BDCR |= RCC_BDCR_BDRST;
	RCC->BDCR &= ~RCC_BDCR_BDRST;
	
	// Wait untill LSE clock is ready
	while((RCC->BDCR & RCC_BDCR_LSERDY) == 0) {
		RCC->BDCR |= RCC_BDCR_LSEON; // Enable LSE oscillator
	}
	
	// Select LSE as RTC clock source
	// RTCSEL[l:B]: ee = No Clock, Bl = LSE, 1e = LSI, 11 = HSE
	RCC->BDCR &= ~RCC_BDCR_RTCSEL; // Clear RTCSEL bits
	RCC->BDCR |= RCC_BDCR_RTCSEL_0; // Select LSE as RTC clock
	
	// Disable power interface clock
	RCC->APB1ENR1 &= ~RCC_APB1ENR1_PWREN;
	
	// Enable RTC Clock
	RCC->BDCR |= RCC_BDCR_RTCEN;
}

void RTC_Init(void)
{
	// Enable RTC Clock
	LCD_RTC_Clock_Enable();
	
	// Disable write protection of RTC registers
	RTC->WPR = 0xCA;
	RTC->WPR = 0x53;
	
	// Enter initialization mode to program TR and DR registers
	RTC->ISR |= RTC_ISR_INIT;
	
	// Wait until INITF has been set
	while((RTC->ISR & RTC_ISR_INITF) == 0);
	
	// Hour format: 0 = 24 hour/day; 1 = AM/PM hour
	RTC->CR &= ~RTC_CR_FMT;
	
	// Generate a lHz clock for the RTC time counter
	// LSE = 32.768 kHz= 2^15 Hz
	RTC->PRER |= (127<<16); // Asynch_Prescaler = 127
	RTC->PRER |= 255; // Synch_Prescaler = 255
	
	// Set time as 11:32:00 am
	RTC->TR = 0U<<22 | 1U<<20 | 1U<<16 | 3U<<12 | 2U<<8;
	
	// Set date as 2023/04/19
	RTC->DR = 2U<<20 | 3U<<16 | 0U<<12 | 4U<<8 | 1U<<4 | 9U;
	
	// Exit initialization mode
	RTC->ISR &= ~RTC_ISR_INIT;
	
	// Enable write protection for RTC registers
	RTC->WPR = 0xFF;
}

void backgroundDraw(void){
		ssd1306_Fill(White);
		//Clock Shape
		ssd1306_DrawCircle(64,32,32,Black); //Screen is 128x64, (Midpoint X, Midpoint Y, Radius, Draw Color)
		//Numbers on Clock Face, Subtract ~2 per character from the x value of Set Cursor to align
		
		//Clock Face 3
		ssd1306_SetCursor(90,30);
		ssd1306_WriteString("3", Font_6x8, Black);
		//Clock Face 6
		ssd1306_SetCursor(62,54);
		ssd1306_WriteString("6", Font_6x8, Black);
		//Clock Face 9
		ssd1306_SetCursor(34,30);
		ssd1306_WriteString("9", Font_6x8, Black);
		//Clock Face 12
		ssd1306_SetCursor(60,4);
		ssd1306_WriteString("12", Font_6x8, Black);
	
}

volatile int secondsOnes;
volatile int secondsTens;
volatile int seconds;

volatile int minutesOnes;
volatile int minutesTens;
volatile int minutes;

volatile int hoursOnes;
volatile int hoursTens;
volatile int hours;

void secondHand(void){
	//Collect the ones and tens values then combine them into a single decimal number
	secondsOnes = ((RTC->TR)&0x00000F);
	secondsTens = ((RTC->TR>>4)&0x0000F);
	seconds = (secondsTens*10)+secondsOnes;
	
	//60 Seconds means 6 degrees per second, the sin and cos functions work in radians so they must be converted over.
	int secondX = 64 + 32*cos(seconds*(Pi/30)-Pi/2);
	int secondY = 32 + 32*sin(seconds*(Pi/30)-Pi/2);
	
	ssd1306_Line(64,32,secondX,secondY,Black); //(Xstart, Ystart, Xend, Yend, Draw Color)
}

void minuteHand(void){
	//Collect the ones and tens values then combine them into a single decimal number
	minutesOnes = ((RTC->TR>>8)&0x000F);
	minutesTens = ((RTC->TR>>12)&0x00F);
	minutes = (minutesTens*10)+minutesOnes;

	int minuteX = 64 + 24*cos(minutes*(Pi/30)-(Pi/2));
	int minuteY = 32 + 24*sin(minutes*(Pi/30)-(Pi/2));
	
	ssd1306_Line(64,32,minuteX,minuteY,Black); //(Xstart, Ystart, Xend, Yend, Draw Color)
}

void hourHand(void){
	//Collect the ones and tens values then combine them into a single decimal number
	hoursOnes = ((RTC->TR>>16)&0x0F);
	hoursTens = ((RTC->TR>>20)&0xF);
	hours = (hoursTens*10)+hoursOnes;
	
	//12 hours means 30 degrees per hour.
	int hourX = 64 + 16*cos(hours*(Pi/6)-(Pi/2));
	int hourY = 32 + 16*sin(hours*(Pi/6)-(Pi/2));
	
	ssd1306_Line(64,32,hourX,hourY,Black); //(Xstart, Ystart, Xend, Yend, Draw Color)
}

void digitalSideCLK(void){
		char second[64] = "";
		sprintf(second, "%d%d", (RTC->TR >> 4) & 0x0000F,(RTC->TR >> 0) & 0x00000F);
		char minute[64] = "";
		sprintf(minute, "%d%d", (RTC->TR >> 12) & 0x00F,(RTC->TR >> 8) & 0x000F);
		char hour[64] = "";
		sprintf(hour, "%d%d", (RTC->TR >> 20),(RTC->TR >> 16) & 0x0F);
		
		//Digital Clock for comparison
		ssd1306_SetCursor(2,0);
		ssd1306_WriteString(hour, Font_11x18, Black);
		ssd1306_SetCursor(2,18);
		ssd1306_WriteString(minute, Font_11x18, Black);
		ssd1306_SetCursor(2,36);
		ssd1306_WriteString(second, Font_11x18, Black);
}

	
int main(void){
	System_Clock_Init(); // Switch System Clock = 80 MHz
	I2C_GPIO_init();
	I2C_Initialization(I2C1);
	ssd1306_Init();
	RTC_Init();
	
	while(1){
		//RTC Reference Material
		//char time[64] = "";
		//char date[64] = "";
		//sprintf(time, "%d%d:%d%d:%d%d", (RTC->TR >> 20),(RTC->TR >> 16) & 0x0F,(RTC->TR >> 12) & 0x00F,(RTC->TR >> 8) & 0x000F,(RTC->TR >> 4) & 0x0000F,(RTC->TR >> 0) & 0x00000F);
		//sprintf(date, "%d%d/%d%d/20%d%d", (RTC->DR >> 12) & 0x00F,(RTC->DR >> 8) & 0x000F,(RTC->DR >> 4) & 0x0000F,(RTC->DR >> 0) & 0x00000F, (RTC->DR >> 20),(RTC->DR >> 16) & 0x0F);
		
		backgroundDraw();
		
		secondHand();
		minuteHand();
		hourHand();
		digitalSideCLK();

		ssd1306_UpdateScreen();	
	}
	
	while(1);	 // Deadloop
}
