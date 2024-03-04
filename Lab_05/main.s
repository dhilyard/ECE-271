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
STEP_IN1 EQU 5
STEP_IN2 EQU 8
STEP_IN3 EQU 6
STEP_IN4 EQU 9
DEGREES EQU 360
	
	AREA    main, CODE, READONLY
	EXPORT	__main				; make __main visible to linker
	ENTRY
	
delayOverhead PROC
	MOV r4, #0 ; i	
	MOV r5, #2500 ; .1 sec
delay CMP r4, r5
	BGE endloop
	ADD r4, r4, #1
	B delay
endloop
	BX LR

	ENDP 
	LTORG
		
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
	LTORG

Stepper_Pin_Init PROC
	
	;Load GPIOC Base
	LDR r0, =GPIOC_BASE
	
	;In1 MODER
	LDR r1, [r0, #GPIO_MODER]
	BIC r1, r1, #(3<<(2*STEP_IN1))
	ORR r1, r1, #(1<<(2*STEP_IN1))
	STR r1, [r0, #GPIO_MODER]
	
	;In2 MODER
	LDR r1, [r0, #GPIO_MODER]
	BIC r1, r1, #(3<<(2*STEP_IN2))
	ORR r1, r1, #(1<<(2*STEP_IN2))
	STR r1, [r0, #GPIO_MODER]
	
	;In3 MODER
	LDR r1, [r0, #GPIO_MODER]
	BIC r1, r1, #(3<<(2*STEP_IN3))
	ORR r1, r1, #(1<<(2*STEP_IN3))
	STR r1, [r0, #GPIO_MODER]
	
	;In4 MODER
	LDR r1, [r0, #GPIO_MODER]
	BIC r1, r1, #(3<<(2*STEP_IN4))
	ORR r1, r1, #(1<<(2*STEP_IN4))
	STR r1, [r0, #GPIO_MODER]

	;In1 OSPEEDR
	LDR r1, [r0, #GPIO_OSPEEDR]
	BIC r1, r1, #(3<<(2*STEP_IN1))
	ORR r1, r1, #(2<<(2*STEP_IN1))
	STR r1, [r0, #GPIO_OSPEEDR]
	
	;In2 OSPEEDR
	LDR r1, [r0, #GPIO_OSPEEDR]
	BIC r1, r1, #(3<<(2*STEP_IN2))
	ORR r1, r1, #(2<<(2*STEP_IN2))
	STR r1, [r0, #GPIO_OSPEEDR]
	
	;In3 OSPEEDR
	LDR r1, [r0, #GPIO_OSPEEDR]
	BIC r1, r1, #(3<<(2*STEP_IN3))
	ORR r1, r1, #(2<<(2*STEP_IN3))
	STR r1, [r0, #GPIO_OSPEEDR]
	
	;In4 OSPEEDR
	LDR r1, [r0, #GPIO_OSPEEDR]
	BIC r1, r1, #(3<<(2*STEP_IN4))
	ORR r1, r1, #(2<<(2*STEP_IN4))
	STR r1, [r0, #GPIO_OSPEEDR]
	
	;In1 OTYPER
	LDR r1, [r0, #GPIO_OTYPER]
	BIC r1, r1, #(1<<(STEP_IN1))
	STR r1, [r0, #GPIO_OTYPER]

	;In2 OTYPER
	LDR r1, [r0, #GPIO_OTYPER]
	BIC r1, r1, #(1<<(STEP_IN2))
	STR r1, [r0, #GPIO_OTYPER]
	
	;In3 OTYPER
	LDR r1, [r0, #GPIO_OTYPER]
	BIC r1, r1, #(1<<(STEP_IN3))
	STR r1, [r0, #GPIO_OTYPER]
	
	;In4 OTYPER
	LDR r1, [r0, #GPIO_OTYPER]
	BIC r1, r1, #(1<<(STEP_IN4))
	STR r1, [r0, #GPIO_OTYPER]
	
	;In1 PUPDR
	LDR r1, [r0, #GPIO_PUPDR]
	BIC r1, r1, #(3<<(2*STEP_IN1))
	STR r1, [r0, #GPIO_PUPDR]
	
	;In2 PUPDR
	LDR r1, [r0, #GPIO_PUPDR]
	BIC r1, r1, #(3<<(2*STEP_IN2))
	STR r1, [r0, #GPIO_PUPDR]
	
	;In3 PUPDR
	LDR r1, [r0, #GPIO_PUPDR]
	BIC r1, r1, #(3<<(2*STEP_IN3))
	STR r1, [r0, #GPIO_PUPDR]
	
	;In4 PUPDR
	LDR r1, [r0, #GPIO_PUPDR]
	BIC r1, r1, #(3<<(2*STEP_IN4))
	STR r1, [r0, #GPIO_PUPDR]
	
	BX LR
	
	ENDP
	LTORG
		
FullStep PROC
	
	MOV r2, #0
	MOV r3, #DEGREES
	MOV r6, #64
	MOV r7, #45
	MUL r8, r3, r6
	UDIV r8, r8, r7 ;(360*64)/45
	
	;Load GPIOC Base
	LDR r0, =GPIOC_BASE

stepLoop
	ADD r2, r2, #1 ; i++
	
	BL delayOverhead
	;Step 1
	LDR r1, [r0, #GPIO_ODR]
	ORR r1, r1, #(1<<STEP_IN1)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN2)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN3)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	ORR r1, r1, #(1<<STEP_IN4)
	STR r1, [r0, #GPIO_ODR]
	
	BL delayOverhead
	;Step 2
	LDR r1, [r0, #GPIO_ODR]
	ORR r1, r1, #(1<<STEP_IN1)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	ORR r1, r1, #(1<<STEP_IN2)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN3)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN4)
	STR r1, [r0, #GPIO_ODR]
	
	BL delayOverhead
	;Step 3
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN1)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	ORR r1, r1, #(1<<STEP_IN2)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	ORR r1, r1, #(1<<STEP_IN3)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN4)
	STR r1, [r0, #GPIO_ODR]
	
	BL delayOverhead
	;Step 4
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN1)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN2)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	ORR r1, r1, #(1<<STEP_IN3)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	ORR r1, r1, #(1<<STEP_IN4)
	STR r1, [r0, #GPIO_ODR]
	
	CMP r8, r2
	BGE stepLoop
	BX LR
	
	ENDP
	LTORG
		
FullStepCCW PROC
	
	MOV r2, #0
	MOV r3, #DEGREES
	MOV r6, #64
	MOV r7, #45
	MUL r8, r3, r6
	UDIV r8, r8, r7 ;(360*64)/45
	
	;Load GPIOC Base
	LDR r0, =GPIOC_BASE

stepLoopCCW
	ADD r2, r2, #1 ; i++
	
	BL delayOverhead
	;Step 1
	LDR r1, [r0, #GPIO_ODR]
	ORR r1, r1, #(1<<STEP_IN1)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	ORR r1, r1, #(1<<STEP_IN2)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN3)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN4)
	STR r1, [r0, #GPIO_ODR]
	
	BL delayOverhead
	;Step 2
	LDR r1, [r0, #GPIO_ODR]
	ORR r1, r1, #(1<<STEP_IN1)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN2)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN3)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	ORR r1, r1, #(1<<STEP_IN4)
	STR r1, [r0, #GPIO_ODR]
	
	BL delayOverhead
	;Step 3
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN1)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN2)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	ORR r1, r1, #(1<<STEP_IN3)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	ORR r1, r1, #(1<<STEP_IN4)
	STR r1, [r0, #GPIO_ODR]
	
	BL delayOverhead
	;Step 4
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN1)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	ORR r1, r1, #(1<<STEP_IN2)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	ORR r1, r1, #(1<<STEP_IN3)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN4)
	STR r1, [r0, #GPIO_ODR]
	
	CMP r8, r2
	BGE stepLoopCCW
	BX LR
	
	ENDP
	LTORG
		
HalfStep PROC
	
	MOV r2, #0
	MOV r3, #DEGREES
	MOV r6, #64
	MOV r7, #45
	MUL r8, r3, r6
	UDIV r8, r8, r7 ;(360*64)/45
	
	;Load GPIOC Base
	LDR r0, =GPIOC_BASE

halfStepLoop
	ADD r2, r2, #1 ; i++
	
	BL delayOverhead
	;Step 1
	LDR r1, [r0, #GPIO_ODR]
	ORR r1, r1, #(1<<STEP_IN1)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN2)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN3)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	ORR r1, r1, #(1<<STEP_IN4)
	STR r1, [r0, #GPIO_ODR]
	
	BL delayOverhead
	;Step 2
	LDR r1, [r0, #GPIO_ODR]
	ORR r1, r1, #(1<<STEP_IN1)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN2)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN3)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN4)
	STR r1, [r0, #GPIO_ODR]
	
	BL delayOverhead
	;Step 3
	LDR r1, [r0, #GPIO_ODR]
	ORR r1, r1, #(1<<STEP_IN1)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	ORR r1, r1, #(1<<STEP_IN2)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN3)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN4)
	STR r1, [r0, #GPIO_ODR]

	BL delayOverhead
	;Step 4
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN1)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	ORR r1, r1, #(1<<STEP_IN2)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN3)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN4)
	STR r1, [r0, #GPIO_ODR]
	
	BL delayOverhead
	;Step 5
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN1)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	ORR r1, r1, #(1<<STEP_IN2)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	ORR r1, r1, #(1<<STEP_IN3)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN4)
	STR r1, [r0, #GPIO_ODR]
	
	BL delayOverhead
	;Step 6
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN1)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN2)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	ORR r1, r1, #(1<<STEP_IN3)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN4)
	STR r1, [r0, #GPIO_ODR]

	BL delayOverhead
	;Step 7
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN1)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN2)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	ORR r1, r1, #(1<<STEP_IN3)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	ORR r1, r1, #(1<<STEP_IN4)
	STR r1, [r0, #GPIO_ODR]
	
	BL delayOverhead
	;Step 8
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN1)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN2)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN3)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	ORR r1, r1, #(1<<STEP_IN4)
	STR r1, [r0, #GPIO_ODR]
	
	CMP r8, r2
	BGE halfStepLoop
	BX LR
	
	ENDP
	LTORG

HalfStepCCW PROC
	
	MOV r2, #0
	MOV r3, #DEGREES
	MOV r6, #64
	MOV r7, #45
	MUL r8, r3, r6
	UDIV r8, r8, r7 ;(360*64)/45
	
	;Load GPIOC Base
	LDR r0, =GPIOC_BASE

halfStepLoopCCW
	ADD r2, r2, #1 ; i++
	
	BL delayOverhead
	;Step 1
	LDR r1, [r0, #GPIO_ODR]
	ORR r1, r1, #(1<<STEP_IN1)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	ORR r1, r1, #(1<<STEP_IN2)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN3)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN4)
	STR r1, [r0, #GPIO_ODR]
	
	BL delayOverhead
	;Step 2
	LDR r1, [r0, #GPIO_ODR]
	ORR r1, r1, #(1<<STEP_IN1)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN2)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN3)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN4)
	STR r1, [r0, #GPIO_ODR]
	
	BL delayOverhead
	;Step 3
	LDR r1, [r0, #GPIO_ODR]
	ORR r1, r1, #(1<<STEP_IN1)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN2)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN3)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	ORR r1, r1, #(1<<STEP_IN4)
	STR r1, [r0, #GPIO_ODR]

	BL delayOverhead
	;Step 4
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN1)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN2)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN3)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	ORR r1, r1, #(1<<STEP_IN4)
	STR r1, [r0, #GPIO_ODR]
	
	BL delayOverhead
	;Step 5
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN1)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN2)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	ORR r1, r1, #(1<<STEP_IN3)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	ORR r1, r1, #(1<<STEP_IN4)
	STR r1, [r0, #GPIO_ODR]
	
	BL delayOverhead
	;Step 6
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN1)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN2)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	ORR r1, r1, #(1<<STEP_IN3)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN4)
	STR r1, [r0, #GPIO_ODR]

	BL delayOverhead
	;Step 7
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN1)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	ORR r1, r1, #(1<<STEP_IN2)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	ORR r1, r1, #(1<<STEP_IN3)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN4)
	STR r1, [r0, #GPIO_ODR]
	
	BL delayOverhead
	;Step 8
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN1)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	ORR r1, r1, #(1<<STEP_IN2)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN3)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<STEP_IN4)
	STR r1, [r0, #GPIO_ODR]
	
	CMP r8, r2
	BGE halfStepLoopCCW
	BX LR
	
	ENDP
	LTORG
	
	
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
	
	BL Stepper_Pin_Init


while
	BL HalfStepCCW
	B while
	
DEADLOOP B DEADLOOP     		; dead loop & program hangs here

	ENDP
	LTORG
					
	ALIGN			

	AREA    myData, DATA, READWRITE
	ALIGN
array	DCD   1, 2, 3, 4
	END
