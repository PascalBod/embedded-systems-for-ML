# Exercise 15 - Queue

## Purpose of the exercise

Use a queue to let two producers send their data to one consumer.

## Reference information

* [FreeRTOS queues API](https://freertos.org/Documentation/02-Kernel/04-API-references/06-Queues/00-QueueManagement)

## Instructions

Import the `15-Queue` project.

The source code of this project is organized in a way slightly different from the previous projects, in order to be more consistent with Silicon Labs environment. The application is no more implemented in `app.c`. `app.c` only calls the application initialization function(s). The application initiailization function(s) is/are defined in other files. Here, as this is still a very simple project, we have only one such other file, `cons_prod.c`.

Go through these two files, to understand this new architecture. Some code is missing, and it's up to you to add it, according to the `TODO` instructions in `cons_prod.c`.

Once the application works, simulate some processing time spent on every message received by the consumer task, by adding a wait period in the while loop. Start with a small period, e.g. 200 ms. Then increase it. Does a problem appear when the period is longer than some value? 

In a second step, instead of experimenting, determine by calculation whether there is a maximum value for the period, after which a problem appears. If this maximum value exists, what is its value? Does the result of this step match the result of the previous one?

If the processing time is greater than the maximum value, could increasing the size of the queue be a solution?

The queue API provides the `xQueueSendToBack` and `xQueueSendToFront` functions. We use the `xQueueSendToBack` one. What can be the use of `xQueueSendToFront`?



