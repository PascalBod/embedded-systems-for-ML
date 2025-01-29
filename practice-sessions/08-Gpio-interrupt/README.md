# Exercise 08 - GPIO interrupt

## Purpose of the exercise

Use an interrupt to detect button presses.

## Reference documents

* [xG24 Dev Kit User Guide](https://www.silabs.com/documents/public/user-guides/ug524-brd2601b-user-guide.pdf)
* [xG24 Dev Kit schematic](https://www.silabs.com/documents/public/schematic-files/BRD2601B-A01-schematic.pdf)
* [EFR32xG24 Reference Manual](https://www.silabs.com/documents/public/reference-manuals/efr32xg24-rm.pdf)
* [SDK GPIO module API](https://docs.silabs.com/gecko-platform/5.0.2/platform-emlib-efr32xg24/gpio)
* [SDK GPIOINT module API](https://docs.silabs.com/gecko-platform/5.0.2/platform-driver/gpioint)

## What you have to do

Import the `08-Gpio-interrupt` project.

Build and flash the application. Display the output, piping it to the ts command:
```shell
$ pyserial-miniterm --raw -e /dev/ttyACM0 115200 | ts "%H:%M:%.S"
```

Check what happens when you press the button 0.

Read and understand the source code.

Requested tasks:
* Modify the source code so that the number of interrupts is incremented only when the button is released.
* With the above configuration, imagine that the button is periodically (pressed and) released at a very high rate. What can be the impact on displayed messages?
* Imagine that the button is released at a very very high rate? What can be the impact?

## Note: the `volatile` keyword

The `int_nb` variable is defined in the following way:
```
static volatile uint8_t int_nb;
```

The `volatile` keyword tells the compiler that the value of the related object can be modified at unpredictable moments. Consequently, it should not apply the usual optimizations to it.

This keywork must be used for every definition impacted by an interruption.