##  Lab Report ##

What is something cool you did in this lab?
-----------
Your answer: I controlled my stepper motor using the new delay function made in this lab.


Post-lab Questions
-------

* Answers to Question 1: As shown below, Cortex-M supports up to 256 interrupts. What is the interrupt number of SysTick defined in CMSIS? <br>
SysTick is -1 as it is a Processor Exception and defined by ARM specifically.

* Answers to Question 2:What is the default interrupt priority of SysTick? Does a higher priority value represent a higher urgency? <br> It defaults to 0x00, the highest priority as a lower number is a higher priority and a higher number is a lower priority. It is easier to think about it the priority ordering as a line, the closer to the front you are the earlier you go.

* Answers to Question 3: Is the SysTick interrupt enabled by default? <br>
No it is disabled by default we have to enable it with SysTick_CTRL

* Answers to Question 4 (if appliable):Suppose a clock of 16MHz is used to drive the system timer. What is the maximum interval between two consecutive SysTick interrupts we can possibly obtain? <br>
.04 seconds is the maximum interval.


Suggestions and Comments
-------

* Do you have comments or suggestions for the lab and course?
N/A

* Do you have comments or suggestions for the textbook and lab handouts? Any typos or errors?
N/A


Lab Credits
-------
Did you received any help from someone other than the instructor and lab teaching assistants?

Your answer: 
Worked with Blake Knedler for troubleshooting

