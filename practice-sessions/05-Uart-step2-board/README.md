# Exercise 05 - UART - step 2

## Purpose of the exercise

Echo every character received on the serial port.

## What you have to do

Import the `05-Uart-step2-board` project.

Read and understand the existing source code in `app.c`.

In `app.c`, add required code:
* To select the right GPIO pins of the microcontroller
* To transmit to the PC every character received from it

On the PC, use *pyserial-miniterm* to send and receive characters.

## Reference documentation

* [USART peripheral library API](https://docs.silabs.com/gecko-platform/5.0.1/platform-emlib-efr32xg24/usart)
