##  Lab Report ##

Your Name: Drew Hilyard
-----------


Lab Description
-------
Bonus Lab 2 has a goal of making an analog watch on the LCD using the on-chip real-time clock. This lab required learning quite a lot about drawing on the LCD but in the end I produced hours, minutes and second hands that all move as time does. One thing to note is that while minutes and seconds have 60 different points they move to due to limitations the hours hand only has the 12 hours to move to. This Lab leaned on the RTC Clock bonus lab for the RTC functions but otherwise is all newly written code to accomplish the goal of the lab. Each hand has its own function that the while loop in main calls in a chain allowing them to each update their locations for the current time, the time is set at 11:32 so that all hands can be seen and non 12 positions instantly. The digital equivalent of the analog time is provided on the left side to make comparing the two easier.