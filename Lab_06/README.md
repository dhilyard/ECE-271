    ##  Lab Report ##

Your Name: Drew Hilyard
-----------


Lab Status
-------
Answer the following questions by put a "x" in the bracket.
- Yes [X], No []: Complete pre-lab
- Yes [X], No []: Complete basic lab requirements
- Yes [X], No []: Complete something cool

<br>What is something cool you did in this lab?<br>
Your answer: My EXTI not only controls the LED but also controls the stepper motor choosing a direction based on the status of the LED.


Pre-lab Questions
-------

* Answers to Question 1: <br>
    Polling is good for when the status is frequently changing but wastes a lot of CPU cycles since the CPU needs to wait and check a device at regular intervals. Interrupts are good when a status needs to be called rarely and interrupts waste a lot less CPU cycles by only using the CPU when a status changes.

* Answers to Question 2: <br>
This assumes the prior setup as shown in the textbook has been done for the sake of answer brevity 
<br>

    //Enables EXTI 13 Interrupt by unmasking
    EXTI->IMR1 |= (1<<13)

    //Set EXTI 13 priority to 1
    NVIC_SetPriority(EXTI15_10_IRQn, 1)

    //Enable EXTI 13 Interrupt
    NVIC_EnableIRQ(EXTI15_10_IRQn)

* Answers to Question 3 <br>
EXTI15_10_IRQHandler

* Answers to Question 4 
<br>

    SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13;
    SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC; //Sets to 1 to select PC13
    SYSCFG->EXTICR[3] &= ~(0xF000);

* Answers to Question 5 <br>
Set EXTI1_IMR1_IM13 to 1 <br>
Set EXTI_RTSR1_RT13 to 0 <br>
Set EXTI_FTSR1_FT13 to 1 


Post-lab Questions
------- 

* Answers to Question 1: <br>
    Software can clear PR bits by writing a 1 to it.
* Answers to Question 2: <br>
    If not cleared the interrupt handler will repeatedly think the interrupt is being triggered and loop the function.
* Answers to Question 3: <br>
    Software can set the EXTI flag and trigger the external interrupt via the interrupt event register EXTI_SWIER
Suggestions and Comments
-------

* Do you have comments or suggestions for the lab and course?
N/A

* Do you have comments or suggestions for the textbook and lab handouts? Any typos or errors?
N/A


Lab Credits
-------
Did you received any help from someone other than the instructor and lab teaching assistants?

Your answer: Nope, this was done over break so the only resources utilized were course provided or online explanations.


