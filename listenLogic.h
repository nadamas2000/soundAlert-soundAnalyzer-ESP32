/**
* @file listenLogic.h
* @brief Logic of listening mode
*
* This library provides the logic about listening mode.
* 
* @author Nahum Manuel Martín
* @date 2023/06/25
*/

#include "alerts.h"
#include "fft.h"
#include "board.h"
#include "display.h"


// -------------- Listening global variables and constants ------------------
const int LISTEN_SAMPLES = 1024;
float max_LISTEN = 0;
int maxI_LISTEN = 0;


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

/**
* @brief Displays the relevant information for the listening mode on the display.
*/
void showListeningInfo();


// ---------------- Sound analyze ----------------------
/**
* @brief Reads the sound data from the microphone.
* @param data The array to store the sound data.
*/
void getSound(float _Complex *data);

/**
* @brief Analyzes the sound data using Fast Fourier Transform (FFT).
* @param data The sound data to analyze.
*/
void analyzeSound(float _Complex *data);

/**
* @brief Extracts the relevant information from the analyzed sound data.
* @param data The analyzed sound data.
*/
void getRellevantInfo(float _Complex *data);


// --------------- Check alerts ------------------------
/**
 * @brief Checks if there is a match between the analyzed sound data and the defined alerts.
 *
 * This function checks if the maximum intensity and its corresponding index
 * match any of the defined alerts.
 *
 * @return true if an alert is matched, false otherwise.
 */
bool alertMatching();


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

void showListeningInfo() {
  display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  display.setCursor(0, 0);
  display.println("Mark: " + String(maxI_LISTEN));
  display.setCursor(0, FONT_HEIGHT);
  display.println("AHz: " + String(max_LISTEN));
  display.setCursor(0, FONT_HEIGHT * 2);
  display.println("Hz: " + String(maxI_LISTEN * 15.2256 * (1024.0 / LISTEN_SAMPLES)));
  display.display();
}


// ---------------- Sound analyze ----------------------
void getSound(float _Complex *data) {
  static long chrono;
  static const int LISTEN_MAX_FREQ = 16; // kHz
  static const unsigned long sampling_period_us = round(1000ul * (1.0 / LISTEN_MAX_FREQ)); // 1/Hz = T(s) -> 1/kHz = T(ms)
  
  for (int i = 0; i < LISTEN_SAMPLES; i++) {
    chrono = micros();
    data[i] = analogRead(MIC_PIN);    
    while (micros() - chrono < sampling_period_us); // only if analogRead time < sampling_period_us
  }
}

void analyzeSound(float _Complex *data) {
  static const int log2Sample = log(LISTEN_SAMPLES) / log(2);
  
  getSound(data);   
  applyWindow (data, log2Sample, HAMMING, FFT_FORWARD);
  performFFT(data, log2Sample, FFT_FORWARD);
}

void getRellevantInfo(float _Complex *data) {
  max_LISTEN = 0;
  maxI_LISTEN = 0;
  for (int i = 1; i < LISTEN_SAMPLES; i++) {
    if (creal(data[i]) > max_LISTEN) {
      max_LISTEN = creal(data[i]);
      maxI_LISTEN = i;    
    }
  }
}


// --------------- Check alerts ------------------------
bool alertMatching() {
  bool alertMatch = false;
  for (short i = 0; i < N_ALERT_TYPES; i++) {
    if (maxI_LISTEN == alerts[i].iteratorMark and max_LISTEN > alerts[i].minIntensity){
      alertMatch = true;
      alerts[i].alertStatus = true; 
      alerts[i].intensityMark = max_LISTEN;
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
  static float _Complex data[LISTEN_SAMPLES];
  
  if (!alert and mode != -1) printListeningLogo();
  
  analyzeSound(data);
  getRellevantInfo(data);
  alert = alertMatching();
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
  if (debug and !alert) showListeningInfo();
  
}

