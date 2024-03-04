;******************** (C) Yifeng ZHU *******************************************
; @file    main.s
; @author  Yifeng Zhu
; @date    Jan-17-2023
;*******************************************************************************

	INCLUDE core_cm4_constants.s		; Load Constant Definitions
	INCLUDE stm32l476xx_constants.s      

; Green LED <--> PA.5
LED_PIN	EQU	5
BUTTON_PIN EQU 13
	
	AREA    main, CODE, READONLY
	EXPORT	__main				; make __main visible to linker
	ENTRY
	
delayOverhead PROC
	B delay
delay
	MOV r2, #0 ; i	
	MOV r3, #65535 ; .1 sec
	MOV r4, #1 ; 12 = 1 sec
	MOV r5, #0 ; LoopCounting Variable

loopInc ;Incrementing Loop for delay.
	CMP r2, r3 ; check r2 < 65535
	BGE counter ; Branch if r2 >= 65535
	ADD r2, r2, #1 ; r2++
	B loopInc
counter
	ADD r5, r5, #1 ; When 1 full count of 65535 happens add 1 to r5
	MOV r2, #0 ; r2 = 0
	CMP r5, r4 ; check r5 < r4 (Set delay value in earlier loop)
	BGE endloop ; Full Delay has completed
	B loopInc
endloop
	B reset ; reset to default pin values.

	ENDP
		
buttonControl PROC
	
btnPinSet ; Overarching Button loop with defines
	LDR r0, =GPIOC_BASE
	LDR r1, [r0, #GPIO_IDR]
	LDR r2, =GPIO_IDR_IDR_13
	
	B btnReP ;Button was released to pause
	
btnReP ;Button was released to pause
	LDR r0, =GPIOC_BASE
	LDR r1, [r0, #GPIO_IDR]
	LDR r2, =GPIO_IDR_IDR_13
	
	AND r3, r1, r2
	CMP r3, r2
	BNE btnReP
	B btnPa2

btnPrCN ; Button was pressed to continue
	LDR r0, =GPIOC_BASE
	LDR r1, [r0, #GPIO_IDR]
	LDR r2, =GPIO_IDR_IDR_13
	
	AND r3, r1, r2
	CMP r3, r2
	BEQ btnPrCN
	B btnRe2

btnReCN ; Button was released to continue
	LDR r0, =GPIOC_BASE
	LDR r1, [r0, #GPIO_IDR]
	LDR r2, =GPIO_IDR_IDR_13
	
	AND r3, r1, r2
	CMP r3, r2
	BNE btnReCN
	B while
	
btnPa2
	B btnPrCN ;Button pressed to continue
btnRe2
	B btnReCN ;Button released for continue
	
	ENDP


	
	
__main	PROC
		
    ; Enable the clock to GPIO Port A	
	LDR r0, =RCC_BASE
	LDR r1, [r0, #RCC_AHB2ENR]
	ORR r1, r1, #RCC_AHB2ENR_GPIOAEN
	STR r1, [r0, #RCC_AHB2ENR]

	; MODE: 00: Input mode, 01: General purpose output mode
    ;       10: Alternate function mode, 11: Analog mode (reset state)
	LDR r0, =GPIOA_BASE
	LDR r1, [r0, #GPIO_MODER]
	BIC r1, r1, #(3<<(2*LED_PIN))
	ORR r1, r1, #(1<<(2*LED_PIN))
	STR r1, [r0, #GPIO_MODER]
	
	; GPIO Speed: Low speed (00), Medium speed (01), 
	;				Fast speed (10), High speed (11)
	LDR r1, [r0, #GPIO_OTYPER]
	BIC r1, r1, #(3<<(2*LED_PIN))
	STR r1, [r0, #GPIO_OTYPER]
	
	; GPIO Output Type: Output push-pull (0, reset), Output open drain (1) 
	LDR r1, [r0, #GPIO_PUPDR]
	BIC r1, r1, #(3<<(2*LED_PIN))
	STR r1, [r0, #GPIO_PUPDR]
	
	; GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), 
	;				Pull-down (10), Reserved (11)
	LDR r1, [r0, #GPIO_ODR]
	ORR r1, r1, #(1<<LED_PIN)
	STR r1, [r0, #GPIO_ODR]
	
	; Enable the clock to GPIO Port C
	LDR r0, =RCC_BASE
	LDR r1, [r0, #RCC_AHB2ENR]
	ORR r1, r1, #RCC_AHB2ENR_GPIOCEN
	STR r1, [r0, #RCC_AHB2ENR]
	
	; MODE: 00: Input mode, 01: General purpose output mode
    ;       10: Alternate function mode, 11: Analog mode (reset state)
	LDR r0, =GPIOC_BASE
	LDR r1, [r0, #GPIO_MODER]
	BIC r1, r1, #(3<<(2*BUTTON_PIN))
	STR r1, [r0, #GPIO_MODER]
	
	; GPIO Output Type: Output push-pull (0, reset), Output open drain (1) 
	LDR r1, [r0, #GPIO_PUPDR]
	BIC r1, r1, #(3<<(2*BUTTON_PIN))
	STR r1, [r0, #GPIO_PUPDR]



while
	BL delayOverhead ;Delay Procedure
	
reset
	LDR r0, =GPIOA_BASE
	LDR r1, [r0, #GPIO_ODR]
	EOR r1, r1, #(1<<LED_PIN)
	STR r1, [r0, #GPIO_ODR]	
	
	LDR r0, =GPIOC_BASE
	LDR r1, [r0, #GPIO_IDR]
	LDR r2, =GPIO_IDR_IDR_13
	
	AND r3, r1, r2
	CMP r3, r2
	BNE btnPinSet
	
	B while
	
DEADLOOP B DEADLOOP     		; dead loop & program hangs here

	ENDP
					
	ALIGN			

	AREA    myData, DATA, READWRITE
	ALIGN
array	DCD   1, 2, 3, 4
	END
