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
    RTOS for ML presentation - exercise 06.
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

#include "app_log.h"
#include "em_cmu.h"
#include "em_eusart.h"
#include "em_gpio.h"
#include "sl_sleeptimer.h"

// EUSART used for SPI. We have to use EUSART1 as USART0 signals can't
// be connected to PC port. See EFR32MG24 Datasheet section 6.7.
#define SPI_IMU_EUSART      EUSART1
#define EUSART_ROUTE_INDEX  1
#define EUSART_CLOCK        cmuClock_EUSART1

// See xG24 User's Guide, section 3.4.4, for pin definitions.
#define ENABLE_IMU_PORT gpioPortC
#define ENABLE_IMU_PIN  9
#define SPI_IMU_SCLK_PORT gpioPortC
#define SPI_IMU_SCLK_PIN  1
#define SPI_IMU_COPI_PORT gpioPortC
#define SPI_IMU_COPI_PIN  3
#define SPI_IMU_CIPO_PORT gpioPortC
#define SPI_IMU_CIPO_PIN  2
#define SPI_IMU_CS_PORT gpioPortA
#define SPI_IMU_CS_PIN  7

// IMU SPI registers.
#define IMU_REG_SMPLRT_DIV 0x19
// 1 kHz/(199 + 1) = 5 Hz.
#define IMU_CMD_SMPLRT     199

#define IMU_REG_CONFIG  0x1a
// Filter choice for gyroscope and temperature, required for sample rate.
#define IMU_CMD_CONFIG  0x01

#define IMU_REG_GYRO_CONFIG 0x1b
// Required FCHOICE_B value for setting the sample rate.
#define IMU_CMD_GYRO_CONFIG 0x00

#define IMU_REG_ACCEL_CONFIG 0x1c
// Full scale: +/-4g.
#define IMU_CMD_ACCEL_CONFIG 0x08
#define IMU_ACCEL_FS 4

#define IMU_REG_USER_CTRL 0x6a
// Disable DMP, disable FIFO, disable I2C, reset accelerometer path.
#define IMU_CMD_USER_CTRL 0x11

#define IMU_REG_PWR_MGMT_1 0x6b
// DEVICE_RESET.
#define IMU_CMD_SOFT_RESET 0x81
// TEMP_DIS. And select best clock.
#define IMU_CMD_TEMP_DIS 0x09
// CLKSEL: auto select the best available clock source.
#define IMU_CMD_BEST_CLK 0x01

#define IMU_REG_PWR_MGMT_2 0x6c
#define IMU_CMD_GOFF_AON 0x07

#define IMU_REG_WHO_AM_I 0x75
// ICM-20689 ID.
#define IMU_DATA_WHO_AM_I 0x98

#define IMU_REG_ACCEL_XOUT_H 0x3b
#define IMU_REG_ACCEL_XOUT_L 0x3c

// Period between two acceleration read operations, in ms.
#define READ_PERIOD_MS 100

typedef enum {
  IMU_OK,
  IMU_ERROR,
} imu_status_t;

static uint64_t prev_ms;

//----------------------------------------------------------------------
// Low-level SPI operations for the IMU.
//----------------------------------------------------------------------

static void spi_imu_init(void)
{

  EUSART_SpiInit_TypeDef init = EUSART_SPI_MASTER_INIT_DEFAULT_HF;
  EUSART_SpiAdvancedInit_TypeDef advancedInit = EUSART_SPI_ADVANCED_INIT_DEFAULT;

  init.bitRate = 3300000UL;
  init.advancedSettings = &advancedInit;

  advancedInit.autoCsEnable = false;
  advancedInit.msbFirst = true;

  // Enable required clock.
  CMU_ClockEnable(EUSART_CLOCK, true);

  // Configure GPIO pins for SPI.
  GPIO_PinModeSet(SPI_IMU_COPI_PORT, SPI_IMU_COPI_PIN, gpioModePushPull, 0);
  GPIO_PinModeSet(SPI_IMU_CIPO_PORT, SPI_IMU_CIPO_PIN, gpioModeInput, 0);
  GPIO_PinModeSet(SPI_IMU_SCLK_PORT, SPI_IMU_SCLK_PIN, gpioModePushPull, 0);
  GPIO_PinModeSet(SPI_IMU_CS_PORT, SPI_IMU_CS_PIN, gpioModePushPull, 1);

  // Route EUSART signals to GPIO signals.
  GPIO->EUSARTROUTE[EUSART_ROUTE_INDEX].TXROUTE =
      ((SPI_IMU_COPI_PORT << _GPIO_EUSART_TXROUTE_PORT_SHIFT) |
          (SPI_IMU_COPI_PIN << _GPIO_EUSART_TXROUTE_PIN_SHIFT));
  GPIO->EUSARTROUTE[EUSART_ROUTE_INDEX].RXROUTE =
      ((SPI_IMU_CIPO_PORT << _GPIO_EUSART_RXROUTE_PORT_SHIFT) |
          (SPI_IMU_CIPO_PIN << _GPIO_EUSART_RXROUTE_PIN_SHIFT));
  GPIO->EUSARTROUTE[EUSART_ROUTE_INDEX].SCLKROUTE =
      ((SPI_IMU_SCLK_PORT << _GPIO_EUSART_SCLKROUTE_PORT_SHIFT) |
          (SPI_IMU_SCLK_PIN << _GPIO_EUSART_SCLKROUTE_PIN_SHIFT));

  // Enable the routes.
  GPIO->EUSARTROUTE[EUSART_ROUTE_INDEX].ROUTEEN =
      GPIO_EUSART_ROUTEEN_RXPEN | GPIO_EUSART_ROUTEEN_TXPEN |
      GPIO_EUSART_ROUTEEN_SCLKPEN;

  // Initialize SPI.
  EUSART_SpiInit(SPI_IMU_EUSART, &init);

}

// Handles SPI CS.
static void spi_imu_select(bool cs)
{

  if (cs) {
      GPIO_PinOutClear(SPI_IMU_CS_PORT, SPI_IMU_CS_PIN);
  } else {
      GPIO_PinOutSet(SPI_IMU_CS_PORT, SPI_IMU_CS_PIN);
  }

}

static void spi_imu_write(uint8_t addr, uint8_t data)
{

  // Tell the IMU we want to talk to it.
  spi_imu_select(true);

  // Send register address with MSB = 0 (write).
  EUSART_Spi_TxRx(SPI_IMU_EUSART, (addr & 0x7F));

  // Send data.
  EUSART_Spi_TxRx(SPI_IMU_EUSART, data);

  // Deselect the IMU.
  spi_imu_select(false);

}

static void spi_imu_read(uint8_t addr, int num_bytes, uint8_t *data)
{

  // Tell the IMU we want to talk to it.
  spi_imu_select(true);

  // Send register address with MSB = 1 (read).
  EUSART_Spi_TxRx(SPI_IMU_EUSART, (addr | 0x80));

  // Read data.
  while ( num_bytes-- ) {
    *data++ = EUSART_Spi_TxRx(SPI_IMU_EUSART, 0xff);
  }

  // Deselect the IMU.
  spi_imu_select(false);

}

//----------------------------------------------------------------------
// IMU operations.
//----------------------------------------------------------------------

// Resets the IMU and checks that WHO AMI I register can be read.
// Select SPI bus, and set automatic clock source.
// Returns IMU_OK in case of success, IMU_ERROR otherwise.
static imu_status_t imu_init(void)
{

  // See ICM-20689 datasheet, section 11.28.
  spi_imu_write(IMU_REG_PWR_MGMT_1, IMU_CMD_SOFT_RESET);

  // Wait a bit for reset completion. We could wait on RESET bit as well.
  sl_sleeptimer_delay_millisecond(100);

  // Read WHO AM I register. See ICM-20689 datasheet, section 11.32.
  uint8_t data;
  spi_imu_read(IMU_REG_WHO_AM_I, 1, &data);
  if (data != IMU_DATA_WHO_AM_I) {
      return IMU_ERROR;
  }

  // Disable I2C, reset accelerometer signal path.
  spi_imu_write(IMU_REG_USER_CTRL, IMU_CMD_USER_CTRL);

  // Select best clock.
  spi_imu_write(IMU_REG_PWR_MGMT_1, IMU_CMD_BEST_CLK);

  // Wait a bit for clock stabilization.
  sl_sleeptimer_delay_millisecond(30);

  return IMU_OK;

}

// Enables the accelerometer and disables temperature sensor and gyro.
// Always returns IMU_OK.
static imu_status_t imu_enable_accel(void)
{

  // Disable temperature sensor, keep best clock. See ICM-20689 datasheet, section 11.28.
  spi_imu_write(IMU_REG_PWR_MGMT_1, IMU_CMD_TEMP_DIS);

  // Disable gyro, enable accelerometer.  See ICM-20689 datasheet, section
  // 11.39.
  spi_imu_write(IMU_REG_PWR_MGMT_2, IMU_CMD_GOFF_AON);

  // Wait a bit otherwise the configuration below is not taken into account.
  sl_sleeptimer_delay_millisecond(30);

  // Set full scale value. See ICM-20689 datasheet, section 11.12.
  spi_imu_write(IMU_REG_ACCEL_CONFIG, IMU_CMD_ACCEL_CONFIG);
  // Check.
  uint8_t config;
  spi_imu_read(IMU_REG_ACCEL_CONFIG, 1, &config);
  app_log_info("Config: %02x\n", config);
  return IMU_OK;

}

// Sets sample rate to 5 Hz.
// Always returns IMU_OK.
static imu_status_t imu_set_sample_rate(void)
{

  // Set CONFIG > DLPF_CFG, just to be sure we don't have random data.
  spi_imu_write(IMU_REG_CONFIG, IMU_CMD_CONFIG);
  // Set GYRO_CONFIG > FCHOICE_B to 0 so that we can set sample rate.
  spi_imu_write(IMU_REG_GYRO_CONFIG, IMU_CMD_GYRO_CONFIG);
  // Set sample rate divider.
  spi_imu_write(IMU_REG_SMPLRT_DIV, IMU_CMD_SMPLRT);

  return IMU_OK;

}

// Gets acceleration vector, in millig.
static imu_status_t imu_get_accel(
    int32_t *accel_x, int32_t *accel_y, int32_t *accel_z)
{

  uint8_t a[6];

  uint16_t accel;
  int16_t accel_signed;

  spi_imu_read(IMU_REG_ACCEL_XOUT_H, 6, a);

  accel = (a[0] << 8) + a[1];
  accel_signed = (int16_t)accel;
  *accel_x = (IMU_ACCEL_FS * accel_signed * 1000) / 32768;

  accel = (a[2] << 8) + a[3];
  accel_signed = (int16_t)accel;
  *accel_y = (IMU_ACCEL_FS * accel_signed * 1000) / 32768;

  accel = (a[4] << 8) + a[5];
  accel_signed = (int16_t)accel;
  *accel_z = (IMU_ACCEL_FS * accel_signed * 1000) / 32768;

  return IMU_OK;

}

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{

  // To let us know which application we are running.
  app_log("06-SPI - v0.2.3\n");

  // Enable clock for GPIO block.
  CMU_ClockEnable(cmuClock_GPIO, true);
  // Before powering up the IMU, nCS and SCL/SCLK must be low, for ensuring a
  // proper power-on-reset sequence and the use of SPI. See ICM-20689 datasheet,
  // section 4.19.
  GPIO_PinModeSet(SPI_IMU_CS_PORT, SPI_IMU_CS_PIN, gpioModePushPull, 0);
  GPIO_PinModeSet(SPI_IMU_SCLK_PORT, SPI_IMU_SCLK_PIN, gpioModePushPull, 0);
  // Power the IMU. See xG24 User's Guide, section 3.4.4.
  GPIO_PinModeSet(ENABLE_IMU_PORT, ENABLE_IMU_PIN, gpioModePushPull, 1);

  // Configure EUSART SPI for the IMU.
  spi_imu_init();

  // Reset the IMU.
  imu_status_t imu_rs = imu_init();
  if (imu_rs != IMU_OK) {
      app_log_error("Error returned by imu_reset: %d\n", imu_rs);
  } else {
      app_log_info("Successful IMU reset\n");
  }
  // Configure it.
  imu_enable_accel();
  imu_set_sample_rate();

  prev_ms = 0;

}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{

  uint64_t ticks = sl_sleeptimer_get_tick_count64();
  uint64_t ms;
  sl_status_t rs = sl_sleeptimer_tick64_to_ms(ticks, &ms);
  if (rs != SL_STATUS_OK) {
      app_log_error("Error from sl_sleeptimer_tick64_to_ms: %lu\n", rs);
      return;
  }
  if (ms - prev_ms > READ_PERIOD_MS) {
      // Get new acceleration data.
      int32_t accel_x, accel_y, accel_z;
      imu_get_accel(&accel_x, &accel_y, &accel_z);
      app_log_info("X: %ld - Y: %ld - Z: %ld\n",
                   accel_x, accel_y, accel_z);
      prev_ms = ms;
  }

}
