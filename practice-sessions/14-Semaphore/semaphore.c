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
    RTOS for ML presentation - exercise 14.
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
#include <stddef.h>
#include <stdint.h>

#include "app_log.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "semphr.h"
#include "sl_udelay.h"
#include "task.h"

#define TASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define TASK_PRIORITY   10

// Maximum time to wait for a semaphore to become available, in ms.
#define SEM_MAX_WAIT_PERIOD_MS 600000

#define TASK_WORKING_TIME_MS 10000

// Maximum delay for sl_udelay_wait: 100000 µs.
#define RESET_DELAY_US  100000

#define NUMBER_OF_ACTIVE_TASKS 2

typedef struct {
    uint32_t id;
    uint32_t wait_time_ms;
    SemaphoreHandle_t semaphore;
} params_t;

static StaticTask_t xTaskBuffer_1;
static StaticTask_t xTaskBuffer_2;
static StaticTask_t xTaskBuffer_3;
static StaticTask_t xTaskBuffer_4;

static StackType_t  xStack_1[TASK_STACK_SIZE];
static StackType_t  xStack_2[TASK_STACK_SIZE];
static StackType_t  xStack_3[TASK_STACK_SIZE];
static StackType_t  xStack_4[TASK_STACK_SIZE];

// Task parameters. They must stay accessible until the end
// of the tasks. Thus, they are created in static storage.
static params_t params1;
static params_t params2;
static params_t params3;
static params_t params4;

static void reset_on_fatal_error(void) {

  app_log_error("About to reset\n");
  sl_udelay_wait(RESET_DELAY_US);
  NVIC_SystemReset();

}

void task_code(void *pvParameters) {

    params_t *paramsPtr = (params_t *)pvParameters;
    uint32_t id = paramsPtr->id;

    app_log_info("Task %lu started\n", id);

    // TODO:
    // - Wait for paramsPtr->wait_time_ms
    // - Try to take the semaphore
    // - Wait for TASK_WORKING_TIME_MS
    // - Give the semaphore
    // - Write log messages allowing to understand what's going on
    // - In case of error, reset.

    app_log_info("%lu - Exiting\n", id);

    vTaskDelete(NULL);

}

void semaphore_init(void) {

    TaskHandle_t xHandle;

    // Create the semaphore.
    SemaphoreHandle_t semaphore = xSemaphoreCreateCounting(
            NUMBER_OF_ACTIVE_TASKS,
            NUMBER_OF_ACTIVE_TASKS);
    if (semaphore == NULL) {
        app_log_error("Error from xSemaphoreCreateCounting\n");
        reset_on_fatal_error();
    }

    // Prepare task parameters.
    params1.id = 1;
    params1.semaphore = semaphore;
    params1.wait_time_ms = 5000;
    //
    params2.id = 2;
    params2.semaphore = semaphore;
    params2.wait_time_ms = 7000;
    //
    params3.id = 3;
    params3.semaphore = semaphore;
    params3.wait_time_ms = 10000;
    //
    params4.id = 4;
    params4.semaphore = semaphore;
    params4.wait_time_ms = 14000;

    xHandle = xTaskCreateStatic(
        task_code,              // Task code.
        "Task1",                // Task name.
        TASK_STACK_SIZE,        // Task stack depth (in words).
        &params1,               // Task parameters.
        TASK_PRIORITY,          // Priority.
        xStack_1,               // Stack buffer.
        &xTaskBuffer_1          // Task control block.
    );
    if (xHandle == NULL) {
        app_log_error("NULL returned by xTaskCreateStatic\n");
        reset_on_fatal_error();
    }

    xHandle = xTaskCreateStatic(
        task_code,              // Task code.
        "Task2",                // Task name.
        TASK_STACK_SIZE,        // Task stack depth (in words).
        &params2,               // Task parameters.
        TASK_PRIORITY,          // Priority.
        xStack_2,               // Stack buffer.
        &xTaskBuffer_2          // Task control block.
    );
    if (xHandle == NULL) {
        app_log_error("NULL returned by xTaskCreateStatic\n");
        reset_on_fatal_error();
    }

    xHandle = xTaskCreateStatic(
        task_code,              // Task code.
        "Task3",                // Task name.
        TASK_STACK_SIZE,        // Task stack depth (in words).
        &params3,               // Task parameters.
        TASK_PRIORITY,          // Priority.
        xStack_3,               // Stack buffer.
        &xTaskBuffer_3          // Task control block.
    );
    if (xHandle == NULL) {
        app_log_error("NULL returned by xTaskCreateStatic\n");
        reset_on_fatal_error();
    }

    xHandle = xTaskCreateStatic(
        task_code,              // Task code.
        "Task4",                // Task name.
        TASK_STACK_SIZE,        // Task stack depth (in words).
        &params4,               // Task parameters.
        TASK_PRIORITY,          // Priority.
        xStack_4,               // Stack buffer.
        &xTaskBuffer_4          // Task control block.
    );
    if (xHandle == NULL) {
        app_log_error("NULL returned by xTaskCreateStatic\n");
        reset_on_fatal_error();
    }

}
