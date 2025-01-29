# Exercise 09 - Timer interrupt

## Purpose of the exercise

By using an ISR attached to a timer, periodically display a message.

## Reference documents

* [SDK Sleep Timer module API](https://docs.silabs.com/gecko-platform/5.0.2/platform-service/sleeptimer)

## Guidelines

Import the `09-Timer-interrupt` project.

The number of a microcontroller's physical timers is limited. For instance, the EFR32MG24 has two 32-bit counters, three 16-bit counters and one 24-bit low-energy timer. To let the application developer use as many timers as they needs, an intermediate software layer provides as many software timers as required, these software timers being mapped to one physical timer. This intermediate layer is provided by the *Sleep Timer* component.

Look at the Sleep Timer API, more precisely at the `sl_sleeptimer_start_timer` function, and at the `sl_sleeptimer_timer_callback_t` type definition.

Add required code so that a message is displayed every second. Use the `WAIT_PERIOD_MS` macro to define the period in milliseconds. Use the `timeout_flag` boolean flag to communicate between the ISR and the background task.

Do NOT use the `sl_sleeptimer_start_periodic_timer` function :-)
