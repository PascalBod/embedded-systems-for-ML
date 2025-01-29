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
    RTOS for ML presentation - exercise 05 step 1.
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

#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"
#include "sl_sleeptimer.h"

// Define UART pins.
// TODO: define the right ports and pins.
#define UART_TX_PORT gpioPortA
#define UART_TX_PIN  0
#define UART_RX_PORT gpioPortA
#define UART_RX_PIN  0

// Period of time between two successive character transmissions, in ms.
#define UART_TX_PERIOD_MS  500

// Set UART initialization block content to default values.
static USART_InitAsync_TypeDef usart_init = USART_INITASYNC_DEFAULT;

static uint64_t prev_ms;

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{

  // Enable required clocks: for USART0 block and for GPIO block.
  // The EFR32BG24 has only one USART, named USART0.
  CMU_ClockEnable(cmuClock_USART0, true);
  CMU_ClockEnable(cmuClock_GPIO, true);

  // Initialize GPIO pins used by USART0.
  // See section 24.3.1 of the Reference Manual.
  // Out argument (the 4th one) is not used, for gpioModePushPull.
  GPIO_PinModeSet(UART_TX_PORT, UART_TX_PIN, gpioModePushPull, 1);
  // Out argument (the 4th one): no filter.
  GPIO_PinModeSet(UART_RX_PORT, UART_RX_PIN, gpioModeInput, 0);

  // Route USART0 pins to GPIO pins.
  // See section 24.6.173 of the Reference Manual.
  GPIO->USARTROUTE[0].TXROUTE = (UART_TX_PORT << _GPIO_USART_TXROUTE_PORT_SHIFT) |
      (UART_TX_PIN << _GPIO_USART_TXROUTE_PIN_SHIFT);
  GPIO->USARTROUTE[0].RXROUTE = (UART_RX_PORT << _GPIO_USART_RXROUTE_PORT_SHIFT) |
      (UART_RX_PIN << _GPIO_USART_RXROUTE_PIN_SHIFT);

  // Now, enable the routes.
  // See section 24.6.167 of the Reference Manual.
  GPIO->USARTROUTE[0].ROUTEEN = GPIO_USART_ROUTEEN_RXPEN | GPIO_USART_ROUTEEN_TXPEN;

  // Initialize USART0. USART_INITASYNC_DEFAULT uses the following
  // characteristics:
  // - speed: 115200 b/s
  // - data bits: 8
  // - parity: none
  // - stop bits: 1
  // - no hardware flow control
  // Modifying a characteristic is done with a simple assignment. For instance,
  // to set speed to 9600 b/s:
  // usart_init.baudrate = 9600;
  USART_InitAsync(USART0, &usart_init);

  prev_ms = 0;

}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{

  // TODO: transmit the '*' character every UART_TX_PERIOD_MS millisecond.
  // Use a Sleep Timer, as in exercise 03 (GPIO output).
  // Note: there is no log channel available. So it's not possible to send log
  // messages to the PC.

}
