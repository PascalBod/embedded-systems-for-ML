# Exercise 17

## Purpose of the exercise

In exercise 14, we saw that the `app_log` functions were not thread-safe. In the current exercise, you have to develop a log server, which will guarantee that log print requests are done by only one task, the log server task. Tasks which need to print log message will send log requests to the log server task, instead of directly printing their log messages. This way, all log print requests are *serialized*.

Each log message must have a time stamp, the number of ticks elapsed since last reset. This time stamp allows to get a precise information about when the log message was created.

## Instructions

Using Simplicity Studio, copy the `15-Queue` project into a new one, named `17-Log-server`.

Create two new files in the project: `logger.c` and `logger.h`. 

In `logger.c`, define two empty functions:
* `logger_code`, the log server task code
* `logger_init`, which will start the log server task

Additionally, define a queue, named `logger_queue`. It will be the input queue of the log server task.

Think carefully about the type of message the log server task has to accept.

Then, implement the `logger_code` and `logger_init` functions.

In `logger.h`, declare the global objects. The server queue can be declared as follows:
```
extern QueueHandle_t logger_queue;
```

The log server task must print each log message using the following format:
```
<time stamp> <message string>
```

Note: the producer and consumer tasks can build the message strings they want to log with the `snprintf` C library function. Using `snprintf` in an embedded application must be done with caution: it consumes processing power and RAM. We accept ot use it here because our application is very small, and consumes little memory. In a production application, where Flash and RAM may be more sollicited, binary log messages would replace string log messages.

Documentation: [https://linux.die.net/man/3/snprintf](https://linux.die.net/man/3/snprintf).
