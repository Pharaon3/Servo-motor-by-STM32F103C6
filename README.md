# Servo motor by STM32F103C6

Servo motor control with STM32F103C6
This project was done from Sep 11 to Sep 15, 2022.
The client want to rotate a servo motor with STM32F103C6.
He wants to rotate it from 0 degree to 180 degree and invers.
I consult the site https://controllerstech.com/servo-motor-with-stm32/  to get logic to control servo motor.
I download some sample codes from https://github.com/yohanes-erwin/stm32f103-keil  
There is an example to control the servo-motor but I couldn't re-use it because the signal port was not PA0(in sample code) but PB6(in client's kit).
In STM32F103C6, PB6 isn't PWM port of any Timer.
In addition delay function makes error in working. I realized it later, and this made the project difficult and long to finish.
So I use Timer Over Interrupt to count samples of signals and make PWM signal manually.
Therefore, the signal should be inversed and I realized it later.
Finally, the client shows being satisfied and released the project.
