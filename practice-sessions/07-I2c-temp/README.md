# Exercise 07 - I2C

## Purpose of the exercise

Get temperature data from the Si7021 sensor, using the I2C bus.

## What you have to do

Browse the [Si7021 data sheet](https://www.silabs.com/documents/public/data-sheets/Si7021-A20.pdf), to get an overview of interactions that can happen over the I2C bus.

Import the `07-I2c-temp` project.

Read and understand the existing source code in `app.c`.

Build the application, flash it and check the ouput, piping it to the ts command:
```shell
$ pyserial-miniterm --raw -e /dev/ttyACM0 115200 | ts "%H:%M:%.S"
```

Answer the following questions:
* What is the maximum number of temperature samples per second that can be provided by the sensor?
* How long does it take to the application to retrieve one temperature sample?
* Deduce the maximum number of samples per second that can be retrieved by the application
