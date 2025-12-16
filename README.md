# Introduction to Embedded Systems for Machine Learning

## Modifications in progress

07-Dec-2025: bare metal and FreeRTOS examples up to and including practice session 13 have been migrated to Simplicity SDK Suite v2025.6.2.

# Overview

This repository contains a presentation providing an introduction to embedded systems. The targeted audience is machine learning application developers, who want to better understand what an embedded system is, and how it can/must be used.

The presentation DOES NOT provide information about machine learning.

The following topics are covered:
* Interfaces: GPIO, ADC, UART, SPI, I2C
* Software development over bare metal
* Software development with an RTOS

Practice sessions provide hands-on experience of the presented concepts.

The hardware target for the practice sessions is the [Silicon Labs EFR32xG24 Dev Kit](https://www.silabs.com/development-tools/wireless/efr32xg24-dev-kit?tab=overview), based on the [EFR32MG24 wireless System on Chip (SoC)](https://www.silabs.com/wireless/zigbee/efr32mg24-series-2-socs).

This presentation is used as a support for the 22-hour course taught at [IMT Atlantique](https://www.imt-atlantique.fr/en) in January.

## Prerequisites for the practice sessions

### Software

[Silicon Labs Simplicity Studio 5](https://www.silabs.com/developer-tools/simplicity-studio) (based on Eclipse CDT) is used for bare metal (i.e. no RTOS) practice sessions and for FreeRTOS-based practice sessions.

Simplicity Studio 5, abbreviated as SSv5 hereinafter, can be installed on Linux, Apple macOS (Arm or Intel) and Microsoft Windows.

The sections below explain how to create a Linux virtual machine (VM) so that the configuration specific to SSv5 can be isolated from your other projects. A drawback is that the host PC must be powerful enough and must have enough memory (a minimum of 8 GB must be assigned to the VM).

You can also decide on installing SSv5 directly on the OS of your PC. In this case, you can refer to [the instructions provided by Silicon Labs](https://docs.silabs.com/simplicity-studio-5-users-guide/latest/ss-5-users-guide-getting-started/install-ss-5-and-software). But be prepared to possibly have to spend some time tinkering with your setup on your own :-)

### Hardware

As mentionned above, the practice sessions use the following hardware:
* [Silicon Labs EFR32xG24 Dev Kit](https://www.silabs.com/development-tools/wireless/efr32xg24-dev-kit?tab=overview)

### Software development competencies

* Basic knowledge of *Git* - [git user manual](https://git-scm.com/docs/user-manual)
* Basic knowledge of *GitHub* - [About GitHub and Git](https://docs.github.com/en/get-started/start-your-journey/about-github-and-git)
* Basic knowledge of *Linux* (knowing the most common commands...) - [An Introduction to Linux Basics, from DigitalOcean](https://www.digitalocean.com/community/tutorials/an-introduction-to-linux-basics)
* Basic knowledge of *VirtualBox* (knowing how to create a virtual machine...) - [VirtualBox end-user documentation](https://www.virtualbox.org/wiki/End-user_documentation)
* Good knowledge of one programming language

C language is used, for the practice sessions. [This Standford Computer Science Education document](http://cslibrary.stanford.edu/101/EssentialC.pdf) provides a good presentation of the language. 

### What to do before going through the presentation

1. Create a [Linux Mint VM, configure it and install Simplicity Studio](https://github.com/PascalBod/lm-efr32-simplicityStudio)
2. Get an EFR32xG24 Dev Kit (see the link provided in the *Hardware* section above.
3. If you need it, spend some time browsing the documents listed above (Git, GitHub, Linux, C, etc.)
4. Ensure that you can build and program the sample application, as presented in the *VM repository above*lm-ef32-simplicityStudio* repository (the one linked in list item 1 just above)

## Licenses

The presentation is licensed under the  [Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License](https://creativecommons.org/licenses/by-nc-sa/4.0/).

The source code is licensed under the [GNU General Public License v3](https://www.gnu.org/licenses/gpl-3.0.html#license-text).

## Sponsorship

Solutions to the practice sessions are available to IMT Atlantique students attending the course, and to sponsors. To become a sponsor, check the [sponsorship page](https://github.com/sponsors/PascalBod).

## Presentation

* [Preamble](https://pascalbod.github.io/embedded-systems-for-ML/010-Preamble.html)
  * Document history
  * Licenses
  * Credits
  * How to navigate
* [Foreword](https://pascalbod.github.io/embedded-systems-for-ML/020-Foreword.html)
  * Who am I?
  * Needs of embedded ML applications
  * Technical domains
  * Presentation goal
* [Introduction](https://pascalbod.github.io/embedded-systems-for-ML/030-Introduction.html)
  * Hardware progress
  * Embedded applications
* [Microcontrollers and boards](https://pascalbod.github.io/embedded-systems-for-ML/040-MicrocontrollersAndBoards.html)
  * Reminders: memory
  * Microcontroller
  * Board architecture
  * Important characteristics?
  * Some common microcontroller families
    * Arm cores
    * STM32
    * EFR32
    * ESP32
    * RISC-V
* [Software development - Introduction](https://pascalbod.github.io/embedded-systems-for-ML/050-SoftDevIntroduction.html)
  * Cross development
  * Debugging
* [Interfaces - Part 1](https://pascalbod.github.io/embedded-systems-for-ML/052-InterfacesPart1.html)
  * GPIO
  * Hardware API
* [Execution environments](https://pascalbod.github.io/embedded-systems-for-ML/054-ExecutionEnvironments)
  * Bare metal
  * RTOS
  * OS
  * **Practice sessions:**
    * *Hello World!* in bare metal
    * A debug session
* [Peripherals](https://pascalbod.github.io/embedded-systems-for-ML/060-Peripherals.html)
  * Sensors
  * Actuators
* [Interfaces - Part 1 - continued](https://pascalbod.github.io/embedded-systems-for-ML/070-InterfacesPart1Continued.html)
  * More about GPIO
  * **Practice sessions:**
    * Display button state changes
    * Make the red LED blink
* [Software development - Design pattern](https://pascalbod.github.io/embedded-systems-for-ML/080-SoftDevDesignPattern.html)
  * Finite State Machine
* [Interfaces - Part 2](https://pascalbod.github.io/embedded-systems-for-ML/090-InterfacesPart2.html)
  * ADC
  * UART
  * SPI
  * I2C
  * **Practice sessions:**
    * ADC: convert an analog signal
    * UART: send characters to the PC
    * UART: echo received characters
    * SPI: get data from the inertial sensor
    * I2C: get data from the temperature sensor
* [Software development - Part 3](https://pascalbod.github.io/embedded-systems-for-ML/100-SoftDevPart3.html)
  * Floating-point arithmetic
  * Memory: code and data
  * Memory: static, automatic and dynamic storage
  * Stack and heap
  * How an application starts
  * Interrupts and background task
  * Sleep modes
  * **Practice sessions:**
    * GPIO and interrupt
    * Timer and interrupt
    * Application and interrupts
* [More about what an RTOS is](https://pascalbod.github.io/embedded-systems-for-ML/120-MoreAboutRtos.html)
  * Problems and solution
  * Soft and hard real time
  * Benefits
  * Drawbacks
  * Components
* [Tasks](https://pascalbod.github.io/embedded-systems-for-ML/130-Tasks.html)
  * States
  * The scheduler
  * **Practice session:**
    * Creating and starting a task
* [Concurrency control](https://pascalbod.github.io/embedded-systems-for-ML/140-ConcurrencyControl.html)
  * Shared resources
  * Critical section
  * Mutex
  * Priority inversion
  * Deadlock
  * Semaphore
  * **Practice sessions:**
    * Sharing data bug
    * Mutex
    * Semaphore
* [Communication](https://pascalbod.github.io/embedded-systems-for-ML/150-Communication.html)
  * Queues
  * Benefits
  * **Practice session:**
    * Using a queue
    * Time-stamped button presses
    * A log server
* [Concurrency summary](https://pascalbod.github.io/embedded-systems-for-ML/160-ConcurrencySummary.html)
* [Time](https://pascalbod.github.io/embedded-systems-for-ML/170-Time.html)
  * Timer
  * Time functions
  * Device time
* [Memory allocation](https://pascalbod.github.io/embedded-systems-for-ML/180-MemoryAllocation.html)
  * Dynamic memory allocation
  * Drawbacks
* [Middleware](https://pascalbod.github.io/embedded-systems-for-ML/190-Middleware.html)




