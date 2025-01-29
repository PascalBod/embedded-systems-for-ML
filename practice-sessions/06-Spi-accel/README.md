# Exercise 06 - SPI

## Purpose of the exercise

Get accelerometer data from the ICM-20689 IMU using the SPI bus.

## What you have to do

Install the *moreutils* package, in order to get the *ts* command:
```shell
$ sudo apt install moreutils
```

Browse the [ICM-20689 Datasheet](https://invensense.tdk.com/wp-content/uploads/2021/03/DS-000143-ICM-20689-TYP-v1.1.pdf), to get an overview of interactions that can happen over the SPI bus.

Import the `06-Spi-accel` project.

Read and understand the existing source code in `app.c`.

Build the application, flash it and check the ouput, piping it to the ts command:
```shell
$ pyserial-miniterm --raw -e /dev/ttyACM0 115200 | ts "%H:%M:%.S"
```

Answer the following questions:
* What is the maximum number of samples per second that can be provided by the IMU?
* How long does it take to the application to retrieve one acceleration sample?
* Deduce the maximum number of samples per second that can be retrieved by the application
* With the current version of the application, is it possible that a sample is lost, i.e. that a new sample is provided by the IMU while the application has not read yet the previous one? Note: the answer must not be a simple "yes" or "no" :-)
* The current application architecture has a deficiency regarding the way it retrieves the samples. What is the deficiency?
