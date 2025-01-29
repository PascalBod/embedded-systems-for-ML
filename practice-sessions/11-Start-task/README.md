# Exercise 11 - Creating an starting a task

## Purpose of the exercise

Build and run the Silicon Labs FreeRTOS sample application which makes a LED blink.

## Guidelines

Using the Simplicity Studio wizard, create a new project. Use the keyword `FreeRTOS` for the example project filter, and select the *Platform - Blink Kernel FreeRTOS* example project. Name the new project `11-Start-task`.

Build the project and flash the Dev Kit.

Read the *Project architecture* section below, and understand how the code works.

The LED blinks, with red color. Actually, the LED is an RGB model. Check the [Dev Kit schematic](https://www.silabs.com/documents/public/schematic-files/BRD2601B-A01-schematic.pdf), to find which GPIO pins control the three LED colors. The information is also present in the [Dev Kit User Guide](https://www.silabs.com/documents/public/user-guides/ug524-brd2601b-user-guide.pdf).

In Simplicity Studio, search the Simple LED component, which is used to control the red color, and check in its configuration how it is connected to the expected GPIO pin.

Add two new Simple LED component instances, one for the green color and one for the blue color. If you don't modify the instance names, you will get `led1` and `led2`. Configure them for the respective GPIO pins.

Simplicity Studio automagically creates two new structures, `sl_led_led1` and `sl_led_led2`, which can now be referenced by the source code.

Run one task for each of them, and start them, so that the green color and the blue color blink as well, but with different periods.

When this is done, and if this is not the case yet, try to use only one `blink_task` function, i.e. calling `xTaskCreateStatic` function three times, but with the same task code each time.

In the `xTaskCreateStatic` function documentation (see below), check what is said about `pvParameters`, `puxStackBuffer` and `pxTaskBuffer` parameters. In which type of storage have they to be defined?

## Project architecture

### FreeRTOS

Open the `11-Start-task.slcp` file, and search the `FreeRTOS` component, **RTOS > FreeRTOS > FreeRTOS**. Click the **Configure** button, to display the various FreeRTOS configuration parameters. Among them:
* Minimal (default) stack size, in words (a word = 4 bytes). `configMINIMAL_STACK_SIZE` is set to this value (see `blink.c`)
* Total heap size, in bytes
* Kernel tick frequency. By default it is set to 1000 Hz. This is the frequency at which the scheduler checks if the currently running task has to be replaced by another one

### How the task is started

Defined in `main.c`, the `main` function initializes the system (call to `sl_system_init`), then the application (call to `app_init`).

The FreeRTOS component being present in the project, `SL_CATALOG_KERNEL_PRESENT` is defined (this is automagically done by Simplicity Studio, when the component is added). Consequently, the `sl_system_kernel_start` is called. It starts the FreeRTOS scheduler. Any task created beforehand will be started.

`app_init` has to be defined by the developer, in `app.c`. The idea is that `app_init` should create all application tasks. Currently, there is only one, created by `blink_init`, defined in `blink.c`.

### Dynamic memory

Remember what we said about dynamic memory allocation? In `blink.c`, as `EXAMPLE_USE_STATIC_ALLOCATION` is not defined, the FreeRTOS `xTaskCreateStatic` function does not allocate any dynamic storage, but relies on a memory block provided by the developer. Additionally, the last argument must point to a buffer where task information will be stored.

### API

The `xTaskCreateStatic` function is described [here](https://www.freertos.org/Documentation/02-Kernel/04-API-references/01-Task-creation/02-xTaskCreateStatic).

### Code of the task

The task runs the function passed as the first argument. Here, this is the `blink_task` function. This function must never return, unless it deletes itself by calling `vTaskDelete(NULL)`.

### Naming conventions

The names of the various FreeRTOS objects adhere to naming conventions presented [here](https://www.freertos.org/Documentation/02-Kernel/06-Coding-guidelines/02-FreeRTOS-Coding-Standard-and-Style-Guide#naming-conventions).
