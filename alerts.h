/**
* @file alerts.h
* @brief Alert definitions
*
* This library provides an Alert struct to create alert types.
* 
* @author Nahum Manuel Martín
* @date 2023/06/25
*/

#pragma once

// ---------- Libraries --------------
#include "images.h"

// ---------- Struct Definition --------------
/**
 * @brief Struct representing an alert element.
 */
struct AlertElement {
  // Fixed data
  unsigned short freq; /**< Frequency in Hz. Fixed information. */
  int minIntensity; /**< Minimum intensity. Fixed parameter. */
  int iteratorRangeMin; /**< Minimum value of Iterator range. Fixed parameter. */
  int iteratorRangeMax; /**< Maximum value of Iterator range. Fixed parameter. */

  // Additional information
  int intensityMark; /**< Intensity mark. */

  // Image 1 properties
  short image1_xPos; /**< X position of image 1. */
  short image1_yPos; /**< Y position of image 1. */
  const Xbm *image1; /**< Pointer to image 1. */

  // Image 2 properties
  short image2_xPos; /**< X position of image 2. */
  short image2_yPos; /**< Y position of image 2. */
  const Xbm *image2; /**< Pointer to image 2. */

  bool alertStatus; /**< Alert status. */
};

// ---------- Constants --------------
const unsigned char N_ALERT_TYPES = 2;
static AlertElement alerts[N_ALERT_TYPES];

// ---------- Function Prototypes --------------
/**
 * @brief Initializes the alerts.
 */
void initAlerts();

// ---------- Def. Alerts --------------
void initAlerts() {
  // Initialize alert 1
  alerts[0].alertStatus = false;
  alerts[0].iteratorRangeMin = 92;
  alerts[0].iteratorRangeMax = 93;
  alerts[0].minIntensity = 40000;
  alerts[0].freq = 1400; // additional info, no compute
  alerts[0].image1_xPos = (DISPLAY_WIDTH / 3) - (arrow_left_img.getWidth() / 2);
  alerts[0].image1_yPos = (DISPLAY_HEIGHT - arrow_left_img.getHeight()) / 2;
  alerts[0].image1 = &arrow_left_img;
  alerts[0].image2_xPos = ((DISPLAY_WIDTH / 3) * 2)  - (door_img.getWidth() / 2);
  alerts[0].image2_yPos = (DISPLAY_HEIGHT - door_img.getHeight()) / 2;
  alerts[0].image2 = &door_img;

  // Initialize alert 2
  alerts[1].alertStatus = false;
  alerts[1].iteratorRangeMin = 85;
  alerts[1].iteratorRangeMax = 86;
  alerts[1].minIntensity = 20000;
  alerts[1].freq = 1300; // additional info, no compute
  alerts[1].image1_xPos = (DISPLAY_WIDTH / 3) - (arrow_down_img.getWidth() / 2);
  alerts[1].image1_yPos = (DISPLAY_HEIGHT - arrow_down_img.getHeight()) / 2;
  alerts[1].image1 = &arrow_down_img;
  alerts[1].image2_xPos = ((DISPLAY_WIDTH / 3) * 2)  - (phone_img.getWidth() / 2);
  alerts[1].image2_yPos = (DISPLAY_HEIGHT - phone_img.getHeight()) / 2;
  alerts[1].image2 = &phone_img;
}
