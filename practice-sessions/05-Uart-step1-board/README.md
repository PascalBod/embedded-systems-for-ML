# Exercise 05 - UART - step 1

## Purpose of the exercise

Send a character to the computer over a (virtual) serial port.

## What you have to do

Import the `05-Uart-step1-board` project.

Read and understand the existing source code in `app.c`.

In `app.c`, add required code:
* To select the right GPIO pins of the microcontroller
* To transmit the '*' character every UART_TX_PERIOD_MS millisecond to the PC

To check that the characters are received by the PC, run *pyserial-miniterm* (refer to exercise 01 - Hello World).

## Reference documentation

* [USART peripheral library API](https://docs.silabs.com/gecko-platform/5.0.1/platform-emlib-efr32xg24/usart)
