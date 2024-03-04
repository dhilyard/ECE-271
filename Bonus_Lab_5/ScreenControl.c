#include <string.h>
#include <stdio.h>
#include "stm32l476xx.h"
#include "SysClock.h"
#include "I2C.h"
#include "ssd1306.h"
#include "ssd1306_tests.h"

volatile int buttonCounter=0;
char msgHold[64]="";
char message[64] = "";

void ScreenInitalize(void){
	System_Clock_Init();
	I2C_GPIO_init();
	I2C_Initialization(I2C1);

	ssd1306_Init();
	ssd1306_Fill(Black);
	ssd1306_SetCursor(2,0);
	ssd1306_WriteString(message, Font_11x18, White);		
	ssd1306_UpdateScreen();	

}

void messageWrite(void){
	//write buttonCounter to the screen
		ssd1306_Fill(Black);
		sprintf(message,"%i",buttonCounter);
		ssd1306_SetCursor(2,0);
		ssd1306_WriteString(message, Font_11x18, White);		
		ssd1306_UpdateScreen();	

}