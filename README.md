# Introduction to Embedded Systems for Machine Learning

## Overview

This repository contains a presentation providing an introduction to embedded systems. The targeted audience is machine learning application developers, who want to better understand what an embedded system is, and how it can be used.

The following topics are covered:
* Interfaces: GPIO, ADC, UART, SPI, I2C
* Software development over bare metal
* Software development with an RTOS

Practice sessions provide hands-on experience of the presented concepts.

The hardware target for the practice sessions is the [Silicon Labs EFR32xG24 Dev Kit](https://www.silabs.com/development-tools/wireless/efr32xg24-dev-kit?tab=overview), based on the [EFR32MG24 wireless System on Chip (SoC)](https://www.silabs.com/wireless/zigbee/efr32mg24-series-2-socs).

The presentation will be used in the *Machine Learning and Embedded Systems* course taught at [IMT Atlantique](https://www.imt-atlantique.fr/en) in January 2025.

The presentation DOES NOT provide any information about Machine Learning topic.

## Prerequisites for the practice sessions

### Software

The provided practice sessions rely on the development environment presented [here](https://github.com/PascalBod/lm-efr32-simplicityStudio): *Silicon Labs Simplicity Studio* installed in a *Linux Mint Xfce* virtual machine.

Using a virtual machine allows to isolate all specific configuration relating to Simplicity Studio from your other projects. A drawback is that the host computer must be powerful enough and must have more memory.

### Hardware

As mentionned above, the practice sessions use the following hardware:
* [Silicon Labs EFR32xG24 Dev Kit](https://www.silabs.com/development-tools/wireless/efr32xg24-dev-kit?tab=overview)

### Software development competencies

* Basic knowledge of *git* - [git user manual](https://git-scm.com/docs/user-manual)
* Basic knowledge of *GitHub* - [About GitHub and Git](https://docs.github.com/en/get-started/start-your-journey/about-github-and-git)
* Basic knowledge of *Linux* (knowing the most common commands...) - [An Introduction to Linux Basics, from DigitalOcean](https://www.digitalocean.com/community/tutorials/an-introduction-to-linux-basics)
* Basic knowledge of *VirtualBox* (knowing how to create a virtual machine...) - [VirtualBox end-user documentation](https://www.virtualbox.org/wiki/End-user_documentation)
* Good knowledge of one programming language

C language is used, for the practice sessions. [This Standford Computer Science Education document](http://cslibrary.stanford.edu/101/EssentialC.pdf) provides a good presentation of the language. 

### What to do before going through the presentation

1. Install *Simplicity Studio* on your computer. It's a good thing to install it in a virtual machine as presented in the *Software* section above. This way, you can reproduce the exact same environment than the one used here. You can also directly install  Simplicity Studio in your usual environment, but in this case, you are on your own.
2. Get an EFR32xG24 Dev Kit (see the link provided in the *Hardware* section above.
3. Ensure that you can build and program the sample application, as presented in the *lm-efr32-simplicityStudio* repository, introduced in the *Software* section above.

> [!Note]
> If you are an IMT Atlantique student attending the January 2025 course, contact your professor. He will provide you with an access to a pre-configured virtual machine.
> A few notes about using this VM:
> * The memory assigned to the VM is 4 GB. If your PC has enough RAM, don't hesitate to increase the VM memory to 6 or 8 GB.
> * You still need to create a personal account on the Silicon Labs website, as described [here](https://github.com/PascalBod/lm-efr32-simplicityStudio?tab=readme-ov-file#prerequisites-1).
> * A VM shared directory is predefined. As the host directory does not exist when you start the VM, you will get an error message. Ignore it. Then, you can configure the share directory, as explained [here](https://github.com/PascalBod/lm-vm) (look for the *share files* phrases. There are two of them).
> * The only declared user is `developer`. The associated password is `developer`. 
> * From time to time, a window may ask you to choose a password for a new keyring. Click the **Cancel** button.

> [!IMPORTANT]
> Do not upgrade Simplicity Studio, the Silicon Labs SDK, or the toolchain. Provided versions are:
> * Simplicity Studio: 5.9.3.3
> * SDK: 2024.6.2
> * toolchain: GNU ARM 12.2.1.20221205

## Presentation

The presentation is at this URL: https://pascalbod.github.io/RTOS-presentation. 

Source code of the presentation is in the `docs` directory. 

The presentation is licensed under the  [Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License](https://creativecommons.org/licenses/by-nc-sa/4.0/).