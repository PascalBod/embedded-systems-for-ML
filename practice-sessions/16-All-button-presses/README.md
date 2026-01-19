# Practice session 16

## Purpose of the practice session

In practice session 8, we saw how to use an interrupt to count the number of button presses with a good accuracy, as long as button presses frequency is not too high. With the current practice session, we will use a queue to do better than simply count the number of presses. In some applications, it can be important to know when an event, like a button press, occurred. Thanks to the queue, we will be able to get the sequence of relative times for each button press.

## Reference information

* [FreeRTOS queues API](https://freertos.org/Documentation/02-Kernel/04-API-references/06-Queues/00-QueueManagement)

## Instructions

Create a new project, named `16-Button-presses`.

Using what you learned in practice session 8 (GPIO interruption) and in practice session 15, develop the following application:
* An interruption is triggered each time button 0 is released.
* The associated ISR sends the current relative time (number of ticks elapsed since last reset) to a queue.
* A task waits on the queue. Each time a new message is received, it increments a counter, and display the counter value, and the received relative time.

One possible way to reuse what you already did in previous practice sessions is to adhere to the following steps:
* In the Project Explorer view, select the project `15-Queue`
* Copy and paste it as `16-Button-presses` project, with **Edit/Copy** and **Edit/Paste**
* Add the *GPIOINT* component to the project
* Right-click on the `cons_prod.c` file in the Project Explorer view, select **Rename...** and rename it to `counter.c`
* Rename `cons_prod.h` to `count.h` in a similar way
* Check that the project builds
* Keep only the consumer task. You can rename it "counter" task
* Configure the GPIO pin, add the ISR code