/**
 * @file board.h
 *
 * @brief Wifi Kit 32 V3 configuration board.
 *
 * @author Nahum Manuel Martín
 * @date 2023/06/25
 */

#pragma once

#include "Arduino.h"
#include <SPI.h>
#include <Wire.h>
#include "driver/rtc_io.h"

/**
 * @def MIC_PIN
 * @brief Pin number for the microphone.
 */
const unsigned char MIC_PIN = 2;

/**
 * @def GPIO_MIC_PIN
 * @brief GPIO pin number for the microphone.
 */
gpio_num_t GPIO_MIC_PIN = GPIO_NUM_2;

/**
 * @def BATTERY_PIN
 * @brief Pin number for the battery.
 */
const unsigned char BATTERY_PIN = 1;

/**
 * @def BUTTON_P_PIN
 * @brief Pin number for the push button.
 */
const unsigned char BUTTON_P_PIN = 0;

/**
 * @def MAX_READ_VALUE
 * @brief Maximum value for analog reading.
 *
 * This value is used with ADC_11db attenuation.
 */
const unsigned short MAX_READ_VALUE = 3000;

/**
 * @def SILENCE
 * @brief Threshold value for silence detection.
 */
const unsigned short SILENCE = 1450;

/**
 * @brief Initializes the board and its components.
 *
 * This function initializes the serial communication and configures the hardware inputs.
 */
void initBoard() {
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  
  // ------------ Hardware inputs parameters --------------  
  analogReadResolution(12);
  analogSetClockDiv(1);  
  analogSetAttenuation(ADC_11db);  
  analogSetPinAttenuation(BATTERY_PIN, ADC_6db);
  analogSetPinAttenuation(MIC_PIN, ADC_11db);
}

/**
 * @brief Puts the board into sleep mode.
 *
 * This function configures the necessary settings for deep sleep mode and puts the board into sleep.
 */
void goToSleep() {  
  rtc_gpio_pulldown_en(GPIO_MIC_PIN);
  esp_sleep_enable_ext0_wakeup(GPIO_MIC_PIN, 1); // 1 = High, 0 = Low
  esp_deep_sleep_start();
}