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
const uint8_t MIC_PIN = 2;

/**
 * @def GPIO_MIC_PIN
 * @brief GPIO pin number for the microphone.
 */
gpio_num_t GPIO_MIC_PIN = GPIO_NUM_2;

/**
 * @def BATTERY_PIN
 * @brief Pin number for the battery.
 */
const uint8_t BATTERY_PIN = 1;

/**
 * @def BUTTON_P_PIN
 * @brief Pin number for the push button.
 */
const uint8_t BUTTON_P_PIN = 0;

/**
 * @def MAX_READ_VALUE
 * @brief Maximum value for analog reading.
 *
 * This value is used with ADC_11db attenuation.
 */
const uint16_t MAX_READ_VALUE = 3000;

/**
 * @def SILENCE
 * @brief Threshold value for silence detection.
 */
const uint16_t SILENCE = 1450;

/**
 * @brief Initializes the board and its components.
 *
 * This function initializes the serial communication and configures the hardware inputs.
 */
void initBoard() {
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  
  // ------------ Hardware inputs parameters --------------
  /**
   * @fn analogReadResolution()
   * @brief Sets the resolution of the analog-to-digital converter (ADC).
   *
   * This function sets the number of bits used for analog-to-digital conversion.
   * The default resolution is 10 bits.
   *
   * @param bits The number of bits for ADC resolution.
   */
  analogReadResolution(12);

  /**
   * @fn analogSetClockDiv()
   * @brief Sets the divider for the ADC clock.
   *
   * This function sets the division factor for the ADC clock.
   * The default value is 1, and the valid range is 1 - 255.
   *
   * @param div The division factor for the ADC clock.
   */
  analogSetClockDiv(1);
  
  /**
   * @fn analogSetAttenuation()
   * @brief Sets the input attenuation for ALL ADC inputs.
   *
   * This function sets the input attenuation for all ADC inputs.
   * The default attenuation is ADC_11db, and the valid range is ADC_0db, ADC_2_5db, ADC_6db, ADC_11db.
   *
   * @param att The input attenuation value.
   */
  analogSetAttenuation(ADC_11db);
  
  /**
   * @fn analogSetPinAttenuation()
   * @brief Sets the input attenuation for a specific ADC pin.
   *
   * This function sets the input attenuation for a specific ADC pin.
   * The default attenuation is ADC_11db, and the valid range is ADC_0db, ADC_2_5db, ADC_6db, ADC_11db.
   *
   * @param pin The pin number.
   * @param att The input attenuation value.
   */
  analogSetPinAttenuation(BATTERY_PIN, ADC_6db);
  analogSetPinAttenuation(MIC_PIN, ADC_11db);
}

/**
 * @brief Puts the board into sleep mode.
 *
 * This function configures the necessary settings for deep sleep mode and puts the board into sleep.
 */
void goToSleep() {
  /**
   * @fn rtc_gpio_pulldown_en()
   * @brief Enables or disables the pull-down resistor for a specific GPIO pin in deep sleep mode.
   *
   * This function enables or disables the pull-down resistor for a specific GPIO pin in deep sleep mode.
   * When the pull-down resistor is enabled, it ensures that the GPIO pin is pulled down to a low logic level.
   * This can be useful for minimizing power consumption when the pin is not actively driven.
   *
   * @param gpio_num The GPIO number of the pin.
   */
  rtc_gpio_pulldown_en(GPIO_MIC_PIN);
  
  /**
   * @fn esp_sleep_enable_ext0_wakeup()
   * @brief Enables wakeup from deep sleep using an external signal.
   *
   * This function enables wakeup from deep sleep using an external signal.
   * It configures the wakeup source and trigger type.
   *
   * @param ext0_gpio_num The GPIO number for the wakeup signal.
   * @param level_low Wakeup trigger type: 0 = Low level trigger, 1 = High level trigger.
   */
  esp_sleep_enable_ext0_wakeup(GPIO_MIC_PIN, 1); // 1 = High, 0 = Low
  
  /**
   * @fn esp_deep_sleep_start()
   * @brief Puts the ESP32 into deep sleep mode.
   *
   * This function puts the ESP32 into deep sleep mode.
   * It will not return and the program execution will stop until the device is reset or woken up by an external signal.
   */
  esp_deep_sleep_start();
}