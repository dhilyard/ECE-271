##  Lab Report ##

Your Name: Drew Hilyard
-----------


Lab Status
-------
Answer the following questions by put a "x" in the bracket.
- Yes [X], No []: Complete pre-lab
- Yes [X], No []: Complete basic lab requirements
- Yes [X], No []: Complete something cool
  - If you have completed something cool, give it a short summary below: 
	I've made it so that the * deletes the previous input and that the # deletes the entire string.


Answers to Demo Questions
-------
This part is required only if you are requested to write your answers here. 

* Question 1: Do all keypresses show up on display? 
  All the intended ones yes, * and # don't as they have alternate functions.


* Question 2: How do you debounce the key presses?
  With a short delay between the clear operation and the rest of keypad scan.


* Question 3: Why did we need to use external pull-up resistors instead of built-in ones?
  We had to use external because the built in ones are not strong enough to accomplish what we need.


Answers to After-lab Questions
-------

* Question 1: Can the scan algorithm detect all keys pressed when multiple keys are pressed? (Hint:
ghosting or ghost key)
With this keypad ghosting is absolutely a problem due to the fact that there are only the 8 wires. This means that crossed wires will happen whenever 2 buttons intersect in a cross shape as that means they're using 1 identical wire. That being said the scan algorithm does detect that multiple keys are being pressed but it can only interpret one at a time throwing the other away. Even if you press non-colliding ones only one will register. I tested with 4 and 9 pressing both at the same time repeatedly, this resulted in a mixed output of the two numbers as it can only interpret one at a time.



Suggestions and Comments
-------

* Do you have comments or suggestions for the lab and course?
Not at this time.

* Do you have comments or suggestions for the textbook and lab handouts? Any typos or errors?
Not at this time.


Lab Credits
-------
Did you received any help from someone other than the instructor and lab teaching assistants?
Blake Knedler and I assisted each other with troubleshooting, Zach Holm has dropped the ECE program so he is no longer in the lab.
