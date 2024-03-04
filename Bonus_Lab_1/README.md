##  Lab Report ##

Your Name: Drew Hilyard
-----------


Lab Description
-------
Bonus Lab 1 is using a buzzer to not only make accurate sound notes but to also play a song/melody. To ensure the timer was a correct delay I used SysTick functions from Lab 7 as the delay timer and PWM code from Lab 8 was used as the basis for the PWM initalization of PWM for the buzzer. The melody was made using an array using defines for note frequencies and delays based on what the note is in the song. I choose to use Happy Birthday as a very simple and recognizable option for my song and translated it to the melody array. To play the melody the playSong function calls the playNote function which simply reads down the array playing each frequency for the length of time designated by its note, converting it to be the correct number for PWM and altering the pulse. Afterwards there is a short delay in the playSong loop and then it moves to the next note. After the song plays there is a short gap and then the note A4 (440hz) plays to indicate that the buzzer is making the correct sounds.