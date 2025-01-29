# Exercise 01 - step 1 - Hello World

## Purpose of the exercise

Display a message on the virtual serial link created over the USB connection.

## A few words about EFR32MG24 libraries

Many libraries are provided by Silicon Labs for helping the developer in writing code. The libraries contain abstraction code implementing a "high-level" API (Application Programming Interface) to the various microcontroller blocks: peripherals, interfaces, etc. (more information on this topic later in the presentation).

For Simplicity Studio, these libraries are provided as "software components" which can be added to a project in a simple way.

There are pros and cons to this architecture. Adding, configuring and removing software support to a given interface or peripheral is easy. On the other hand, it can be sometimes difficult to understand exactly how a library works. 

## The exercise

1. Ensure you completed the [prerequisites](https://github.com/PascalBod/RTOS-presentation?tab=readme-ov-file#prerequisites-for-the-practical-exercises).
2. [Create a new project](https://github.com/PascalBod/lm-efr32-simplicityStudio?tab=readme-ov-file#sample-application), still based on the **Empty C project**, naming it `01-HelloWorld`.
3. Check that you can build it and that you can program the board.
4. Replace the content of the `readme.md` file (right-click and **Open With > Text Editor**) by:
    ```
    # HelloWorld
    
    This example project shows how to display a message on the
    virtual serial link created over the USB connection.          
    ```
5. If the content of the `01-HelloWorld.slcp` file is not displayed in an editor tab yet, double-click the filename in the Project Explorer view.
6. Click on the **SOFTWARE COMPONENTS** tab.
7. Using the search box, install the following components, in this order:
   * **Services > IO Stream > IO Stream: STDLIB Configuration**
   * **Services > IO Stream > Driver > IO Stream: USART**
   * **Services > IO Stream > IO Stream: Retarget STDIO**
   * **Application > Utility > Log**

Notes:
* Only the part of the string after the last **>** character must be used to search.
* When the search result is displayed, click the small triangles on the left-hand side until the result is fully unfolded. Then, click the **Install** button.
* For the USART component, keep the default name for the instance.

8. Double-click the `app.c` file in order to display its content.
9. Add the following include directive:
    ```
    #include "app_log.h"
    ```
10. Add the following function call in the `app_init` function:
    ```
        app_log("Hello World!\n");
    ```
12. Save the file.
13. Build the application, and program the board.
14. In a terminal window, start a serial terminal connected to the board:
    ```
    $ pyserial-miniterm /dev/ttyACM0 115200
    ```
15. Click the board reset button. Each click should display the `Hello World!` message.

The next section, below, provides information about how the application source code is architectured.

## Application architecture

When creating an empty C project, two source-code files are generated for you by Simplicity Studio:
* `main.c`
* `app.c`

Some other source-code files are generated in the `autogen` and `simplicity_sdk_2024.6.1` directories. We can ignore them for now.

An application is made of your code, and of the code used by the required libraries. In this example, the application prints a message. In a desktop application, the message would be printed in a terminal window. Here, the message is sent over a serial link. The serial link is created over the USB connection between the microcontroller board and the development computer. On the computer, the message is displayed by the serial terminal application.

Initializing the serial link and redirecting the message to the serial link is performed by some code automatically added by Simplicity Studio. This code has to be run along with your code. This is done by `main.c`:
```C
int main(void)
{
  // Initialize Silicon Labs device, system, service(s) and protocol stack(s).
  sl_system_init();

  // Initialize the application.
  app_init();

  while (1) {
    // Do not remove this call: Silicon Labs components process action routine
    // must be called from the super loop.
    sl_system_process_action();

    // Application process.
    app_process_action();
  }
}
```

You have to insert your code in the `app_init` and `app_process_action` functions. You have to do this in the `app.c` file.

## Roles of components

The components we added to the project provide the following functions:
* **IO Stream: STDLIB Configuration**: the IO Stream is a generic stream which provides a common interface to perform I/O operations on different physical interfaces. By default, write operations performed on the IO Stream are buffered, i.e. are kept in memory until there is enough data to write, or a specific event occurs. This component disables the buffering feature and outputs data as soon as possible
* **IO Stream: USART**: this component redirects the IO Stream over the Universal Synchronous Asynchronous Receiver Transceiver (USART) interface
* **IO Stream: Retarget STDIO**: this component retargets stdio (standard C I/O functions) to the IO Stream
* **Log**: this component provides various functions which can be used to write log messages over the IO Stream

Note: the `main` function is the default entry point of a C program.
