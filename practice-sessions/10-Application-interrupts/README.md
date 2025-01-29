# Exercise 10 - Application and interrupts

## Purpose of the exercise

Develop an application which conforms to the following requirements:
* After the user has pressed twice the button 0, wait for a given period, WAIT_PERIOD_MS, and then display a message. Only button release is to be considered
* The two successive releases of the button must occur in a given interval of time. If the second release occurs too soon or too late, the application must reenter its idle state
* If the button is (pressed and) released after two conformant presses and before the end of WAIT_PERIOD_MS, the application must reenter the idle state

## Guidelines

Reuse what you have seen in the previous exercises:
* Finite state machine
* GPIO interrupt
* Timer interrupt

Proceed step by step. Start by writing an application which simply waits for a button press. Then, improve it so that it starts a timer for the minimal wait period before the second press. Then, improve it so that if the button is pressed a second time before the end of the minimal period, the application reenters its idle state. Etc.