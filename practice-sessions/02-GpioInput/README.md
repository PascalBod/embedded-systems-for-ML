# Exercise 02 - GPIO Input

## Purpose of the exercise

Display a message when the BTN0 button is pushed or released.

## The exercise

1. Create a new project, again based on the **Empty C project**, naming it `02-GpioInput`.
2. Check that you can build it and that you can program the board.
3. Add the same components as in exercise 01.
4. Add a log message output at initialization time: `"02-GpioInput - v0.1.0\n"`
5. Add the following include directives in `app.c`:
    ```
    #include "em_cmu.h"
    #include "em_gpio.h"
    ```
6. Add the following function call in the `app_init` function:
    ```
        CMU_ClockEnable(cmuClock_GPIO, true);
    ```
7. Open the `02-GpioInput.slcp` file and display the **Platform > Peripheral > GPIO** component information. In the **DOCUMENTATION** block, click the **Open in Browser** link. The displayed web page describes the functions which can be used to configure and use the GPIO pins of the EFR32MG24.
8. Read the descriptions of the `GPIO_PinModeSet` and `GPIO_PinInGet` functions.
9. Check the board schematic (either in the presentation or [here](https://www.silabs.com/documents/public/schematic-files/BRD2601B-A01-schematic.pdf)) in order to know which GPIO pin should be used to check BTN0 button state
10. Use the two GPIO functions to initialize the GPIO pin, and read the button state. Your code must display a message when the button is pushed and when the button is released, and only then.

Note: displaying the name of the application and its version number at initialization time allows you to check that the board is running the right version of the right application.

Note: by default, in order to decrease energy consumption, the clock required by the GPIO block to work is disabled. The call to `CMU_ClockEnable` allows to enable this clock.

Note: the two GPIO functions are provided by the EFR32MG24 libraries. If they were not provided, you would have to directly access the microcontroller registers controlling its GPIO pins. To get an idea about the kind of effort this may require, take a look at the [Reference Manual](https://www.silabs.com/documents/public/reference-manuals/efr32xg24-rm.pdf), from page 807 to page 981.

## Limitation

Once your application works, answer the following questions: imagine that you can push the button and release it very rapidly. Would your application be able to see all button state changes? Why?