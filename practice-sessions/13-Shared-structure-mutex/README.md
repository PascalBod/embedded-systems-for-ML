# Exercise 13 - Shared structure and mutex

## Purpose of the exercise

Add a mutex to the exercise 12 code so that the value of `a` is not modified between the test of its value and its assignement to `b`, in task1.

## Reference information

* [FreeRTOS semaphores and mutexes API](https://freertos.org/Documentation/02-Kernel/04-API-references/10-Semaphore-and-Mutexes/00-Semaphores)

## Guidelines

Inside Simplicity Studio, in the **Project Explorer** view, copy and paste the `12-Shared-structure-bug` project into a project named `13-Shared-structure-mutex`.

Create a mutex, and use it to make atomic the part of task1 which tests the value of `a` and then assigns it to `b`.

You should use the following API functions:
* `xSemaphoreCreateMutex`
* `xSemaphoreTake`
* `xSemaphoreGive`

A semaphore is another type of concurrency control object. It will be introduced in a next section. In FreeRTOS, mutex and semaphore objects share the same API.