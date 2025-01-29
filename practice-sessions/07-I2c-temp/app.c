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
    RTOS for ML presentation - exercise 07.
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
#include "em_cmu.h"
#include "sl_i2cspm_instances.h"
#include "sl_sleeptimer.h"

// See xG24 User's Guide, section 3.4.1, for pin definitions.
#define ENABLE_IMU_PORT gpioPortC
#define ENABLE_IMU_PIN  9

// See Si7021 datasheet for definitions below.
#define SI7021_ADDRESS  0x40
#define SI7021_ID       0x15
#define READ_ELECTRONIC_ID_1  0xfc
#define READ_ELECTRONIC_ID_2  0xc9
#define MEASURE_TEMP_NO_HMM 0xf3
#define READ_TEMP           0xe0

// Period of time for sensor wakeup, in ms.
#define SI2071_WAKEUP_TIME_MS 80

// Max allowed period between measurement command and result, in ms.
#define MAX_MEASUREMENT_PERIOD_MS 1000

// Period of time between two successive measurements, in ms.
#define MEASUREMENT_PERIOD_MS 1000

typedef enum {
  ST_WAIT_MEASUREMENT,
  ST_WAIT_BEFORE_REQ,
  ST_ERROR,
} state_t;

static state_t current_state;

static I2C_TransferSeq_TypeDef seq;
static uint8_t command[2];
static uint8_t data[6];

static uint64_t prev_wait_time_ms;

// The function below is borrowed from Silicon Labs Si2071 example. See Si7021
// Data Sheet section 5.1.2.
/***************************************************************************//**
 * Function to decode Temperature from the read Temperature value
 *
 * This function decodes Temperature using the formula provided in the Si7021
 * datasheet. Returns the value in Celsius.
 ******************************************************************************/
static uint32_t decode_temp(uint8_t* read_register)
{
  uint32_t tempValue;
  float actual_temp;
  uint32_t rounded_temp;

  // Formula to decode read Temperature from the Si7021 Datasheet
  tempValue = ((uint32_t) read_register[0] << 8) + (read_register[1] & 0xfc);
  actual_temp = (((tempValue) * 175.72f) / 65536) - 46.85f;

  // Round the temperature to an integer value
  if (actual_temp < 0) {
    rounded_temp = (uint32_t)(actual_temp - 0.5f);
  } else {
    rounded_temp = (uint32_t)(actual_temp + 0.5f);
  }

  return rounded_temp;
}

// Returns false in case of error, true otherwise.
// Side effects: updates command, seq and data.
static bool check_sensor(void) {

  I2C_TransferReturn_TypeDef i2c_rs;

  // Set the device I2C address. Keep LSB for R/W bit.
  seq.addr = SI7021_ADDRESS << 1;

  // Check that we really have an Si7021. See Si7021 Data Sheet section 5.3.
  command[0] = READ_ELECTRONIC_ID_1;
  command[1] = READ_ELECTRONIC_ID_2;
  seq.buf[0].data = command;
  seq.buf[0].len = 2;
  // The sensor returns 6 bytes. The device identification is in the first byte.
  seq.buf[1].data = data;
  seq.buf[1].len = 6;
  // Send the command. The name of the first argument results from the value
  // chosen in I2CSPM component configuration.
  seq.flags = I2C_FLAG_WRITE_READ;
  i2c_rs = I2CSPM_Transfer(sl_i2cspm_si7021, &seq);
  if (i2c_rs != i2cTransferDone) {
      app_log_warning("Device identification read error: %d\n", i2c_rs);
      return false;
  }

  // Check the device identification.
  app_log_info("Device identification: %02x\n", data[0]);
  if (data[0] != SI7021_ID) {
      app_log_error("Not the right sensor!\n");
      return false;
  }

  return true;

}

// Returns false in case of error, true otherwise.
// Side effects: updates command, seq and data.
static bool send_measurement_command(void) {

  I2C_TransferReturn_TypeDef i2c_rs;

  // Send the measure temperature command, in no hold mode.
  command[0] = MEASURE_TEMP_NO_HMM;
  seq.buf[0].data = command;
  seq.buf[0].len = 1;
  seq.buf[1].data = NULL;
  seq.buf[1].len = 0;
  // Send the command.
  seq.flags = I2C_FLAG_WRITE;
  i2c_rs = I2CSPM_Transfer(sl_i2cspm_si7021, &seq);
  if (i2c_rs == i2cTransferDone) {
      app_log_info("Measurement command sent\n");
      // Prepare I2C data for next I2C interaction.
      seq.buf[0].data = data;
      seq.buf[0].len = 2;
      seq.buf[1].data = NULL;
      seq.buf[1].len = 0;
      seq.flags = I2C_FLAG_READ;
      return true;
  }
  // At this stage, unexpected status.
  app_log_error("I2C status for measurement command: %d\n", i2c_rs);
  return false;

}

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{

  sl_status_t sl_rs;
  bool bool_rs;

  // To let us know which application we are running.
  app_log("07-I2C - v0.4.4\n");

  // Power the sensor. See xG24 User's Guide, section 3.4.1.
  GPIO_PinModeSet(ENABLE_IMU_PORT, ENABLE_IMU_PIN, gpioModePushPull, 1);

  // Wait for sensor to become ready. See Si7021 Data Sheet, table 2.
  sl_sleeptimer_delay_millisecond(SI2071_WAKEUP_TIME_MS);

  bool_rs = check_sensor();
  if (!bool_rs) {
      current_state = ST_ERROR;
      return;
  }

  bool_rs = send_measurement_command();
  if (!bool_rs) {
      current_state = ST_ERROR;
      return;
  }

  // Get and save current time in ms.
  uint64_t ticks = sl_sleeptimer_get_tick_count64();
  sl_rs = sl_sleeptimer_tick64_to_ms(ticks, &prev_wait_time_ms);
  if (sl_rs != SL_STATUS_OK) {
      app_log_error("Error from sl_sleeptimer_tick64_to_ms: %lx\n", sl_rs);
      current_state = ST_ERROR;
      return;
  }
  // At this stage, we were able ot initialize the time variable.
  current_state = ST_WAIT_MEASUREMENT;

}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{

  uint64_t ticks;
  uint64_t ms;
  I2C_TransferReturn_TypeDef i2c_rs;
  sl_status_t sl_rs;
  bool bool_rs;

  switch (current_state) {

    case ST_WAIT_MEASUREMENT:
      // Check if the measure is available.
      i2c_rs = I2CSPM_Transfer(sl_i2cspm_si7021, &seq);
      if (i2c_rs == i2cTransferNack) {
          // No measure available yet. Have we been waiting for too long?
          ticks = sl_sleeptimer_get_tick_count64();
          sl_rs = sl_sleeptimer_tick64_to_ms(ticks, &ms);
          if (sl_rs != SL_STATUS_OK) {
              app_log_error("Error from sl_sleeptimer_tick64_to_ms: %lx\n",
                            sl_rs);
              current_state = ST_ERROR;
              break;;
          }
          if (ms - prev_wait_time_ms > MAX_MEASUREMENT_PERIOD_MS) {
              app_log_warning("We have been waiting for too long\n");
              current_state = ST_ERROR;
              break;
          }
          // At this stage, we can still wait. Stay in same state.
          break;
      }
      if (i2c_rs == i2cTransferDone) {
          // Measure is ready, and was read.
          app_log_info("Data: %02x %02x\n", data[0], data[1]);
          uint32_t temp = decode_temp(data);
          app_log_info("Temperature: %ld\n", temp);
          // Now wait before next measurement.
          uint64_t ticks = sl_sleeptimer_get_tick_count64();
          sl_rs = sl_sleeptimer_tick64_to_ms(ticks, &prev_wait_time_ms);
          if (sl_rs != SL_STATUS_OK) {
              app_log_error("Error from sl_sleeptimer_tick64_to_ms: %lx\n",
                            sl_rs);
              current_state = ST_ERROR;
              break;
          }
          current_state = ST_WAIT_BEFORE_REQ;
          break;
      }
      // At this stage, unexpected return status.
      app_log_warning("I2C status for measure read: %d\n", i2c_rs);
      current_state = ST_ERROR;
      break;

    case ST_WAIT_BEFORE_REQ:
      // Have we been waiting for long enough?
      ticks = sl_sleeptimer_get_tick_count64();
      sl_rs = sl_sleeptimer_tick64_to_ms(ticks, &ms);
      if (sl_rs != SL_STATUS_OK) {
          app_log_error("Error from sl_sleeptimer_tick64_to_ms: %lx\n",
                        sl_rs);
          current_state = ST_ERROR;
          break;;
      }
      if (ms - prev_wait_time_ms > MEASUREMENT_PERIOD_MS) {
          // Time for a new measurement.
          bool_rs = send_measurement_command();
          if (!bool_rs) {
              current_state = ST_ERROR;
              break;
          }
          // At this stage, command successfully sent.
          // Get and save current time in ms.
          uint64_t ticks = sl_sleeptimer_get_tick_count64();
          sl_rs = sl_sleeptimer_tick64_to_ms(ticks, &prev_wait_time_ms);
          if (sl_rs != SL_STATUS_OK) {
              app_log_error("Error from sl_sleeptimer_tick64_to_ms: %lx\n",
                            sl_rs);
              current_state = ST_ERROR;
              break;
          }
          current_state = ST_WAIT_MEASUREMENT;
          break;
      }
      // At this stage, we still have to wait. Stay in same state.
      break;

    case ST_ERROR:
      break;

    default:
      app_log_error("Unexpected state: %d\n", current_state);
      current_state = ST_ERROR;

  }

}
