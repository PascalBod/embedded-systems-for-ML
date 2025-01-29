# Exercise 04 - ADC

## Purpose of the exercise

In a first step: get the value of an internal voltage.

In a second step: get this value every second.

The ADC block of the EFR32MG24 is feature rich. Consequently, it's complicated. To save you having to read the 60 related pages of the Reference Manual, a skeleton application is provided. It contains the required initialization and configuration code.

Additionally, the sections below provide an overview of the ADC block.

Note: the skeleton application is created from the *Hello World* application. In step 1, the **Platform > Peripheral > IADC** component is added. In step 2, the **Services > Timers > Sleep Timer** component is added.

## EFR32MG24 ADC

### Reference information

* [EFR32xG24 Wireless SoC Reference Manual](https://www.silabs.com/documents/public/reference-manuals/efr32xg24-rm.pdf) - section 23, starting on page 745
* [AN1189: Incremental Analog to Digital Converter (IADC)](https://www.silabs.com/documents/public/application-notes/an1189-efr32-iadc.pdf)

### IADC

There are several ways to convert an analog signal to data. The EFR32MG24 converter is an Incremental Analog to Digital Converter (IADC). It combines techniques from Successive Approximation Register (SAR) ADCs and Delta-Sigma ADCs.

### Reference voltage

The reference voltage can be internal and fixed (1.21 V), internal and based on the analog power supply voltage, or external.

The ADC cannot measure voltages larger than the reference voltage.

### Analog gain

The analog signal can be attenuated or amplified before being converted. The gain can be set to 0.5, 1, 2, 3 or 4.

2, 3 and 4 gain values may require a slower clock (see farther below).

Gain value also impacts the maximal signal voltage which can be converted.

### Conversion modes

The IADC can be in one of three different modes: normal mode, high-speed mode and high-accuracy mode.

In normal and high-speed modes, the maximum resolution is 12 bits. In normal mode, the sampling rate can be up to 1 Msps. In the high-speed mode, it can be up to 2 Msps.

In high-accuracy mode, the maximum resolution is 17 bits. Sampling rate can be up to 10.7 ksps.

### Operating modes

The IADC has two modes of operations: single channel mode and scan mode.

In single channel mode, the IADC converts one input. The conversion can be one time or continuous.

In scan mode, the IADC converts a sequence of different inputs. The conversion can be one time or continuous. The conversions are done without CPU intervention.

### Output data

Data produced by the conversion process is written into a FIFO (First In First Out) queue. There are two FIFOs: one for single channel mode and one for scan mode.

### Clocks

The IADC logic requires several clocks. The first one is used by the IADC registers and the FIFO read logic. The second one, CLK_SRC_ADC, is used for the other operations.

CLK_SRC_ADC is derived from the clock provided by the Clock Management Unit (CMU). It must not be faster than 40 MHz.

ADC_CLK is derived from CLK_SRC_ADC and is used to drive the ADC front-end and the state machine logic. Its maximal value depends on the conversion mode and on the analog gain (between 2.5 MHz and 20 MHz).

### Configuration groups

The IADC provides two independent groups of configurations for setting conversion mode, clock prescaler, reference voltage selection, sampling rate, analog gain, etc.

## The exercise

### Step 1

The ADC block is initialized and configured by `app_init`. Additionally, `app_init` requests a conversion. Read the code and the associated comments, along with the above sections, and get some understanding of the various steps involved in the initialization, in the configuration and in the output of data resulting from a conversion.

In `app_process_action`, add code to:
* Wait for the end of the conversion started by `app_init`
* Get data resulting from the conversion and display it
* Calculate and display associated voltage

Keep in mind information provided by the comments:
* Resolution is 12 bits. This means that maximum data value is 4096
* Reference voltage is 1.21 V and gain is 0.5. This means that maximum voltage is 2.42 V
* The voltage converted by the ADC is 1/4 of the real voltage

### Step 2

Start a new conversion every second, and display resulting voltage. Use a sleep timer, as in exercise 03, to wait between two successive conversions.

Note: the ADC block provides a timer, which can be used to trigger repeated conversions, without intervention from the CPU, i.e. without having to add control code. But maximum timer period is shorter than what we need. Consequently, we can't use this timer.