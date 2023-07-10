/**
* @file listenLogic.h
* @brief Logic of listening mode
*
* This library provides the logic about listening mode.
* 
* @author Nahum Manuel Martín
* @date 2023/06/25
*/

#pragma once

#include "alerts.h"
#include "board.h"
#include "display.h"
#include "soundInfo.h"
#include "pair.h"


// ----------------- Main listening mode -----------------
/**
 * @brief Performs the main operation of the listening mode.
 *
 * This function is responsible for analyzing the sound data, checking for alerts,
 * and displaying relevant information on the display. If an alert is detected,
 * it can also initiate a communication process if necessary.
 *
 * @param mode The current display mode.
 * @param debug Debug mode to show technical information.
 * @param lastActivity Reference to the last activity timestamp.
 * @param awakeDuration Reference to the duration of device awake time.
 */
void listen(short mode, bool debug, unsigned long &lastActivity, int &awakeDuration);


// ---------------- Printing images and info --------------------
/**
* @brief Displays the listening logo on the display.
*/
void printListeningLogo();

/**
* @brief Draws the alert images on the display for a given AlertElement.
* @param a The AlertElement containing the image properties.
*/
void drawAlertImages(AlertElement &a);

/**
* @brief Displays the alert information on the display.
* @param debug Debug mode to show technical information.
*/
void printAlert(bool debug);

// --------------- Check alerts ------------------------
/**
 * @brief Checks if there is a match between the analyzed sound data and the defined alerts.
 *
 * This function checks if the maximum intensity and its corresponding index
 * match any of the defined alerts.
 *
 * @param maxA The maximum intensity value.
 * @param maxI The index of the maximum intensity value.
 * @return True if an alert is matched, false otherwise.
 */
bool alertMatching(const float maxA, const int maxI);


// ---------------- Printing images and info --------------------
void printListeningLogo() {
  short xCenter = (DISPLAY_WIDTH - mic_logo_img.getWidth()) / 2;
  short yCenter = (DISPLAY_HEIGHT - mic_logo_img.getHeight()) / 2;
  display.clearDisplay();
  display.drawXBitmap(xCenter, yCenter, mic_logo_img.getData(), mic_logo_img.getWidth(), mic_logo_img.getHeight(), SSD1306_WHITE);
  display.display();
}

void drawAlertImages(AlertElement &a) {
  display.drawXBitmap(
    a.image1_xPos,
    a.image1_yPos,
    a.image1->getData(),
    a.image1->getWidth(),
    a.image1->getHeight(),
    SSD1306_WHITE
  );
  display.drawXBitmap(
    a.image2_xPos,
    a.image2_yPos,
    a.image2->getData(),
    a.image2->getWidth(),
    a.image2->getHeight(),
    SSD1306_WHITE
  );
}

void printAlert(bool debug) {
  display.clearDisplay();
  if (debug) {
    display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);  
    for (short i = 0; i < N_ALERT_TYPES; i++) {
      if (alerts[i].alertStatus) {
        display.setCursor(0, 0);
        display.println("Alert! " + String(i));
        display.setCursor(0, FONT_HEIGHT);
        display.println("Hz: " + String(alerts[i].freq));
        display.setCursor(0, FONT_HEIGHT * 2);
        display.println("Mark: " + String(alerts[i].iteratorMark));
        display.setCursor(0, FONT_HEIGHT * 3);
        display.println("Intensity: " + String(alerts[i].intensityMark));
      }
    }  
  } else {
    for (short i = 0; i < N_ALERT_TYPES; i++) {
      if (alerts[i].alertStatus) drawAlertImages(alerts[i]);
    }
  }
  
  display.display();
}

// --------------- Check alerts ------------------------
bool alertMatching(const float maxA, const int maxI) {
  bool alertMatch = false;
  for (short i = 0; i < N_ALERT_TYPES; i++) {
    if (maxI == alerts[i].iteratorMark and maxA > alerts[i].minIntensity){
      alertMatch = true;
      alerts[i].alertStatus = true; 
      alerts[i].intensityMark = maxA;
      for (int j = 0; j < N_ALERT_TYPES; j++) { // Clear other alert match
        if (j != i) alerts[j].alertStatus = false;
      }      
    }
  }
  return alertMatch;
}


// ----------------- Main listening mode -----------------
void listen(short mode, bool debug, unsigned long &lastActivity, int &awakeDuration) {
  static bool alert = false;
  
  if (!alert and mode != -1) printListeningLogo();  
  
  Pair<float, int> maxVal = analyzeSound();
  alert = alertMatching(maxVal.first, maxVal.second);
  if (alert) {
    lastActivity = millis();
    awakeDuration = (2 * 60 * 1000); // 2 minutes showing alert
    printAlert(debug);
    /* 
    * You can implement here a Wifi communication if it's considered necessary.
    * Think that you may need a communication queue with non-repeatable elements
    * and keep the device awake until the end of the transmission queue.
    */
  }

  // info in listening mode.
  if (debug and !alert) showListeningInfo(0, maxVal.first, maxVal.second);
  
}

