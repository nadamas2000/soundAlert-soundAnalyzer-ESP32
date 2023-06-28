/**
 * @file display.h
 * @brief This file contains the configuration for Adafruit display module.
 *
 * The display module handles the communication with the OLED display and provides
 * functions for initializing and interacting with the display.
 *
 * @author Nahum Manuel Martín
 * @date 2023/06/25
 */

#pragma once

#include "board.h"

// --------------- Display parameters ------------------
// Adafruit display defines
#define DISPLAY_WIDTH 128   // OLED display width, in pixels
#define DISPLAY_HEIGHT 64   // OLED display height, in pixels
#define SDA_OLED 0x11       // I2C SDA
#define SCL_OLED 0x12       // I2C SCL
#define NO_ADAFRUIT_SSD1306_COLOR_COMPATIBILITY

/**
 * @brief Reset pin for OLED display.
 * Set to -1 if sharing Arduino reset pin.
 */
const unsigned char OLED_RESET = 21;

/**
 * @brief Screen address for OLED display.
 * See datasheet for Address.
 * Use 0x3D for 128x64, 0x3C for 128x32 or Wifi Kit 32 (128x64).
 */
const char SCREEN_ADDRESS = 0x3C;

#include <Adafruit_SSD1306.h>
/**
 * @brief Adafruit SSD1306 display object.
 */
Adafruit_SSD1306 display(DISPLAY_WIDTH, DISPLAY_HEIGHT, &Wire1, OLED_RESET);

/**
 * @brief Font size 1 - Height in pixels.
 */
const unsigned short FONT_HEIGHT = 9;

/**
 * @brief Font size 1 - Width in pixels.
 */
const unsigned short FONT_WIDTH = 6;


// ---------------------- Initialize display ----------------------
/**
 * @brief Initializes the display.
 */
void initDisplay() {
  // Display communication Adafruit
  Wire1.begin(SDA_OLED, SCL_OLED);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay(); // Disable Adafruit logo
  display.display();
}