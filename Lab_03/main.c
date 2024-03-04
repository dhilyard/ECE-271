#include <string.h>
#include <stdio.h>
#include "stm32l476xx.h"
#include "SysClock.h"
#include "I2C.h"
#include "ssd1306.h"
#include "ssd1306_tests.h"
#include "keypad.h"

// Note: The code gets Warning L6989W
// Report: this is an known issue for STM32L4xx.
// https://www.keil.com/support/docs/3853.htm
// We can ignore this warning because we are not using
// external memory.

uint8_t Data_Receive[6];
uint8_t Data_Send[6];


void I2C_GPIO_init(void);

//Array to assign keys with
unsigned char key_map [4][4] = {
		{'1','2','3','A'},
		{'4','5','6','B'},
		{'7','8','9','C'},
		{'*','0','#','D'},
	};

int iterate_rows(char message[64], uint8_t clmn){
	int row;
		//Set Column to correct pin. 
		switch(clmn){
			case 0:
					clmn=KEYPAD_C1;
					break;
			case 1:
					clmn=KEYPAD_C2;
					break;
			case 2:
					clmn=KEYPAD_C3;
					break;
			case 3:
					clmn=KEYPAD_C4;
					break;
		}
		//These all check if the respective row value is zero.
		//Row 1 check
		GPIOC->ODR&=~(1<<KEYPAD_R1);
		GPIOC->ODR|=~(1<<KEYPAD_R1);
		if(!(GPIOC->IDR&(1<<clmn))){
			row=0;
			return row;
		}
		//Row 2 check
		GPIOC->ODR&=~(1<<KEYPAD_R2);
		GPIOC->ODR|=~(1<<KEYPAD_R2);
		if(!(GPIOC->IDR&(1<<clmn))){
			row=1;
			return row;
		}
		//Row 3 check
		GPIOC->ODR&=~(1<<KEYPAD_R3);
		GPIOC->ODR|=~(1<<KEYPAD_R3);
		if(!(GPIOC->IDR&(1<<clmn))){
			row=2;
			return row;
		}
		//Row 4 check
		GPIOC->ODR&=~(1<<KEYPAD_R4);
		GPIOC->ODR|=~(1<<KEYPAD_R4);
		if(!(GPIOC->IDR&(1<<clmn))){
			row=3;
			return row;
		}
		//If nothing pressed
		return 0;
	}

void add_input (uint8_t *keyPressed,char msgHold[64],char message[64],char key){
	if(*keyPressed==0){
		if(key=='*'){//Delete Last Input
			msgHold[strlen(msgHold)-1]='\0';
			
		}else if(key=='#'){//Clear Screen
				msgHold[0]='\0';
		}else{//Adding all other inputs to message
			strncat(msgHold,&key,1);
		}
		//Update Screen
		sprintf(message,"%s",msgHold);
		ssd1306_SetCursor(2,0);
		ssd1306_WriteString(message, Font_11x18, White);		
		ssd1306_UpdateScreen();	
		*keyPressed = 1;
	}
}
	
void keypad_scan(char message[64],char msgHold[64], uint8_t *keyPressed){
	GPIOC->ODR = ~0xF;
	uint8_t row, clmn;
	uint32_t c1,c2,c3,c4;
	char key;
	
	delay(1);
	c1 = (GPIOC->IDR & 1<<KEYPAD_C1);
	c2 = (GPIOC->IDR & 1<<KEYPAD_C2);
	c3 = (GPIOC->IDR & 1<<KEYPAD_C3);
	c4 = (GPIOC->IDR & 1<<KEYPAD_C4);
	
	ssd1306_Fill(Black);
	
	//Checks for Column and then calls all the other functions so they work in tandom.
	if(c1==0){
		clmn=0;
		row=iterate_rows(message,clmn);
		key=key_map[row][clmn];
		add_input(keyPressed,msgHold,message,key);
	}else if(c2==0){
		clmn=1;
		row=iterate_rows(message,clmn);
		key=key_map[row][clmn];
		add_input(keyPressed,msgHold,message,key);
	}else if(c3==0){
		clmn=2;
		row=iterate_rows(message,clmn);
		key=key_map[row][clmn];
		add_input(keyPressed,msgHold,message,key);
	}else if(c4==0){
		clmn=3;
		row=iterate_rows(message,clmn);
		key=key_map[row][clmn];
		add_input(keyPressed,msgHold,message,key);
	}else{
		*keyPressed=0;
	}
};
	
int main(void){
	uint8_t keyPressed=0;
	unsigned char key;
	char message[64] = "";
	char msgHold[64]="";
	
	System_Clock_Init(); // Switch System Clock = 80 MHz
	I2C_GPIO_init();
	I2C_Initialization(I2C1);
	configure_KEYPAD_PINS();
	

	//ssd1306_TestAll();
	ssd1306_Init();
	ssd1306_Fill(Black);
	ssd1306_SetCursor(2,0);
	ssd1306_WriteString(message, Font_11x18, White);		
	ssd1306_UpdateScreen();	
	
	while(1){
		keypad_scan(message,msgHold,&keyPressed);
	}
	while(1);	 // Deadloop
}

