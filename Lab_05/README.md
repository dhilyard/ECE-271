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

* Question 1: What instruction is used to call a function in ARM Thumb-2 assembly?  <br>
BL{function name}

* Question 2: What instruction is used to return from a function in ARM Thumb-2 assembly?<br>
BX LR<br>

* Question 3: With the standard ARM EABI, what register is used to pass a single 32-bit integer argument to a function? <br>
r0 to r3 is the default 

* Question 4: If your function is not a leaf function (meaning it calls another function), what must you do with your link register (LR) before calling any other function?<br>
POP{r11,lr} to preserve it.




Answers to After-lab Questions
-------

* Question 1: Why is an ABI helpful when writing code? Why not just pick any register we want for passing parameters?<br>
An ABI is helpful because it means we don't have to juggle what registers have been used or not and can instead reuse them for multiple purposes.

* Question 2: Why does C store local variables on the stack rather than forcing you to use all global variables?<br>
Because it is a waste of resource to have a variable go beyond the necessary scope of what its accomplishing, so local variables are used for exactly what they need and then are left in that function.


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
