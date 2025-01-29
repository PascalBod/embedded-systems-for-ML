/***************************************************************************//**
 * @file
 * @brief Blink examples functions
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
    RTOS for ML presentation - exercise 12.
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

#include <stdbool.h>
#include <stdint.h>

#include "app_log.h"
#include "em_system.h"
#include "FreeRTOS.h"
#include "sl_udelay.h"
#include "task.h"

#define TASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define TASK_PRIORITY   10

#define LONG_DELAY_MS   200
#define SHORT_DELAY_MS  10

// Maximum delay for sl_udelay_wait: 100000 µs.
#define RESET_DELAY_US  100000

static volatile uint32_t a;
static volatile uint32_t b;

static StaticTask_t xTaskBuffer_1;
static StaticTask_t xTaskBuffer_2;
static StaticTask_t xTaskBuffer_3;

static StackType_t  xStack_1[TASK_STACK_SIZE];
static StackType_t  xStack_2[TASK_STACK_SIZE];
static StackType_t  xStack_3[TASK_STACK_SIZE];

static TickType_t long_delay;
static TickType_t short_delay;

static void reset_on_fatal_error(void) {

  app_log_error("About to reset\n");
  sl_udelay_wait(RESET_DELAY_US);
  NVIC_SystemReset();

}

static void task1_code(void *arg) {

  (void)arg;

  // Simulate some processing.
  vTaskDelay(long_delay);
  //

  while (true) {
      if (a == 1) {
          // Simulate some processing.
          vTaskDelay(long_delay);
          //
          b = a;
      }
      if (b != 1) {
          app_log_warning("b value: %lu\n", b);
      }
      // Simulate some processing.
      vTaskDelay(long_delay);
      //
  }

}

static void task2_code(void *arg) {

  (void)arg;

  // Simulate some processing.
  vTaskDelay(short_delay);
  //

  while (true) {
      // Simulate some processing.
      vTaskDelay(short_delay + 9);
      //
      a = 2;
  }

}

static void task3_code(void *arg) {

  (void)arg;

  while (true) {
      // Simulate some processing.
      vTaskDelay(short_delay);
      //
      a = 1;
  }

}

void share_init(void) {

  TaskHandle_t xHandle;

  xHandle = xTaskCreateStatic(task1_code,
                              "task1",
                              TASK_STACK_SIZE,
                              NULL,
                              TASK_PRIORITY,
                              xStack_1,
                              &xTaskBuffer_1);
  if (xHandle == NULL) {
      app_log_error("NULL returned by xTaskCreateStatic\n");
      reset_on_fatal_error();
  }

  xHandle = xTaskCreateStatic(task2_code,
                              "task2",
                              TASK_STACK_SIZE,
                              NULL,
                              TASK_PRIORITY,
                              xStack_2,
                              &xTaskBuffer_2);
  if (xHandle == NULL) {
      app_log_error("NULL returned by xTaskCreateStatic\n");
      reset_on_fatal_error();
  }

  xHandle = xTaskCreateStatic(task3_code,
                              "task3",
                              TASK_STACK_SIZE,
                              NULL,
                              TASK_PRIORITY,
                              xStack_3,
                              &xTaskBuffer_3);
  if (xHandle == NULL) {
      app_log_error("NULL returned by xTaskCreateStatic\n");
      reset_on_fatal_error();
  }

  long_delay =  pdMS_TO_TICKS(LONG_DELAY_MS);
  short_delay =  pdMS_TO_TICKS(SHORT_DELAY_MS);

  a = 1;
  b = 1;

}
