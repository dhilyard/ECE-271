##  Lab Report ##

Your Name: Drew Hilyard
-----------


Lab Status
-------
Answer the following questions by put a "x" in the bracket.
- Yes [X], No []: Complete pre-lab
- Yes [X], No []: Complete basic lab requirements
- Yes [X], No []: Complete something cool
  - If you have completed something cool, give it a short summary below: <br>
  This something cool is the same as Lab 1 where the button pauses and resumes the blinking of the LED. 


Answers to Pre-lab Questions
-------
This part is required only if you are requested to write your answers here. 

* Question 1: Why is RCC_AHB2ENR defined as 0x4C? Hints: Look at the RCC_TypeDef structure
defined in stm32l476xx.h. <br>
0x4C is the address offset from the start of the struct.


* Question 2: Learn from the example above, and translate the following C code into assembly:<br>
GPIOA->ODR |= GPIO_ODR_ODR_5; <br>

LDR r1, =GPIO_BASE <br>
LDR r3,[r1,#GPIO_ODR] <br>
ORR r3,r3,#GPIO_ODR_ODR_5 <br>
STR r3,[r1,#GPIO_ODR] <br>


Answers to After-lab Questions
-------

* Question 1: When loading the pointer value into r1, we use = to do a PC-relative load from a value in
the literal area. Why can’t we specify a 32-bit immediate target to a move instruction like
the following? <br>
MOV r0, #0xdeadbeef <br>
The value is too large to fit in the instruction. The equals sign puts the value in reach so the assembler can do PC-relative loading.


* Question 2: In the example code, we ORR with an immediate constant pre-pended with #. What would
happen if your immediate constant did not fit? How could you work around that issue? <br>
Load the constant into a register with '=' then orr with that register.

* Question 3: How does the CMP instruction differ from the SUB instruction?<br>
CMP doesn't store the result of the subtraction operation.

* Question 4: Can we use the BIC instruction to turn off LED? If not, why? If so, how? <br>
Yes because BIC can wipe its own bits. It cannot really turn the LED back on but that is not the requirement.<br>
LDR r0, =GPIOA_BASE <br>
LDR r1, [r0, #GPIO_ODR] <br>
BIC r1, r1, #(F'<<'LED_PIN) <br>
The apostrophes are unnecessary for the code markdown just didn't like the symbols out in the open.
Suggestions and Comments
-------

* Do you have comments or suggestions for the lab and course?
N/A

* Do you have comments or suggestions for the textbook and lab handouts? Any typos or errors?
N/A


Lab Credits
-------
Did you received any help from someone other than the instructor and lab teaching assistants?<br>
Blake Knedler and I helped each other troubleshoot and brainstorm.
