/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

/*
    RTOS for ML presentation - exercise 08.
    Copyright (C) 2024  Pascal Bodin

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include <stdint.h>

#include "app_log.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "gpiointerrupt.h"

// Button 0.
#define BTN0_PORT gpioPortB
#define BTN0_PIN  2
#define BTN0_FILTER false

// External pin interrupt to be used. See section 24.3.10.1 of the Reference
// Manual for information about the interrupts which can be assigned to
// GPIO input pins.
#define EXT_PIN_INT 0

static volatile uint8_t int_nb;

static uint8_t prev_int_nb;

// GPIOINT_IrqCallbackPtrExt_t.
static void on_gpio_change(uint8_t intNo) {

  (void)intNo;
  int_nb++;

}

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{

  app_log_info("Gpio-interrupt - v0.2.3\n");

  int_nb = 0;
  prev_int_nb = 0;

  // Enable GPIO block clock.
  CMU_ClockEnable(cmuClock_GPIO, true);
  // Enable BTN0 GPIO pin.
  GPIO_PinModeSet(BTN0_PORT, BTN0_PIN, gpioModeInput, BTN0_FILTER);

  // Initialize the interrupt dispatcher component.
  GPIOINT_Init();
  // Assign the ISR to the interrupt that is used.
  GPIOINT_CallbackRegister(EXT_PIN_INT, on_gpio_change);
  // It is recommended to disable interrupts before configuring the GPIO pin
  // interrupt. See https://docs.silabs.com/gecko-platform/5.0.2/platform-emlib-efr32xg24/gpio#gpio-ext-int-config.
  GPIO_IntDisable(1 << EXT_PIN_INT);
  // Assign the interrupt to the GPIO pin.
  GPIO_ExtIntConfig(BTN0_PORT,
                    BTN0_PIN,
                    EXT_PIN_INT,
                    true,     // Rising edge.
                    true,     // Falling edge.
                    true      // Enable the interrupt.
                    );

}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{

  if (int_nb != prev_int_nb) {
      app_log_info("New number of interrupts: %d\n", int_nb);
      prev_int_nb = int_nb;
  }

}
