##  Lab Report ##

What is something cool you did in this lab?
-----------
Your answer: 
    I made the blinking LED togglable with a button press so that it can be paused.

Post-lab Questions
-------

**Answers to Question 1:** The pushbutton on the board has a hardware debouncing circuit. Explain briefly how the hardware debouncing circuit works. 

The hardware debouncer is a RC Cirucit with a capacitor connected in parallel with the button. When the switch is open the capacitor is charged but once the button is pressed it quickly discharges. This makes up the simple function of the debouncer since if the button is unpressed too quickly the capacitor won't be charged enough to register it.

**Answers to Question 2:** Debouncing can also be done in software. Explain how this could be done in software.

There are a few methods of doing software debouncing, wait-and-see and counter debouncer being easier and better respectively. Wait-and-see will detect that the button was pushed and then look for the input a second time after a short delay (20-50ms) and if the button is still pressed then it will report that said button has been pressed.

**Answers to Question 3 (if appliable):** Why do we configure the pins that drive the pushbutton as no pull-up, no pull-down? Can we configure it as a pull-down?

Without pull-up or pull-down the pin has high impedance also known as Hi-Z. Configuring the pins as pull-down would not achieve the same impact as it would provide a path to ground so we can't configure it as pull-down.

**Answers to Question 4 (if appliable):** Each GPIO pin has four programmable output speeds: low, medium, fast, and high. Specifically, for STM32L4, the slew rate of the high-speed output can be up to 80 MHz. Why is the “low speed” configuration recommended for controlling LED? (Hints: energy and electromagnetic interference)

The low speed slew rate is the best option for multiple reasons, first as the hints mention and has been a running theme it is the most power efficient option and these boards are all about low power usage and being very efficient with any power they need to use. The second part mentioned by the hint is EMI or Electromagnetic interference, something that a large slew rate causes far more of than a small slew rate. EMI can cause malfunctions by transferring to neighboring circuits.


Suggestions and Comments
-------

* Do you have comments or suggestions for the lab and course?
Not at this time.

* Do you have comments or suggestions for the textbook and lab handouts? Any typos or errors?
Maybe add a section to tell people to turn down compiler optimization, but thats all.


Lab Credits
-------
Did you received any help from someone other than the instructor and lab teaching assistants?

Your answer: 
Yes Zach Holm, Blake Knedler and I helped each other to troubleshoot code and bounce ideas off of each other.

