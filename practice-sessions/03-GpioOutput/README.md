# Exercise 03 - GPIO Output

## Purpose of the exercise

Make the board RGB LED blink red.

## The exercise

1. Import the `exercises/03-GpioOutput` project in Simplicity Studio, following these instructions:
   1. **File > Import...**
   2. Use the **Browse** button to select the `exercises/03-GpioOutput` directory.
   3. Select the line containing the **Simplicity Studio (.sls)** project type.
   4. Click the **Next >** button.
   5. Accept the proposed default values and click the **Next >** button.
   6. In the next window, accept the default values again, and click the **Finish** button.
2. Check that you can build it.

The `app.c` file contains some code, detailed in the following sections.

### Application version and block clock

As in the previous exercise, the application displays its name and its version, at initialization time. Then, it enables the GPIO block clock.

### How to measure time

The microcontroller does not know what a second or a millisecond is. But it can count the number of *ticks* since it has been started, a tick being one period of one of a specific microcontroller clock (for several reasons, the microcontroller uses several clocks). A library function provides the number of ticks since startup: `sl_sleeptimer_get_tick_count64`.

When the clock period is known, it's easy to deduce a number of milliseconds from a number of ticks. A library function performs this operation: `sl_sleeptimer_tick64_to_ms`.

Check how these functions are used in `app_init` and `app_process_action`.

### LED control

The board features an RGB LED. Actually, the component contains three different LEDS (a red one, a green one and a blue one), each of them being controllable by the microcontroller.

Check the board schematic (either in the presentation or [here](https://www.silabs.com/documents/public/schematic-files/BRD2601B-A01-schematic.pdf)) in order to know which GPIO pin should be used to control the red LED.

### What you have to do

Add code required to configure the GPIO pin controlling the red LED.

Add code required to make the red LED blink with a total period of 500 ms (250 ms off and 250 ms on). Note: the code providing the number of milliseconds elapsed since startup is already in place.