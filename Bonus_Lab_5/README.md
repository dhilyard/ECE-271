##  Lab Report ##

Your Name: Drew Hilyard
-----------


Lab Description
-------
Bonus Lab 5 is calling C functions in Assembly. This has the repeat goals from Lab 4 of making the LED blink whenever you push the button, the added task is to use the LCD display to display how many times the button has been pushed. The main code is in assembly while controlling the LCD is in C. All of my LCD control functions are in the ScreenControl.c file and are then imported to the main.s file to be used in assembly. A majority of the main.s code is the same code from Lab 4 just slightly retooled for this purpose and the screen control functions were built using Lab 3's as references.